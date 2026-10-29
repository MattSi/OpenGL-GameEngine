#pragma once
#include "Component.h"
class MoveLightComponent :
    public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(MoveLightComponent, Component)

public:

    virtual void Update(float delta) override;
};

