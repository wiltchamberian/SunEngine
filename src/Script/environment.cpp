#include "environment.h"
#include <cassert>

namespace Sun {

	std::shared_ptr<Object_t> Environment::get(const std::string& str) {
		std::shared_ptr<Object_t> res;
		auto it = store.find(str);
		if (it == store.end() && outer != nullptr) {
			res = outer->get(str);
		}
		else if (it != store.end()) {
			res = it->second;
		}
		return res;
	}

	void Environment::set(const std::string& str, const std::shared_ptr<Object_t>& obj) {
		assert(obj != nullptr);
		store[str] = obj;
	}

	Environment* newEnclosedEnvironment(Environment* outer) {
		Environment* ev = new Environment();
		ev->outer = outer;
		return ev;
	}
}