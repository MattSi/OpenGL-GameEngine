#include "EngineCore.h"
#include "Component.h"

IMPLEMENT_ABSTRACT_CLASS(Component)

void Component::Initialize()
{
	Object::Initialize();
}

void Component::Destroy()
{
	Object::Destroy();
	owner = nullptr;
}
  
void Component::Load(json::JSON& data)
{
	Object::Load(data);
	componentType = data.at("componentType").ToString();
	DEBUG_LOG(indent, "Component::Load");	
}

