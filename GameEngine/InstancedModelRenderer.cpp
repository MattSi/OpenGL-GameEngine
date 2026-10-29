#include "EngineCore.h"
#include "InstancedModelRenderer.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"
#include "Entity.h"
#include "Material.h"
#include "RenderSystem.h"
#include "Mesh.h"

namespace
{
    bool EndsWithIgnoreCase(const std::string& str, const std::string& suffix)
    {
        if (str.size() < suffix.size()) return false;
        for (size_t i = 0; i < suffix.size(); ++i)
        {
            char c1 = (char)std::tolower(str[str.size() - suffix.size() + i]);
            char c2 = (char)std::tolower(suffix[i]);
            if (c1 != c2) return false;
        }
        return true;
    }

    float RandomRange(float min, float max)
    {
        float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        return min + r * (max - min);
	}
}

IMPLEMENT_DYNAMIC_CLASS(InstancedModelRenderer);

void InstancedModelRenderer::Load(json::JSON& data)
{
    Component::Load(data);

    if (data.hasKey("modelPath"))
    {
        modelPath = data.at("modelPath").ToString();
    }

    if (data.hasKey("instanceCount"))
        instanceCount = (int)data.at("instanceCount").ToInt();

    if (data.hasKey("instancesPerRow"))
        instancesPerRow = (int)data.at("instancesPerRow").ToInt();

    if (data.hasKey("instanceSpacing"))
        instanceSpacing = (float)data.at("instanceSpacing").ToFloat();

    if (data.hasKey("randomMin"))
    {
        auto r = data.at("randomMin");
        randomMin = glm::vec3(
            (float)r.at("x").ToFloat(),
            (float)r.at("y").ToFloat(),
            (float)r.at("z").ToFloat()
        );
    }

    if (data.hasKey("randomMax"))
    {
        auto r = data.at("randomMax");
        randomMax = glm::vec3(
            (float)r.at("x").ToFloat(),
            (float)r.at("y").ToFloat(),
            (float)r.at("z").ToFloat()
        );
    }

    if (data.hasKey("randomRotation"))
        randomRotation = data.at("randomRotation").ToBool();

    if (data.hasKey("randomScale"))
        randomScale = data.at("randomScale").ToBool();

    if (data.hasKey("color"))
    {
        auto c = data.at("color");
        overrideColor.r = (float)c.at("r").ToFloat();
        overrideColor.g = (float)c.at("g").ToFloat();
        overrideColor.b = (float)c.at("b").ToFloat();
        hasColorOverride = true;
    }

    if (data.hasKey("emissiveStrength"))
    {
        overrideEmission = (float)data.at("emissiveStrength").ToFloat();
        hasEmissionOverride = true;
    }
}

void InstancedModelRenderer::Initialize()
{
    Component::Initialize();

    if (model || modelPath.empty())
        return;

    // Shared shader owned by RenderSystem; do not delete it here.
    Shader* loadingShader = RenderSystem::GetInstance().GetLightingShader();
    if (!loadingShader)
        return;

    model = new Model();
    ownModel = true;

    bool loaded = false;
    if (EndsWithIgnoreCase(modelPath, ".obj"))
    {
        loaded = model->LoadFromOBJ(modelPath, loadingShader);
    }
    else if (EndsWithIgnoreCase(modelPath, ".ase"))
    {
        loaded = model->LoadFromASE(modelPath, loadingShader);
    }

    if (!loaded)
    {
        delete model;
        model = nullptr;
        ownModel = false;
        return;
    }

    ApplyOverridesToModel();
}

void InstancedModelRenderer::Destroy()
{
    if (ownModel && model)
    {
        delete model;
    }
    model = nullptr;
    ownModel = false;
}

void InstancedModelRenderer::Render(const glm::mat4& view, const glm::mat4& proj, float delta)
{
    if (!visible || !model || !owner || instanceCount <= 0)
        return;

    Transform* transform = owner->GetComponent<Transform>();
    glm::mat4 modelMatrix(1.0f);
    if (transform)
    {
        modelMatrix = transform->GetWorldMatrix();
    }

    glm::vec3 viewPos = glm::vec3(glm::inverse(view)[3]);

    Shader* shader = RenderSystem::GetInstance().GetDefaultShader();
    if (!shader || !shader->isValid())
        return;

    shader->use();
    shader->SetInt("uUseInstancing", 1);
    shader->SetInt("uInstancesPerRow", instancesPerRow);
    shader->SetFloat("uInstanceSpacing", instanceSpacing);

    shader->SetVec3("uRandomMin", randomMin);
    shader->SetVec3("uRandomMax", randomMax);
    shader->SetInt("uRandomRotation", randomRotation ? 1 : 0);
    shader->SetInt("uRandomScale", randomScale ? 1 : 0);


    shader->SetMat4("uModel", modelMatrix);
    shader->SetMat4("uView", view);
    shader->SetMat4("uProjection", proj);
    shader->SetVec3("uViewPos", viewPos);

    for (const auto& subMesh : model->GetSubMeshes())
    {
        Material* material = subMesh.material;
        if (!material || !subMesh.mesh)
            continue;

        material->Apply(shader);

        shader->SetVec3("uColor", material->tint);

        subMesh.mesh->DrawInstanced((unsigned int)instanceCount);
    }

    shader->SetInt("uUseInstancing", 0);
}

void InstancedModelRenderer::ApplyOverridesToModel()
{
    if (!model) return;

    for (auto& subMesh : model->GetSubMeshes())
    {
        Material* material = subMesh.material;
        if (!material) continue;

        if (hasColorOverride)
        {
            material->tint *= overrideColor;
            material->diffuseColor *= overrideColor;
        }

        if (hasEmissionOverride)
        {
            material->emissionStrength = overrideEmission;
        }

        if (hasSpecularStrengthOverride)
        {
            material->specularStrength = overrideSpecularStrength;
        }

        if (hasSpecularColorOverride)
        {
            material->specularColor = overrideSpecularColor;
        }
    }
}

 