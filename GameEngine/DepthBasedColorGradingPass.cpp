#include "EngineCore.h"
#include "DepthBasedColorGradingPass.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Mesh.h"


void DepthBasedColorGradingPass::Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx)
{
    if (!inputColor || !outFBO || !shader_ || !screenQuad_ || !ctx.depthTexture)
        return;

    cachedNear_ = ctx.nearPlane;
    cachedFar_ = ctx.farPlane;

    outFBO->bind();
    glViewport(0, 0, ctx.screenWidth, ctx.screenHeight);
    glDisable(GL_DEPTH_TEST);

    shader_->use();

    shader_->SetInt("uColorTex", 0);
    inputColor->bind(0);

    shader_->SetInt("uDepthTex", 1);
    ctx.depthTexture->bind(1);

    shader_->SetFloat("uNear", ctx.nearPlane);
    shader_->SetFloat("uFar", ctx.farPlane);

    shader_->SetVec3("uNearTint", nearTint_);
    shader_->SetVec3("uFarTint", farTint_);

    shader_->SetFloat("uDepthStart", depthStart_);
    shader_->SetFloat("uDepthEnd", depthEnd_);
    shader_->SetFloat("uStrength", strength_);

    screenQuad_->Draw();

    Framebuffer::BindDefault();
}

void DepthBasedColorGradingPass::OnImGui()
{
    ImGui::TextUnformatted("Depth-based Color Grading");
    ImGui::Text("Near Plane: %.3f  Far Plane: %.3f", cachedNear_, cachedFar_);

    ImGui::ColorEdit3("Near Tint", &nearTint_.x);
    ImGui::ColorEdit3("Far Tint", &farTint_.x);

    ImGui::SliderFloat("Depth Start", &depthStart_, 0.0f, 1.0f);
    ImGui::SliderFloat("Depth End", &depthEnd_, 0.0f, 1.0f);
    if (depthEnd_ <= depthStart_ + 0.01f)
        depthEnd_ = depthStart_ + 0.01f;

    ImGui::SliderFloat("Strength", &strength_, 0.0f, 1.0f);
}
