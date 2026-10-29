#pragma once
#include "Component.h"


class Transform;

class TargetMoveController :
    public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(TargetMoveController, Component);

public:

	virtual void Initialize() override;
	virtual void Update(float delta) override;
	virtual void Load(json::JSON& data) override;
	virtual void Destroy() override {};

private:
	float moveSpeed = 5.0f;
	float shiftMultiplier = 2.0f;
	bool useCameraSpace = true;

};

