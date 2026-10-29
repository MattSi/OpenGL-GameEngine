#include "EngineCore.h"
#include "ASEReader.h"

void ASEReader::ParseASEFile(const char* _fileName)
{
	std::vector<std::string> ASEFile;
	ReadFile(_fileName, ASEFile);

	ParseMaterials(ASEFile);
	ParseGeoObjects(ASEFile);
}

void ASEReader::ReadFile(const char* _fileName, std::vector<std::string>& _file)
{
	std::string line;
	std::ifstream file(_fileName);
	while (getline(file, line))
	{
		_file.push_back(line);
	}
	file.close();
}

void ASEReader::ParseMaterials(std::vector<std::string>& _file)
{
	std::vector<std::string> _materials;
	GetBlock("*MATERIAL_LIST", _file, _materials);

	int materialCount = std::stoi(ExtractValue(_materials, "*MATERIAL_COUNT "));
	for (int count = 0; count < materialCount; count++)
	{
		std::string materialTag = std::string("*MATERIAL ") + std::to_string(count);
		std::vector<std::string> _materialBlock;
		GetBlock(materialTag.c_str(), _materials, _materialBlock);

		ASEReader::Material* material = new ASEReader::Material();

		material->Index = count;
		material->Name = ExtractValue(_materialBlock, "*MATERIAL_NAME ");
		material->Class = ExtractValue(_materialBlock, "*MATERIAL_CLASS ");
		material->AmbientColor = ParseStringToVec3(ExtractValue(_materialBlock, "*MATERIAL_AMBIENT "));
		material->DiffuseColor = ParseStringToVec3(ExtractValue(_materialBlock, "*MATERIAL_DIFFUSE "));
		material->SpecularColor = ParseStringToVec3(ExtractValue(_materialBlock, "*MATERIAL_SPECULAR "));
		material->Shine = std::stof(ExtractValue(_materialBlock, "*MATERIAL_SHINE "));
		material->ShineStrength = std::stof(ExtractValue(_materialBlock, "*MATERIAL_SHINESTRENGTH "));
		material->Transparency = std::stof(ExtractValue(_materialBlock, "*MATERIAL_TRANSPARENCY "));
		material->WireSize = std::stof(ExtractValue(_materialBlock, "*MATERIAL_WIRESIZE "));
		material->Shading = ExtractValue(_materialBlock, "*MATERIAL_SHADING ");

		ParseMap("*MAP_DIFFUSE", "DIFFUSE", _materialBlock, material);
		ParseMap("*MAP_SPECULAR", "SPECULAR", _materialBlock, material);
		ParseMap("*MAP_BUMP", "BUMP", _materialBlock, material);

		Materials.push_back(material);
	}
}

void ASEReader::ParseGeoObjects(std::vector<std::string>& _file)
{
	std::vector<std::string> _geomObjects;
	int index = GetBlock("*GEOMOBJECT", _file, _geomObjects);

	while (index != -1) {
		GeoObject* g = new GeoObject();

		g->Name = ExtractValue(_geomObjects, "*NODE_NAME ");
		g->MaterialID = std::stoi(ExtractValue(_geomObjects, "*MATERIAL_REF"));
		g->MeshI.TimeValue = std::stoi(ExtractValue(_geomObjects, "*TIMEVALUE "));
		g->MeshI.NumVertex = std::stoi(ExtractValue(_geomObjects, "*MESH_NUMVERTEX "));
		g->MeshI.NumFaces = std::stoi(ExtractValue(_geomObjects, "*MESH_NUMFACES "));

		std::vector<std::string> dataBlock;

		FindAllTags("*MESH_VERTEX ", _geomObjects, dataBlock);
		ExtractVectorList(g->MeshI.Vertices, dataBlock);

		FindAllTags("*MESH_TVERT ", _geomObjects, dataBlock);
		ExtractVectorList(g->MeshI.TexVertices, dataBlock);

		FindAllTags("*MESH_TFACE ", _geomObjects, dataBlock);
		ExtractVectorList(g->MeshI.TexFaces, dataBlock);

		FindAllTags("*MESH_VERTEXNORMAL ", _geomObjects, dataBlock);
		ExtractVectorList(g->MeshI.VertexNormals, dataBlock);

		FindAllTags("*MESH_FACE ", _geomObjects, dataBlock);
		for (auto& face : dataBlock)
		{
			std::replace(face.begin(), face.end(), '\t', ' ');
			face.erase(0, face.find_first_not_of(' '));

			std::vector<std::string> result;
			std::stringstream ss(face);
			std::string item = "";

			while (std::getline(ss, item, ' '))
			{
				if (item != "") {
					result.push_back(item);
				}
			}

			g->MeshI.Faces.push_back(glm::vec3{
				std::stof(result[3]),
				std::stof(result[5]),
				std::stof(result[7])
				});
		}

		GeoObjects.push_back(g);
		index = GetBlock("*GEOMOBJECT ", _file, _geomObjects, index + 1);
	}
}

