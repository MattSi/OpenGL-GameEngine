#include "EngineCore.h"
#include "AirCraftController.h"
#include "Transform.h"
#include "Entity.h"
#include "InputManager.h"
#include "RenderSystem.h"
#include "Engine.h"

IMPLEMENT_DYNAMIC_CLASS(AirCraftController)


void AirCraftController::Update(float delta)
{
	if (!owner) {
		return;
	}

	auto& data = Engine::GetInstance().toolWindowData;
	if (!data.IsTransform) {
		return;
	}
	bool isModify = false;

	InputManager& inputManager = InputManager::GetInstance();
	RenderSystem& renderSystem = RenderSystem::GetInstance();
	glm::vec3 pos = owner->GetPosition();
	int mx, my;
	inputManager.GetMousePosition(mx, my);
	if (inputManager.IsMouseButtonDown(SDL_BUTTON_LEFT)) {

		float cx = renderSystem.GetWidth() * 0.5f;
		float cy = renderSystem.GetHeight() * 0.5f;

		float dx = (float)(mx - cx);
		float dy = (float)(cy - my);

		float nx = dx / cx;
		float ny = dy / cy;
		nx = glm::clamp(nx, -1.0f, 1.0f);
		ny = glm::clamp(ny, -1.0f, 1.0f);


		float maxSpeed = 5.0f;
		if (data.Translate) {
			
			pos.x += nx * maxSpeed * delta;
			pos.y += ny * maxSpeed * delta;
			isModify = true;
		}
		
		if(data.Rotate) {
			glm::vec3 rot = owner->GetRotation();
			rot.y += nx * 90.0f * delta;
			rot.x += ny * 90.0f * delta;
			owner->SetRotation(rot);
			isModify = true;
		}

		if(data.Scale) {
			glm::vec3 scale = owner->GetScale();
			scale.x += nx * 0.1f * delta;
			scale.y += ny * 0.1f * delta;
			owner->SetScale(scale);
			isModify = true;
		}
	}


	if (inputManager.IsMouseButtonDown(SDL_BUTTON_MIDDLE)) {
		pos.z += 2.0f * delta;
		isModify = true;
	}

	if (isModify)
	{
		owner->SetPosition(pos);
	}
}
