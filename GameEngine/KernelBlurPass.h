#pragma once
#include "PostProcessPass.h"

class Shader;
class Mesh;
class KernelBlurPass :
    public PostProcessPass
{
public:
    KernelBlurPass(Shader* shader, Mesh* screenQuad)
        : PostProcessPass("KernelBlurPass")
        , shader_(shader)
        , screenQuad_(screenQuad) {
    }
    void Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx) override;
	void OnImGui() override;

private:
    Shader* shader_ = nullptr;
    Mesh* screenQuad_ = nullptr;
    

	float strength_ = 1.0f;
};

