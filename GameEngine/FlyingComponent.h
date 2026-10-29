#pragma once
#include "Component.h"
class FlyingComponent : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(FlyingComponent, Component)

public:
	glm::vec3 targetPosition{ 0.0f };
	float speed = 4.0f; // units per second
	bool playing = false;

	void Start(const glm::vec3& targetPos, float s) {
		targetPosition = targetPos;
		speed = s;
		playing = true;
	}

	virtual void Update(float delta) override;
};

