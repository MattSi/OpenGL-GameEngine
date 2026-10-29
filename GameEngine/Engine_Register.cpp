#include "EngineCore.h"

#include "Object.h"	
#include "Entity.h"
#include "Component.h"
#include "LightComponent.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "CameraComponent.h"
#include "CameraTweenScript.h"
#include "MoveLightComponent.h"
#include "LightBallController.h"
#include "AirCraftController.h"
#include "OrbitRotationComponent.h"
#include "InstancedModelRenderer.h"
void Engine_Register()
{
	REGISTER_TYPECLASS(Object);
	REGISTER_TYPECLASS(Entity);
	REGISTER_TYPECLASS(Component);
	REGISTER_TYPECLASS(LightComponent);
	REGISTER_TYPECLASS(MeshRenderer);
	REGISTER_TYPECLASS(ModelRenderer);
	REGISTER_TYPECLASS(CameraComponent);
	REGISTER_TYPECLASS(CameraTweenScript);
	REGISTER_TYPECLASS(MoveLightComponent);
	REGISTER_TYPECLASS(LightBallController);
	REGISTER_TYPECLASS(AirCraftController);
	REGISTER_TYPECLASS(OrbitRotationComponent);
	REGISTER_TYPECLASS(InstancedModelRenderer);
	
	
}
