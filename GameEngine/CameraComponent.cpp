#include "EngineCore.h"
#include "CameraComponent.h"
#include "RenderSystem.h"
#include "Entity.h"
#include "Transform.h"
#include "InputManager.h"
#include "Engine.h"

IMPLEMENT_DYNAMIC_CLASS(CameraComponent)

void CameraComponent::Initialize()
{
	Component::Initialize();

	RenderSystem& renderSystem = RenderSystem::GetInstance();
	SDL_Window* window = renderSystem.GetWindow();

	int width=1280, height=720;

	if (window) {
		SDL_GetWindowSize(window, &width, &height);
	}

	float aspect = height != 0 ? static_cast<float>(width) / static_cast<float>(height) : 16.0f / 9.0f;
	camera.SetPerspective(fovY, aspect, nearPlane, farPlane);	

	if(owner)
	{
		Transform* transform = owner->GetComponent<Transform>();
		if(transform)
		{
			camera.SetPosition(transform->position);
			glm::vec3 rotation = transform->rotation;
			camera.SetYawPitch(rotation.y, rotation.x);
		}
	}

	if(setAsMainCamera)
	{
		renderSystem.SetMainCamera(this);
	}


}

void CameraComponent::Destroy()
{
	RenderSystem& renderSystem = RenderSystem::GetInstance();
	if(renderSystem.GetMainCamera() == this)
	{
		renderSystem.SetMainCamera(nullptr);
	}
	Component::Destroy();
}

void CameraComponent::Update(float delta)
{
	Component::Update(delta);

	if (!owner) {
		return;
	}
	Transform* transform = owner->GetComponent<Transform>();
	if (!transform) {
		return;
	}

	camera.SetPosition(transform->GetPosition());
	glm::vec3 finalRotation = transform->GetRotation();
	camera.SetYawPitch(finalRotation.y, finalRotation.x);
}

void CameraComponent::Load(json::JSON& data)
{
	Component::Load(data);
	if(data.hasKey("fov"))
	{
		fovY = static_cast<float>(data.at("fov").ToFloat());
	}
	if(data.hasKey("near"))
	{
		nearPlane = static_cast<float>(data.at("near").ToFloat());
	}
	if(data.hasKey("far"))
	{
		farPlane = static_cast<float>(data.at("far").ToFloat());
	}
	if(data.hasKey("mainCamera"))
	{
		setAsMainCamera = data.at("mainCamera").ToBool();
	}
}