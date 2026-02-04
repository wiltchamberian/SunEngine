#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include <unordered_map>
#include <map>
#include <memory>
#include "ObjectSys.h"

namespace Sun {

	class Environment {
	public:
		std::shared_ptr<Object_t> get(const std::string& str);
		void set(const std::string& str, const std::shared_ptr<Object_t>& obj);

		std::map<std::string, std::shared_ptr<Object_t>> store;
		Environment* outer = nullptr; //类似链表的结构
	};

	Environment* newEnclosedEnvironment(Environment* outer);

}

#endif