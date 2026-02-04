#ifndef __SUN_META_WRITIER_H
#define __SUN_META_WRITIER_H

#include "export.h"
#include <string>

namespace Sun {

    class MetaParser;

    class SCRIPT_API MetaWriter {
    public:
        void write(MetaParser* parser ,const std::string& path);

    };
}

#endif