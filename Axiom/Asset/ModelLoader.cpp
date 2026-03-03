#include "Asset/ModelLoader.h"

#include "Core/Log.h"
#include "Renderer/Material.h"
#include "Renderer/Mesh.h"
#include "Renderer/Texture2D.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>
#include <filesystem>
#include <vector>
#include <utility>

namespace Axiom
{
    namespace
    {
        struct ModelImportContext
        {
            std::vector<Model::Submesh> Submeshes;
            std::vector<std::shared_ptr<Material>> Materials;
            std::string Directory;
            std::shared_ptr<Texture2D> WhiteTexture;
            std::shared_ptr<Material> FallbackMaterial;
        };

        glm::mat4 ToGlm(const aiMatrix4x4& matrix)
        {
            glm::mat4 result;
            result[0][0] = matrix.a1;
            result[1][0] = matrix.a2;
            result[2][0] = matrix.a3;
            result[3][0] = matrix.a4;
            result[0][1] = matrix.b1;
            result[1][1] = matrix.b2;
            result[2][1] = matrix.b3;
            result[3][1] = matrix.b4;
            result[0][2] = matrix.c1;
            result[1][2] = matrix.c2;
            result[2][2] = matrix.c3;
            result[3][2] = matrix.c4;
            result[0][3] = matrix.d1;
            result[1][3] = matrix.d2;
            result[2][3] = matrix.d3;
            result[3][3] = matrix.d4;
            return result;
        }

        std::shared_ptr<Material> CreateDefaultMaterial()
        {
            const std::string vertexSource = R"(
#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_MVP;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    gl_Position = u_MVP * vec4(a_Position, 1.0);
}
)";

