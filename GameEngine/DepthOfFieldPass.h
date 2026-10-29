#pragma once
#include "PostProcessPass.h"

class Shader;
class Mesh;

class DepthOfFieldPass :
    public PostProcessPass
{
public:
    DepthOfFieldPass(Shader* shader, Mesh* screenQuad)
        : PostProcessPass("DepthOfFieldPass")
        , shader_(shader)
        , screenQuad_(screenQuad)
    {
    }

    void Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx) override;
    void OnImGui() override;

private:
    Shader* shader_ = nullptr;
    Mesh* screenQuad_ = nullptr;

    // 0~1 空间里的 “对焦深度”
    float focusDepth_ = 0.3f;
    float focusRange_ = 0.1f;    
    float maxBlur_ = 8.0f;
};

