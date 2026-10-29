#include "EngineCore.h"
#include "Engine.h"
#include "EngineTime.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "RenderSystem.h"
#include "ConfigLoader.h"
#include "InputManager.h"
#include "ToolWindow.h"


template<> Engine* Singleton<Engine>::instance = nullptr;
extern void Engine_Register();

void Engine::GameLoop()
{

	DEBUG_LOG(0, "Engine::GameLoop");
	Time& engineTime = Time::GetInstance();
	InputManager& inputManager = InputManager::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();
	RenderSystem& renderSystem = RenderSystem::GetInstance();
	renderSystem.LoadScene();
	   
	GameEngine::ToolWindow^ toolWindow = gcnew GameEngine::ToolWindow();
	toolWindow->Show();
	while (!ShouldQuit()) {
		engineTime.Update();
		inputManager.Update();
		
		float dt = engineTime.DeltaTime();
		sceneManager.Update(dt);
		renderSystem.Update(dt);			
	}
}
  

bool Engine::ShouldQuit() const
{
	auto& im = InputManager::GetInstance();
	return im.IsQuitRequested();
}
 
void Engine::Initialize()
{
	DEBUG_LOG(0, "Engine::Initialize");
	Engine_Register();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		M_ASSERT(false, ("SDL_Init Error: " + std::string(SDL_GetError())).c_str());
	}

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	std::string settingsFile = "GameSettings.json";
	AppConfig appConfig = ConfigLoader::LoadAppConfig(settingsFile);


	auto& rs = RenderSystem::GetInstance(appConfig.renderConfig);

	auto& sm = SceneManager::GetInstance(appConfig.sceneConfig.defaultScene);

	auto& am = AssetManager::GetInstance();

	auto& im = InputManager::GetInstance();

	auto& engineTime = Time::GetInstance();


}

void Engine::CleanUp()
{
	DEBUG_LOG(0, "Engine::CleanUp");
	SceneManager::GetInstance().Destroy();
	AssetManager::GetInstance().Destroy();
	RenderSystem::GetInstance().Destroy();
	InputManager::GetInstance().Destroy();
	Time::GetInstance().Destroy();
	SDL_Quit();
}
