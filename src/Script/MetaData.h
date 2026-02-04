#ifndef __SUN_META_DATA_H
#define __SUN_META_DATA_H

#include "export.h"
#include <string>
#include <vector>
#include "Lexer.h"

namespace Sun {

    enum class MetaType {
        UNDEF,
        TEMPLATE,
        CLASS,
        STRUCT,
        ENUM,

        //composite type
        POINTER, //*
        CONST_POINTER, //const XX*
        POINTER2,  //**
        CONST_POINTER2, //const XX**
        REF,
        CONST_REF,

        //terminate
        ENUM_VALUE,
        INT, //int
        UINT, //unsigned int
        INT32,
        INT64,
        UINT32,
        UINT64,
        FLOAT,
        DOUBLE,
        CHAR,
        UCHAR,
    };

    //判断是否为终结符
    inline bool isTerminate(MetaType type) {
        return type >= MetaType::ENUM_VALUE;
    }

    class MetaData;

    struct MetaDataWrapper {
        MetaType type;
        MetaData* data = nullptr;
    };

    //这个类记录,enum,class,struct,enum class的元信息
    class MetaData {
    public:
        MetaData() {}
        MetaData(MetaType type) : metaType(type) {}
        MetaData(MetaType type, const std::string& Name) :metaType(type), name(Name) {}

        MetaData* next = nullptr;

        //类型
        MetaType metaType = MetaType::UNDEF;

        //类，结构体或者枚举 的名字,如果是基础类型(比如int则为"int")
        //如果是ENUM_VALUE,则为其枚举字面字符串
        std::string name;

        //成员
        std::vector<MetaDataWrapper>  members;

        //对于枚举类型存放枚举字面字符("abc = 0x33")
        std::vector<std::string> literals;

        //成员名
        std::vector<std::string> memberNames;
    };

}

#endif