#pragma once
#include "PostProcessPass.h"

class Shader;
class Mesh;
class WaterScenePass :
    public PostProcessPass
{
public:
    WaterScenePass(Shader* shader, Mesh* screenQuad)
        : PostProcessPass("WaterScenePass")
        , shader_(shader)
        , screenQuad_(screenQuad) {
		SetEnabled(true);
    }

    void Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx) override;

    void OnImGui() override;

private:
    Shader* shader_ = nullptr;
    Mesh* screenQuad_ = nullptr;

    float frequency_ = 0.0f;
	float amplitude_ = 0.0f;
    
};

