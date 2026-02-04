#include "ObjectSys.h"

#include "Ast.h"

namespace Sun {

	std::string Function::inspect() {
		std::string buf;
		buf.append("fn");
		buf.append("(");
		for (int i = 0; i < parameters.size()-1; ++i) {
			buf.append(parameters[i]->printString());
			buf.append(", ");
		}
		if (!parameters.empty()) {
			buf.append(parameters.back()->printString());
		}
		buf.append(") {\n");
		buf.append(body->printString());
		buf.append("\n}");

		return buf;
	}
}