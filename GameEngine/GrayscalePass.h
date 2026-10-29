#pragma once
#include "PostProcessPass.h"

class Shader;
class Mesh;

class GrayscalePass : public PostProcessPass
{
public:
	GrayscalePass(Shader* shader, Mesh* screenQuad)
		: PostProcessPass("GrayscalePass")
		, shader_(shader)
		, screenQuad_(screenQuad) {
	}

	void Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx) override;

	void OnImGui() override;
private:
	Shader* shader_ = nullptr;
	Mesh* screenQuad_ = nullptr;

	bool precise = false;
};

