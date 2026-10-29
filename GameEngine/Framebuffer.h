#pragma once
#include "EngineCore.h"
#include "Texture.h"

class Framebuffer
{
public:
	Framebuffer() = default;
	~Framebuffer() {
		destroy();
	}
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator=(const Framebuffer&) = delete;
	Framebuffer(Framebuffer&& other) noexcept { moveFrom(other); }
	Framebuffer& operator=(Framebuffer&& other) noexcept {
		if (this != &other) moveFrom(other);
		return *this;
	}

	Framebuffer(int w, int h, bool withDepth = false, GLenum colorInternalFormat = GL_RGBA16F) {
		create(w, h, withDepth, colorInternalFormat);
	}

	bool create(int w, int h, bool withDepth = false, GLenum colorInternalFormat = GL_RGBA16F);

	void destroy();

	void bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
	}

	static void BindDefault() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void resize(int w, int h, bool withDepth = false, GLenum colorInternalFormat = GL_RGBA16F);

	bool isComplete() const {
		return complete_;
	}

	int width() const noexcept { return width_; }
	int height() const noexcept { return height_; }

	Texture& colorTexture() noexcept {
		return colorTexture_;
	}
	const Texture& colorTexture() const noexcept {
		return colorTexture_;
	}

	Texture* depthTexture() noexcept {
		return hasDepthTex_ ? &depthTexture_ : nullptr;
	}

	const Texture* depthTexture() const noexcept {
		return hasDepthTex_ ? &depthTexture_ : nullptr;
	}

private:
	GLuint fbo_ = 0;
	int width_ = 0;
	int height_ = 0;
	bool complete_ = false;
	bool hasDepthTex_ = false;

	Texture colorTexture_;
	Texture depthTexture_;

	void moveFrom(Framebuffer& o); 
};

