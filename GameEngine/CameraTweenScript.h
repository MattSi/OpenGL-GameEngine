#pragma once
#include "Component.h"
class CameraTweenScript : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(CameraTweenScript, Component);

public:

	glm::vec3 startPosition;
	glm::vec3 targetPosition;
	glm::vec2 startYawPitch;
	glm::vec2 targetYawPitch;
	float duration = 1.0f; // Duration of the tween in seconds
	bool playing = false;
	float elapsed = 0.0f;

	void StartTween() {
		playing = true;
		elapsed = 0.0f;
		lifeState = ComponentLifeState::Alive;
	}

	virtual void Initialize() override;
	virtual void Update(float delta) override;
};

