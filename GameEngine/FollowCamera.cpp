#include "EngineCore.h"
#include "FollowCamera.h"
#include "Entity.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Engine.h"
#include "InputManager.h"

IMPLEMENT_DYNAMIC_CLASS(FollowCamera)

void FollowCamera::Load(json::JSON& data)
{
	Component::Load(data);
	if (data.hasKey("targetEntityName"))
	{
		targetEntityName = data.at("targetEntityName").ToString();
	}
	if (data.hasKey("offset"))
	{
		auto o = data.at("offset");
		offset = glm::vec3(
			(float)o.at("x").ToFloat(),
			(float)o.at("y").ToFloat(),
			(float)o.at("z").ToFloat()
		);
	}
	if (data.hasKey("lerpSpeed"))
	{
		lerpSpeed = (float)data.at("lerpSpeed").ToFloat();
	}

	if (data.hasKey("orbitSpeed")) {
		orbitSpeed = (float)data.at("orbitSpeed").ToFloat();
	}
}

void FollowCamera::Initialize()
{
	Component::Initialize();
	
	/*if(targetEntityName.empty() || !owner) {
		return;
	}
	SceneManager& sceneManager = SceneManager::GetInstance();
	targetEntity = sceneManager.GetActiveScene()->FindEntityByName(targetEntityName);*/
}


void FollowCamera::Update(float delta)
{
	if(targetEntity == nullptr && !targetEntityName.empty()) {
		SceneManager& sceneManager = SceneManager::GetInstance();
		targetEntity = sceneManager.GetActiveScene()->FindEntityByName(targetEntityName);
	}

	if (!owner || !targetEntity) return;


	Transform* camTransform = owner->GetTransform();
	Transform* targetTransform = targetEntity->GetTransform();

	if(camTransform == nullptr || targetTransform == nullptr) {
		return;
	}

	glm::vec3 desiredPos = targetTransform->GetPosition() + offset;

	glm::vec3 currentPos = camTransform->GetPosition();
	float t = 1.0f - std::exp(-lerpSpeed * delta);
	glm::vec3 newPos = glm::mix(currentPos, desiredPos, t);
	camTransform->SetPosition(newPos);





	
	glm::vec3 myPos = camTransform->GetPosition();
	glm::vec3 targetPos = targetTransform->GetPosition();
	glm::vec3 direction = targetPos - myPos;
	float distSq = glm::dot(direction, direction);

	if (distSq > 0.0001f)
	{
		direction = glm::normalize(direction);

		float pitch = glm::degrees(asinf(direction.y));
		// Camera forward is (cos(yaw), sin(pitch), sin(yaw)), so yaw = atan2(z, x).
		// The old -atan2(x, z) was off by 90 degrees and needed a manual slider offset.
		float yaw = glm::degrees(atan2f(direction.z, direction.x));
		float offsetY = Engine::GetInstance().toolWindowData.rotationOffsetY;
		float offsetX = Engine::GetInstance().toolWindowData.rotationOffsetX;
		float offsetZ = Engine::GetInstance().toolWindowData.rotationOffsetZ;

		yaw += offsetY; // Because -Z is forward
		pitch += offsetX;

		glm::vec3 currentRotation = camTransform->GetRotation();
		currentRotation.x = pitch;
		currentRotation.y = yaw;
		currentRotation.z = 0.0f;

		camTransform->SetRotation(currentRotation);
	}
	
}
