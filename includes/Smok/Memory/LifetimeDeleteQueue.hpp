#pragma once

//defines a queue for handling the deletion of objects when we are done with them

#include <deque>
#include <functional>

namespace Smok::Memory
{
	//defines a deletion queue for handling lifetimes
	struct DeletionQueue
	{
		std::deque<std::function<void()>> deletors;

		void push_function(std::function<void()>&& function) {
			deletors.push_back(function);
		}

		void flush() {
			// reverse iterate the deletion queue to execute all the functions
			for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
				(*it)(); //call functors
			}

			deletors.clear();
		}
	};

}