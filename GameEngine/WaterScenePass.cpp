#include "EngineCore.h"
#include "WaterScenePass.h"
#include "Shader.h"
#include "Mesh.h"
#include "Framebuffer.h"
#include "Engine.h"

void WaterScenePass::Execute(Texture* inputColor, Framebuffer* outFBO, const PostProcessContext& ctx)
{
    if (!shader_ || !screenQuad_ || !inputColor || !outFBO)
        return;

    outFBO->bind();
    glViewport(0, 0, ctx.screenWidth, ctx.screenHeight);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);

    shader_->use();

    shader_->SetInt("uInputTex", 0);
    shader_->SetFloat("uTime", ctx.time);

	float __finalFrequency = frequency_;
	float __finalAmplitude = amplitude_;

    auto& data = Engine::GetInstance().toolWindowData;
    if (data.isWaterScene) {
		__finalFrequency = data.Frequency;
		__finalAmplitude = data.Amplitude;
    }
    else {
		__finalFrequency = frequency_;
		__finalAmplitude = amplitude_;
    }
    shader_->SetFloat("uFrequency", __finalFrequency);
    shader_->SetFloat("uAmplitude", __finalAmplitude);

    if (data.isWaterScene && data.TintBlue) {
        shader_->SetBool("uTintBlueEnabled", true);
    }
    else {
		shader_->SetBool("uTintBlueEnabled", false);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputColor->id()); 

    screenQuad_->Draw();
     
    glEnable(GL_DEPTH_TEST);
    Framebuffer::BindDefault();
}
 
void WaterScenePass::OnImGui()
{
	ImGui::SliderFloat("Frequency", &frequency_, 0.1f, 10.0f);
	ImGui::SliderFloat("Amplitude", &amplitude_, 0.0f, 1.0f);
}
