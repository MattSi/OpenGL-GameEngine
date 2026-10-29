#include "EngineCore.h"
#include "SkyboxComponent.h"
#include "RenderSystem.h"

IMPLEMENT_DYNAMIC_CLASS(SkyboxComponent);

void SkyboxComponent::Initialize()
{
	Component::Initialize();


	RenderSystem::GetInstance().SetSkyboxTextures(faces);
}

void SkyboxComponent::Destroy()
{
}

void SkyboxComponent::Load(json::JSON& data)
{
	if (!data.hasKey("cubemap")) {
		DEBUG_LOG(2, "SkyboxComponent::Load - No cubemap key found");
		return;
	}
	json::JSON cubemapJson = data.at("cubemap");
	faces[0] = cubemapJson.at("left").ToString();
	faces[1] = cubemapJson.at("right").ToString();
	faces[2] = cubemapJson.at("top").ToString();
	faces[3] = cubemapJson.at("bottom").ToString();
	faces[4] = cubemapJson.at("back").ToString();
	faces[5] = cubemapJson.at("front").ToString();

	DEBUG_LOG(2, "SkyboxComponent::Load - Loaded cubemap faces:");
}
