#pragma once
#include <unordered_map>

class Shader
{
public:
	Shader() = default;
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;


	bool LoadFromFiles(const std::string& vsPath, const std::string& fsPath);
	bool LoadFromSource(const std::string& vsSrc, const std::string& fsSrc);

	void destroy();
	void use() const { glUseProgram(programID); }
	GLuint id() const { return programID; }


	void SetBool(const char* name, bool value) const { SetInt(name, value ? 1 : 0); }
	void SetInt(const char* name, int value) const;
	void SetFloat(const char* name, float value) const;
	void SetVec2(const char* name, const glm::vec2& value) const;
	void SetVec2(const char* name, float x, float y) const { SetVec2(name, glm::vec2(x, y)); }
	void SetVec3(const char* name, const glm::vec3& value) const;
	void SetVec3(const char* name, float x, float y, float z) const { SetVec3(name, glm::vec3(x, y, z)); }
	void SetVec4(const char* name, const glm::vec4& value) const;
	void SetVec4(const char* name, float x, float y, float z, float w) const { SetVec4(name, glm::vec4(x, y, z, w)); }
	void SetMat2(const char* name, const glm::mat2& mat) const;
	void SetMat3(const char* name, const glm::mat3& mat) const;
	void SetMat4(const char* name, const glm::mat4& mat) const;




	void bindTexture2D(const char* samplerName, GLuint tex, int unit = 0) const;


	bool isValid() const { return programID != 0; }
	std::string name;

private:
	GLuint programID = 0;

	// glGetUniformLocation is slow; cache results per program (-1 is cached too).
	mutable std::unordered_map<std::string, GLint> uniformCache;
	GLint getUniformLocation(const char* name) const;

	// utiils
	bool build(const std::string& vsSrc, const std::string& fsSrc);
	GLuint compile(GLenum type, const std::string& src) const;
	bool link(GLuint vs, GLuint fs);
	bool loadFile(const std::string& path, std::string& out) const;

	static bool checkShader(GLuint shader);
	static bool checkProgram(GLuint program);
	


};

