#include "FactoryProducer.h"
#include "ProgramFactory.h"
#include "GLProgramFactory.h"
#include "ShaderFactory.h"
#include "GLShaderFactory.h"
#include "PCH.h"

namespace Sun {

    ProgramFactory* FactoryProducer::createProgramFactory() {
        ProgramFactory* factory = nullptr;
#ifdef  OPENGL
        factory = new GLProgramFactory();
#endif

        return factory;
    }

    ShaderFactory* FactoryProducer::createShaderFactory() {
        ShaderFactory* factory = nullptr;
#ifdef OPENGL
        factory = new GLShaderFactory();
#endif
        return factory;
    }
}