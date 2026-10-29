#include "EngineCore.h"
#include "PostFogPass.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Mesh.h"


void PostFogPass::Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx)
{
    if (!inputColor || !outFBO || !shader_ || !screenQuad_ || !ctx.depthTexture)
        return;

    outFBO->bind();
    glViewport(0, 0, ctx.screenWidth, ctx.screenHeight);
    glDisable(GL_DEPTH_TEST);

    shader_->use();

    // 颜色纹理
    shader_->SetInt("uColorTex", 0);
    inputColor->bind(0);

    // 深度纹理
    shader_->SetInt("uDepthTex", 1);
    ctx.depthTexture->bind(1);

    shader_->SetVec3("uFogColor", fogColor_);
    shader_->SetFloat("uFogStart", fogStart_);
    shader_->SetFloat("uFogEnd", fogEnd_);

    screenQuad_->Draw();

    Framebuffer::BindDefault();
}

void PostFogPass::OnImGui()
{
    ImGui::TextUnformatted("Depth Fog");
    ImGui::ColorEdit3("Fog Color", &fogColor_.x);

    ImGui::SliderFloat("Fog Start", &fogStart_, 0.7f, 1.0f);
    ImGui::SliderFloat("Fog End", &fogEnd_, 0.7f, 2.0f);

    
    if (fogEnd_ <= fogStart_ + 0.01f)
        fogEnd_ = fogStart_ + 0.01f;
}
