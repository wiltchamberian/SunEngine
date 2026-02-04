#include "ContextBuilder.h"

namespace Sun {

namespace rhi {


    ContextBuilder& ContextBuilder::setAppName(const char* name) {
        appInfo_.applicationName = name;
        return *this;
    }

    ContextBuilder& ContextBuilder::setEngineName(const char* name) {
        appInfo_.engineName = name;
        return *this;
    }

    ContextBuilder& ContextBuilder::setAppVersion(uint32_t version) {
        appInfo_.applicationVersion = version;
        return *this;
    }

    ContextBuilder& ContextBuilder::setEngineVersion(uint32_t version){
        appInfo_.engineVersion = version;
        return *this;
    }

    ContextBuilder& ContextBuilder::setApiVersion(uint32_t version) {
        appInfo_.apiVersion = version;
        return *this;
    }

    ContextBuilder& ContextBuilder::setExtensions(const std::vector<std::string>& extensions) {
        extensions_ = extensions;
        return *this;
    }

    ContextBuilder& ContextBuilder::addExtensions(const char* extension) {
        extensions_.emplace_back(extension);
        return *this;
    }

    ContextBuilder& ContextBuilder::removeExtension(const char* extension) {
        for (auto iter = extensions_.begin(); iter != extensions_.end(); ++iter) {
            if (*iter == extension) {
                extensions_.erase(iter);
                break;
            }
        }
        return *this;
    }

    ContextBuilder& ContextBuilder::setEnabledLayers(const std::vector<std::string>& layers) {
        layers_ = layers;
        return *this;
    }

    ContextBuilder& ContextBuilder::addEnabledLayer(const char* layer) {
        layers_.emplace_back(layer);
        return *this;
    }

    ContextBuilder& ContextBuilder::removeEnabledLayer(const char* layer) {
        for (auto iter = layers_.begin(); iter != layers_.end(); ++iter) {
            if (*iter == layer) {
                extensions_.erase(iter);
                break;
            }
        }
        return *this;
    }
}

}