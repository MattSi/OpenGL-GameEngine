#include "EngineCore.h"
#include "Entity.h"
#include "LightComponent.h"
#include "Transform.h"
#include "LightManager.h"

IMPLEMENT_DYNAMIC_CLASS(LightComponent);
 
void LightComponent::Initialize()
{
	Component::Initialize();	
	SetDirection(direction);

	LightManager& lightManager = LightManager::GetInstance();
	lightManager.RegisterLight(this);
}

void LightComponent::Destroy()
{
	LightManager& lightManager = LightManager::GetInstance();
	lightManager.UnregisterLight(this);
	Component::Destroy();
}

void LightComponent::Load(json::JSON& data)
{
	Component::Load(data);
	if (data.hasKey("lightType"))
	{
		std::string typeStr = data.at("lightType").ToString();
		std::string lowerTypeStr;
		lowerTypeStr.reserve(typeStr.size());
		for(char c: typeStr) {
			lowerTypeStr.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
		}

		if (lowerTypeStr == "directional")
			lightType = LightType::Directional;
		else if (lowerTypeStr == "point")
			lightType = LightType::Point;
		else if (lowerTypeStr == "cone" || lowerTypeStr =="spot" || lowerTypeStr =="spotlight")
			lightType = LightType::Cone;
	}

	if(data.hasKey("color")) {
		auto colorJson = data.at("color");
		color.r = static_cast<float>(colorJson.at("r").ToFloat());
		color.g = static_cast<float>(colorJson.at("g").ToFloat());
		color.b = static_cast<float>(colorJson.at("b").ToFloat());
	}

	if (data.hasKey("intensity"))
	{
		intensity = static_cast<float>(data.at("intensity").ToFloat());
	}

	if(data.hasKey("direction")) {
		auto dirJson = data.at("direction");
		direction.x = static_cast<float>(dirJson.at("x").ToFloat());
		direction.y = static_cast<float>(dirJson.at("y").ToFloat());
		direction.z = static_cast<float>(dirJson.at("z").ToFloat());

		SetDirection(direction);
	}
	else {
		SetDirection(direction);
	}
	if (data.hasKey("range"))
	{
		range = static_cast<float>(data.at("range").ToFloat());
	}
	if(data.hasKey("attenuation")) {
		auto attJson = data.at("attenuation");
		attenuation.x = static_cast<float>(attJson.at("constant").ToFloat());
		attenuation.y = static_cast<float>(attJson.at("linear").ToFloat());
		attenuation.z = static_cast<float>(attJson.at("quadratic").ToFloat());
	}
	if (data.hasKey("innerCutoff"))
	{
		innerCutoff = static_cast<float>(data.at("innerCutoff").ToFloat());
	}
	if (data.hasKey("outerCutoff"))
	{
		outerCutoff = static_cast<float>(data.at("outerCutoff").ToFloat());
	}
	if (data.hasKey("coneAngle")) {
		innerCutoff = static_cast<float>(data.at("coneAngle").ToFloat());
	}
	if (data.hasKey("fallOff")) {
		outerCutoff = static_cast<float>(data.at("fallOff").ToFloat());
	}
	if(data.hasKey("emissiveStrength")) {
		emissionStrength = static_cast<float>(data.at("emissiveStrength").ToFloat());
	}

}

void LightComponent::SetLightType(LightType newType)
{
	lightType = newType;
}

void LightComponent::SetColor(const glm::vec3& newColor)
{
	color = newColor;
	
}

void LightComponent::SetDirection(const glm::vec3& newDirection)
{
	if(glm::length(newDirection) > 0.0f) {
		direction = glm::normalize(newDirection);
	}
	else {
		direction = glm::vec3(0.0f, -1.0f, 0.0f);
	}
}

glm::vec3 LightComponent::GetDirection() const
{
	glm::vec3 dir = direction;
	if (owner) {
		if(Transform* transform = owner->GetComponent<Transform>())
		{
			glm::mat4 worldMatrix = transform->GetWorldMatrix();
			glm::vec4 transformedDir = worldMatrix * glm::vec4(dir, 0.0f); // w=0 for direction
			dir = glm::normalize(glm::vec3(transformedDir));
		}
		return dir;
	}
	if(glm::length(dir)<=0.0f) {
		return glm::vec3(0.0f, -1.0f, 0.0f);
	}
	return glm::normalize(dir);
}

glm::vec3 LightComponent::GetPosition() const {
	if (owner) {
		if (Transform* transform = owner->GetComponent<Transform>())
		{
			return transform->GetPosition();
		}
	}

	return glm::vec3(0.0f);
}

void LightComponent::SetIntensity(float newIntensity)
{
	intensity = newIntensity;
}



void LightComponent::SetAttenuation(const glm::vec3& newAttenuation)
{
	attenuation = newAttenuation;
}

void LightComponent::SetInnerCutoff(float newInnerCutoff)
{
	innerCutoff = newInnerCutoff;
}

void LightComponent::SetOuterCutoff(float newOuterCutoff)
{
	outerCutoff = newOuterCutoff;
}
