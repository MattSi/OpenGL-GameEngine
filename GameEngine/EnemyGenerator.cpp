#include "EngineCore.h"
#include "EnemyGenerator.h"
#include "SceneManager.h"
#include "RenderSystem.h"
#include "Entity.h"
#include "Transform.h"
#include "Scene.h"

IMPLEMENT_DYNAMIC_CLASS(EnemyGenerator)

void EnemyGenerator::Initialize()
{
}

void EnemyGenerator::Update(float delta)
{
	timeSinceLastSpawn += delta;
	if (timeSinceLastSpawn >= spawnInterval)
	{
		SpawnEnemy();
		timeSinceLastSpawn = 0.0f;
	}
}

void EnemyGenerator::Load(json::JSON& data)
{
	Component::Load(data);
	if (data.hasKey("EntityName"))
	{
		enemyName = data.at("EntityName").ToString();
	}
}

void EnemyGenerator::SpawnEnemy()
{
	Scene* scene = SceneManager::GetInstance().GetActiveScene();
	if (!scene) return;
	Entity* prototype = scene->FindEntityByName("Spinning Cube3");
	Entity* target = scene->FindEntityByName("E45 Aircraft Model");	
	RenderSystem& renderSystem = RenderSystem::GetInstance();
	renderSystem.FireFromPrototype(prototype, target);
	//FireFromPrototype
	DEBUG_LOG(2, "Spawning enemy: " + enemyName);
}
