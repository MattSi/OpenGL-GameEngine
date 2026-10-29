#include "EngineCore.h"
#include "Framebuffer.h"

bool Framebuffer::create(int w, int h, bool withDepth, GLenum colorInternalFormat)
{
	destroy();

    width_ = w;
    height_ = h;
    hasDepthTex_ = withDepth;
    complete_ = false;

    glGenFramebuffers(1, &fbo_);
    M_ASSERT(fbo_ != 0, "Framebuffer::create: glGenFramebuffers failed");

    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    colorTexture_.CreateEmpty(
        w, h,
        colorInternalFormat,
        GL_RGBA,           
        GL_FLOAT,           
        false               
    );

    glFramebufferTexture2D(GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        colorTexture_.id(),
        0);

    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    if (withDepth)
    {
        depthTexture_.CreateEmpty(
            w, h,
            GL_DEPTH_COMPONENT24,   // internalFormat
            GL_DEPTH_COMPONENT,     // format
            GL_UNSIGNED_INT,        // type
            false
        );

        glFramebufferTexture2D(GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            depthTexture_.id(),
            0);
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        M_ASSERT(false, "Framebuffer::create: framebuffer not complete");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        complete_ = false;
        return false;
    }

    complete_ = true;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void Framebuffer::resize(int w, int h, bool withDepth, GLenum colorInternalFormat)
{
    create(w, h, withDepth, colorInternalFormat);
}

void Framebuffer::moveFrom(Framebuffer& o)
{

    fbo_ = o.fbo_; o.fbo_ = 0;
    width_ = o.width_; o.width_ = 0;
    height_ = o.height_; o.height_ = 0;
    complete_ = o.complete_; o.complete_ = false;
    hasDepthTex_ = o.hasDepthTex_; o.hasDepthTex_ = false;

    colorTexture_ = std::move(o.colorTexture_);
    depthTexture_ = std::move(o.depthTexture_);

}

void Framebuffer::destroy()
{
    if (fbo_ != 0)
    {
        glDeleteFramebuffers(1, &fbo_);
        fbo_ = 0;
    }

    colorTexture_.destroy();
    depthTexture_.destroy();

    width_ = 0;
    height_ = 0;
    hasDepthTex_ = false;
    complete_ = false;
}