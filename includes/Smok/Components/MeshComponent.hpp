#pragma once

//defines mesh renderer components

#include <BTDSTD/ECS/IComponent.hpp>

#include <Smok/Assets/Mesh.hpp>

#include <BTDSTD/Wireframe/Pipeline/GraphicsPipeline.hpp>

namespace Smok::ECS::Comp
{
	//defines a static mesh renderer component
	struct MeshRender : public BTD::ECS::Comp::IComponent
	{
		//pipeline
		uint64_t pipelineLayoutID = 0, pipelineID = 0, staticMeshID;

		//should specific sub-meshes be renderer
		bool renderSpecificSubMeshes = false;
		std::vector<uint32_t> meshIndexes;
	};
}