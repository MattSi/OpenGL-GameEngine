#include "EngineCore.h"
#include "Texture.h"  

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

bool Texture::LoadFromFiles(const std::string& path, bool generateMips, bool sRGB, bool flipY)
{
    destroy();
    int w, h, comp;
    stbi_set_flip_vertically_on_load(flipY ? 1 : 0);
    unsigned char* pixels = stbi_load(path.c_str(), &w, &h, &comp, 0);
    if (!pixels) return false;

    bool ok = uploadPixels(pixels, w, h, comp, generateMips, sRGB);
    stbi_image_free(pixels);
    return ok;
}

bool Texture::uploadPixels(const unsigned char* pixels, int w, int h, int comp, bool generateMips, bool sRGB)
{
    if (!pixels || w <= 0 || h <= 0 || comp <= 0) return false;

    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (comp == 1 ? GL_RED : comp == 3 ? GL_RGB : GL_RGBA);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, pixels);
    if (generateMips) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    width_ = w; height_ = h; channels_ = comp;
    hasMips_ = generateMips;

    return true;
}

void Texture::destroy()
{
    if (id_) {
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
    width_ = height_ = channels_ = 0;

    hasMips_ = false;
}

bool Texture::CreateEmpty(int w, int h, GLenum internalFormat, GLenum format, GLenum type, bool generateMips)
{
    destroy();
	width_ = w;
	height_ = h;

    switch(format) {
        case GL_RED: channels_ = 1; break;
        case GL_RG: channels_ = 2; break;
        case GL_RGB: channels_ = 3; break;
        case GL_RGBA: channels_ = 4; break;
        default: channels_ = 4; break;
	}

	glGenTextures(1, &id_);
	M_ASSERT(id_ != 0, "Texture::CreateEmpty: glGenTextures failed.");

	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    hasMips_ = false;
    if(generateMips) {
        glGenerateMipmap(GL_TEXTURE_2D);
        hasMips_ = true;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
    return true;
}
