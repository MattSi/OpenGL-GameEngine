#include "EngineCore.h"
#include "Asset.h"

void Asset::Initialize()
{
	Object::Initialize();
}
   
void Asset::Destroy()
{
	Object::Destroy();
}

void Asset::Load(json::JSON& data)
{
	Object::Load(data);
}
  