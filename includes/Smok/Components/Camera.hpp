#pragma once

//defines a camera

#include <BTDSTD/ECS/IComponent.hpp>

#include <glm/gtx/transform.hpp>

namespace Smok::ECS::Comp
{
	//defines a camera
	struct Camera : public BTD::ECS::Comp::IComponent
	{
		glm::mat4 projection = glm::mat4(1.0f), view = glm::mat4(1.0f),
			PV = glm::mat4(1.0f);

		glm::vec2 renderSize = { 0.0f, 0.0f }; //used for the rendering area

		//generates perspective
		inline glm::mat4 GeneratePerspective()
		{
			projection = glm::perspective(glm::radians(70.f), 1700.f / 900.f, 0.1f, 200.0f);
			projection[1][1] *= -1;
			return projection;
		}

		//generates view
		inline glm::mat4 GenerateView(const glm::vec3& position)
		{
			view = glm::translate(glm::mat4(1.f), position);
			return view;
		}

		//generates perspective * view
	};
}