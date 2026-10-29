#pragma once
#include "Singleton.h"

class LightComponent;
class Shader;
class Engine;

class LightManager : public Singleton<LightManager>
{
public:
	void Initialize();
	void CleanUp() override;

	void RegisterLight(LightComponent* light);
	void UnregisterLight(LightComponent* light);

	const std::vector<LightComponent*>& GetLights() const { return lights; }

	void ApplyToShader(Shader* shader) const;

	LightComponent* GetMainDirectionalLight() const;
private:
	std::vector<LightComponent*> lights;

	friend class Singleton<LightManager>;
	friend class Engine;

	const int MAX_POINT_LIGHTS = 8;
	const int MAX_SPOT_LIGHTS = 8;

protected:
	LightManager() = default;
	~LightManager() = default;

	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;
};

