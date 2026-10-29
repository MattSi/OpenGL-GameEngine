#include "EngineCore.h"
#include "TargetMoveController.h"
#include "Transform.h"
#include "Entity.h"
#include "InputManager.h"
#include "RenderSystem.h"
#include "CameraComponent.h"
#include "Engine.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "Scene.h"


IMPLEMENT_DYNAMIC_CLASS(TargetMoveController);
void TargetMoveController::Initialize()
{
}


void TargetMoveController::Load(json::JSON& data)
{
    Component::Load(data);
    if (data.hasKey("moveSpeed"))
        moveSpeed = (float)data.at("moveSpeed").ToFloat();
    if (data.hasKey("shiftMultiplier"))
        shiftMultiplier = (float)data.at("shiftMultiplier").ToFloat();
    if (data.hasKey("useCameraSpace"))
        useCameraSpace = data.at("useCameraSpace").ToBool();
}


void TargetMoveController::Update(float delta)
{
    if(!owner) {
        return;
	}

	Transform* transform = owner->GetTransform();
    if(!transform) {
        return;
	}

	InputManager& input = InputManager::GetInstance();

	glm::vec3 moveDir(0.0f);
	glm::vec3 forward(0.0f, 0.0f, -1.0f);
	glm::vec3 right(1.0f, 0.0f, 0.0f);

    if (useCameraSpace) {
		RenderSystem& renderSystem = RenderSystem::GetInstance();   
		CameraComponent* mainCam = renderSystem.GetMainCamera();

        if (mainCam) {
            auto& cam = mainCam->GetCamera();
            forward = cam.GetForward();
            right = cam.GetRight();

            forward.y = 0.0f;
            if (glm::length(forward) > 0.0f)
                forward = glm::normalize(forward);
        }
    }

    if (input.IsKeyDown(SDL_SCANCODE_W)) moveDir += forward;
    if (input.IsKeyDown(SDL_SCANCODE_S)) moveDir -= forward;
    if (input.IsKeyDown(SDL_SCANCODE_D)) moveDir += right;
    if (input.IsKeyDown(SDL_SCANCODE_A)) moveDir -= right;

    if (input.IsKeyDown(SDL_SCANCODE_E)) moveDir.y += 1.0f;
    if (input.IsKeyDown(SDL_SCANCODE_Q)) moveDir.y -= 1.0f;

    if(input.IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        Scene* scene = SceneManager::GetInstance().GetActiveScene();
        if (!scene) return;
        Entity* prototype = scene->FindEntityByName("Spinning Cube2");
		RenderSystem& renderSystem = RenderSystem::GetInstance();
        renderSystem.FireFromPrototypeForward(prototype);
	}

    if (glm::length(moveDir) > 0.0f) {
        moveDir = glm::normalize(moveDir);

        float speed = moveSpeed;
        if (input.IsKeyDown(SDL_SCANCODE_LSHIFT) || input.IsKeyDown(SDL_SCANCODE_RSHIFT)) {
            speed *= shiftMultiplier;
        }

        glm::vec3 newPos = transform->GetPosition() + moveDir * speed * delta;
        transform->SetPosition(newPos);

    }
}