int ASEReader::GetBlock(const char* _tag, std::vector<std::string>& _lines, std::vector<std::string>& _block, int _start)
{
	_block.clear();

	for (int i = _start; i < _lines.size(); i++) {
		if (_lines[i].find(_tag) != std::string::npos) {
			int indent = 0;

			i++;

			for (int j = i; j < _lines.size(); j++) {
				if(_lines[j].find('{') != std::string::npos) {
					indent++;
				} else if (_lines[j].find('}') != std::string::npos) {
					if (indent == 0) {
						return i;
					}
					indent--;
				}
				_block.push_back(_lines[j]);
			}
		}
	}

	return -1;
}

std::string ASEReader::ExtractValue(std::vector<std::string>& _lines, const char* _tag)
{
	for (int i = 0; i < _lines.size(); i++) {
		if (_lines[i].find(_tag) != std::string::npos) {
			std::string line = _lines[i];

			//replace all the tabs with spaces
			std::replace(line.begin(), line.end(), '\t', ' ');
			// Remove the leading spaces
			line.erase(0, line.find_first_not_of(" \n\r\t"));
			// remove the tag, we can search for the next whitespace
			line.erase(0, line.find(" ") + 1);
			// remove the tag, we can search for the next whitespace
			line.erase(std::remove(line.begin(), line.end(), '\"'), line.end());
			// remove the leading space
			line.erase(0, line.find_first_not_of(" \n\r\t"));

			return line;
		}
	}
	return std::string();
}

void ASEReader::FindAllTags(const char* _tag, std::vector<std::string>& _lines, std::vector<std::string>& _block)
{
	_block.clear();
	for (int i = 0; i < _lines.size(); i++) {
		if (_lines[i].find(_tag) != std::string::npos) {
			_block.push_back(_lines[i]);
		}
	}
}

glm::vec3 ASEReader::ParseStringToVec3(std::string _values)
{
	std::vector<std::string> result;
	std::stringstream ss(_values);
	std::string item = "";

	while (getline(ss, item, ' '))
	{
		result.push_back(item);
	}

	return glm::vec3{std::stof(result[0]), std::stof(result[1]), std::stof(result[2])};
}

void ASEReader::ParseMap(const char* _mapName, const char* _name, std::vector<std::string>& _materialBlock, ASEReader::Material* _material)
{
	std::vector<std::string> _mapBlock;
	GetBlock(_mapName, _materialBlock, _mapBlock);

	if(_mapBlock.size() > 0) {
		ASEReader::Map map;
		map.Name = _name;
		map.TextureFileName = ExtractValue(_mapBlock, "*BITMAP ");
		map.UVW_U_Offset = std::stof(ExtractValue(_mapBlock, "*UVW_U_OFFSET "));
		map.UVW_V_Offset = std::stof(ExtractValue(_mapBlock, "*UVW_V_OFFSET "));
		map.UVW_U_Tiling = std::stof(ExtractValue(_mapBlock, "*UVW_U_TILING "));
		map.UVW_V_Tiling = std::stof(ExtractValue(_mapBlock, "*UVW_V_TILING "));
		_material->Maps.push_back(map);
	}

}

void ASEReader::ExtractVectorList(std::vector<glm::vec3>& _list, std::vector<std::string>& _vertexList)
{
	for (auto& vertex : _vertexList) {
		std::replace(vertex.begin(), vertex.end(), '\t', ' ');
		vertex.erase(0, vertex.find_first_not_of(' '));

		std::vector<std::string> result;
		std::stringstream ss(vertex);
		std::string item = "";

		while(getline(ss, item, ' '))
		{
			if (item != "") {
				result.push_back(item);
			}
		}
		_list.push_back(glm::vec3{
			std::stof(result[2]),
			std::stof(result[3]),
			std::stof(result[4])
			});
	}
}
