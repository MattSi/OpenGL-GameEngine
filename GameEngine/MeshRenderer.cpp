#include "EngineCore.h"
#include "MeshRenderer.h"
#include "Shader.h"
#include "Material.h"
#include "Transform.h"
#include "Entity.h"
#include "Mesh.h"
#include "Texture.h"
#include "PrimitiveMeshes.h"
#include "RenderSystem.h"

IMPLEMENT_DYNAMIC_CLASS(MeshRenderer);

void MeshRenderer::Load(json::JSON& data)
{
	Component::Load(data);
	DEBUG_LOG(indent, "MeshRenderer::Load");
	// Note: Mesh and Material should be assigned after loading, 
	// typically via the AssetManager or similar system.
	if (data.hasKey("primitive")) {
		std::string primitive = data.at("primitive").ToString();
		if(primitive == "Cube") {
			mesh = PrimitiveMeshes::GetCubeMesh();
		} else if(primitive == "Sphere") {
			mesh = PrimitiveMeshes::GetSphereMesh();
		}
	}
	if(data.hasKey("material")) {
		material = new Material();
		json::JSON materialJSON = data.at("material");
		if(materialJSON.hasKey("tint")) {
			auto colorJson = materialJSON.at("tint");
			material->tint.r = static_cast<float>(colorJson.at("r").ToFloat());
			material->tint.g = static_cast<float>(colorJson.at("g").ToFloat());
			material->tint.b = static_cast<float>(colorJson.at("b").ToFloat());
		}
	 
		if (materialJSON.hasKey("emissiveStrength")) {
			float emissionStrength = materialJSON.at("emissiveStrength").ToFloat();
			material->emissionStrength = emissionStrength;
		}
		if (materialJSON.hasKey("shininess")) {
			float shininess = materialJSON.at("shininess").ToFloat();
			material->shininess = shininess;
		}
		if (materialJSON.hasKey("specularStrength")) {
			float specularStrength = materialJSON.at("specularStrength").ToFloat();
			material->specularStrength = specularStrength;
		}

		if (materialJSON.hasKey("diffuseTexture")) {
			std::string path = materialJSON.at("diffuseTexture").ToString();
			material->diffuseTexture = new Texture();
			material->diffuseTexture->LoadFromFiles(path);
		}

		if (materialJSON.hasKey("specularTexture")) {
			std::string path = materialJSON.at("specularTexture").ToString();
			material->specularTexture = new Texture();
			material->specularTexture->LoadFromFiles(path);
		}
	}

	// Shared shader owned by RenderSystem (was compiled and leaked per entity).
	material->shader = RenderSystem::GetInstance().GetCubeShader();
}

void MeshRenderer::Render(const glm::mat4& view, const glm::mat4& proj, float delta)
{
	if(!visible || !mesh || !material ||!owner) {
		return;
	}

	Shader* shader = material->shader;
	if(!shader || !shader->isValid()) {
		return;
	} 

	// 1. Get the model matrix from the owner's Transform component
	Transform* transform = owner->GetComponent<Transform>();
	glm::mat4 model(1.0f);
	if(transform) {
		model = transform->GetWorldMatrix();
	}

	// 2. Set shader uniforms for model, view, projection matrices
	material->Apply(nullptr);
	  
	// 3. Set matrices
	shader->SetMat4("uModel", model);
	shader->SetMat4("uView", view);
	shader->SetMat4("uProjection", proj);
	shader->SetVec3("uColor", material->tint);
	shader->SetVec3("uViewPos", glm::vec3(glm::inverse(view)[3]));

	if (!mesh) DEBUG_LOG(0, "MeshRenderer: mesh is null");
	if (!material) DEBUG_LOG(0, "MeshRenderer: material is null");
	if (!material->shader) DEBUG_LOG(0, "MeshRenderer: shader is null");
	if (!material->shader->isValid()) DEBUG_LOG(0, "MeshRenderer: shader invalid");

	//shader->SetTexture("uMaterial.diffuseTexture", textureID, 0);
	// Multi light sources will be set on the Render System before calling this Render method

	// 4. Draw the mesh
	mesh->Draw();
	
}
