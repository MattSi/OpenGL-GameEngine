#pragma once

class Texture {
public:

    Texture() = default;
    ~Texture() { destroy(); }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept { moveFrom(other); }
    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) moveFrom(other);
        return *this;
    }


    // from file (png/jpg/бн)
    bool LoadFromFiles(const std::string& path, bool generateMips = true, bool sRGB = false, bool flipY = true);

    bool CreateEmpty(int w, int h, 
        GLenum internalFormat = GL_RGBA8,
        GLenum format = GL_RGBA,
		GLenum type = GL_UNSIGNED_BYTE, 
		bool generateMips = false);

    void bind(int unit = 0) const {
        M_ASSERT(id_ != 0, "Failed to bind Assert");
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id_);
    }


    GLuint id()        const { return id_; }
    int    width()     const { return width_; }
    int    height()    const { return height_; }
    int    channels()  const { return channels_; }
    bool   valid()     const { return id_ != 0; }
    bool   hasMips()   const { return hasMips_; }

    void destroy();

private:
    GLuint id_ = 0;
    int    width_ = 0;
    int    height_ = 0;
    int    channels_ = 0;
    bool   hasMips_ = false;

    void moveFrom(Texture& o) {
        id_ = o.id_; o.id_ = 0;
        width_ = o.width_;  height_ = o.height_;
        channels_ = o.channels_;
        hasMips_ = o.hasMips_;
        o.width_ = o.height_ = o.channels_ = 0;
        o.hasMips_ = false;
    }


    bool uploadPixels(const unsigned char* pixels,
        int w, int h, int comp,
        bool generateMips,
        bool sRGB);

};
