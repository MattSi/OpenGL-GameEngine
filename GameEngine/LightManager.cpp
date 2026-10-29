#include "EngineCore.h"
#include "LightManager.h"
#include "LightComponent.h"
#include "Shader.h"

template<> LightManager* Singleton<LightManager>::instance = nullptr;
void LightManager::Initialize()
{
	lights.clear();
}

void LightManager::CleanUp()
{
	lights.clear();
}

void LightManager::RegisterLight(LightComponent* light)
{
	if(!light) {
		return;
	}

	auto it = std::find(lights.begin(), lights.end(), light);
	if(it == lights.end()) 
	{
		lights.push_back(light);
	}
}

void LightManager::UnregisterLight(LightComponent* light)
{
	if(!light) {
		return;
	}
	auto it = std::find(lights.begin(), lights.end(), light);
	if(it != lights.end()) 
	{
		lights.erase(it);
	}
}

LightComponent* LightManager::GetMainDirectionalLight() const
{
	for (LightComponent* light : lights) {
		if (light && light->GetLightType() == LightComponent::LightType::Directional) {
			return light;
		}
	}
	return nullptr;
}

void LightManager::ApplyToShader(Shader* shader) const
{
	if (!shader) return;

	int pointCount = 0;
	int spotCount = 0;

	if (auto* dirLight = GetMainDirectionalLight()) {
		glm::vec3 dir = dirLight->GetDirection();
		float intensity = dirLight->GetIntensity();
		glm::vec3 color = dirLight->GetColor();

		shader->SetVec3("uDirectionalLight.direction", dir);
		shader->SetFloat("uDirectionalLight.intensity", intensity);
		shader->SetVec3("uDirectionalLight.color", color);
		shader->SetFloat("uDirectionalLight.pad0", 0.0f);
	}
	else {
		// No directional light found, set black 
		shader->SetVec3("uDirectionalLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
		shader->SetFloat("uDirectionalLight.intensity", 0.0f);
		shader->SetVec3("uDirectionalLight.color", glm::vec3(0.0f));
		shader->SetFloat("uDirectionalLight.pad0", 0.0f);
	}
	 
	for (auto* light : lights) {
		if (!light) continue;

		switch (light->GetLightType()) {
			case LightComponent::LightType::Point: 
			{
				if (pointCount >= MAX_POINT_LIGHTS) break;
				 
				std::string base = "uPointLights[" + std::to_string(pointCount) + "]";
				shader->SetVec3((base + ".position").c_str(), light->GetPosition());
				shader->SetFloat((base + ".intensity").c_str(), light->GetIntensity());
				shader->SetVec3((base + ".color").c_str(), light->GetColor());
				shader->SetFloat((base + ".range").c_str(), light->GetRange());
				shader->SetVec3((base + ".attenuation").c_str(), light->GetAttenuation());
				shader->SetFloat((base + ".pad0").c_str(), 0.0f);

				++pointCount;
				break;
			}
			case LightComponent::LightType::Cone: 
			{
				if (spotCount >= MAX_SPOT_LIGHTS) break;
				std::string base = "uSpotLights[" + std::to_string(spotCount) + "]";

				shader->SetVec3((base + ".position").c_str(), light->GetPosition());
				shader->SetFloat((base + ".intensity").c_str(), light->GetIntensity());
				shader->SetVec3((base + ".color").c_str(), light->GetColor());
				shader->SetFloat((base + ".range").c_str(), light->GetRange());
				shader->SetVec3((base + ".direction").c_str(), light->GetDirection());
				shader->SetFloat((base + ".innerCutoff").c_str(), light->GetInnerCutoff());
				shader->SetFloat((base + ".outerCutoff").c_str(), light->GetOuterCutoff());
				shader->SetFloat((base + ".pad0").c_str(), 0.0f);
				shader->SetFloat((base + ".pad1").c_str(), 0.0f);
				shader->SetFloat((base + ".pad2").c_str(), 0.0f);
				
				++spotCount;
				break;
			}
			default:
				break;
		}
	}
	shader->SetInt("uNumPointLights", pointCount);
	shader->SetInt("uNumSpotLights", spotCount);
}