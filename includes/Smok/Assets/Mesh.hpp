#pragma once

//defines a mesh for Smok projects
//for importing FBX meshes and such, checkout the "Smok Asset Convertion Tool"

#include <BTDSTD/Wireframe/Pipeline/VertexInputDesc.hpp>
#include <BTDSTD/Wireframe/MeshBuffer.hpp>

#include <BTDSTD/Formats/json.hpp>
#include <BTDSTD/IO/FileInfo.hpp>
#include <BTDSTD/IO/File.hpp>

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

		bool operator==(const Vertex& other) const
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

			//Position will be stored at Location 3
			VkVertexInputAttributeDescription texCordsAttribute = {};
			texCordsAttribute.binding = 0;
			texCordsAttribute.location = 3;
			texCordsAttribute.format = VK_FORMAT_R32G32_SFLOAT;
			texCordsAttribute.offset = offsetof(Vertex, textureCoords);

			description.attributes.push_back(positionAttribute);
			description.attributes.push_back(normalAttribute);
			description.attributes.push_back(colorAttribute);
			description.attributes.push_back(texCordsAttribute);
			return description;
		}
	};

	//checks if a vertex already exists in the list
	static inline bool VertexIsAlreadyInArray(const Vertex* verticesAlreadySorted, const size_t& verticesCount, const Vertex& vertex, size_t& arrayIndex)
	{
		for (size_t i = 0; i < verticesCount; ++i)
		{
			if (verticesAlreadySorted[i] == vertex)
			{
				arrayIndex = i;
				return true;
			}
		}

		return false;
	}

	//defines a mesh
	struct Mesh
	{
		bool canRender = true; //is the mesh enabled for rendering
		uint32_t LODIndex = 0; //the LOD level of this mesh

		std::vector<uint32_t> indices; //the indices
		
		Wireframe::MeshBuffers::IndexBuffer indexBuffer; //the allocated index buffer

		//creates a mesh
		inline bool CreateIndexBuffers(VmaAllocator& allocator)
		{
			indexBuffer.Create(allocator, indices.data(), indices.size());

			canRender = true;
			return true;
		}

		//destroys a mesh
		inline void DestroyIndexBuffers(VmaAllocator& allocator)
		{
			indexBuffer.Destroy(allocator);

			canRender = false;
		}

		//destroys indices
		inline void DeleteIndexData() { indices.clear(); }
	};

	//defines a static mesh
	struct StaticMesh
	{
		Wireframe::MeshBuffers::VertexBuffer vertexBuffer; //the allocated vertex buffer

		std::vector<Vertex> vertices; //the total vertices making up all submeshes

		std::vector<Mesh> meshes; //the individual meshes storing the indices

		//creates a vertex buffer
		inline bool CreateVertexBuffers(VmaAllocator& allocator)
		{
			vertexBuffer.Create(allocator, sizeof(Vertex), vertices.data(), vertices.size());
			return true;
		}

		//destroys a vertex buffer
		inline void DestroyVertexBuffers(VmaAllocator& allocator)
		{
			vertexBuffer.Destroy(allocator);
		}
	};
}

namespace Smok::Asset::Mesh::Serilize
{
	//gets the version of this serilize API
	static inline std::string GetAPIVersionStr() { return "1"; }

	//gets the file extension for a binary smesh file
	static inline std::string GetSmeshBinaryFileExtensionStr() { return "smesh"; }

	//gets the file extension for a decl smesh file
	static inline std::string GetSmeshDeclFileExtensionStr() { return "smeshdecl"; }

	//writes a static mesh to file
	static inline bool WriteStaticMeshDataToFile(const BTD::IO::FileInfo& _declFile, const BTD::IO::FileInfo& _binaryFile, const StaticMesh& data)
	{
		////checks if the file has the right extension, if not throw a warning and add it ourself
		BTD::IO::FileInfo declFile = _declFile;
		if (declFile.extension != GetSmeshDeclFileExtensionStr())
		{
			fmt::print("Smok Asset Mesh Warning: Serilize || WriteStaticMeshDataToFile || \"{}\" does not end in .{}, this is the file extension for Smok Static Mesh Decl files. This warning can be ignored as we will add the extension. But to make it go away, add it to your file. The function \"GetSmeshDeclFileExtensionStr\" can be used to get the extension to add.\n",
				declFile.GetPathStr(), GetSmeshDeclFileExtensionStr());

			declFile.AppendFileExtension(GetSmeshDeclFileExtensionStr());
		}

		////checks if the file has the right extension, if not throw a warning and add it ourself
		BTD::IO::FileInfo binaryFile = _binaryFile;
		if (binaryFile.extension != GetSmeshBinaryFileExtensionStr())
		{
			fmt::print("Smok Asset Mesh Warning: Serilize || WriteStaticMeshDataToFile || \"{}\" does not end in .{}, this is the file extension for Smok Static Mesh Binary files. This warning can be ignored as we will add the extension. But to make it go away, add it to your file. The function \"GetSmeshBinaryFileExtensionStr\" can be used to get the extension to add.\n",
				binaryFile.GetPathStr(), GetSmeshBinaryFileExtensionStr());

			binaryFile.AppendFileExtension(GetSmeshBinaryFileExtensionStr());
		}

		//calculates the binary and decl blobs
		const size_t vertexCount = data.vertices.size();
		BTD::IO::File::WriteWholeBinaryFile(binaryFile, data.vertices.data(), sizeof(Smok::Asset::Mesh::Vertex), vertexCount);

		//build json and write to file
		nlohmann::json declData;
		declData["version"] = GetAPIVersionStr();
		declData["vertexCount"] = vertexCount;

		//stores the array of meshes
		const size_t subMeshCount = data.meshes.size();
		std::vector<std::vector<uint32_t>> submeshIndices;
		for (size_t m = 0; m < subMeshCount; ++m)
		{
			std::vector<uint32_t> indices;
			const size_t indicesCount = data.meshes[m].indices.size();
			for (size_t index = 0; index < indicesCount; ++index)
				indices.emplace_back(data.meshes[m].indices[index]);
			submeshIndices.emplace_back(indices);
		}
		declData["meshCount"] = subMeshCount;
		declData["meshes"] = submeshIndices;
		
		//writes decl data
		BTD::IO::File::WriteWholeTextFile(declFile, declData.dump());

		return true;
	}

