#include "EngineCore.h"
#include "GrayscalePass.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Mesh.h"


void GrayscalePass::Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx)
{
	outFBO->bind();
	glViewport(0, 0, ctx.screenWidth, ctx.screenHeight);
	glDisable(GL_DEPTH_TEST);

	shader_->use();
	shader_->SetBool("uPrecise", precise);
	shader_->SetInt("uInputTex", 0);

	inputColor->bind(0);
	screenQuad_->Draw();

	Framebuffer::BindDefault();
}

void GrayscalePass::OnImGui()
{
	ImGui::TextUnformatted("Grayscale");
	ImGui::Checkbox("Precise grayscale", &precise);
}
