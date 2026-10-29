#pragma once
#include "Component.h"
#include "Camera.h"

class CameraComponent : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(CameraComponent, Component);

public:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void Update(float delta) override;
    virtual void Load(json::JSON& data) override;


    virtual const char* GetTypeName() const override { return "CameraComponent"; }
	Camera& GetCamera() { return camera; }

    void SetTargetEntity(Entity* target) {
        targetEntity = target;
	}
    Entity* GetTargetEntity() const {
        return targetEntity;
	}

private:
	Camera camera;
    float fovY = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    bool setAsMainCamera = true;
	Entity* targetEntity = nullptr;
};

