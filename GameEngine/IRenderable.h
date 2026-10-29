#pragma once

class RenderSystem;

class IRenderable
{
private:
	friend class RenderSystem;

protected:
	IRenderable();
	virtual ~IRenderable() ;

public:
	virtual void Render(float delta) = 0;
	virtual void Render(const glm::mat4& view, const glm::mat4& proj, float delta) = 0;
};

