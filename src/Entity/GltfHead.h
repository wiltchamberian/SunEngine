/*****************************************************************************
* @brief : GltfHead
* @author : acedtang
* @date : 2021/4/19
* @version : ver 1.0
* @inparam :
* @outparam :
*****************************************************************************/

#ifndef __GLTF_HEAD_H
#define __GLTF_HEAD_H

#include "Math/Transform3D.h"
#include "Assets/DataTypes.h"
#include "Util/exception.h"

namespace Sun {

#define GLTF_BYTE 5120 //1byte
#define GLTF_UNSIGNED_BYTE 5121 //1byte
#define GLTF_SHORT 5122 //2byte
#define GLTF_UNSIGNED_SHORT 5123 //2byte
#define GLTF_UNSIGNED_INT 5125 //4byte
#define GLTF_FLOAT 5126 //4byte

    //该结构对应的是gltf2.0中解析出的buffer数据，其内容可能有不同的解释,
    //通过模板参数可以实现不同类型的解读
    struct Data {
        Data() {
            count = 0;
            data = 0;
            byteLen = 0;
            byteStride = -1;
            bufViewIndex = -1;
        }
        FieldType comType() {
            return dataType.comType;
        }
        Type type() {
            return dataType.type;
        }
        DataType dataType;
        int count;
        char* data;
        int byteLen;
        int byteStride;
        int bufViewIndex;
        //修正byteStride
        void check() {
            if (byteStride < 0) {
                byteStride = dataType.getTotalByteNum();
            }
            ASSERT(count * byteStride == byteLen);
        }

        //由于可以表示不同种类的数据，因此存在多种解读数据的方式,
        //实际要根据具体的数据类型调用相应的函数
        template<class _T>
        _T getContent(int i) {
            return *(_T*)(data + i * byteStride);
        }

        template<class _T>
        _T getContent(int i, int j) {
            return *((_T*)(data + i * byteStride) + j);
        }
    };

    struct GltfNode
    {
    public:
        int oriParentIndex_ = 0;
        int oriIndex_ = 0;
        Transform3D relativeTransform_;
        Transform3D globalTransform_;
    };


}

#endif