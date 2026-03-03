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
#include <utility>

namespace Axiom
{
    namespace
    {
        struct ModelImportContext
        {
            std::vector<Model::Submesh> Submeshes;
            std::vector<ModelMaterial> Materials;
            std::string Directory;
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

        void AppendTextures(const aiMaterial& material,
            aiTextureType textureType,
            ModelTexture::Type targetType,
            const std::string& directory,
            std::vector<ModelTexture>& textures)
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
                ModelTexture texture;
                texture.TextureType = targetType;
                texture.Embedded = !pathString.empty() && pathString.front() == '*';

                if (texture.Embedded || pathString.empty())
                {
                    texture.Path = pathString;
                }
                else
                {
                    std::filesystem::path texturePath(pathString);
                    if (texturePath.is_relative() && !directory.empty())
                    {
                        texturePath = std::filesystem::path(directory) / texturePath;
                    }
                    texture.Path = texturePath.lexically_normal().string();
                }

                textures.push_back(std::move(texture));
            }
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
                        const auto* data = reinterpret_cast<const uint8_t*>(embedded->pcData);
                        auto texture = Texture2D::CreateFromPixels(data, embedded->mWidth, embedded->mHeight, true);
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

        ModelMaterial BuildMaterial(const aiMaterial& material,
            const aiScene& scene,
            const std::string& directory)
        {
            ModelMaterial result;
            result.MaterialPtr = CreateDefaultMaterial();

            aiColor4D baseColor;
            if (material.Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS)
            {
                result.BaseColor = glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a);
            }
            else if (material.Get(AI_MATKEY_COLOR_DIFFUSE, baseColor) == AI_SUCCESS)
            {
                result.BaseColor = glm::vec4(baseColor.r, baseColor.g, baseColor.b, baseColor.a);
            }

            AppendTextures(material, aiTextureType_BASE_COLOR, ModelTexture::Type::Diffuse, directory, result.Textures);
            AppendTextures(material, aiTextureType_DIFFUSE, ModelTexture::Type::Diffuse, directory, result.Textures);
            AppendTextures(material, aiTextureType_SPECULAR, ModelTexture::Type::Specular, directory, result.Textures);
            AppendTextures(material, aiTextureType_NORMALS, ModelTexture::Type::Normal, directory, result.Textures);
            AppendTextures(material, aiTextureType_EMISSIVE, ModelTexture::Type::Emissive, directory, result.Textures);

            auto albedo = LoadMaterialTexture(material, scene, aiTextureType_BASE_COLOR, directory);
            if (!albedo)
            {
                albedo = LoadMaterialTexture(material, scene, aiTextureType_DIFFUSE, directory);
            }
            if (!albedo)
            {
                albedo = CreateWhiteTexture();
            }

            if (result.MaterialPtr)
            {
                result.MaterialPtr->SetAlbedoTexture(albedo);
            }

            return result;
        }

        ModelMaterial CreateDefaultModelMaterial()
        {
            ModelMaterial result;
            result.MaterialPtr = CreateDefaultMaterial();
            if (result.MaterialPtr)
            {
                result.MaterialPtr->SetAlbedoTexture(CreateWhiteTexture());
            }
            return result;
        }

        Model::Submesh ProcessMesh(const aiMesh& mesh, const glm::mat4& transform)
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
            submesh.MaterialIndex = mesh.mMaterialIndex;
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

                context.Submeshes.push_back(ProcessMesh(*mesh, nodeTransform));
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

    Model ModelLoader::Load(const std::string& path)
    {
        Model model;

        Assimp::Importer importer;
        const unsigned int flags = aiProcess_Triangulate
            | aiProcess_GenNormals
            | aiProcess_JoinIdenticalVertices
            | aiProcess_FlipUVs;

        const aiScene* scene = importer.ReadFile(path, flags);
        if (!scene || !scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0u)
        {
            Log::CoreError(std::string("Assimp failed to load '") + path + "': " + importer.GetErrorString());
            return model;
        }

        ModelImportContext context;
        context.Directory = std::filesystem::path(path).parent_path().string();

        context.Materials.reserve(scene->mNumMaterials);
        if (scene->mNumMaterials == 0)
        {
            context.Materials.push_back(CreateDefaultModelMaterial());
        }
        else
        {
            for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
            {
                const aiMaterial* material = scene->mMaterials[i];
                if (!material)
                {
                    context.Materials.push_back(CreateDefaultModelMaterial());
                    continue;
                }
                context.Materials.push_back(BuildMaterial(*material, *scene, context.Directory));
            }
        }

        ProcessNode(*scene->mRootNode, *scene, context, glm::mat4(1.0f));

        model.m_Submeshes = std::move(context.Submeshes);
        model.m_Materials = std::move(context.Materials);

        return model;
    }
}
