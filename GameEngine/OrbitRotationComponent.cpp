#include "EngineCore.h"
#include "OrbitRotationComponent.h"
#include "Transform.h"
#include "Entity.h"
#include "Engine.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "Scene.h"

IMPLEMENT_DYNAMIC_CLASS(OrbitRotationComponent)
void OrbitRotationComponent::Update(float delta)
{

	if (!targetEntity || !owner) {
		return;
	}

	Transform* camTf = owner->GetTransform();
	Transform* targetTf = targetEntity->GetTransform();

	if (!camTf || !targetTf) {
		return;
	}


	angle += angularSpeed * delta;

	const glm::vec3& tpos = targetTf->GetPosition();

	glm::vec3  pos;
	pos.x = tpos.x + cos(angle) * radius;
	pos.z = tpos.z + sin(angle) * radius;
	pos.y = tpos.y;

	glm::vec3 currentRotation = owner->GetRotation();
	auto& data = Engine::GetInstance().toolWindowData;
	currentRotation.x += 50 * delta;
	
	 
	camTf->SetRotation(currentRotation);
	camTf->SetPosition(pos);

	timeInterval += delta;
	if(timeInterval >= 1.0f)
	{
		/*timeInterval = 0.0f;
		SceneManager& sceneManager = SceneManager::GetInstance();
		Entity* prototypeEntity = sceneManager.GetActiveScene()->FindEntityByName("BigFish");
		RenderSystem& renderSystem = RenderSystem::GetInstance();
		renderSystem.FireFromPrototypeRandom(prototypeEntity);*/
	}
	
}
