#include "EngineCore.h"
#include "DepthOfFieldPass.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Mesh.h"

void DepthOfFieldPass::Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx)
{
    if (!inputColor || !outFBO || !shader_ || !screenQuad_ || !ctx.depthTexture)
        return;

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

    shader_->SetFloat("uFocusDepth", focusDepth_);
    shader_->SetFloat("uFocusRange", focusRange_);
    shader_->SetFloat("uMaxBlur", maxBlur_);

    screenQuad_->Draw();

    Framebuffer::BindDefault();
}

void DepthOfFieldPass::OnImGui()
{
    ImGui::TextUnformatted("Depth of Field");
    ImGui::SliderFloat("Focus Depth", &focusDepth_, 0.0f, 1.0f);
    ImGui::SliderFloat("Focus Range", &focusRange_, 0.01f, 0.5f);
    ImGui::SliderFloat("Max Blur", &maxBlur_, 0.0f, 20.0f);
}
