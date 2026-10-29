#pragma once

#include "config.h"
class IRenderable;
class Engine;
struct SDL_Window;
class Camera;
class CameraComponent;
class Shader;
class LightComponent;
class Shader;
class Entity;
class Mesh;
class Texture;
class Framebuffer;

class RenderSystem : public Singleton<RenderSystem>
{
public:	

	void Initialize(const RenderConfig& cfg);
	void Initialize() {};
	void CleanUp() override;

	// register and unregister renderable objects
	void AddRenderable(IRenderable* renderable) {
		renderables.push_back(renderable);
	}
	void RemoveRenderable(IRenderable* renderable) {
		renderables.remove(renderable);
	}

	// Main Camera
	void SetMainCamera(CameraComponent* camera) {
		mainCameraComponent = camera;
	}
	CameraComponent* GetMainCamera() const noexcept { return mainCameraComponent; }

	SDL_Window* GetWindow() const noexcept { return window; }
	SDL_GLContext GetGLContext() const noexcept { return glContext; }

	
	//void SetIndent(int ind) { indent = ind; }
	void LoadScene();
	void Update(float delta);
	Shader* GetDefaultShader() const { return defaultShader; }
	// Shared shaders owned by RenderSystem. Components must not delete these.
	Shader* GetLightingShader() const { return lightingShader; }
	Shader* GetCubeShader() const { return cubeShader; }
	void FireFromPrototype(Entity* prototype, Entity* target);
	void FireFromPrototypeRandom(Entity* prototype);
	void FireFromPrototypeForward(Entity* prototype);
	Entity* CloneEntityShallow(Entity* original, const std::string& newName);
	void OnResize(int newWidth, int newHeight);
	void SetSkyboxTextures(const std::array<std::string, 6>& faces);

	int GetWidth() const noexcept { return width; }	
	int GetHeight() const noexcept { return height; }
	void FocusCameraOnEntity(Entity* entity);
	void DrawCrosshair();
private:
	
	void DrawDebugHUD();
	std::string name = "DefaultRenderer";
	int		width = 800;
	int		height = 600;
	bool fullScreen = false;

	std::list<IRenderable*> renderables;
	std::vector<LightComponent*> lightComponents;

	SDL_Window* window = nullptr;
	CameraComponent* mainCameraComponent = nullptr;

	SDL_GLContext glContext = nullptr;

	friend class Engine;
	friend class Singleton<RenderSystem>;

	int indent = 0;

	
	glm::mat4 viewMatrix{ 1.0f };
	glm::mat4 projectionMatrix{ 1.0f };

	int currentToolMode = 0;

	Shader* lightingShader = nullptr;
	Shader* colorByPositionShader = nullptr;
	Shader* defaultShader = nullptr;
	Shader* cubeShader = nullptr;

	Mesh* skyboxMesh = nullptr;
	GLuint skyboxCubemap = 0;
	Shader* skyboxShader = nullptr;
	bool skyboxReady = false;	
	// True when cubemap textures are missing; the skybox shader then draws a procedural space sky.
	bool skyboxProcedural = false;

	void RenderSkybox();

	Framebuffer* sceneFramebuffer = nullptr;

	GLuint quadVAO = 0;
	GLuint quadVBO = 0;

	Shader* grayscaleShader = nullptr;	
	Shader* invertShader = nullptr;
	Shader* kernelBlurShader = nullptr;	
	Shader* kernelSharpenShader = nullptr;
	Shader* edgeDetectShader = nullptr;
	Shader* fogShader = nullptr;
	Shader* dofShader = nullptr;
	Shader* physicalDoFShader = nullptr;
	Shader* depthBasedColorGradingShader = nullptr;
	Shader* waterSceneShader = nullptr;	

	int screenWidth = 0;
	int screenHeight = 0;

protected:
	RenderSystem() = default;
	~RenderSystem() = default;

	RenderSystem(const RenderSystem&) = delete;
	RenderSystem& operator=(const RenderSystem&) = delete;
	
	void DrawToolWindow();
	
	
	void Load(const json::JSON& data);

	// for internal use
	void InitOpenGLState();
	void RenderAll(float delta);
	
	
};

