#include "EngineCore.h"
#include "CameraTweenScript.h"
#include "CameraComponent.h"	
#include "Entity.h"

IMPLEMENT_DYNAMIC_CLASS(CameraTweenScript)

void CameraTweenScript::Initialize()
{
	Component::Initialize();

	CameraComponent* cameraComp = owner->GetComponent<CameraComponent>();

	if (cameraComp) {
		startPosition = cameraComp->GetCamera().GetPosition();
		startYawPitch = { cameraComp->GetCamera().GetYaw(), cameraComp->GetCamera().GetPitch() };
	}
}

void CameraTweenScript::Update(float delta)
{
	if (lifeState != ComponentLifeState::Alive || !playing) return;

	elapsed += delta;
	float t = glm::clamp(elapsed / duration, 0.0f, 1.0f);
	t = t * t * (3.0f - 2.0f * t);

	// Interpolate position and rotation
	glm::vec3 newPosition = glm::mix(startPosition, targetPosition, t);
	glm::vec2 newYawPitch = glm::mix(startYawPitch, targetYawPitch, t);

	// Apply to camera
	CameraComponent* cameraComp = owner->GetComponent<CameraComponent>();
	if (cameraComp) {
		cameraComp->GetCamera().SetPosition(newPosition);
		cameraComp->GetCamera().SetYawPitch(newYawPitch.x, newYawPitch.y);
		
	}

	Transform* transform = owner->GetComponent<Transform>();
	if(transform) {
		transform->SetPosition(newPosition);
		transform->SetRotation(glm::vec3(newYawPitch.y, newYawPitch.x, 0.0f));
	}
	// Check if tween is complete
	if (t >= 1.0f) {
		playing = false;
		MarkForDestroy();
	}
}


