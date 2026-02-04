/*****************************************************************************
* @brief : datatypes
* 数据类型
* @author : acedtang
* @date : 2021/4/2
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __DATA_TYPES_H
#define __DATA_TYPES_H

namespace Sun {
    //分量类型
    enum class FieldType {
        BYTE,
        UBYTE,
        SHORT,
        USHORT,
        INT,
        UINT,
        FLOAT,
        DOUBLE,
    };
    enum class Type {
        SCALAR,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4
    };
    struct DataType {
        FieldType comType;
        Type  type;
        int getComByteLen() {
            int comLen = 0;
            switch (comType) {
            case FieldType::USHORT:
            case FieldType::SHORT:
            {
                comLen = 2;
            }
            break;
            case FieldType::BYTE: {
                comLen = 1;
            }
            break;
            case FieldType::UBYTE: {
                comLen = 1;
            }
            break;
            case FieldType::INT:
            {
                comLen = 4;
            }
            break;
            case FieldType::UINT: {
                comLen = 4;
            }
            case FieldType::FLOAT:
            {
                comLen = 4;
            }
            break;
            case FieldType::DOUBLE:
            {
                comLen = 8;
            }
            break;
            }
            return comLen;
        }
        int getComCount() {
            int n = 0;
            switch (type) {
                case Type::SCALAR: {
                    n = 1;
                }
                break;
                case Type::VEC2: {
                    n = 2;
                }
                break;
                case Type::VEC3: {
                    n = 3;
                }
                break;
                case Type::VEC4: {
                    n = 4;
                }
                break;
                case Type::MAT3: {
                    n = 9;
                }
                break;
                case Type::MAT4: {
                    n = 16;
                }
                break;
                default:
                    break;
            }
            return n;
        }
        int getTotalByteNum() {
            return getComByteLen() * getComCount();
        }
    };

}


#endif