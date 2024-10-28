#pragma once

//defines a ECS Registery for all components

#include <Smok/Components/Camera.hpp>
#include <Smok/Components/MeshComponent.hpp>
#include <Smok/Components/Transform.hpp>

#include <deque>

namespace Smok::ECS
{

	class ECSManager {
	public:
		// Add a component to an entity
		template <typename ComponentType>
		void addComponent(uint64_t entity, ComponentType component) {
			auto& componentMap = getComponentMap<ComponentType>();
			componentMap[entity] = component;
		}

		// Get a reference to a component for a given entity
		template <typename ComponentType>
		ComponentType* getComponent(uint64_t entity) {
			auto& componentMap = getComponentMap<ComponentType>();
			if (componentMap.find(entity) != componentMap.end()) {
				return &componentMap[entity];
			}
			return nullptr;
		}

		// Remove a component from an entity
		template <typename ComponentType>
		void removeComponent(uint64_t entity) {
			auto& componentMap = getComponentMap<ComponentType>();
			componentMap.erase(entity);
		}

		// Query entities that have all of the specified components
		template <typename... ComponentTypes>
		std::vector<uint64_t> queryEntities() {
			std::vector<uint64_t> result;

			if constexpr (sizeof...(ComponentTypes) == 0) {
				return result;  // No components specified, return empty list
			}

			std::vector<uint64_t> potentialEntities = getEntitiesWith<ComponentTypes...>();
			for (uint64_t entity : potentialEntities) {
				if (hasComponents<ComponentTypes...>(entity)) {
					result.push_back(entity);
				}
			}

			return result;
		}

	private:
		// Component storage
		template <typename ComponentType>
		std::unordered_map<uint64_t, ComponentType>& getComponentMap() {
			static std::unordered_map<uint64_t, ComponentType> componentMap;
			return componentMap;
		}

		// Check if an entity has all of the specified components
		template <typename... ComponentTypes>
		bool hasComponents(uint64_t entity) {
			return (... && (getComponent<ComponentTypes>(entity) != nullptr));
		}

		// Get the entities that have the first component type (used to limit the search space)
		template <typename FirstComponentType, typename... RestComponentTypes>
		std::vector<uint64_t> getEntitiesWith() {
			auto& componentMap = getComponentMap<FirstComponentType>();
			std::vector<uint64_t> entities;

			for (const auto& [entity, component] : componentMap) {
				entities.push_back(entity);
			}

			return entities;
		}
	};

	//defines a ecs register
	struct ECSRegistery
	{
		std::deque<Comp::Camera> cameras;
		std::deque<Comp::Transform> transforms;
		std::deque<Comp::MeshRender> meshRenderers;

		//adds a entity ref to a component
		inline void AddEntityRefToComponent(Comp::IComponent* comp, const uint64_t ID, const bool isActive = true)
		{
			comp->ID = ID;
			comp->isActive = isActive;
		}

		//adds a camera component
		inline Comp::Camera* CreateComponent_Camera(const Comp::Camera& data, const uint64_t& ID, const bool isActive = true)
		{
			Comp::Camera* comp = &cameras.emplace_back(data);
			AddEntityRefToComponent(comp, ID, isActive);
			return comp;
		}

		//gets a camera component
		inline Comp::Camera* GetComponent_Camera(const uint64_t& ID)
		{
			for (size_t i = 0; i < cameras.size(); ++i)
			{
				if (cameras[i].ID == ID)
					return &cameras[i];
			}

			return nullptr;
		}

		//adds a transform component
		inline Comp::Transform* CreateComponent_Transform(const Comp::Transform& data, const uint64_t& ID, const bool isActive = true)
		{
			Comp::Transform* comp = &transforms.emplace_back(data);
			AddEntityRefToComponent(comp, ID, isActive);
			return comp;
		}

		//gets a transform component
		inline Comp::Transform* GetComponent_Transform(const uint64_t& ID)
		{
			for (size_t i = 0; i < transforms.size(); ++i)
			{
				if (transforms[i].ID == ID)
					return &transforms[i];
			}

			return nullptr;
		}

		//adds a mesh renderer component
		inline Comp::MeshRender* CreateComponent_MeshRenderer(const Comp::MeshRender& data, const uint64_t& ID, const bool isActive = true)
		{
			Comp::MeshRender* comp = &meshRenderers.emplace_back(data);
			AddEntityRefToComponent(comp, ID, isActive);
			return comp;
		}

		//gets a mesh renderer component
		inline Comp::MeshRender* GetComponent_MeshRenderer(const uint64_t& ID)
		{
			for (size_t i = 0; i < meshRenderers.size(); ++i)
			{
				if (meshRenderers[i].ID == ID)
					return &meshRenderers[i];
			}

			return nullptr;
		}

		//get every pair of components for transform and mesh renderer
		inline size_t GetComponentPairs_Transform_MeshRenderer(const std::vector<uint64_t>& IDs, bool doesItNeedToBeActive,
			std::vector<Comp::Transform>& transComps, std::vector<Comp::MeshRender>& mrs)
		{
			const size_t IDCount = IDs.size();
			transComps.clear(); mrs.clear();

			size_t compCount = 0;
			for (size_t i = 0; i < IDCount; ++i)
			{
				Comp::IComponent* trans = GetComponent_Transform(IDs[i]);
				if (!trans || doesItNeedToBeActive && !trans->isActive)
					continue;

				Comp::IComponent* mr = GetComponent_MeshRenderer(IDs[i]);
				if (!mr || doesItNeedToBeActive && mr && !mr->isActive)
					continue;

				printf("Comp found\n");

				transComps.emplace_back(*GetComponent_Transform(IDs[i]));
				mrs.emplace_back(*GetComponent_MeshRenderer(IDs[i]));
				compCount++;
			}

			return compCount;
		}
	};
}