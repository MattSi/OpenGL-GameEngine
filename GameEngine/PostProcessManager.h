#pragma once
#include "PostProcessPass.h"

class Engine;
class Texture;
class Framebuffer;


class PostProcessManager : public Singleton<PostProcessManager>
{
public:
	void Initialize(int screenWidth, int screenHeight);
	void Initialize();
	void Resize(int newWidth, int newHeight);
	void CleanUp() override;

	void Apply(Texture* sceneColorTex, Texture* sceneDepthTex, bool outToScreen, float nearPlane, float farPlane, float totalTime);
	void OnImGui();
	void AddPass(const std::shared_ptr<PostProcessPass>& pass) {
		passes.push_back(pass);
	}
private:
	Framebuffer* pingFBO = nullptr;
	Framebuffer* pongFBO = nullptr;
	Texture* pingColor = nullptr;
	Texture* pongColor = nullptr;

	int screenWidth = 0;
	int screenHeight = 0;

	std::vector<std::shared_ptr<PostProcessPass>> passes;

	friend class Singleton<PostProcessManager>;
	friend class Engine;
	PostProcessManager() = default;
	~PostProcessManager() = default;
	PostProcessManager(const PostProcessManager&) = delete;
	PostProcessManager& operator=(const PostProcessManager&) = delete;
};

