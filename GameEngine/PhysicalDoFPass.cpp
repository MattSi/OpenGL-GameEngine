#include "EngineCore.h"
#include "PhysicalDoFPass.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Mesh.h"


void PhysicalDoFPass::Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx)
{
    if (!inputColor || !outFBO || !shader_ || !screenQuad_ || !ctx.depthTexture)
        return;

	cachedNearPlane_ = ctx.nearPlane;
	cachedFarPlane_ = ctx.farPlane;

	outFBO->bind();
	glViewport(0, 0, ctx.screenWidth, ctx.screenHeight);
	glDisable(GL_DEPTH_TEST);

	shader_->use();

	shader_->SetInt("uColorTex", 0);
	inputColor->bind(0);

	shader_->SetInt("uDepthTex", 1);
	ctx.depthTexture->bind(1);

	float texelX = 1.0f / static_cast<float>(ctx.screenWidth);
	float texelY = 1.0f / static_cast<float>(ctx.screenHeight);
	shader_->SetVec2("uTexelSize", glm::vec2(texelX, texelY));

	shader_->SetFloat("uNear", cachedNearPlane_);
	shader_->SetFloat("uFar", cachedFarPlane_);

	shader_->SetFloat("uFocalLengthMm", focalLengthMm_);
	shader_->SetFloat("uFStop", fStop_);
	shader_->SetFloat("uFocusDistance", focusDistance_);
	shader_->SetFloat("uMaxBlurRadius", maxBlurRadiusPx_);
	screenQuad_->Draw();
	Framebuffer::BindDefault();
}
  
void PhysicalDoFPass::OnImGui()
{
    ImGui::TextUnformatted("Physical DOF");
	ImGui::Text("Near Plane: %.3f", cachedNearPlane_);
	ImGui::Text("Far Plane : %.3f", cachedFarPlane_);
    ImGui::SliderFloat("Focal Length (mm)", &focalLengthMm_, 15.0f, 120.0f);
    ImGui::SliderFloat("f-stop", &fStop_, 1.2f, 16.0f);
    ImGui::SliderFloat("Focus Distance", &focusDistance_, 1.0f, 100.0f);
    ImGui::SliderFloat("Max Blur Radius", &maxBlurRadiusPx_, 0.0f, 40.0f);
}
