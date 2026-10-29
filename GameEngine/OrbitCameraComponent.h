#pragma once
#include "Component.h"
class OrbitCameraComponent :
    public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(OrbitCameraComponent, Component)

public:

	
	void SetTargetEntity(Entity* target) {
		targetEntity = target;
	}
	virtual void Update(float delta) override;
private:
	Entity* targetEntity = nullptr;
};

