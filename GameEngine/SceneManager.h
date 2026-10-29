#pragma once

class Scene;
class Engine;

class SceneManager : public Singleton<SceneManager>
{
public:
	void SetIndent(int ind) { indent = ind; }
	void LoadScene(const std::string& _filename);
	void UnloadScene(const std::string& sceneName);
	Scene* GetActiveScene() const { return activeScene; }
	size_t GetActiveSceneIndex() const { return activeSceneIndex; }
	size_t GetSceneCount() const { return scenes.size(); }

	bool ActivateSceneByName(const std::string& sceneName);	
	bool ActivateSceneByIndex(size_t index);

	bool NextScene();
	bool PreviousScene();


private:
	std::vector<class Scene*> scenes;

	Scene* activeScene = nullptr;
	size_t activeSceneIndex = static_cast<size_t>(-1);
	int indent=0;

	friend class Engine;
	friend class Singleton<SceneManager>;

	void Initialize(const std::string& configPath) ;
	void Initialize() {}
	void CleanUp() override;

	void Update(float delta);

	~SceneManager() = default;
	SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
};

