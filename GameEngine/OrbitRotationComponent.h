#pragma once
#include "Component.h"
class OrbitRotationComponent :
    public Component
{

	DECLARE_DYNAMIC_DERIVED_CLASS(OrbitRotationComponent, Component)

public:

	float radius = 25.0f;
	float height = 10.0f;
	float angularSpeed = 0.5f; // Radians per second
	float angle = 0.0f;

	float timeInterval = 0.0f;
	void SetTargetEntity(Entity* target) {
		targetEntity = target;
	}
	virtual void Update(float delta) override;
private:
	Entity* targetEntity = nullptr;
};

