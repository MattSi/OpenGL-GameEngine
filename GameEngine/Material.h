#pragma once

class Shader;
class Texture;

class Material
{
public:
	Shader* shader = nullptr;

	Texture* diffuseTexture = nullptr;
	Texture* specularTexture = nullptr;
	Texture* emissionTexture = nullptr;
	Texture* normalTexture = nullptr;

	glm::vec3 tint = glm::vec3(1.0f, 1.0f, 1.0f); // RGB tint
	glm::vec3 diffuseColor = glm::vec3(1.0f, 1.0f, 1.0f);	// Diffuse color
	glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f); // Specular color

	float shininess = 32.0f; // Specular shininess factor
	float specularStrength = 1.0f; // Specular strength
	float emissionStrength = 0.05f; // Emissive strength


public:
	Material() = default;
	explicit Material(Shader* shaderProgram) : shader(shaderProgram) {}
	
	void Apply(Shader* overrideShader) const;
};

