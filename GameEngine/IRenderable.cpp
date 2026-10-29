#include "EngineCore.h"
#include "IRenderable.h"
#include "RenderSystem.h"

IRenderable::IRenderable()
{
	RenderSystem::GetInstance().AddRenderable(this);
}

IRenderable::~IRenderable()
{
	RenderSystem::GetInstance().RemoveRenderable(this);
}
   