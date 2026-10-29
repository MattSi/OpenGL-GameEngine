#include "EngineCore.h"
#include "Entity.h"
#include "Component.h"	
#include "CameraComponent.h"
#include "LightComponent.h"	
#include "MeshRenderer.h"
#include "ModelRenderer.h"
 
IMPLEMENT_DYNAMIC_CLASS(Entity)

void Entity::Initialize()
{
	Object::Initialize();
	DEBUG_LOG(indent, "Entity::Initialize");

	for (auto comp : components) {
		comp->Initialize();
	}
}

void Entity::Destroy()
{
	Object::Destroy();
	DEBUG_LOG(indent + 2, "Entity::Destroy");
	for (auto comp : components) {
		comp->Destroy();
		delete comp;
	}
	components.clear();
	transform = nullptr;
}
 
void Entity::Load(json::JSON& data)
{
	Object::Load(data);;
	DEBUG_LOG(indent, "Entity::Load");

	 
	json::JSON componentsJson = data.at("Components");
	for (auto& comp : componentsJson.ArrayRange()) {

		std::string compType = comp.at("componentType").ToString();
		Component* component = static_cast<Component*>(CreateObject(compType.c_str()));
		component->Load(comp);

		AddComponent(component);
	}
} 
 
void Entity::RemoveComponent(Component* component)
{
	if (!component) return;
	component->MarkForDestroy();
}

void Entity::AddComponent(Component* component)
{
	if (!component) return;
	component->SetOwner(this);
	components.push_back(component);
	if (component->GetTypeName() == std::string("Transform"))
	{
		if (!transform) {
			transform = static_cast<Transform*>(component);
		}
	}
}

void Entity::Update(float delta)
{
	
	for (auto comp : components) {
		comp->Update(delta);
	}

	// Deal with removals
	for(auto it = components.begin(); it != components.end(); )
	{
		Component* comp = *it;
		if (comp->lifeState == ComponentLifeState::PendingDestroy)
		{
			if (comp == transform) {
				++it;
				continue;
			}
			comp->Destroy();
			delete comp;
			it = components.erase(it);
		}
		else
		{
			++it;
		}
	}
}