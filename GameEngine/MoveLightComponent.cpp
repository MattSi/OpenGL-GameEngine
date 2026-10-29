#include "EngineCore.h"
#include "MoveLightComponent.h"
#include "Entity.h"
#include "Engine.h"
#include "ModelRenderer.h"


IMPLEMENT_DYNAMIC_CLASS(MoveLightComponent)
void MoveLightComponent::Update(float delta)
{
	if (!owner) {
		return;
	}

	auto& data = Engine::GetInstance().toolWindowData;
	if (!data.IsMoveLight) {
		return;
	}

	// Ship Rotation Rate
	glm::vec3 currentRotation = owner->GetRotation();
	currentRotation.x += data.ShipRotationRate * 50 * delta;
	owner->SetRotation(currentRotation);

	// Specular Strength
	float specStrength = data.SpecularStrength;

	ModelRenderer* modelRenderer = owner->GetComponent<ModelRenderer>();
	if (modelRenderer) {
		modelRenderer->overrideSpecularStrength = specStrength * 0.005f;
		modelRenderer->hasSpecularStrengthOverride = true;
	}

	// Specular Color
	glm::vec3 specColor = data.SpecularColor;
	if (modelRenderer) {
		modelRenderer->overrideSpecularColor = specColor;
		modelRenderer->hasSpecularColorOverride = true;
	}
}
