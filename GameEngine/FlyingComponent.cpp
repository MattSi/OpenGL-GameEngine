#include "EngineCore.h"
#include "FlyingComponent.h"
#include "Transform.h"
#include "Entity.h"

IMPLEMENT_DYNAMIC_CLASS(FlyingComponent)
void FlyingComponent::Update(float delta)
{
	if (lifeState != ComponentLifeState::Alive || !playing || !owner) {
		return;
	}

	Transform* t = owner->GetComponent<Transform>();
	if (!t) { return; }

	glm::vec3 pos = t->GetPosition();
	glm::vec3 direction = targetPosition - pos;
	float distanceToTarget = glm::length(direction);

	if (distanceToTarget < 0.1f) {
		playing = false;
		lifeState = ComponentLifeState::PendingDestroy;
		owner->MarkForDestroy();
		return;
	}

	direction = direction / distanceToTarget; // Normalize
	pos += direction * speed * delta;
	t->SetPosition(pos);
}