            const std::string fragmentSource = R"(
#version 330 core
out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Albedo;
uniform vec4 u_BaseColor;

void main()
{
    color = texture(u_Albedo, v_TexCoord) * u_BaseColor;
}
)";

            return Material::Create(vertexSource, fragmentSource);
        }

        std::shared_ptr<Texture2D> CreateWhiteTexture()
        {
            const uint8_t whitePixel[4] = { 255, 255, 255, 255 };
            return Texture2D::CreateFromPixels(whitePixel, 1, 1, true);
        }

        std::shared_ptr<Texture2D> LoadMaterialTexture(const aiMaterial& material,
            const aiScene& scene,
            aiTextureType textureType,
            const std::string& directory)
        {
            const unsigned int count = material.GetTextureCount(textureType);
            for (unsigned int i = 0; i < count; ++i)
            {
                aiString path;
                if (material.GetTexture(textureType, i, &path) != AI_SUCCESS)
                {
                    continue;
                }

                std::string pathString = path.C_Str();
                if (pathString.empty())
                {
                    continue;
                }

                if (pathString.front() == '*')
                {
                    const aiTexture* embedded = scene.GetEmbeddedTexture(pathString.c_str());
                    if (!embedded)
                    {
                        continue;
                    }

                    if (embedded->mHeight == 0)
                    {
                        const auto* data = reinterpret_cast<const uint8_t*>(embedded->pcData);
                        auto texture = Texture2D::CreateFromMemory(data, embedded->mWidth);
                        if (texture)
                        {
                            return texture;
                        }
                    }
                    else
                    {
                        const uint32_t width = embedded->mWidth;
                        const uint32_t height = embedded->mHeight;
                        const size_t texelCount = static_cast<size_t>(width) * static_cast<size_t>(height);
                        std::vector<uint8_t> pixels;
                        pixels.resize(texelCount * 4u);

                        for (size_t texelIndex = 0; texelIndex < texelCount; ++texelIndex)
                        {
                            const aiTexel& texel = embedded->pcData[texelIndex];
                            const size_t pixelIndex = texelIndex * 4u;
                            pixels[pixelIndex + 0u] = texel.r;
                            pixels[pixelIndex + 1u] = texel.g;
                            pixels[pixelIndex + 2u] = texel.b;
                            pixels[pixelIndex + 3u] = texel.a;
                        }

                        auto texture = Texture2D::CreateFromPixels(pixels.data(), width, height, true);
                        if (texture)
                        {
                            return texture;
                        }
                    }
                    continue;
                }

                std::filesystem::path texturePath(pathString);
                if (texturePath.is_relative() && !directory.empty())
                {
                    texturePath = std::filesystem::path(directory) / texturePath;
                }

                auto texture = Texture2D::CreateFromFile(texturePath.lexically_normal().string());
                if (texture)
                {
                    return texture;
                }
            }

            return nullptr;
        }

        std::shared_ptr<Material> BuildMaterial(const aiMaterial& material,
            const aiScene& scene,
            const std::string& directory,
            const std::shared_ptr<Texture2D>& fallbackTexture)
        {
            auto result = CreateDefaultMaterial();
            if (!result)
            {
                return nullptr;
            }

            glm::vec4 baseColor(1.0f);
            aiColor4D aiBaseColor;
            if (material.Get(AI_MATKEY_BASE_COLOR, aiBaseColor) == AI_SUCCESS)
            {
                baseColor = glm::vec4(aiBaseColor.r, aiBaseColor.g, aiBaseColor.b, aiBaseColor.a);
            }
            else if (material.Get(AI_MATKEY_COLOR_DIFFUSE, aiBaseColor) == AI_SUCCESS)
            {
                baseColor = glm::vec4(aiBaseColor.r, aiBaseColor.g, aiBaseColor.b, aiBaseColor.a);
            }

            auto albedo = LoadMaterialTexture(material, scene, aiTextureType_BASE_COLOR, directory);
            if (!albedo)
            {
                albedo = LoadMaterialTexture(material, scene, aiTextureType_DIFFUSE, directory);
            }
            if (!albedo)
            {
                albedo = fallbackTexture ? fallbackTexture : CreateWhiteTexture();
            }

            result->SetTexture("u_Albedo", albedo);
            result->SetVec4("u_BaseColor", baseColor);
            return result;
        }

        std::shared_ptr<Material> CreateDefaultModelMaterial(const std::shared_ptr<Texture2D>& fallbackTexture)
        {
            auto material = CreateDefaultMaterial();
            if (!material)
            {
                return nullptr;
            }

            auto texture = fallbackTexture ? fallbackTexture : CreateWhiteTexture();
            material->SetTexture("u_Albedo", texture);
            material->SetVec4("u_BaseColor", glm::vec4(1.0f));
            return material;
        }

        Model::Submesh ProcessMesh(const aiMesh& mesh,
            const glm::mat4& transform,
            const std::shared_ptr<Material>& material)
        {
            std::vector<Mesh::Vertex> vertices;
            vertices.reserve(mesh.mNumVertices);

            for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
            {
                Mesh::Vertex vertex{};
                vertex.Position = glm::vec3(mesh.mVertices[i].x,
                    mesh.mVertices[i].y,
                    mesh.mVertices[i].z);

                if (mesh.HasNormals())
                {
                    vertex.Normal = glm::vec3(mesh.mNormals[i].x,
                        mesh.mNormals[i].y,
                        mesh.mNormals[i].z);
                }
                else
                {
                    vertex.Normal = glm::vec3(0.0f, 0.0f, 1.0f);
                }

                if (mesh.HasTextureCoords(0))
                {
                    vertex.TexCoord = glm::vec2(mesh.mTextureCoords[0][i].x,
                        mesh.mTextureCoords[0][i].y);
                }
                else
                {
                    vertex.TexCoord = glm::vec2(0.0f);
                }

                vertices.push_back(vertex);
            }

            std::vector<uint32_t> indices;
            indices.reserve(mesh.mNumFaces * 3u);
            for (unsigned int i = 0; i < mesh.mNumFaces; ++i)
            {
                const aiFace& face = mesh.mFaces[i];
                for (unsigned int index = 0; index < face.mNumIndices; ++index)
                {
                    indices.push_back(face.mIndices[index]);
                }
            }

            Model::Submesh submesh;
            submesh.MeshPtr = Mesh::Create(vertices, indices);
            submesh.MaterialPtr = material;
            submesh.Transform = transform;
            return submesh;
        }

        void ProcessNode(const aiNode& node,
            const aiScene& scene,
            ModelImportContext& context,
            const glm::mat4& parentTransform)
        {
            const glm::mat4 nodeTransform = parentTransform * ToGlm(node.mTransformation);

            for (unsigned int i = 0; i < node.mNumMeshes; ++i)
            {
                const aiMesh* mesh = scene.mMeshes[node.mMeshes[i]];
                if (!mesh)
                {
                    continue;
                }

                std::shared_ptr<Material> material;
                if (mesh->mMaterialIndex < context.Materials.size())
                {
                    material = context.Materials[mesh->mMaterialIndex];
                }
                if (!material)
                {
                    material = context.FallbackMaterial;
                }

                context.Submeshes.push_back(ProcessMesh(*mesh, nodeTransform, material));
            }

            for (unsigned int i = 0; i < node.mNumChildren; ++i)
            {
                const aiNode* child = node.mChildren[i];
                if (!child)
                {
                    continue;
                }
                ProcessNode(*child, scene, context, nodeTransform);
            }
        }
    }

    std::shared_ptr<Model> ModelLoader::Load(const std::string& path)
    {
        auto model = std::make_shared<Model>();

        Assimp::Importer importer;
        const unsigned int flags = aiProcess_Triangulate
            | aiProcess_GenNormals
            | aiProcess_JoinIdenticalVertices
            | aiProcess_FlipUVs;

        const aiScene* scene = importer.ReadFile(path, flags);
        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u)
        {
            Log::CoreError(std::string("Assimp failed to load '") + path + "': " + importer.GetErrorString());
            return nullptr;
        }

        ModelImportContext context;
        context.Directory = std::filesystem::path(path).parent_path().string();
        context.WhiteTexture = CreateWhiteTexture();
        context.FallbackMaterial = CreateDefaultModelMaterial(context.WhiteTexture);

        context.Materials.reserve(scene->mNumMaterials);
        if (scene->mNumMaterials == 0)
        {
            context.Materials.push_back(context.FallbackMaterial);
        }
        else
        {
            for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
            {
                const aiMaterial* material = scene->mMaterials[i];
                if (!material)
                {
                    context.Materials.push_back(context.FallbackMaterial);
                    continue;
                }
                auto built = BuildMaterial(*material, *scene, context.Directory, context.WhiteTexture);
                context.Materials.push_back(built ? built : context.FallbackMaterial);
            }
        }

        ProcessNode(*scene->mRootNode, *scene, context, glm::mat4(1.0f));

        model->m_Submeshes = std::move(context.Submeshes);

        return model;
    }
}
