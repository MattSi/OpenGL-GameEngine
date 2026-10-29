#pragma once
#include "Component.h"
#include "IRenderable.h"


class Model;
class InstancedModelRenderer :
    public Component, public IRenderable
{
    DECLARE_DYNAMIC_DERIVED_CLASS(InstancedModelRenderer, Component);

public:
	std::string modelPath;
	bool visible = true;


	int instanceCount = 100;
	int instancesPerRow = 5;
	float instanceSpacing = 2.5f;

	Model* model = nullptr;
	bool ownModel = false;


    bool  hasColorOverride = false;
    glm::vec3 overrideColor = glm::vec3(1.0f);

    bool  hasEmissionOverride = false;
    float overrideEmission = 0.0f;

    bool  hasSpecularStrengthOverride = false;
    float overrideSpecularStrength = 1.0f;

    bool  hasSpecularColorOverride = false;
    glm::vec3 overrideSpecularColor = glm::vec3(1.0f);

public:
    virtual void Load(json::JSON& data) override;
    virtual void Initialize() override;
    virtual void Destroy() override;

    void Render(const glm::mat4& view, const glm::mat4& proj, float delta) override;
    void Render(float) override {} 

    const char* GetTypeName() const override { return "InstancedModelRenderer"; }

private:
	void ApplyOverridesToModel();
  

	glm::vec3 randomMin = glm::vec3(-2.f, 0.0f, 40.0f);
	glm::vec3 randomMax = glm::vec3(2.f, 0.0f, -40.0f);
    bool randomRotation = true;
	bool randomScale = false;

	
};

