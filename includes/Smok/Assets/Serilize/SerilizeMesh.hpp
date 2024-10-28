#pragma once

//serilizes the vertex data of a mesh to disc

#include <Smok/Assets/Mesh.hpp>

namespace Smok::Asset::Serilize::Mesh
{
	//defines a mesh serilize data blob

	//defines the offsets for where a mesh serilize blob is stored

	//defines a blob of serilized mesh binary data
	struct SerilizedBlob_Mesh
	{
		//offsets
		size_t startOffsetForVertexData = 0, endOffsetForVertexData = 0,
			startOffsetForIndexData = 0, endOffsetForIndexData = 0;

		//total counts
		size_t totalVertexCount = 0, totalIndexCount = 0;

		//data
		std::vector<Smok::Asset::Mesh::Vertex> vertices; //the vertices
		std::vector<uint32_t> indices; //the indices
	};

	//takes a mesh and generates serilize data
	inline SerilizedBlob_Mesh GenerateSerilizeData_Mesh(Asset::Mesh::Mesh& mesh)
	{
		SerilizedBlob_Mesh data;

		data.vertices = mesh.vertices;
		data.indices = mesh.indices;

		data.totalVertexCount = data.vertices.size();
		data.totalIndexCount = data.indices.size();

		return data;
	}

	//defines a blob for serilized static mesh


	//takes a static mesh and generates serilize data
	inline bool GenerateSerilizeData_StaticMesh(Asset::Mesh::StaticMesh& staticMesh, const std::string& assetName)
	{
		return true;
	}

	//serlilize a static mesh asset to disc
	inline bool WriteStaticMeshToDisc(Asset::Mesh::StaticMesh& staticMesh,
		const std::string& dir, const std::string& filename, const std::string& assetName)
	{

		return true;
	}

	//load a static mesh asset from disc
}