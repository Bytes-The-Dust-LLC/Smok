#pragma once

//defines a asset manager for Smok
//this uses wrapper objects with a little extra needed for them

#include <Smok/Assets/AssetManagerAssets.hpp>

#include <BTDSTD/Maps/StringIDRegistery.hpp>

namespace Smok::Asset::AssetManager
{
	//defines a queue task for loading a asset

	//defines a asset manager
	struct AssetManager
	{
		BTD::Map::IDStringRegistery assetNameRegistery;
		std::unordered_map<uint64_t, Asset_PipelineLayout> pipelineLayouts;
		std::unordered_map<uint64_t, Asset_GraphicsPipeline> pipelines;
		std::unordered_map<uint64_t, Asset_StaticMesh> staticMeshes;

		//inits the asset manager
		inline bool Init()
		{
			//reserve space in the assets

			//run the thread for loading asset data
		}

		//destroys all assets
		inline void Destroy(VmaAllocator& _allocator, Wireframe::Device::GPU* GPU)
		{
			//wait for loading thread to be finished and then close

			//clean up assets loaded
			for (auto& m : staticMeshes)
			{
				if (!m.second.assetIsCreated)
					continue;

				for (size_t i = 0; i < m.second.asset.meshes.size(); ++i)
					m.second.asset.meshes[i].DestroyIndexBuffers(_allocator);
				m.second.asset.DestroyVertexBuffers(_allocator);

				m.second.assetIsCreated = false;
			}

			for (auto& p : pipelines)
				p.second.asset.Destroy(GPU);

			for (auto& l : pipelineLayouts)
				l.second.asset.Destroy(GPU);
		}

		//registers a graphics pipeline
		inline uint64_t RegisterAsset_GraphicsPipeline(const std::string& name, const BTD::IO::FileInfo& pipelineDataSettingFile,
			const BTD::IO::FileInfo& vertexShaderDataSettingFile, const BTD::IO::FileInfo& fragmentShaderDataSettingFile)
		{
			uint64_t ID = assetNameRegistery.GenerateID(name);

			pipelines[ID] = Asset_GraphicsPipeline();
			pipelines[ID].ID = ID;
			pipelines[ID].pipelineDataSettingFile = pipelineDataSettingFile;
			pipelines[ID].vertexShaderDataSettingFile = vertexShaderDataSettingFile;
			pipelines[ID].fragmentShaderDataSettingFile = fragmentShaderDataSettingFile;
			pipelines[ID].asset = Wireframe::Pipeline::GraphicsPipeline();
			return ID;
		}

		//registers a pipeline layout
		inline uint64_t RegisterAsset_PipelineLayout(const std::string& name, const BTD::IO::FileInfo& pushConstantDataSettingFile)
		{
			uint64_t ID = assetNameRegistery.GenerateID(name);
			pipelineLayouts[ID] = Asset_PipelineLayout();
			pipelineLayouts[ID].ID = ID;
			pipelineLayouts[ID].pushConstantDataSettingFile = pushConstantDataSettingFile;
			pipelineLayouts[ID].asset = Wireframe::Pipeline::PipelineLayout();
			return ID;
		}

		//registers a static mesh
		inline uint64_t RegisterAsset_StaticMesh(const std::string& name,
			const BTD::IO::FileInfo& declFile, const BTD::IO::FileInfo& binaryFile)
		{
			uint64_t ID = assetNameRegistery.GenerateID(name);
			staticMeshes[ID] = Asset_StaticMesh();
			staticMeshes[ID].ID = ID;
			staticMeshes[ID].asset = Smok::Asset::Mesh::StaticMesh();
			staticMeshes[ID].declFile = declFile;
			staticMeshes[ID].binaryFile = binaryFile;
			return ID;
		}

		//gets if a ID matches the static mesh asset
		inline bool AssetIsRegistered_StaticMesh(const uint64_t& ID) const
		{
			return (staticMeshes.find(ID) != staticMeshes.end() ? true : false);
		}
	};
}