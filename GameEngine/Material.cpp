#include "EngineCore.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "LightManager.h"



void Material::Apply(Shader* overrideShader) const
{
	Shader* shaderToUse = overrideShader ? overrideShader : shader;
	if (!shaderToUse || !shaderToUse->isValid()) {
		return;
	}
	shaderToUse->use();

	shaderToUse->SetVec3("uMaterialTint", tint);
	shaderToUse->SetFloat("uMaterialShininess", shininess);
	shaderToUse->SetFloat("uMaterialSpecularStrength", specularStrength);
	shaderToUse->SetFloat("uMaterialEmissiveStrength", emissionStrength);
	shaderToUse->SetVec3("uMaterialSpecularColor", specularColor);

	int nextSlot = 0;

	auto bindTex = [&](Texture* tex,
		const char* samplerName,
		const char* hasName)
		{
			if (tex && tex->valid())
			{
				shaderToUse->SetBool(hasName, true);
				shaderToUse->SetInt(samplerName, nextSlot);
				tex->bind(nextSlot);
				++nextSlot;
			}
			else
			{
				shaderToUse->SetBool(hasName, false);
			}
		};

	bindTex(diffuseTexture, "uDiffuseMap", "uHasDiffuseMap");
	bindTex(specularTexture, "uSpecularMap", "uHasSpecularMap");
	bindTex(emissionTexture, "uEmissionMap", "uHasEmissionMap");
	bindTex(normalTexture, "uNormalMap", "uHasNormalMap");

	LightManager::GetInstance().ApplyToShader(shaderToUse);
}
