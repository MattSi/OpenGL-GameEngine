#include "EngineCore.h"
#include "Mesh.h"


Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	DestroyBuffers();
}

Mesh::Mesh(Mesh&& other) noexcept
{
	// Move constructor
	m_vao = other.m_vao;
	m_vbo = other.m_vbo;
	m_ebo = other.m_ebo;
	m_indexCount = other.m_indexCount;
	m_vertexCount = other.m_vertexCount;
	other.m_vao = 0;
	other.m_vbo = 0;
	other.m_ebo = 0;
	other.m_indexCount = 0;
	other.m_vertexCount = 0;

}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	// TODO: insert return statement here
	if (this != &other) {
		DestroyBuffers();
		m_vao = other.m_vao;
		m_vbo = other.m_vbo;
		m_ebo = other.m_ebo;
		m_indexCount = other.m_indexCount;
		m_vertexCount = other.m_vertexCount;
		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ebo = 0;
		other.m_indexCount = 0;
		other.m_vertexCount = 0;
	}

	return *this;
}


void Mesh::CreateBuffers()
{
	if (m_vao == 0) {
		// Generate and bind VAO, VBO, EBO
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ebo);
	}
}

void Mesh::DestroyBuffers()
{
	if (m_ebo != 0) {
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
	}
	if (m_vbo != 0) {
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
	}
	if (m_vao != 0) {
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	

	m_vertexCount = 0;
	m_indexCount = 0;
	
}

void Mesh::SetData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	m_vertexCount = static_cast<unsigned int>(vertices.size());
	m_indexCount = static_cast<unsigned int>(indices.size());

	if(m_vertexCount == 0 || m_indexCount == 0) {
		DestroyBuffers();
		return;
	}  
	     
	CreateBuffers();

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	// Locaton 0: position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

	// Location 1: normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	// Location 2: texCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoords)));

	// Location 3: tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));


	glBindVertexArray(0);
}
  

void Mesh::Draw() const
{
	if(m_vao == 0 || m_indexCount == 0) {
		return;
	}
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::DrawInstanced(unsigned int instanceCount) const
{
	if (m_vao == 0 || instanceCount == 0)
		return;

	glBindVertexArray(m_vao);

	if (m_indexCount > 0)
	{
		glDrawElementsInstanced(
			GL_TRIANGLES,
			m_indexCount,
			GL_UNSIGNED_INT,
			nullptr,
			instanceCount
		);
	}
	else
	{
		glDrawArraysInstanced(
			GL_TRIANGLES,
			0,
			m_vertexCount,
			instanceCount
		);
	}

	glBindVertexArray(0);
}

