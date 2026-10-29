#include "EngineCore.h"
#include "OrbitCameraComponent.h"
#include "Transform.h"
#include "Entity.h"

IMPLEMENT_DYNAMIC_CLASS(OrbitCameraComponent)
void OrbitCameraComponent::Update(float delta)
{
	if(!targetEntity || !owner) {
		return;
	}

	Transform* camTf = owner->GetTransform();
	Transform* targetTf = targetEntity->GetTransform();

	if(!camTf || !targetTf) {
		return;
	}

	const glm::vec3 tpos = targetTf->GetPosition();
	glm::vec3 camPos = tpos + 20.f;
	//camTf->SetPosition(camPos);
	camTf->LookAt(tpos);
}
