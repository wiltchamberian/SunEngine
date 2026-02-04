#ifndef __OBJECT_SYS_H
#define __OBJECT_SYS_H

#include <cstdint>
#include <string>
#include <unordered_map>

namespace Sun {

	using ObjectType = const char*;

#define INTEGER_OBJ "INTEGER"
#define BOOLEAN_OBJ "BOOLEAN"
#define NULL_OBJ "NULL"
#define RETURN_VALUE "RETURN_VALUE"
#define ERROR_OBJ "ERROR_OBJ"
#define FUNCTION_OBJ "FUNCTION"

	typedef struct Object_s {
		Object_s() {}
		bool isReturn = false;
		virtual std::string inspect() = 0;
		virtual ObjectType type() = 0;
		virtual std::int64_t intValue() { return 0; }
		virtual void setIntValue(std::int64_t v) {}
		virtual bool boolValue(){ return false; }
		virtual void setBoolValue(bool bl) {}
	}Object_t;

	struct Integer:public Object_t{
		Integer(std::int64_t val) :value(val) {}
		std::int64_t value;

		std::string inspect() {
			return std::to_string(value);
		}

		std::int64_t intValue() {
			return value;
		}

		void setIntValue(std::int64_t v) {
			value = v;
		}

		bool boolValue() {
			return value != 0;
		}

		ObjectType type() {
			return INTEGER_OBJ;
		}
	};

	struct Boolean : public Object_t {
		Boolean() {}
		Boolean(bool bl) :value(bl) {}
		bool value;
		std::string inspect() {
			return value?"true":"false";
		}
		ObjectType type() {
			return BOOLEAN_OBJ;
		}
		bool boolValue() {
			return value;
		}
		std::int64_t intValue() {
			return value ? 1 : 0;
		}
		void setBoolValue(bool bl) {
			value = bl;
		}
	};

	struct Null :public Object_t {
		std::string inspect() {
			return "null";
		}
		ObjectType type() {
			return NULL_OBJ;
		}
		bool boolValue() {
			return false;
		}
	};

	struct Error : public Object_t{
		Error() {}
		Error(const char* d) :message(d) {}
		std::string message;
		int rowNo = 0;
		ObjectType type() {
			return ERROR_OBJ;
		}
		std::string inspect() {
			return "ERROR: " + message;
		}
	};

	struct Function : public Object_t {
		Function(const std::vector<class Identifier*>& vec) :parameters(vec), body(nullptr), ev(nullptr) {}
		Function(const std::vector<class Identifier*>& vec,class BlockStatement* bd, class Environment* env) 
			:parameters(vec), body(bd), ev(env) {}
		std::vector<class Identifier*> parameters;
		class BlockStatement* body;
		class Environment* ev;
		ObjectType type() {
			return FUNCTION_OBJ;
		}
		std::string inspect();
	};
}



#endif