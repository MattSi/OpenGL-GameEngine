#include "EngineCore.h"
#include "Scene.h"
#include "Entity.h"

void Scene::RemoveEntity(Entity* entity)
{
}

Entity* Scene::FindEntityById(int id)
{
	return nullptr;
}

Entity* Scene::FindEntityByName(const std::string& name)
{
	for (auto* entity : entities)
	{
		if (entity && entity->GetName() == name)
		{
			return entity;
		}
	}
	return nullptr;
}

Scene::Scene()
{
}

void Scene::AddEntity(Entity* entity)
{
	pendingEntities.push_back(entity);
}

void Scene::Initialize()
{
	DEBUG_LOG(indent, "Scene::Initialize");
	for (auto entity : entities) {
		entity->Initialize();
	}
}

void Scene::Update(float delta)
{
	if(!pendingEntities.empty()) {
		DEBUG_LOG(indent, "Scene::Update - Adding pending entities: " + std::to_string(pendingEntities.size()));
		for(auto entity : pendingEntities) {
			entities.push_back(entity);
			entity->Initialize();
		}
		pendingEntities.clear();
	}

	// DEBUG_LOG(indent, "Scene::Update");
	for (auto entity : entities) {
		if(entity->GetLifeState() == EntityLifeState::Alive) {
			entity->Update(delta);
		}
	}

	for(auto it= entities.begin(); it != entities.end(); ) {
		Entity* entity = *it;
		if(entity->GetLifeState() == EntityLifeState::PendingDestroy) {
			entity->Destroy();
			delete entity;
			it = entities.erase(it);
		} else {
			++it;
		}
	}
}

void Scene::Destroy()
{
	DEBUG_LOG(indent, "Scene::Destroy");
	for (auto entity : entities) {
		entity->Destroy();
	}
	entities.clear();
}

void Scene::Load(json::JSON data)
{
	DEBUG_LOG(indent, "Scene::Load");
	sceneName = data.at("name").ToString();
	id = data.at("id").ToInt();

	DEBUG_LOG(indent, "name: " + sceneName);
	DEBUG_LOG(indent, "id: " + std::to_string(id));


	json::JSON entitiesJson = data.at("Entities");
	for (auto& ent : entitiesJson.ArrayRange())
	{		
		Entity* newEntity = static_cast<Entity*>(CreateObject("Entity"));
		//newEntity->SetIndent(indent + 2);
		newEntity->Load(ent);		
		entities.push_back(newEntity);
	}
}	
