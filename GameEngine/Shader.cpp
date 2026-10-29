#include "EngineCore.h"
#include "Shader.h"


Shader::~Shader()
{
	destroy();
}

bool Shader::LoadFromFiles(const std::string& vsPath, const std::string& fsPath)
{
	std::string vs, fs;

	M_ASSERT(loadFile(vsPath, vs), ("[Shader] Failed to read VS: " + vsPath).c_str());
	M_ASSERT(loadFile(fsPath, fs), ("[Shader] Failed to read FS: " + fsPath).c_str());

	return LoadFromSource(vs, fs);
}

bool Shader::LoadFromSource(const std::string& vsSrc, const std::string& fsSrc)
{
	destroy();
	return build(vsSrc, fsSrc);
}

bool Shader::build(const std::string& vsSrc, const std::string& fsSrc)
{
	GLuint vs = compile(GL_VERTEX_SHADER, vsSrc);
	M_ASSERT(vs != 0, "Failed to compile vertex shader");
	GLuint fs = compile(GL_FRAGMENT_SHADER, fsSrc);
	M_ASSERT(fs != 0, "Failed to compile fragment shader");

	if (!link(vs, fs)) {
		glDeleteShader(vs);
		glDeleteShader(fs);
		return false;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	return true;
}

GLuint Shader::compile(GLenum type, const std::string& src) const
{
	GLuint shader = glCreateShader(type);
	const char* s = src.c_str();
	glShaderSource(shader, 1, &s, nullptr);
	glCompileShader(shader);
	if (!checkShader(shader)) {
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

bool Shader::link(GLuint vs, GLuint fs)
{
	uniformCache.clear();
	programID = glCreateProgram();
	glAttachShader(programID, vs);
	glAttachShader(programID, fs);
	glLinkProgram(programID);

	if (!checkProgram(programID)) {
		glDeleteProgram(programID);
		programID = 0;
		return false;
	}
	return true;
}

 
bool Shader::checkShader(GLuint shader)
{
	GLint status = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		GLint logLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		std::vector<char> errorMessage(logLen > 1 ? logLen : 1);
		glGetShaderInfoLog(shader, logLen, nullptr, errorMessage.data());
		M_ASSERT(false, errorMessage.data());
		return false;
	}
	return true;
}
 
bool Shader::checkProgram(GLuint program)
{
	GLint status = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status)
	{
		GLint logLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		std::vector<char> errorMessage(logLen > 1 ? logLen : 1);
		glGetProgramInfoLog(program, logLen, nullptr, errorMessage.data());
		M_ASSERT(false, errorMessage.data());
		return false;
	}
	return true;
}
 
 

void Shader::destroy()
{
	uniformCache.clear();
	if (programID != 0) {
		glDeleteProgram(programID);
		programID = 0;
	}
}


GLint Shader::getUniformLocation(const char* name) const
{
	auto it = uniformCache.find(name);
	if (it != uniformCache.end()) {
		return it->second;
	}
	GLint loc = glGetUniformLocation(programID, name);
	uniformCache.emplace(name, loc);
	return loc;
}

void Shader::SetVec4(const char* name, const glm::vec4& value) const
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0) {
		glUniform4fv(loc, 1, &value[0]);
	}
}

void Shader::SetMat2(const char* name, const glm::mat2& mat) const
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0) {
		glUniformMatrix2fv(loc, 1, GL_FALSE, &mat[0][0]);
	}
}

void Shader::SetMat3(const char* name, const glm::mat3& mat) const
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0) {
		glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
	}
}

void Shader::SetMat4(const char* name, const glm::mat4& mat) const
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
	}
}

void Shader::SetVec3(const char* name, const glm::vec3& value) const
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0) {
		glUniform3fv(loc, 1, &value[0]);
	}
}



void Shader::SetVec2(const char* name, const glm::vec2& value) const
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0) {
		glUniform2fv(loc, 1, &value[0]);
	}
}

void Shader::SetInt(const char* name, int value) const
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0) {
		glUniform1i(loc, value);
	}
}

void Shader::SetFloat(const char* name, float value) const
{
	GLint loc = getUniformLocation(name);
	if (loc >= 0) {
		glUniform1f(loc, value);
	}
}
void Shader::bindTexture2D(const char* samplerName, GLuint tex, int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, tex);
	SetInt(samplerName, unit); // e.g. SetInt("uTex", 0);
}


bool Shader::loadFile(const std::string& path, std::string& out) const
{
	std::ifstream ifs(path, std::ios::binary);
	if (!ifs) return false;
	std::ostringstream oss;
	oss << ifs.rdbuf();
	out = oss.str();
	return true;
}

