#pragma once
#include "PostProcessPass.h"

class Shader;
class Mesh;
class DepthBasedColorGradingPass :
    public PostProcessPass
{
public:
    DepthBasedColorGradingPass(Shader* shader, Mesh* screenQuad)
        : PostProcessPass("DepthColorGradingPass")
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
   
	glm::vec3 nearTint_ = glm::vec3(1.05f, 1.0f, 0.95f);
	glm::vec3 farTint_ = glm::vec3(0.95f, 1.0f, 1.05f);

    float depthStart_ = 0.1f;
	float depthEnd_ = 1.0f;

	float strength_ = 1.0f;

	float cachedNear_ = 0.1f;
	float cachedFar_ = 100.0f;
};

