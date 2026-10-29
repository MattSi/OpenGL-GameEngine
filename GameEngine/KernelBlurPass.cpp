#include "EngineCore.h"
#include "KernelBlurPass.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Mesh.h"


void KernelBlurPass::Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx)
{
    if (!inputColor || !outFBO || !shader_ || !screenQuad_)
        return;

    outFBO->bind();
    glViewport(0, 0, ctx.screenWidth, ctx.screenHeight);
    glDisable(GL_DEPTH_TEST);

    shader_->use();

    shader_->SetInt("uInputTex", 0);
    inputColor->bind(0);

    float texelX = 1.0f / static_cast<float>(ctx.screenWidth);
    float texelY = 1.0f / static_cast<float>(ctx.screenHeight);
    shader_->SetVec2("uTexelSize", glm::vec2(texelX, texelY));

    shader_->SetFloat("uStrength", strength_);

    screenQuad_->Draw();

    Framebuffer::BindDefault();
}
 
void KernelBlurPass::OnImGui()
{
	ImGui::TextUnformatted("Kernel Blur");
	ImGui::SliderFloat("Strength", &strength_, 0.0f, 8.0f);
}
