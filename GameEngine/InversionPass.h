#pragma once
#include "PostProcessPass.h"

class Shader;
class Mesh;

class InversionPass : public PostProcessPass
{
public:
    InversionPass(Shader* shader, Mesh* screenQuad)
        : PostProcessPass("InversionPass")
        , shader_(shader)
        , screenQuad_(screenQuad) {
    }

    void Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx) override;

    void OnImGui() override;

private:
    Shader* shader_ = nullptr;
    Mesh* screenQuad_ = nullptr;
};

