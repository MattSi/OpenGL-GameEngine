#pragma once
class ASEReader
{
public:
	struct Map
	{
		std::string Name;
		std::string TextureFileName;
		float UVW_U_Offset;
		float UVW_V_Offset;
		float UVW_U_Tiling;
		float UVW_V_Tiling;	
	};

	struct Material
	{
		int Index;
		std::string Name;
		std::string Class;
		glm::vec3 AmbientColor;
		glm::vec3 DiffuseColor;
		glm::vec3 SpecularColor;
		float Shine;
		float ShineStrength;
		float Transparency;
		float WireSize;
		std::string Shading;
		float XP_Falloff;
		float SelfIllum;
		std::vector<ASEReader::Map> Maps;
	};

	struct MeshInfo
	{
		int TimeValue;
		int NumVertex;
		std::vector<glm::vec3> Vertices;
		int NumFaces;
		std::vector<glm::ivec3> Faces;

		int NumTexVertex;
		std::vector<glm::vec3> TexVertices;
		int NumTVFaces;
		std::vector<glm::vec3> TexFaces;

		std::vector<glm::vec3> VertexNormals;
	};

	struct GeoObject
	{
		std::string Name;
		MeshInfo MeshI;
		int MaterialID;
	};

public:
	// Methods
	void ParseASEFile(const char* _fileName);

	// Members
	std::string FileName;
	std::vector<ASEReader::Material*> Materials;
	std::vector<ASEReader::GeoObject*> GeoObjects;

private:
	void ParseMaterials(std::vector<std::string>& _file);
	void ParseGeoObjects(std::vector<std::string>& _file);
	void ReadFile(const char* _fileName, std::vector<std::string>& _file);

	int GetBlock(const char* _tag, std::vector<std::string>& _lines, std::vector<std::string>& _block, int _start = 0);
	void FindAllTags(const char* _tag, std::vector<std::string>& _lines, std::vector<std::string>& _block);
	std::string ExtractValue(std::vector<std::string>& _lines, const char* _tag);
	glm::vec3 ParseStringToVec3(std::string _values);
	void ParseMap(const char* _mapName, const char* _name, std::vector<std::string>& _materialBlock, ASEReader::Material* _material);
	void ExtractVectorList(std::vector<glm::vec3>& _list, std::vector<std::string>& _vertexList);
};

