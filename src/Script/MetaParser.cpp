#include "MetaParser.h"
#include <cassert>

namespace Sun {

    MetaParser::MetaParser(Lexer* lexer) :
        lexer_(lexer)
    {
        reset();
    }

    void MetaParser::setParseConfig(const ParseConfig& config) {


    }

    void MetaParser::parse() {
        if (lexer_ == nullptr) return;
        metaDatas_.clear();

        while (true) {
            skipComment();

            SToken token = lexer_->nextToken();
            if (token.type == T_EOF) {
                break;
            }
            
            if (token.type == T_CLASS) {
                parseClass(MetaType::CLASS);
            }
            else if (token.type == T_STRUCT) {
                parseClass(MetaType::STRUCT);
            }
            else if (token.type == T_ENUM) {
                parseEnum();
            }
            else {
                continue;
            }
        }

    }

    void MetaParser::parseClass(MetaType type) {
        SToken tk = lexer_->nextToken();
        assert(tk.type == T_IDENT);

        //提取类元数据
        MetaData* meta = allocator_.alloc(type, tk.literal);
        
        //提取类成员信息
        assert(lexer_->nextToken().type == T_LPAREN);
        for (SToken token = lexer_->nextToken(); token.type != T_RBRACE; token=lexer_->nextToken()) {

            MetaData* data = search(token);
            MetaDataWrapper wrapper;
            wrapper.data = data;
            wrapper.type = MetaType::CLASS;
            meta->members.push_back(wrapper);
            
            SToken st = lexer_->nextToken();
            if (st.type == T_MUL) {
                
            }
            assert(st.type == T_IDENT);

            if (config_.recordMemberName) {
                meta->memberNames.push_back(st.literal);
            }
            st = lexer_->nextToken();
            assert(st.type == T_SEMICOLON);

        }

        metaDatas_.push_back(meta);
    }

    void MetaParser::parseEnum() {
        SToken tk = lexer_->nextToken();
        assert(tk.type == T_IDENT);

        //提取类元数据
        MetaData* meta = allocator_.alloc(MetaType::ENUM, tk.literal);

        //提取枚举成员信息
        assert(lexer_->nextToken().type == T_LPAREN);
        for (SToken token = lexer_->nextToken(); token.type != T_RBRACE; token = lexer_->nextToken()) {
            assert(token.type == T_IDENT);

            lexer_->rollback();
            meta->literals.push_back(lexer_->tokenStringBefore(','));

            lexer_->readChar();
        }

        metaDatas_.push_back(meta);
        return;
    }

    MetaData* MetaParser::search(const SToken& tk) {
        for (auto& it : metaDatas_) {
            if (it->name == tk.literal) {
                return it;
            }
        }
        assert(false);
        return nullptr;
    }

    void MetaParser::reset() {
        clearMetaDatas();
        buildBasicMetaDatas();
    }

    void MetaParser::clearMetaDatas() {
        for (int i = 0; i < metaDatas_.size(); ++i) {
            allocator_.destroy(metaDatas_[i]);
        }
        metaDatas_.clear();
    }

    void MetaParser::buildBasicMetaDatas() {
        MetaData* meta = nullptr;

#define PUSH_IN(type) meta = allocator_.alloc(type);\
        metaDatas_.push_back(meta);

        PUSH_IN(MetaType::INT);
        PUSH_IN(MetaType::INT32);
        PUSH_IN(MetaType::INT64);
        PUSH_IN(MetaType::UINT32);
        PUSH_IN(MetaType::UINT64);
        PUSH_IN(MetaType::FLOAT);
        PUSH_IN(MetaType::DOUBLE);
        PUSH_IN(MetaType::CHAR);
        PUSH_IN(MetaType::UCHAR);
    }

    void MetaParser::skipComment() {
        bool bingo = lexer_->expect("//");
        if (bingo) {
            lexer_->skipToNext("\n");
            return;
        }
        bingo = lexer_->expect("/*");
        if (bingo) {
            lexer_->skipToNext("*/");
            return;
        }
    }
}