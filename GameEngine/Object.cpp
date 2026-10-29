#include "EngineCore.h"
#include "Object.h"


IMPLEMENT_ABSTRACT_CLASS(Object)

void Object::Initialize()
{
	/*UUID _uid;
	CreateUUID(&_uid);

	uid = GUIDToSTRCODE(_uid);
	guid = GUIDTostring(_uid);

	initialized = true;*/

	UUID _uid;
	CreateUUID(&_uid);

	uid = GUIDToSTRCODE(_uid);
	guid = GUIDTostring(_uid);

	initialized = true;
}
void Object::Destroy()
{
}

void Object::Load(json::JSON& data)
{
	name = data.at("name").ToString();
	id = data.at("id").ToInt();
}