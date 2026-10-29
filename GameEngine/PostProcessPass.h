#pragma once

class Texture;
class Camera;
class Framebuffer;

struct PostProcessContext {
	int screenWidth = 0;
	int screenHeight = 0;
	Texture* depthTexture = nullptr;
	Camera* camera = nullptr;
	float time = 0.0f;

	float nearPlane = 0.1f;
	float farPlane = 100.0f;
};

class PostProcessPass
{
public:
	PostProcessPass(const std::string& name) : passName(name),enabled(false) {}
	virtual ~PostProcessPass() = default;

	const std::string& GetName() const { return passName; }

	bool IsEnabled() const { return enabled; }	
	void SetEnabled(bool en) { enabled = en; }

	virtual void Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& context) = 0;
	virtual void OnImGui() {}

protected:
	std::string passName = "UnnamedPass";
	bool enabled = false;
};

