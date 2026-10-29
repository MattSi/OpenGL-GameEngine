#pragma once

class Mesh;
class Material;
class Shader;

struct SubMesh
{
	Mesh* mesh = nullptr;
	Material* material = nullptr;
};

class Model
{
public:
	Model() = default;
	~Model();

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	bool LoadFromOBJ(const std::string& path, class Shader* defaultShader);

	bool LoadFromASE(const std::string& path, class Shader* defaultShader);

	void Draw() const;

	const std::string& GetSourcePath() const {
		return sourcePath;
	}
	const std::vector<SubMesh>& GetSubMeshes() const {
		return subMeshes;
	}

private:
	std::string sourcePath;
	std::vector<SubMesh> subMeshes;
	
};

