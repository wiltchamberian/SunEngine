#include "MetaWriter.h"
#include "MetaParser.h"
#include "Util/FileIO.h"
#include <algorithm>
#include <cassert>

namespace Sun {

    void MetaWriter::write(MetaParser* parser,const std::string& path) {
        std::string data;

        std::vector<MetaData*> metas = parser->metaDatas_;
        //std::sort(metas.begin(), metas.end(), [](const MetaData*& t1,const MetaData*& t2)->bool {

        //    });

        for (int i = 0; i < metas.size(); ++i) {
            assert(metas[i] != nullptr);

            switch (metas[i]->metaType) {
            case MetaType::CLASS: 
            {

            }
            break;
            case MetaType::STRUCT:
            {

            }
            break;
            case MetaType::ENUM:
            {

            }
            break;
            default:
                break;        
            }
        }


        FileIO::writeFile(data.c_str(), data.size(), path);
    }
}