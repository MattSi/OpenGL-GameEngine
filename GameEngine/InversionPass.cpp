#include "EngineCore.h"
#include "InversionPass.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Mesh.h"

void InversionPass::Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx)
{
	outFBO->bind();
	glViewport(0, 0, ctx.screenWidth, ctx.screenHeight);
	glDisable(GL_DEPTH_TEST);

	shader_->use();
	shader_->SetInt("uInputTex", 0);

	inputColor->bind(0);
	screenQuad_->Draw();

	Framebuffer::BindDefault();
}
 
 
void InversionPass::OnImGui()
{
	ImGui::TextUnformatted("Inversion");
}