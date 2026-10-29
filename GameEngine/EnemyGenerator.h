#pragma once
#include "Component.h"

class Entity;
class EnemyGenerator :
    public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(EnemyGenerator, Component);
public:

    virtual void Initialize() override;
    virtual void Update(float delta) override;
	virtual void Load(json::JSON& data) override;

    float spawnInterval = 1.0f; // seconds
    float timeSinceLastSpawn = 0.0f;
	std::string enemyName = "";
private:
	void SpawnEnemy();
	Entity* entityToSpawn = nullptr;
};

