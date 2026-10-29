#include "EngineCore.h"
#include "Transform.h"


IMPLEMENT_DYNAMIC_CLASS(Transform);
void Transform::Initialize()
{
	Component::Initialize();
	std::cout << "Transform::Initialize()\n";
}
void Transform::Destroy()
{
	Component::Destroy();
	std::cout << "Transform::Destroy()\n";
}

void Transform::Load(json::JSON& data)
{
	Component::Load(data);
	std::cout << "Transform::Load()\n";
	if (data.hasKey("position"))
	{
		auto posJson = data.at("position");
		position.x = static_cast<float>(posJson.at("x").ToFloat());
		position.y = static_cast<float>(posJson.at("y").ToFloat());
		position.z = static_cast<float>(posJson.at("z").ToFloat());
		DEBUG_LOG(indent, "Transform::Load Position: "
			+ std::to_string(posJson.at("x").ToFloat()) + ", "
			+ std::to_string(posJson.at("y").ToFloat()) + ", "
			+ std::to_string(posJson.at("z").ToFloat()));

		initialPosition = position;
	} 
	if (data.hasKey("rotation"))
	{
		auto rotJson = data.at("rotation");
		rotation.x = static_cast<float>(rotJson.at("x").ToFloat());
		rotation.y = static_cast<float>(rotJson.at("y").ToFloat());
		rotation.z = static_cast<float>(rotJson.at("z").ToFloat());

		DEBUG_LOG(indent, "Transform::Load Rotation: "
			+ std::to_string(rotJson.at("x").ToFloat()) + ", "
			+ std::to_string(rotJson.at("y").ToFloat()) + ", "
			+ std::to_string(rotJson.at("z").ToFloat()));
		DEBUG_LOG(indent, "Transform::Load Rotation: " 
			+ std::to_string(rotation.x) + ", "
			+ std::to_string(rotation.y) + ", "
			+ std::to_string(rotation.z));
		initialRotation = rotation;
	}
	if (data.hasKey("scale"))
	{
		auto scaleJson = data.at("scale");
		scale.x = static_cast<float>(scaleJson.at("x").ToFloat());
		scale.y = static_cast<float>(scaleJson.at("y").ToFloat());
		scale.z = static_cast<float>(scaleJson.at("z").ToFloat());
		initialScale = scale;
	}
}
