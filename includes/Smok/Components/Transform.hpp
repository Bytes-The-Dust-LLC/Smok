#pragma once

//defines a transform component

#include <Smok/Components/IComponent.hpp>

#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstdint>

namespace Smok::ECS::Comp
{
	//defines a transform
	struct Transform : public IComponent
	{
		bool rotationDataIsInRadians = false; //is internal data in radians

		glm::vec3 position = { 0.0f, 0.0f, 0.0f }, eularRotation = { 0.0f, 0.0f, 0.0f },
			scale = { 1.0f, 1.0f, 1.0f };

		glm::quat rotation;

		glm::mat4 modelMatrix = glm::mat4(1.0f);

		//generates the model matrix regardless of dirty flag, does not reset dirty flag
		inline glm::mat4 GenerateModelMatrix_Forced()
		{
			modelMatrix = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation);
			return modelMatrix;
		}

		//gets the model matrix, if dirty flag is true, will generate one and reset flag
		inline glm::mat4 ModelMatrix()
		{
			if (isDirty)
			{
				modelMatrix = GenerateModelMatrix_Forced();
				isDirty = false;
			}

			return modelMatrix;
		}

		//gets the eular rotation in radians
		inline glm::vec3 GetEularRotation_Radians() const {(rotationDataIsInRadians == true ? eularRotation : glm::radians(eularRotation));}

		//gets the eular rotation in degrees
		inline glm::vec3 GetEularRotation_Degrees() const { (rotationDataIsInRadians == false ? eularRotation : glm::degrees(eularRotation)); }

		//gets eular rotation from quaterion rotation || returned eular WILL BE in radians
		inline glm::vec3 GetEularRotationFromQuaterion() const { return glm::eulerAngles(rotation); }

		//sets the roation || assumes passed in data IS in radians || this is desired over setting directly so the dirty flag is set
		inline void SetEularRotation_Radians(const glm::vec3 rot)
		{
			eularRotation = rot;
			rotation = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::rotate(glm::mat4(1.0f), rot.z, glm::vec3(0.0f, 0.0f, 1.0f));
			rotationDataIsInRadians = true;
			isDirty = true;
		}

		//sets the roation || assumes passed in data is NOT in radians || this is desired over setting directly so the dirty flag is set
		inline void SetEularRotation(const glm::vec3 rot) { SetEularRotation_Radians(rot); }

		//sets rotation on X axis || assumes passed in data is NOT in radians || this is desired over setting directly so the dirty flag is set
		inline void SetRotateX(const double& angel)
		{
			SetEularRotation_Radians({ glm::radians(angel), eularRotation.y, eularRotation.z });
		}

		//sets rotation on Y axis || assumes passed in data is NOT in radians || this is desired over setting directly so the dirty flag is set
		inline void SetRotateY(const double& angel)
		{
			SetEularRotation_Radians({ eularRotation.x, glm::radians(angel), eularRotation.z });
		}

		//sets rotation on Z axis || assumes passed in data is NOT in radians || this is desired over setting directly so the dirty flag is set
		inline void SetRotateZ(const double& angel)
		{
			SetEularRotation_Radians({ eularRotation.x, eularRotation.y, glm::radians(angel) });
		}

		//rotates on X axis || assumes passed in data is NOT in radians || this is desired over setting directly so the dirty flag is set
		inline void RotateX(const double& rate)
		{
			SetEularRotation_Radians({ eularRotation.x += glm::radians(rate), eularRotation.y, eularRotation.z });
		}

		//rotates on Y axis || assumes passed in data is NOT in radians || this is desired over setting directly so the dirty flag is set
		inline void RotateY(const double& rate)
		{
			SetEularRotation_Radians({ eularRotation.x, eularRotation.y += glm::radians(rate), eularRotation.z });
		}

		//rotates on Z axis || assumes passed in data is NOT in radians || this is desired over setting directly so the dirty flag is set
		inline void RotateZ(const double& rate)
		{
			SetEularRotation_Radians({ eularRotation.x, eularRotation.y, eularRotation.z += glm::radians(rate) });
		}

		//returns the world Forward
		static inline glm::vec3 WorldForward() { return { 0.0f, 0.0f, -1.0f }; }

		//returns the world Left
		static inline glm::vec3 WorldLeft() { return { 1.0f, 0.0f, 0.0f }; }

		//returns the local forward
		inline glm::vec3 Forward() { return rotation * WorldForward(); }

		//returns the local left
		inline glm::vec3 Left() { return rotation * WorldLeft(); }
	};
}