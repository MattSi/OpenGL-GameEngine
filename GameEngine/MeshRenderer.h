#pragma once
#include "Component.h"
#include "IRenderable.h"

class Mesh;
class Material;

class MeshRenderer : public Component, public IRenderable
{
	DECLARE_DYNAMIC_DERIVED_CLASS(MeshRenderer, Component);
public:

	virtual void Load(json::JSON& data) override;
	Mesh* mesh = nullptr;
	Material* material = nullptr;

	bool visible = true;	
	bool castShadows = true;

	// view and projection matrices are passed from the Render System
	void Render(const glm::mat4& view, const glm::mat4& proj, float delta) override;
	void Render(float delta) override
	{
		// Not used in this implementation
	}

	const char* GetTypeName() const override { return "MeshRenderer"; }
};

