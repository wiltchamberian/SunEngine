#ifndef __COMMON_H
#define __COMMON_H

#include <string>
#include <vector>
#include "Events.h"
#include "Math/Vector3D.h"
#include "Math/Matrix4x4.h"
#include "Common/Macros.h"
#include "Common/BasicDataType.h"
#include "glsl.h"

namespace Sun {

#define TO_STR(s) (#s)
#define TAG(s) (#s)

#define INVALID_HANDLE -1

    extern bool g_is_server;

    extern bool g_is_journal;

    // mouse pos
    extern float g_lastX;
    extern float g_lastY;

    enum RenderPassType {
        RASTERIZATION,
        RAY_TRACING,
    };

    //comType
    enum class ComponentType {
        COM_ACTOR,
        COM_STATIC_MESH,
        COM_SKELETAL_MESH,
        COM_SPHERE,
        COM_FLUID2D,
    };

    enum RenderPassComType {
        RP_STATIC_MESH,
        RP_SKELETAL_MESH,
        RP_SPHERE,
    };

    enum class GRAPHICS_API_TYPE {
        G_OPENGL,
        G_VULKAN,
    };

    extern GRAPHICS_API_TYPE globalGetGraphics_API_Type();

    struct RenderInputInfo {
        RenderPassType passType;
        float dtime = 0.0;
        SEvent ev;
        int width = 0;
        int height = 0;
        //proj*view
        Matrix4x4 vp_;
        //view
        Matrix4x4 view_;
        //照相机或者视点的世界坐标
        vec3 viewPos_;
    };

    struct VboEboHandle {
        unsigned int vbo = INVALID_HANDLE; 
        unsigned int ebo = INVALID_HANDLE;    
    };
    
    struct GpuVertexDataHandle {
        //顶点数组句柄
        unsigned int vao = INVALID_HANDLE;

        //顶点个数
        int vertexCount = 0;
        //索引个数(0表示不使用索引数组)
        int indexCount= 0;

        std::vector<VboEboHandle> vec;

        //实例化渲染，0表示不使用实例化渲染，>0表示实列个数
        int numInstance = 0;

        //绘制类型:三角形样式
        EPrimitiveType mode = EPrimitiveType::TRIANGLES;

        inline friend bool operator < (const GpuVertexDataHandle& h1, const GpuVertexDataHandle& h2) {
            return h1.vao < h2.vao;
        }

        bool isValid() {
            return vao != INVALID_HANDLE;
        }

    };

    struct ShaderCreateInfo {
        std::string name;
        EShaderType type;
    };
    struct ProgramCreateInfo {
        std::string dir_;
        std::vector<ShaderCreateInfo> infos_;
        //glsl global uniform init

        std::vector<Uniform> uniformInfos_;
        std::string shaderProgramName_; //used for map key
    };

    struct Extend {
        int width;
        int height;
    };

    struct Rect {
        int x = 0;
        int y = 0;
        int w = 0;
        int h = 0;
    };

    //x,y,w,h分别是viewport时相对整个窗口位置的比例,should all in [0,1],
    //举例来说，窗口为width*height, 
    //则等价于执行glViewport(x*width,y*height,w*width,h*height)
    struct ViewPortRatio {
        float x = 0.0;
        float y = 0.0;
        float w = 1.0;
        float h = 1.0;
    };

#define FORCE_INLINE __forceinline

#define ALBEDO 0
#define METALLIC 1
#define ROUGHNESS 2
#define NORMAL 3
#define PBR_MAP_NUM 4

#ifndef OUT
#define OUT
#endif
#ifndef IN
#define IN
#endif
#ifndef IN_OUT
#define IN_OUT
#endif

}





#endif

