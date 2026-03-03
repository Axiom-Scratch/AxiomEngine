#include "Asset/Model.h"

namespace Axiom
{
    std::vector<Model::Submesh>& Model::GetSubmeshes()
    {
        return m_Submeshes;
    }

    const std::vector<Model::Submesh>& Model::GetSubmeshes() const
    {
        return m_Submeshes;
    }
}
