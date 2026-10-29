#pragma once
#include "Component.h"

class Transform : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Transform, Component);
public:
    void Initialize() override;
    void Destroy() override;
    void Update(float delta) override {};
	void Load(json::JSON& data) override ;

    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 initialPosition{ 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation{ 0.0f, 0.0f, 0.0f }; 
	glm::vec3 initialRotation{ 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	glm::vec3 initialScale{ 1.0f, 1.0f, 1.0f };

public:
    inline void SetPosition(const glm::vec3& p) { position = p; }
    inline void SetRotation(const glm::vec3& r) { rotation = r; }
    inline void SetScale(const glm::vec3& s) { scale = s; }

    inline const glm::vec3& GetPosition() const { return position; }
    inline const glm::vec3& GetRotation() const { return rotation; }
    inline const glm::vec3& GetScale()    const { return scale; }

	inline void ResetPosition() { position = initialPosition; }
	inline void ResetRotation() { rotation = initialRotation; }
	inline void ResetScale() { scale = initialScale; }

	void LookAt(const glm::vec3& targetPoint) {
		glm::vec3 dir = glm::normalize(targetPoint - position);

		float yaw = atan2(dir.x, dir.z) + glm::pi<float>(); // -Z forward
		float pitch = atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z));

		rotation.x = glm::degrees(pitch);
		rotation.y = glm::degrees(yaw);
		rotation.z = 0.0f;
	}

    glm::mat4 GetLocalMatrix() const {
        glm::mat4 m(1.0f);
		m = glm::translate(m, position);
		m = glm::rotate(m, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		m = glm::rotate(m, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		m = glm::rotate(m, glm::radians(rotation.z), glm::vec3(0, 0, 1));
		m = glm::scale(m, scale);
		return m;
    }

    glm::mat4 GetWorldMatrix() const {
        // For now, local and world matrices are the same
		// if (parent) return parent->GetWorldMatrix() * GetLocalMatrix();
		// else return GetLocalMatrix();
        return GetLocalMatrix();
	}

	const char* GetTypeName() const override { return "Transform"; }
};

