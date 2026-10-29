#pragma once

#include "Object.h"

enum class ComponentLifeState {
	Alive,
	PendingDestroy,
	Destroyed
};

class Entity;

class Component : public Object
{	
	DECLARE_ABSTRACT_DERIVED_CLASS(Component, Object)

public:
	virtual void Initialize();
	
	virtual void Update(float delta) {};
	virtual void Load(json::JSON& data) ;

	virtual const char* GetTypeName() const { return "Component"; }
	
	void MarkForDestroy()
	{
		lifeState = ComponentLifeState::PendingDestroy;
	}
	void SetOwner(Entity* entity)
	{
		owner = entity;
	}
	Entity* GetOwner() const { return owner; }

protected:
	std::string componentType = "Default Component Type";
	Entity* owner = nullptr;
	ComponentLifeState lifeState = ComponentLifeState::Alive;
	virtual void Destroy();
private:
	friend class Entity;
};

