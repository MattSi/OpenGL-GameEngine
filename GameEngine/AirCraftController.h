#pragma once
#include "Component.h"
class AirCraftController :
    public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(AirCraftController, Component)

public:
	
	bool isTranslate = true;
	bool isRotate = false;
	bool isScale = false;

	virtual void Update(float delta) override;
};

