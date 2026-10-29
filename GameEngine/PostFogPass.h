#pragma once
#include "PostProcessPass.h"

class Shader;
class Mesh;

class PostFogPass :
    public PostProcessPass
{
public:
    PostFogPass(Shader* shader, Mesh* screenQuad)
        : PostProcessPass("PostFogPass")
        , shader_(shader)
        , screenQuad_(screenQuad) {
	}

	void Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx) override;
	void OnImGui() override;

private:
    Shader* shader_ = nullptr;
    Mesh* screenQuad_ = nullptr;
	
	glm::vec3 fogColor_ = glm::vec3(0.6f, 0.7f, 0.8f);
    float fogStart_ = 0.2f;
	float fogEnd_ = 0.8f;
};

