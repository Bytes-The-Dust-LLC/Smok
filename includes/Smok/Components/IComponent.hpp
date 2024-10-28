#pragma once

//defines a common component interface

#include <cstdint>

namespace Smok::ECS::Comp
{
	//defines a component interface
	struct IComponent
	{
		uint64_t ID = 0; //the ID associated with this component
		bool isDirty = true; //data has changed
		bool isActive = true; //is the component active
	};
}