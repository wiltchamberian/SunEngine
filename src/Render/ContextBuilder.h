/*
*/

#ifndef __SUN_RHI_CONTEXT_BUILDER_H
#define __SUN_RHI_CONTEXT_BUILDER_H

#include "RHIDefine.h"

namespace Sun {

namespace rhi {

    class ContextBuilder {
    public:
        virtual HContext build() = 0;
        virtual void destroy(HContext) = 0;

        ContextBuilder& setAppName(const char* name);
        ContextBuilder& setEngineName(const char* name);
        ContextBuilder& setAppVersion(uint32_t version);
        ContextBuilder& setEngineVersion(uint32_t version);
        ContextBuilder& setApiVersion(uint32_t version);
        ContextBuilder& setExtensions(const std::vector<std::string>& extensions);
        ContextBuilder& addExtensions(const char* extension);
        ContextBuilder& removeExtension(const char* extension);
        ContextBuilder& setEnabledLayers(const std::vector<std::string>& layers);
        ContextBuilder& addEnabledLayer(const char* layer);
        ContextBuilder& removeEnabledLayer(const char* layer);

    protected:
        ApplicationInfo             appInfo_;
        std::vector<std::string>    extensions_;
        std::vector<std::string>    layers_;
        
    };

}   

}

#endif