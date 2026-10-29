#include "EngineCore.h"
#include "LightBallController.h"

#include "Transform.h"
#include "Entity.h"
#include "InputManager.h"
#include "RenderSystem.h"	
#include "Engine.h"

IMPLEMENT_DYNAMIC_CLASS(LightBallController)
void LightBallController::Update(float delta)
{
	if (!owner) {
		return;
	}

	auto& data = Engine::GetInstance().toolWindowData;
	if (!data.IsMoveLight) {
		return;
	}
	bool isModify = false;

	InputManager& inputManager = InputManager::GetInstance();
	RenderSystem& renderSystem = RenderSystem::GetInstance();
	glm::vec3 pos = owner->GetPosition();
	int mx, my;
	inputManager.GetMousePosition(mx, my);
	float cx = renderSystem.GetWidth() * 0.5f;
	float cy = renderSystem.GetHeight() * 0.5f;

	float dx = (float)(mx - cx);
	float dy = (float)(cy - my);

	float nx = dx / cx;
	float ny = dy / cy;
	nx = glm::clamp(nx, -1.0f, 1.0f);
	ny = glm::clamp(ny, -1.0f, 1.0f);


	float maxSpeed = 5.0f;
	if (inputManager.IsMouseButtonDown(SDL_BUTTON_LEFT)) {
	
		
		pos.x += nx * maxSpeed * delta;
		pos.y += ny * maxSpeed * delta;
		isModify = true;
	}

	if (inputManager.IsMouseButtonDown(SDL_BUTTON_MIDDLE)) {
		pos.z += ny * maxSpeed * delta;
		isModify = true;
	}

	
	if (isModify)
	{
		owner->SetPosition(pos);
	}
	
}
