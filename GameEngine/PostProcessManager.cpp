#include "EngineCore.h"
#include "PostProcessManager.h"
#include "Framebuffer.h"
#include "Shader.h"
#include "Engine.h" 

template<> PostProcessManager* Singleton<PostProcessManager>::instance = nullptr;
namespace {
    GLuint g_FullscreenVAO = 0;
    GLuint g_FullscreenVBO = 0;

    Shader* g_BlitShader = nullptr;
    void CreateFullscreenQuad()
    {
        if (g_FullscreenVAO != 0)
            return;

        const float quadVertices[] = {
            // pos      // uv
            -1.f, -1.f, 0.f, 0.f,
             1.f, -1.f, 1.f, 0.f,
            -1.f,  1.f, 0.f, 1.f,

             1.f, -1.f, 1.f, 0.f,
             1.f,  1.f, 1.f, 1.f,
            -1.f,  1.f, 0.f, 1.f
        };

        glGenVertexArrays(1, &g_FullscreenVAO);
        glGenBuffers(1, &g_FullscreenVBO);

        glBindVertexArray(g_FullscreenVAO);
        glBindBuffer(GL_ARRAY_BUFFER, g_FullscreenVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        glBindVertexArray(0);
    }

    void DestroyFullscreenQuad()
    {
        if (g_FullscreenVBO != 0)
        {
            glDeleteBuffers(1, &g_FullscreenVBO);
            g_FullscreenVBO = 0;
        }
        if (g_FullscreenVAO != 0)
        {
            glDeleteVertexArrays(1, &g_FullscreenVAO);
            g_FullscreenVAO = 0;
        }
    }

    void InitBlitShader()
    {
        if (g_BlitShader)
            return;

        g_BlitShader = new Shader();
        bool ok = g_BlitShader->LoadFromFiles("Assets/Shaders/blit.vert", "Assets/Shaders/blit.frag");
        (void)ok;
    }
     
    void DestroyBlitShader()
    {
        if (g_BlitShader)
        {
            delete g_BlitShader;
            g_BlitShader = nullptr;
        }
    }

    void BlitToScreen(Texture* tex, int screenW, int screenH)
    {
        if (!tex) return;

        InitBlitShader();
        CreateFullscreenQuad();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, screenW, screenH);
        glDisable(GL_DEPTH_TEST);

        g_BlitShader->use();
        g_BlitShader->SetInt("u_Texture", 0);

        tex->bind(0);

        glBindVertexArray(g_FullscreenVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}



void PostProcessManager::Initialize(int w, int h)
{

    screenWidth = w;
    screenHeight = h;

    // ping FBO
    pingFBO = new Framebuffer();
    pingFBO->create(w, h, /*withDepth=*/false, GL_RGBA16F);
    pingColor = &pingFBO->colorTexture();

    // pong FBO
    pongFBO = new Framebuffer();
    pongFBO->create(w, h, /*withDepth=*/false, GL_RGBA16F);
    pongColor = &pongFBO->colorTexture();
}

void PostProcessManager::Initialize()
{
	
}

void PostProcessManager::Resize(int newWidth, int newHeight)
{
    screenWidth = newWidth;
    screenHeight = newHeight;

    if (pingFBO)
    {
        pingFBO->resize(newWidth, newHeight, false, GL_RGBA16F);
        pingColor = &pingFBO->colorTexture();
    }

    if (pongFBO)
    {
        pongFBO->resize(newWidth, newHeight, false, GL_RGBA16F);
        pongColor = &pongFBO->colorTexture();
    }
}

// Called by Singleton::Destroy(). Must run while the GL context is still alive.
// pingColor/pongColor point into the FBOs, so they are not deleted separately.
void PostProcessManager::CleanUp()
{
    if (pingFBO)
    {
        delete pingFBO;
        pingFBO = nullptr;
    }
    if (pongFBO)
    {
        delete pongFBO;
        pongFBO = nullptr;
    }

    pingColor = nullptr;
    pongColor = nullptr;

    passes.clear();

	DestroyFullscreenQuad();
	DestroyBlitShader();
}

void PostProcessManager::Apply(Texture* sceneColorTex, Texture* sceneDepthTex, bool outToScreen, float nearPlane, float farPlane, float totalTime)
{
    if(!sceneColorTex)
		return;

	bool anyEnabled = false;
    for (auto& p : passes) {
        if (p && p->IsEnabled()) {
            anyEnabled = true;
            break;
		}
    }

    if (!anyEnabled) {
        if(outToScreen) {
            BlitToScreen(sceneColorTex, screenWidth, screenHeight);
        }
		return;
    }

	PostProcessContext ctx;
    ctx.screenWidth = screenWidth;
    ctx.screenHeight = screenHeight;
    ctx.depthTexture = sceneDepthTex;
    ctx.time = totalTime;
	ctx.nearPlane = nearPlane;
	ctx.farPlane = farPlane;

	Texture* currentInput = sceneColorTex;
	Framebuffer* currentOutput = nullptr;
	bool usePing = true;

    for (auto& p : passes) {
		if (!p || !p->IsEnabled())
			continue;

		currentOutput = usePing ? pingFBO : pongFBO;
		p->Execute(currentInput, currentOutput, ctx);
		currentInput = usePing ? pingColor : pongColor;
		usePing = !usePing;
    }

    if (outToScreen)
    {
        BlitToScreen(currentInput, screenWidth, screenHeight);
    }
    else
    {
		// If I have a final FBO, just blit to screen for now
        BlitToScreen(currentInput, screenWidth, screenHeight);
    }
}

void PostProcessManager::OnImGui()
{
    if (ImGui::Begin("Post Process"))
    {
        for (auto& pass : passes)
        {
            if (!pass) continue;

            ImGui::PushID(pass.get());

            bool enabled = pass->IsEnabled();
            if (ImGui::Checkbox(pass->GetName().c_str(), &enabled))
            {
                pass->SetEnabled(enabled);
            }

            if (enabled)
            {
                ImGui::Indent();
                pass->OnImGui();
                ImGui::Unindent();
            }

            ImGui::Separator();
            ImGui::PopID();
        }
    }
    ImGui::End();
}

