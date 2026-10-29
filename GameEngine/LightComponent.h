
#include "Component.h"


class LightComponent : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(LightComponent, Component);

public:
    enum class LightType
    {
        Directional=0,
        Point=1,
        Cone=2
	};


    void Initialize() override;
    void Destroy() override;
    void Load(json::JSON& data) override;

	const char* GetTypeName() const override { return "LightComponent"; }

	void SetLightType(LightType newType);
	LightType GetLightType() const noexcept { return lightType; }

	void SetColor(const glm::vec3& newColor);
	glm::vec3 GetColor() const noexcept { return color; }

	void SetIntensity(float newIntensity);
	float GetIntensity() const noexcept { return intensity; }

	void SetDirection(const glm::vec3& newDirection);
	glm::vec3 GetDirection() const;

	glm::vec3 GetPosition() const;	

	void SetAttenuation(const glm::vec3& newAttenuation);
	glm::vec3 GetAttenuation() const noexcept { return attenuation; }

	void SetInnerCutoff(float newInnerCutoff);
	float GetInnerCutoff() const noexcept { return std::cos(glm::radians(innerCutoff));; }

	void SetOuterCutoff(float newOuterCutoff);
	float GetOuterCutoff() const noexcept { return std::cos(glm::radians(outerCutoff));; }

	void SetRange(float newRange) { range = newRange; }
	float GetRange() const noexcept { return range; }

	float GetInnerCutoffDegrees() const noexcept { return innerCutoff; }
	float GetOuterCutoffDegrees() const noexcept { return outerCutoff; }

	void SetEmissionStrength(float value) { emissionStrength = value; }
	float GetEmissionStrength() const noexcept { return emissionStrength; }


private:
    LightType lightType = LightType::Point;
    glm::vec3 color{ 1.0f };
	float intensity = 1.0f;
	glm::vec3 direction{ 0.0f, -1.0f, 0.0f };
	glm::vec3 attenuation{ 1.0f, 0.09f, 0.032f }; // constant, linear, quadratic
	float range = 25.f; // for point lights
	float innerCutoff = 12.5f; // for spotlights
	float outerCutoff = 17.5f; // for spotlights
	float emissionStrength = 0.05f;

};
