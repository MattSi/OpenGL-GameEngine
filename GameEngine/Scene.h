#pragma once

class Entity;
class SceneManager;	
class IRenderable;

namespace json {
	class JSON;
}

class Scene
{
public:
	void AddEntity(Entity* entity);
	void RemoveEntity(Entity* entity);
	Entity* FindEntityById(int id);
	Entity* FindEntityByName(const std::string& name);
	void SetIndent(int ind) { indent = ind; }
	const std::vector<Entity*>& GetEntities() const noexcept {
		return entities;
	}

 private:
	 std::string sceneName = "Default Scene";
	 int id = -1;
	 std::vector<Entity*> entities;
	 std::vector<Entity*> pendingEntities;
	 int indent = -1;
	 friend class SceneManager;	 
protected:
	Scene();
	virtual ~Scene() = default;
	void Initialize();
	void Update(float delta);
	void Destroy();
	void Load(json::JSON data);
};

