#pragma once

//defines a mesh for Smok projects
//for importing FBX meshes and such, checkout the "Smok Asset Convertion Tool"

#include <BTDSTD/Wireframe/Pipeline/VertexInputDesc.hpp>
#include <BTDSTD/Wireframe/MeshBuffer.hpp>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Smok::Asset::Mesh
{
	//defines a vertex
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 textureCoords;

		bool operator==(const Vertex& other)
		{
			return (position == other.position && normal == other.normal && color == other.color);
		}

		//generates the vertex input description
		static inline Wireframe::Pipeline::VertexInputDescription GenerateVertexInputDescription()
		{
			Wireframe::Pipeline::VertexInputDescription description;

			//we will have just 1 vertex buffer binding, with a per-vertex rate
			VkVertexInputBindingDescription mainBinding = {};
			mainBinding.binding = 0;
			mainBinding.stride = sizeof(Vertex);
			mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			description.bindings.push_back(mainBinding);

			//Position will be stored at Location 0
			VkVertexInputAttributeDescription positionAttribute = {};
			positionAttribute.binding = 0;
			positionAttribute.location = 0;
			positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
			positionAttribute.offset = offsetof(Vertex, position);

			//Normal will be stored at Location 1
			VkVertexInputAttributeDescription normalAttribute = {};
			normalAttribute.binding = 0;
			normalAttribute.location = 1;
			normalAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
			normalAttribute.offset = offsetof(Vertex, normal);

			//Position will be stored at Location 2
			VkVertexInputAttributeDescription colorAttribute = {};
			colorAttribute.binding = 0;
			colorAttribute.location = 2;
			colorAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
			colorAttribute.offset = offsetof(Vertex, color);

			description.attributes.push_back(positionAttribute);
			description.attributes.push_back(normalAttribute);
			description.attributes.push_back(colorAttribute);
			return description;
		}
	};

	//takes a array of vertices and generates a array of indices and vertices
	static inline void GenerateUniqueVerticesAndIndicesArrays(const std::vector<Vertex>& verticesToSort,
		std::vector<Vertex>& uniqueVertices, std::vector<uint32_t>& uniqueIndices)
	{
		const size_t vertexSortCount = verticesToSort.size();
		uniqueVertices.clear(); uniqueIndices.clear();
		uniqueVertices.reserve(vertexSortCount); uniqueIndices.reserve(vertexSortCount);

		size_t currentUniqueVertexCount = 0;
		size_t currentUniqueIndexCount = 0;
		size_t nextIndicesIndex = 0;

		for (size_t v = 0; v < vertexSortCount; ++v)
		{
			//checks if the vertex matchea a already existing index
			bool wasFound = false;
			for (size_t i = 0; i < currentUniqueIndexCount; ++i)
			{
				if (uniqueVertices[uniqueIndices[i]] == verticesToSort[v])
				{
					wasFound = true;
					break;
				}
			}

			//if it does not, add it
			if (!wasFound)
			{
				uniqueVertices.emplace_back(verticesToSort[v]);
				uniqueIndices.emplace_back(nextIndicesIndex);
				nextIndicesIndex++;
				currentUniqueIndexCount++; currentUniqueVertexCount++;
			}
		}
	}

	//defines a mesh
	struct Mesh
	{
		bool canRender = true; //is the mesh enabled for rendering
		uint32_t LODIndex = 0; //the LOD level of this mesh

		std::vector<Vertex> vertices; //the vertices
		std::vector<uint32_t> indices; //the indices
		
		Wireframe::MeshBuffers::VertexBuffer vertexBuffer; //the allocated vertex buffer
		Wireframe::MeshBuffers::IndexBuffer indexBuffer; //the allocated index buffer

		//creates a mesh
		inline bool CreateVertexAndIndexBuffers(VmaAllocator& allocator)
		{
			vertexBuffer.Create(allocator, sizeof(Vertex), vertices.data(), vertices.size());
			indexBuffer.Create(allocator, indices.data(), indices.size());

			canRender = true;
			return true;
		}

		//destroys a mesh
		inline void DestroyVertexAndIndexBuffers(VmaAllocator& allocator)
		{
			indexBuffer.Destroy(allocator);
			vertexBuffer.Destroy(allocator);

			canRender = false;
		}

		//destroys vertices
		inline void DeleteVertexData() { vertices.clear(); }

		//destroys indices
		inline void DeleteIndexData() { indices.clear(); }
	};

	//defines a static mesh
	struct StaticMesh
	{
		std::vector<Mesh> meshes;

		//loads a Smok Static Mesh file
		//for importing FBX meshes and such, checkout the "Smok Asset Convertion Tool"

		//saves a Static Mesh as a Smok Static Mesh file
	};
}