#include "EngineCore.h"
#include "Camera.h"


Camera::Camera()
{
	RecalculateDirections();
	projectionDirty = true;
}
 
void Camera::SetPerspective(float fovY, float aspect, float nearPlane, float farPlane)
{
	this->fovY = fovY;
	this->aspect = aspect;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;


	projectionDirty = true;
}

void Camera::SetPosition(const glm::vec3& position)
{
	this->position = position;
}

void Camera::SetYawPitch(float yaw, float pitch)
{
	this->yaw = yaw;
	this->pitch = glm::clamp(pitch, -89.0f, 89.0f);

	RecalculateDirections();
}

void Camera::MoveWorld(const glm::vec3& delta)
{
	position += delta;
}

void Camera::MoveLocal(const glm::vec3& delta)
{
	position += right * delta.x;
	position += up * delta.y;
	position += forward * delta.z;
}

void Camera::AddYawPitch(float deltaYaw, float deltaPitch)
{
	yaw		+= deltaYaw;
	pitch	+= deltaPitch;
	pitch	 = glm::clamp(pitch, -89.0f, 89.0f);
	RecalculateDirections();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	if (projectionDirty) {
		RebuildProjectionMatrix();
	}
	return cachedProjection;
}

void Camera::RecalculateDirections()
{
	// Convert degrees to radians
	float yawRad = glm::radians(yaw);
	float pitchRad = glm::radians(pitch);

	// Calculate forward vector
	glm::vec3 f;
	// Right-handed coordinate system, -Z is forward
	f.x = std::cos(yawRad) * std::cos(pitchRad);
	f.y = std::sin(pitchRad);
	f.z = std::sin(yawRad) * std::cos(pitchRad);

	forward = glm::normalize(f);

	const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

	// Recalculate right and up vectors
	right	= glm::normalize(glm::cross(forward, worldUp));
	up		= glm::normalize(glm::cross(right, forward));
}

void Camera::RebuildProjectionMatrix() const
{
	cachedProjection	= glm::perspective(glm::radians(fovY), aspect, nearPlane, farPlane);
	projectionDirty		= false;
}