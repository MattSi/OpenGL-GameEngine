#pragma once
#include "Object.h"
#include "Component.h"
#include "Transform.h"

class Scene;
enum class EntityLifeState {
	Alive,
	PendingDestroy,
	Destroyed
};

class Entity final: public Object
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Entity, Object)


public:
	void Load(json::JSON& data) override;
	void Initialize() override;
	void Destroy() override;
	void Update(float delta);


	Component* CreateComponent(const std::string& componentType);
	void RemoveComponent(Component* component);
	
	template<typename T>
	std::vector<T*> GetComponents() const {
		std::vector<T*> result;
		for (auto* c : components)
		{
			T* casted = dynamic_cast<T*>(c);
			if (casted)
			{
				result.push_back(casted);
			}
		}
		return result;
	}

	template<typename T>
	T* GetComponent() const {
		static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
		for (auto* c : components)
		{
			T* casted = dynamic_cast<T*>(c);
			if (casted)
			{
				return casted;
			}
		}
		return nullptr;
	}
	
	void AddComponent(Component* component);

	void SetPosition(const glm::vec3& pos) {
		if (transform) {
			transform->SetPosition(pos);
		}
	}
	glm::vec3 GetPosition() const {
		if (transform) {
			return transform->GetPosition();
		}
		return glm::vec3(0.0f);
	}

	void SetRotation(const glm::vec3& rot) {
		if (transform) {
			transform->SetRotation(rot);
		}
	}
	glm::vec3 GetRotation() const {
		if (transform) {
			return transform->GetRotation();
		}
		return glm::vec3(0.0f);
	}
	void SetScale(const glm::vec3& scale) {
		if (transform) {
			transform->SetScale(scale);
		}
	}
	glm::vec3 GetScale() const {
		if (transform) {
			return transform->GetScale();
		}
		return glm::vec3(1.0f);
	}
	Transform* GetTransform() const { return transform; }
	void MarkForDestroy() {
		if(lifeState == EntityLifeState::Alive) {
			lifeState = EntityLifeState::PendingDestroy;
		}
	}

	EntityLifeState GetLifeState() const { return lifeState; }
private:
	std::vector<Component*> components;
	EntityLifeState lifeState = EntityLifeState::Alive;
	friend class Scene;
protected:
	Transform* transform = nullptr;

};


