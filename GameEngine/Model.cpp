#include "EngineCore.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderSystem.h"
#include "ASEReader.h"

namespace
{
	void ComputeTangents(std::vector<Vertex>& vertices,
		const std::vector<unsigned int>& indices)
	{
		for (auto& v : vertices)
			v.tangent = glm::vec3(0.0f);

		for (size_t i = 0; i + 2 < indices.size(); i += 3)
		{
			Vertex& v0 = vertices[indices[i]];
			Vertex& v1 = vertices[indices[i + 1]];
			Vertex& v2 = vertices[indices[i + 2]];

			glm::vec3 edge1 = v1.position - v0.position;
			glm::vec3 edge2 = v2.position - v0.position;

			glm::vec2 deltaUV1 = v1.texCoords - v0.texCoords;
			glm::vec2 deltaUV2 = v2.texCoords - v0.texCoords;

			float det = (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
			if (fabs(det) < 1e-6f) {
				continue; 
			}
			float f = 1.0f / det;

			glm::vec3 tangent;
			tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			v0.tangent += tangent;
			v1.tangent += tangent;
			v2.tangent += tangent;
		}

		for (auto& v : vertices)
		{
			if (glm::length(v.tangent) > 0.0f)
				v.tangent = glm::normalize(v.tangent);
			else
				v.tangent = glm::vec3(1.0f, 0.0f, 0.0f);
		}
	}
}

Model::~Model()
{
	for (auto& sm : subMeshes) {
		delete sm.mesh;
		delete sm.material;
	}

	subMeshes.clear();
}

static std::string GetDirectoryFromPath(const std::string& path)
{
	const size_t pos = path.find_last_of("\\/");
	if (pos == std::string::npos) {
		return std::string();
	}
	return path.substr(0, pos + 1);
}

bool Model::LoadFromOBJ(const std::string& path, class Shader* defaultShader)
{
	objl::Loader objLoader;
	if (!objLoader.LoadFile(path)) {
		// Optional assets (e.g. Mixamo models not shipped in the repo) may be missing.
		// Warn and skip instead of aborting the whole program.
		std::cerr << "WARNING: Failed to load OBJ file '" << path << "', skipping.\n";
		return false;
	}

	sourcePath = path;
	std::string baseDir = GetDirectoryFromPath(path);
	subMeshes.clear();

	for (const auto& srcMesh : objLoader.LoadedMeshes)
	{
		// 1. Create Mesh
		std::vector<Vertex> vertexData;
		std::vector<unsigned int> indices = srcMesh.Indices;

		for (const auto& v : srcMesh.Vertices)
		{
			Vertex vert{};
			vert.position = glm::vec3(v.Position.X, v.Position.Y, v.Position.Z);
			vert.normal = glm::vec3(v.Normal.X, v.Normal.Y, v.Normal.Z);
			vert.texCoords = glm::vec2(v.TextureCoordinate.X, v.TextureCoordinate.Y);

			vertexData.push_back(vert);
		}

		ComputeTangents(vertexData, indices);

		Mesh* mesh = new Mesh();
		mesh->SetData(vertexData, indices);


		// 2. Create Material
		Material* mat = new Material(defaultShader);
		const objl::Material& srcMat = srcMesh.MeshMaterial;

		// Colors
		glm::vec3 Ka(1.0f);
		glm::vec3 Kd(1.0f);
		glm::vec3 Ks(0.0f);
		glm::vec3 Ke(0.0f);

		Ka = glm::vec3(srcMat.Ka.X, srcMat.Ka.Y, srcMat.Ka.Z);
		Kd = glm::vec3(srcMat.Kd.X, srcMat.Kd.Y, srcMat.Kd.Z);
		Ks = glm::vec3(srcMat.Ks.X, srcMat.Ks.Y, srcMat.Ks.Z);

		mat->diffuseColor = Kd;
		mat->specularColor = Ks;

		mat->shininess = (srcMat.Ns > 0.0f) ? srcMat.Ns : 32.0f;

		float ksAvg = (Ks.r + Ks.g + Ks.b) / 3.0f;
		mat->specularStrength = ksAvg;

		float keLen = glm::length(Ke);
		if (keLen > 0.0f)
		{
			mat->emissionStrength = keLen;
		}


		if (!srcMat.map_Kd.empty()) {
			std::string texPath = baseDir + srcMat.map_Kd;
			Texture* tex = new Texture();
			if (tex->LoadFromFiles(texPath, true, true, true)) {
				mat->diffuseTexture = tex;
			}
			else {
				delete tex;
			}
		}

		// Base color. The shader only reads tint, so Kd was ignored before (Ka was used instead).
		// Textured materials show the texture as-is; untextured ones (e.g. CC0 low-poly packs
		// that color meshes via Kd) take their color from Kd.
		mat->tint = mat->diffuseTexture ? glm::vec3(1.0f) : Kd;
		 
		if (!srcMat.map_Ks.empty())
		{
			std::string texPath = baseDir + srcMat.map_Ks;
			Texture* tex = new Texture();
			if (tex->LoadFromFiles(texPath, true, true, true))
				mat->specularTexture = tex;
			else
				delete tex;
		}

		// for bump/normal, use srcMat.map_bump
		if (!srcMat.map_bump.empty())
		{
			std::string texPath = baseDir + srcMat.map_bump;
			Texture* tex = new Texture();
			if(tex->LoadFromFiles(texPath, true, true, true))
				mat->normalTexture = tex;
			else
				delete tex;
		}

		// 3. push SubMesh
		SubMesh sm;
		sm.mesh = mesh;
		sm.material = mat;
		subMeshes.push_back(sm);
	}

	return !subMeshes.empty();
}

void Model::Draw() const
{
	for (const auto& sm : subMeshes) {
		if (!sm.mesh || !sm.material) {
			continue;
		}
		Shader* overrideShader = RenderSystem::GetInstance().GetDefaultShader();
		sm.material->Apply(overrideShader);
		sm.mesh->Draw();
	}
}

// 假设：和 OBJ 一样，Model 里有：
// std::vector<SubMesh> subMeshes;
// std::string sourcePath;
// void ComputeTangents(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
// std::string GetDirectoryFromPath(const std::string& path);

bool Model::LoadFromASE(const std::string& path, class Shader* defaultShader)
{
    // ASEReader does not handle a missing file (it would throw on an empty material list).
    if (!std::filesystem::exists(path)) {
        std::cerr << "WARNING: Failed to load ASE file '" << path << "', skipping.\n";
        return false;
    }

    ASEReader ase;
    ase.ParseASEFile(path.c_str());

    sourcePath = path;
    subMeshes.clear();

    std::string baseDir = GetDirectoryFromPath(path);
    if (!baseDir.empty() && (baseDir.back() != '/' && baseDir.back() != '\\'))
        baseDir.push_back('/');

	// -----------1. Convert ASE Materials to Engine Materials ----------
	// Index = ASEReader::Material.Index
    std::vector<Material*> engineMaterials;
    engineMaterials.resize(ase.Materials.size(), nullptr);

    auto loadTextureIfAny = [&](const std::string& texFile, Texture*& outTex)
        {
            if (texFile.empty())
                return;

            std::string texPath = texFile;
            
            if (texPath.find(':') == std::string::npos &&
                !(texPath.size() > 0 && (texPath[0] == '/' || texPath[0] == '\\')))
            {
                texPath = baseDir + texPath;
            }

            Texture* tex = new Texture();
            if (tex->LoadFromFiles(texPath, true, true, true))
            {
                outTex = tex;
            }
            else
            {
                delete tex;
            }
        };

    for (ASEReader::Material* srcMat : ase.Materials)
    {
        if (!srcMat) continue;

        Material* mat = new Material(defaultShader);

        // 颜色
        glm::vec3 Ka = srcMat->AmbientColor;
        glm::vec3 Kd = srcMat->DiffuseColor;
        glm::vec3 Ks = srcMat->SpecularColor;

        mat->tint = Ka;
        mat->diffuseColor = Kd;
        mat->specularColor = Ks;

        // gloss/shine
        mat->shininess = (srcMat->Shine > 0.0f) ? srcMat->Shine : 32.0f;

        float ksAvg = (Ks.r + Ks.g + Ks.b) / 3.0f;
        mat->specularStrength = ksAvg;

        // 自发光，用 SelfIllum 做一个强度
        mat->emissionStrength = srcMat->SelfIllum;

        // 贴图：ASEReader::Map.Name = "DIFFUSE"/"SPECULAR"/"BUMP"
        for (const ASEReader::Map& m : srcMat->Maps)
        {
            if (m.TextureFileName.empty())
                continue;

            if (m.Name == "DIFFUSE")
            {
                loadTextureIfAny(m.TextureFileName, mat->diffuseTexture);
            }
            else if (m.Name == "SPECULAR")
            {
                loadTextureIfAny(m.TextureFileName, mat->specularTexture);
            }
            else if (m.Name == "BUMP")
            {
                loadTextureIfAny(m.TextureFileName, mat->normalTexture);
            }
        }

        if (srcMat->Index >= 0 && (size_t)srcMat->Index < engineMaterials.size())
        {
            engineMaterials[srcMat->Index] = mat;
        }
        else
        {
            engineMaterials.push_back(mat);
        }
    }

	// -----------2. Convert ASE GeoObjects to SubMeshes ----------
    for (ASEReader::GeoObject* g : ase.GeoObjects)
    {
        if (!g) continue;
        const ASEReader::MeshInfo& mi = g->MeshI;

        // 采用 每个三角形 3 个顶点 的方式展开，保证 UV/Normal 对齐简单
        std::vector<Vertex> vertexData;
        std::vector<unsigned int> indices;

        const auto& positions = mi.Vertices;
        const auto& texVertices = mi.TexVertices; // x = u, y = v
        const auto& faces = mi.Faces;       // 每个 ivec3，顶点索引
        const auto& texFaces = mi.TexFaces;    // 每个 ivec3，对应 texVertices 的索引

        vertexData.reserve(faces.size() * 3);
        indices.reserve(faces.size() * 3);

        for (size_t f = 0; f < faces.size(); ++f)
        {
            glm::ivec3 posIdx(
                static_cast<int>(faces[f].x),
                static_cast<int>(faces[f].y),
                static_cast<int>(faces[f].z));

            glm::ivec3 uvIdx;

            if (f < texFaces.size())
            {
                // 如果 TexFaces 是 vec3，这里转成 ivec3；如果本来就是 ivec3 也没问题
                uvIdx = glm::ivec3(
                    static_cast<int>(texFaces[f].x),
                    static_cast<int>(texFaces[f].y),
                    static_cast<int>(texFaces[f].z));
            }
            else
            {
                uvIdx = posIdx;
            }

            for (int k = 0; k < 3; ++k)
            {
                Vertex v{};
                int pi = 0;
                if (k == 0) pi = posIdx.x;
                if (k == 1) pi = posIdx.y;
                if (k == 2) pi = posIdx.z;

                int ti = 0;
                if (k == 0) ti = uvIdx.x;
                if (k == 1) ti = uvIdx.y;
                if (k == 2) ti = uvIdx.z;

                // 位置
                if (pi >= 0 && (size_t)pi < positions.size())
                {
                    v.position = positions[pi];
                }
                else
                {
                    v.position = glm::vec3(0.0f);
                }

                // UV
                if (!texVertices.empty() && ti >= 0 && (size_t)ti < texVertices.size())
                {
                    glm::vec3 tv = texVertices[ti];
                    v.texCoords = glm::vec2(tv.x, tv.y); // 如果需要可以做 1 - v.y 翻转
                }
                else
                {
                    v.texCoords = glm::vec2(0.0f);
                }

                // Normal 先设零，后面用面法线累加
                v.normal = glm::vec3(0.0f);

                vertexData.push_back(v);
                indices.push_back(static_cast<unsigned int>(vertexData.size() - 1));
            }
        }

		// -----------3. Calculate Normals -----------
        for (size_t i = 0; i + 2 < indices.size(); i += 3)
        {
            unsigned int i0 = indices[i];
            unsigned int i1 = indices[i + 1];
            unsigned int i2 = indices[i + 2];

            glm::vec3 p0 = vertexData[i0].position;
            glm::vec3 p1 = vertexData[i1].position;
            glm::vec3 p2 = vertexData[i2].position;

            glm::vec3 e1 = p1 - p0;
            glm::vec3 e2 = p2 - p0;
            glm::vec3 n = glm::normalize(glm::cross(e1, e2));

            vertexData[i0].normal += n;
            vertexData[i1].normal += n;
            vertexData[i2].normal += n;
        }

        for (auto& v : vertexData)
        {
            if (glm::length(v.normal) > 0.0f)
                v.normal = glm::normalize(v.normal);
            else
                v.normal = glm::vec3(0, 1, 0);
        }

        // ---------- 4. 计算切线 ----------
        ComputeTangents(vertexData, indices);

        // ---------- 5. 创建 Mesh ----------
        Mesh* mesh = new Mesh();
        mesh->SetData(vertexData, indices);

        // ---------- 6. 选择对应 Material ----------
        Material* mat = nullptr;
        if (g->MaterialID >= 0 && (size_t)g->MaterialID < engineMaterials.size())
        {
            mat = engineMaterials[g->MaterialID];
        }

        if (!mat)
        {
            // 没有对应材质的话就用一个默认的
            mat = new Material(defaultShader);
        }

        // ---------- 7. 组装 SubMesh ----------
        SubMesh sm;
        sm.mesh = mesh;
        sm.material = mat;
        subMeshes.push_back(sm);
    }

    return !subMeshes.empty();
}