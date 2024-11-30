#pragma once

//defines extra asset wrappers for the asset manager
//theses wrap the settings and raw asset objects normally found in Smok and Wireframe
//they add a little extra overhead but are needed for asset management
//in the future, might just merge them all into one

#include <Smok/Assets/Mesh.hpp>

#include <BTDSTD/Wireframe/Pipeline/GraphicsPipeline.hpp>

namespace Smok::Asset::AssetManager
{
	//defines the types of assets
	enum class AssetType
	{
		GraphicsPipeline = 0,
		PipelineLayout,

		StaticMesh,

		Count
	};

	//defines a common asset
	struct IAsset
	{
		uint64_t ID = 0; //the ID

		AssetType type = AssetType::Count; //the type of asset

		bool settingDataIsLoaded = false; //is the settings file data loaded
		bool assetIsCreated = false; //is the asset itself created
	};

	//defines a asset for a pipline layout
	struct Asset_PipelineLayout : public IAsset
	{
		BTD::IO::FileInfo pushConstantDataSettingFile;
		Wireframe::Pipeline::PushConstant pushConstSettings;

		Wireframe::Pipeline::PipelineLayout asset; //the asset

		//create the layout
		inline bool Create(const Wireframe::Pipeline::PipelineLayout_CreateInfo& info, Wireframe::Device::GPU* GPU)
		{
			//if asset is already created

			//if data is not loaded

			//creates
			assetIsCreated = asset.Create(info, GPU);
			return assetIsCreated;
		}

		//destroys the layout
		inline void Destory(Wireframe::Device::GPU* GPU)
		{
			//if asset is not created

			asset.Destroy(GPU);
		}

		//loads the push constant data
		inline bool LoadPushConstantSettings()
		{
			if (settingDataIsLoaded)
			{
				//throw warning
				return true;
			}

			//loads the settings file
			settingDataIsLoaded = Wireframe::Pipeline::Serilize::LoadPipelineLayoutPushConstantDataFromFile(pushConstantDataSettingFile, pushConstSettings);
			return settingDataIsLoaded;
		}
	};

	//defines a asset for a graphics pipeline
	struct Asset_GraphicsPipeline : public IAsset
	{
		BTD::IO::FileInfo pipelineDataSettingFile;
		Wireframe::Pipeline::PipelineSettings pipelineSettings;

		BTD::IO::FileInfo vertexShaderDataSettingFile;
		Wireframe::Shader::Serilize::ShaderSerilizeData vertexSettings;

		BTD::IO::FileInfo fragmentShaderDataSettingFile;
		Wireframe::Shader::Serilize::ShaderSerilizeData fragmentSettings;

		Wireframe::Pipeline::GraphicsPipeline asset; //the asset

		//creates the graphics pipeline
		inline bool Create(Wireframe::Device::GPU* GPU, Wireframe::Pipeline::PipelineLayout& pipelineLayout, VkRenderPass& renderpass)
		{
			//if asset is already created

			//if data is not loaded

			//creates shaders
			Wireframe::Shader::ShaderModule meshVertShader;
			if (!meshVertShader.Create(vertexSettings.binaryFilepath.c_str(), GPU)) {}
			Wireframe::Shader::ShaderModule meshFragShader;
			if (!meshFragShader.Create(fragmentSettings.binaryFilepath.c_str(), GPU)) {}

			pipelineSettings._shaderStages = { Wireframe::Shader::GenerateShaderStageInfoForPipeline(meshVertShader, Wireframe::Shader::Util::ShaderStage::Vertex),
				Wireframe::Shader::GenerateShaderStageInfoForPipeline(meshFragShader, Wireframe::Shader::Util::ShaderStage::Fragment) };

			//creates the pipeline
			asset.Create(pipelineSettings, pipelineLayout, renderpass, GPU);

			//cleans up shaders since we don't need them taking up VRAM anymore
			meshFragShader.Destroy(GPU);
			meshVertShader.Destroy(GPU);

			assetIsCreated = true;
			return assetIsCreated;
		}

		//loads the pipeline settings files and shaders
		inline bool LoadPipelineSettingsAndShaders()
		{
			if (settingDataIsLoaded)
			{
				//throw warning
				return true;
			}

			//loads the settings file
			Wireframe::Pipeline::Serilize::LoadPipelineSettingsDataFromFile(pipelineDataSettingFile, pipelineSettings);

			//loads the shaders
			Wireframe::Shader::Serilize::LoadShaderDataFromFile(vertexShaderDataSettingFile, vertexSettings, false);
			Wireframe::Shader::Serilize::LoadShaderDataFromFile(fragmentShaderDataSettingFile, fragmentSettings, false);

			settingDataIsLoaded = true;
			return settingDataIsLoaded;
		}
	};

	//defines a asset for a static mesh
	struct Asset_StaticMesh : public IAsset
	{
		BTD::IO::FileInfo declFile; //the decl file describing how to import the binary data
		BTD::IO::FileInfo binaryFile; //the binary file actually storing all the tasty vertices and indices

		Smok::Asset::Mesh::StaticMesh asset; //the asset

		//loads the settings

		//initalizes the mesh
		inline bool InitalizeMesh(VmaAllocator& allocator)
		{
			//generate vertex and index buffers
			asset.CreateVertexBuffers(allocator);
			for (size_t i = 0; i < asset.meshes.size(); ++i)
				asset.meshes[i].CreateIndexBuffers(allocator);

			return true;
		}
	};
}