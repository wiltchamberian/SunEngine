#ifndef __SUN_META_PARSER_H
#define __SUN_META_PARSER_H

#include "export.h"
#include "Lexer.h"
#include "MetaData.h"
#include "MetaAllocator.h"
#include <vector>

namespace Sun {

    struct ParseConfig {
    public:
        //是否记录成员字段名
        bool recordMemberName = false;
        
    };

    //元信息解析器
    class SCRIPT_API MetaParser {
    public:
        friend class MetaWriter;

        MetaParser(Lexer* lexer);

        void setParseConfig(const ParseConfig& config);

        void parse();

        
    protected:
        void reset();
        void skipComment();
        void parseEnum();
        void parseClass(MetaType type);
        void clearMetaDatas();

        //构建基础元数据类型
        void buildBasicMetaDatas();
 
        //查找tk是否已经存在
        MetaData* search(const SToken& tk);
        Lexer* lexer_ = nullptr;

        ParseConfig config_;

        //解析结果
        std::vector<MetaData*> metaDatas_;
        //枚举值类型，单独存储

        MetaAllocator allocator_;
    };


}

#endif 