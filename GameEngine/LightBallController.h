#pragma once
#include "Component.h"
class LightBallController :
    public Component
{

	DECLARE_DYNAMIC_DERIVED_CLASS(LightBallController, Component)

public:
	bool isActive = false;
	bool isTranslate = true;
	bool isRotate = false;
	bool isScale = false;

	virtual void Update(float delta) override;
};

