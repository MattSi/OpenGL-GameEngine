#pragma once
#include "Component.h"


class Entity;
class Transform;

class FollowCamera : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(FollowCamera, Component)

public:
    virtual void Update(float delta) override;
	virtual void Load(json::JSON& data) override;
	virtual void Initialize() override;

private:
	std::string targetEntityName;
	glm::vec3 offset = glm::vec3(0.0f, 5.0f, 10.0f);
	float lerpSpeed = 5.0f;	
	float orbitSpeed = 90.0f;          
	float yawOffset = 0.0f;

	Entity* targetEntity = nullptr;
};

