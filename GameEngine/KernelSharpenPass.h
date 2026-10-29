#pragma once
#include "PostProcessPass.h"
class Shader;
class Mesh;

class KernelSharpenPass :
    public PostProcessPass
{
public:
    KernelSharpenPass(Shader* shader, Mesh* screenQuad)
        : PostProcessPass("KernelSharpenPass")
        , shader_(shader)
        , screenQuad_(screenQuad)
    {
    }

    void Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx) override;
    void OnImGui() override;

private:
    Shader* shader_ = nullptr;
    Mesh* screenQuad_ = nullptr;

	// 0.0 - no effect, 1.0 - default strength , > 1 - stronger effect
    float strength_ = 1.0f;
};

