#pragma once

// Right handed camera class
// +X Right, +Y Up, -Z Forward
class Camera
{
public:
	Camera();

	void SetPerspective(float fovY, float aspect, float nearPlane, float farPlane);
	void SetPosition(const glm::vec3& position);
	void SetYawPitch(float yaw, float pitch); //Yaw for Y axis, Pitch for X axis

	void MoveWorld(const glm::vec3& delta);

	// Move in local space, x = right, y = up, z = forward
	void MoveLocal(const glm::vec3& delta);

	// Rotate in local space, yaw around Y axis, pitch around X axis
	void AddYawPitch(float deltaYaw, float deltaPitch);

	// ==== Get State ====
	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetForward() const { return forward; }
	const glm::vec3& GetUp() const { return up; }
	const glm::vec3& GetRight() const { return right; }

	const float GetYaw() const { return yaw; }
	const float GetPitch() const { return pitch; }

	float GetFovY() const { return fovY; }
	float GetAspect() const { return aspect; }
	float GetNearPlane() const { return nearPlane; }
	float GetFarPlane() const { return farPlane; }

	// ==== Set Aspect Ratio ====
	void SetAspect(float aspect) {
		this->aspect = aspect;
		projectionDirty = true;
	}

	// ==== Get Matrices ====
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	// Move for GameController input
	float moveSpeed = 5.0f; // units per second
	float mouseSensitivity = 0.1f; // degrees per pixel

private:

	// Position
	glm::vec3 position{ 0.0f,0.0f,3.0f };

	// Orientation vectors
	glm::vec3 forward{ 0.0f,0.0f,-1.0f };
	glm::vec3 up{ 0.0f,1.0f,0.0f };
	glm::vec3 right{ 1.0f,0.0f,0.0f };

	// Euler angles
	float yaw = -90.0f;   // Rotation around Y axis
	float pitch = 0.0f; // Rotation around X axis

	// Perspective parameters
	float fovY = 60.f; // in degrees
	float aspect = 16.0f / 9.0f;
	float nearPlane = 0.1f;
	float farPlane = 100.f;

	mutable glm::mat4 cachedProjection{ 1.0f };
	mutable bool projectionDirty = true;

	void RecalculateDirections();
	void RebuildProjectionMatrix() const;
};

