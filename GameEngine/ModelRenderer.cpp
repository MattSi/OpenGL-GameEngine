#include "EngineCore.h"
#include "ModelRenderer.h"
#include "Model.h"
#include "Shader.h"
#include "Transform.h"
#include "Entity.h"
#include "Material.h"
#include "Mesh.h" 
#include "RenderSystem.h"
#include "InstancedModelRenderer.h"

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
}

IMPLEMENT_DYNAMIC_CLASS(ModelRenderer);
 
void ModelRenderer::Load(json::JSON& data)
{
	Component::Load(data);

	// Load model if specified
	if (data.hasKey("modelPath"))
	{
		modelPath = data.at("modelPath").ToString();
	}

	if (data.hasKey("fallbackModelPath"))
	{
		fallbackModelPath = data.at("fallbackModelPath").ToString();
	}
	if (data.hasKey("fallbackScale"))
	{
		auto s = data.at("fallbackScale");
		fallbackScale = glm::vec3(
			static_cast<float>(s.at("x").ToFloat()),
			static_cast<float>(s.at("y").ToFloat()),
			static_cast<float>(s.at("z").ToFloat()));
		hasFallbackScale = true;
	}

	if (data.hasKey("color"))
	{
		auto c = data.at("color");
		overrideColor.r = static_cast<float>(c.at("r").ToFloat());
		overrideColor.g = static_cast<float>(c.at("g").ToFloat());
		overrideColor.b = static_cast<float>(c.at("b").ToFloat());
		hasColorOverride = true;
	}
	if (data.hasKey("emissiveStrength"))
	{
		overrideEmission = static_cast<float>(data.at("emissiveStrength").ToFloat());
		hasEmissionOverride = true;
	}
}
 
void ModelRenderer::Destroy()
{
	if (ownModel && model)
	{
		delete model;
	}
	model = nullptr;
	ownModel = false;
}

void ModelRenderer::Initialize()
{
	Component::Initialize();

	if(model || modelPath.empty())
		return;

	// TryLoadModel uses the shared shader owned by RenderSystem; do not delete it here.
	bool loaded = TryLoadModel(modelPath);
	if (!loaded && !fallbackModelPath.empty()) {
		std::cerr << "INFO: Using fallback model '" << fallbackModelPath << "' for '" << modelPath << "'." << std::endl;
		loaded = TryLoadModel(fallbackModelPath);
		if (loaded && hasFallbackScale && owner) {
			owner->SetScale(fallbackScale);
		}
	}

	if (loaded) {
		ApplyOverridesToModel();
	}
}

bool ModelRenderer::TryLoadModel(const std::string& path)
{
	Shader* defaultShader = RenderSystem::GetInstance().GetLightingShader();
	if (!defaultShader || path.empty()) {
		return false;
	}

	model = new Model();
	ownModel = true;

	bool loaded = false;
	if (EndsWithIgnoreCase(path, ".obj")) {
		loaded = model->LoadFromOBJ(path, defaultShader);
	} else if (EndsWithIgnoreCase(path, ".ase")) {
		loaded = model->LoadFromASE(path, defaultShader);
	}

	if (!loaded) {
		delete model;
		model = nullptr;
		ownModel = false;
	}
	return loaded;
}

void ModelRenderer::SetModel(Model* mdl)
{
	if(ownModel && model && model != mdl) {
		delete model;
	}
	model = mdl;
	ownModel = false;
}

void ModelRenderer::Render(const glm::mat4& view, const glm::mat4& proj, float delta)
{
	if (!visible || !model || !owner) {
		return;
	}

	Transform* transform = owner->GetComponent<Transform>();
	glm::mat4 modelMatrix(1.0f);

	if (transform) {
		modelMatrix = transform->GetWorldMatrix();
	}

	glm::vec3 viewPos = glm::vec3(glm::inverse(view)[3]);
	for (const auto& subMesh : model->GetSubMeshes()) {
		Material* material = subMesh.material;
		if(!material)
			continue;

		Shader* shader = RenderSystem::GetInstance().GetDefaultShader();
		if (!shader || !shader->isValid()) {
			continue;
		} 


		// Overrides are applied once at load time. Doing it here multiplied tint every frame.
		material->Apply(shader);
		shader->SetMat4("uModel", modelMatrix);
		shader->SetMat4("uView", view);
		shader->SetMat4("uProjection", proj);
		shader->SetVec3("uColor", material->tint);
		shader->SetVec3("uViewPos", viewPos);


		shader->SetInt("uUseInstancing", 0);
		if (subMesh.mesh)
		{
			subMesh.mesh->Draw();
		}
	}
}   

void ModelRenderer::ApplyOverridesToModel()
{
	if (!model) return;

	for(auto& subMesh : model->GetSubMeshes()) {
		Material* material = subMesh.material;
		if (!material) continue;

		if (hasColorOverride) {
			material->tint *= overrideColor;
			material->diffuseColor *= overrideColor;
		}

		if (hasEmissionOverride) {
			material->emissionStrength = overrideEmission;
		}

		if(hasSpecularStrengthOverride) {
			material->specularStrength = overrideSpecularStrength;
		}

		if (hasSpecularColorOverride) {
			material->specularColor = overrideSpecularColor;
		}
	}
}


