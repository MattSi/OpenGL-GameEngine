#pragma once
#include "PostProcessPass.h"

class Shader;
class Mesh;

class PhysicalDoFPass :
    public PostProcessPass
{
public:
    PhysicalDoFPass(Shader* shader, Mesh* screenQuad)
        : PostProcessPass("PhysicalDoFPass")
        , shader_(shader)
        , screenQuad_(screenQuad)
    {
    }

    void Execute(Texture* inputColor,
        Framebuffer* outFBO,
        const PostProcessContext& ctx) override;

    void OnImGui() override;

private:
    Shader* shader_ = nullptr;
	Mesh* screenQuad_ = nullptr;

    float focalLengthMm_ = 50.0f;
	float fStop_ = 2.8f;
    float focusDistance_ = 10.0f;

	float maxBlurRadiusPx_ = 20.0f;

	float cachedNearPlane_ = 0.1f;
	float cachedFarPlane_ = 100.0f;
};

