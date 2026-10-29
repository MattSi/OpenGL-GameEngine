#include "EngineCore.h"
#include "SceneManager.h"
#include "Scene.h"

template<> SceneManager* Singleton<SceneManager>::instance = nullptr;


void SceneManager::Initialize(const std::string& configPath)
{
	DEBUG_LOG(indent, "SceneManager::Initialize");
	LoadScene(configPath);
	//LoadScene("Scene02.json");
	ActivateSceneByIndex(0);
}

void SceneManager::CleanUp()
{
	DEBUG_LOG(indent, "SceneManager::CleanUp");
	activeScene = nullptr;
	activeSceneIndex = static_cast<size_t>(-1);
	for (auto scene : scenes) {
		scene->Destroy();
		delete scene;
	}
	scenes.clear();
}

void SceneManager::Update(float delta)
{
	if (!activeScene) return;
	activeScene->Update(delta);
}	

void SceneManager::LoadScene(const std::string& _filename)
{
	DEBUG_LOG(indent, "SceneManager::Load");
	std::ifstream file(_filename);
	if (!file.is_open()) {
		DEBUG_LOG(indent, "SceneManager::Load() - Failed to open file: " + _filename);
		return ;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();
	json::JSON root = json::JSON::Load(content);

	if (root.hasKey("SceneManager")) {
		json::JSON sceneManager = root.at("SceneManager");

		if (sceneManager.hasKey("scenes"))
		{
			json::JSON scenesJson = sceneManager.at("scenes");
			for (auto& sceneJson : scenesJson.ArrayRange())
			{
				Scene* newScene = new Scene();
				newScene->SetIndent(indent + 4);
				newScene->Load(sceneJson);
				newScene->Initialize();
				scenes.push_back(newScene);
			}
		}
	}

	if (root.hasKey("AssetManager")) {
		// TODO : Load assets
	}

	activeScene = scenes.size() > 0 ? scenes[0] : nullptr;
	return ;
}

void SceneManager::UnloadScene(const std::string& sceneName)
{
}

bool SceneManager::ActivateSceneByName(const std::string& sceneName)
{
	if (scenes.empty() || sceneName.empty()) return false;
	for (size_t i = 0; i < scenes.size(); ++i) {
		if (scenes[i]->sceneName == sceneName) {
			activeScene = scenes[i];
			activeSceneIndex = i;
			return true;
		}
	}
	return false;
}

bool SceneManager::ActivateSceneByIndex(size_t index)
{
	if (scenes.empty() || index >=scenes.size()) return false;
	activeScene = scenes[index];
	activeSceneIndex = index;
	return true;
}

bool SceneManager::NextScene()
{
	if (scenes.empty()) return false;
	if (activeScene == nullptr || activeSceneIndex >= scenes.size())
	{
		activeScene = scenes[0];
		activeSceneIndex = 0;
		return true;
	}
	activeSceneIndex = (activeSceneIndex + 1) % scenes.size();
	activeScene = scenes[activeSceneIndex];
	return true;
} 

bool SceneManager::PreviousScene()
{
	if (scenes.empty()) return false;
	if (activeScene == nullptr || activeSceneIndex >= scenes.size())
	{
		activeScene = scenes[0];
		activeSceneIndex = 0;
		return true;
	}
	activeSceneIndex = (activeSceneIndex +scenes.size() - 1) % scenes.size();
	activeScene = scenes[activeSceneIndex];
	return true;
}