	//laods a static mesh from file
	static inline bool LoadStaticMeshDataFromFile(const BTD::IO::FileInfo& declFile, const BTD::IO::FileInfo& binaryFile, StaticMesh& data)
	{
		//checks if the file has the right extension, if not throw a warning and add it ourself
		if (declFile.extension != GetSmeshDeclFileExtensionStr())
		{
			fmt::print("Smok Asset Mesh Error: Serilize || LoadStaticMeshDataFromFile || \"{}\" does not end in .{}, this is the file extension for Smok Static Mesh Decl files. We can not validate this is a correct file. The function \"WriteStaticMeshDataToFile\" can be used to generate valid Smok Mesh files.\n",
				declFile.GetPathStr(), GetSmeshDeclFileExtensionStr());
			return false;
		}

		//checks if the files exist
		if (!declFile.Exists())
		{
			fmt::print("Smok Asset Mesh Error: Serilize || LoadStaticMeshDataFromFile || \"{}\" does not exist. We need this file for loading static mesh data. The function \"WriteStaticMeshDataToFile\" can be used to generate valid Smok Mesh files.\n",
				declFile.GetPathStr());
			return false;
		}

		//checks if the file has the right extension, if not throw a warning and add it ourself
		if (binaryFile.extension != GetSmeshBinaryFileExtensionStr())
		{
			fmt::print("Smok Asset Mesh Error: Serilize || LoadStaticMeshDataFromFile || \"{}\" does not end in .{}, this is the file extension for Smok Static Mesh Binary files. We can not validate this is the correct file The function \"WriteStaticMeshDataToFile\" can be used to generate valid Smok Mesh files.\n",
				binaryFile.GetPathStr(), GetSmeshBinaryFileExtensionStr());
			return false;
		}

		//checks if the file exists
		if (!binaryFile.Exists())
		{
			fmt::print("Smok Asset Mesh Error: Serilize || LoadStaticMeshDataFromFile || \"{}\" does not exist. We need this file for loading static mesh data. The function \"WriteStaticMeshDataToFile\" can be used to generate valid Smok Mesh files.\n",
				binaryFile.GetPathStr());
			return false;
		}

		//loads the decl file and check it's version, throw a warning
		nlohmann::json decl = nlohmann::json::parse(BTD::IO::File::ReadWholeTextFile(declFile).data);
		if (decl["version"] != GetAPIVersionStr())
		{
			const std::string ver = decl["version"];
			fmt::print("Smok Asset Mesh Warning: Serilize || LoadStaticMeshDataFromFile || \"{}\" is not the same version as the version of Smok you are using. The file's version is \"{}\", while the version you are using to load the file is \"{}\". We can not say there won't be issues loading this data. The function \"WriteStaticMeshDataToFile\" can be used to generate valid Smok Mesh files, matching this version of Smok.\n",
				binaryFile.GetPathStr(), ver, GetAPIVersionStr());
		}

		//gets the mesh data
		const size_t vertexCount = decl["vertexCount"];
		data.vertices.resize(vertexCount);
		BTD::IO::File::ReadWholeBinaryFile(binaryFile, data.vertices.data(), sizeof(Smok::Asset::Mesh::Vertex), vertexCount);

		//generates the sub-meshes
		const size_t subMeshCount = decl["meshCount"];
		std::vector<std::vector<uint32_t>> submeshes = decl["meshes"].get<std::vector<std::vector<uint32_t>>>();
		data.meshes.resize(subMeshCount);
		for (size_t m = 0; m < subMeshCount; ++m)
			data.meshes[m].indices = submeshes[m];

		return true;
	}
}