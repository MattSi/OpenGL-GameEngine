#include "EngineCore.h"
#include "PrimitiveMeshes.h"

static Mesh* s_CubeMesh = nullptr;
static Mesh* s_SphereMesh = nullptr;
static Mesh* s_FullscreenQuadMesh = nullptr;

namespace PrimitiveMeshes {

	Mesh* GetCubeMesh() {
		if (s_CubeMesh) {
			return s_CubeMesh;
		}

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

        auto v = [&](float x, float y, float z,
            float nx, float ny, float nz,
            float u, float t) {
                Vertex vert;
                vert.position = glm::vec3(x, y, z);
                vert.normal = glm::vec3(nx, ny, nz);
                vert.texCoords = glm::vec2(u, t);
                vertices.push_back(vert);
            };
         
        float h = 0.5f;

        // Front (+Z)
        v(-h, -h, h, 0, 0, 1, 0, 0);
        v(h, -h, h, 0, 0, 1, 1, 0);
        v(h, h, h, 0, 0, 1, 1, 1);
        v(-h, h, h, 0, 0, 1, 0, 1);

        // Back (-Z)
        v(h, -h, -h, 0, 0, -1, 0, 0);
        v(-h, -h, -h, 0, 0, -1, 1, 0);
        v(-h, h, -h, 0, 0, -1, 1, 1);
        v(h, h, -h, 0, 0, -1, 0, 1);

        // Left (-X)
        v(-h, -h, -h, -1, 0, 0, 0, 0);
        v(-h, -h, h, -1, 0, 0, 1, 0);
        v(-h, h, h, -1, 0, 0, 1, 1);
        v(-h, h, -h, -1, 0, 0, 0, 1);

        // Right (+X)
        v(h, -h, h, 1, 0, 0, 0, 0);
        v(h, -h, -h, 1, 0, 0, 1, 0);
        v(h, h, -h, 1, 0, 0, 1, 1);
        v(h, h, h, 1, 0, 0, 0, 1);

        // Top (+Y)
        v(-h, h, h, 0, 1, 0, 0, 0);
        v(h, h, h, 0, 1, 0, 1, 0);
        v(h, h, -h, 0, 1, 0, 1, 1);
        v(-h, h, -h, 0, 1, 0, 0, 1);

        // Bottom (-Y)
        v(-h, -h, -h, 0, -1, 0, 0, 0);
        v(h, -h, -h, 0, -1, 0, 1, 0);
        v(h, -h, h, 0, -1, 0, 1, 1);
        v(-h, -h, h, 0, -1, 0, 0, 1);

        auto addFace = [&](unsigned int start) {
            indices.push_back(start + 0);
            indices.push_back(start + 1);
            indices.push_back(start + 2);
            indices.push_back(start + 2);
            indices.push_back(start + 3);
            indices.push_back(start + 0);
            };

        addFace(0);   // front
        addFace(4);   // back
        addFace(8);   // left
        addFace(12);  // right
        addFace(16);  // top
        addFace(20);  // bottom

        s_CubeMesh = new Mesh();
        s_CubeMesh->SetData(vertices, indices);
        return s_CubeMesh;
	}

	Mesh* GetSphereMesh() {
        if (s_SphereMesh)
            return s_SphereMesh;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        float radius = 0.5f;
        const unsigned int sectorCount = 32;
        const unsigned int stackCount = 16;

        for (unsigned int i = 0; i <= stackCount; ++i)
        {
            float stackAngle = glm::pi<float>() / 2 - glm::pi<float>() * (float)i / (float)stackCount; // +pi/2 .. -pi/2
            float xy = radius * std::cos(stackAngle);
            float y = radius * std::sin(stackAngle);

            for (unsigned int j = 0; j <= sectorCount; ++j)
            {
                float sectorAngle = 2.0f * glm::pi<float>() * (float)j / (float)sectorCount;

                float x = xy * std::cos(sectorAngle);
                float z = xy * std::sin(sectorAngle);

                glm::vec3 pos(x, y, z);
                glm::vec3 normal = glm::normalize(pos);
                float u = (float)j / (float)sectorCount;
                float v = (float)i / (float)stackCount;

                Vertex vert;
                vert.position = pos;
                vert.normal = normal;
                vert.texCoords = glm::vec2(u, v);
                vertices.push_back(vert);
            }
        }

        for (unsigned int i = 0; i < stackCount; ++i)
        {
            unsigned int k1 = i * (sectorCount + 1);
            unsigned int k2 = k1 + sectorCount + 1;

            for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        s_SphereMesh = new Mesh();
        s_SphereMesh->SetData(vertices, indices);
        return s_SphereMesh;
	}

    Mesh* GetFullscreenQuad() {
        if(s_FullscreenQuadMesh)
			return s_FullscreenQuadMesh;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;


        auto v = [&](float x, float y, float u, float v)
            {
                Vertex vert;
                vert.position = glm::vec3(x, y, 0.0f);
                vert.normal = glm::vec3(0, 0, 1);      
                vert.texCoords = glm::vec2(u, v);
                vertices.push_back(vert);
            };

        // Fullscreen quad covering [-1,1]
        //
        //   (-1,1) -------- (1,1)
        //      |             |
        //      |             |
        //   (-1,-1) ------- (1,-1)
        //
        v(-1.f, -1.f, 0.f, 0.f);
        v(1.f, -1.f, 1.f, 0.f);
        v(1.f, 1.f, 1.f, 1.f);
        v(-1.f, 1.f, 0.f, 1.f);

        indices = {
            0, 1, 2,
            2, 3, 0
        };

        s_FullscreenQuadMesh = new Mesh();
		s_FullscreenQuadMesh->SetData(vertices, indices);

        return s_FullscreenQuadMesh;
    }
}