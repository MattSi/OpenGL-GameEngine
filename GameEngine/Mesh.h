#pragma once

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	Mesh(const Mesh&) = delete;	
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept;
	Mesh& operator=(Mesh&& other) noexcept;

	void SetData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	void Draw() const;

	void DrawInstanced(unsigned int instanceCount) const;
	inline unsigned int GetVertexCount() const { return m_vertexCount; }
	inline unsigned int GetIndexCount() const { return m_indexCount; }

private:
	unsigned int m_vao = 0;
	unsigned int m_vbo = 0;
	unsigned int m_ebo = 0;

	unsigned int m_indexCount = 0;
	unsigned int m_vertexCount = 0;

	void CreateBuffers();
	void DestroyBuffers();
};

