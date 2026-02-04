#include "repl.h"
#include "Parser.h"

namespace Sun {

    SCRIPT_API void start_clever() {
        std::cout << "Hello! This is the Clever Programming Language!\n\Feel free to type in commands\n>> ";
        Lexer* lex = new Lexer();
        Parser* parser = new Parser(lex);
        Environment* ev = new Environment();
        //const std::string monkeyFace = " __, __\n\
        //    .--. . - " " - . .--.\n\
        //    / .. \ / . - . . - . \ / .. \\n\
        //    | | '| / Y \ |' | |\n\
        //    | \ \ \ 0 | 0 / / / |\n\
        //    \ '- ,\.-"""""""-./, -' /\n\
        //    '' - ' /_ ^ ^ _\ ' - ''\n\
        //    | \._ _./ |\n\
        //    \ \ '~' / /\n\
        //    '._ ' -= -' _.'\n\
        //    '-----'\n\
        //    ";

        char buf[1024];
        std::string str;
        const char* mode = "[eval]";
        while (std::getline(std::cin, str)) {
            if (str == "[parser]") {
                mode = "[parser]";
                std::cout << ">> ";
                continue;
            }
            else if (str == "[eval]") {
                mode = "[eval]";
                std::cout << ">> ";
                continue;
            }
            //std::string str(buf);
            parser->clearAndSetInput(str);
            std::shared_ptr<Program> program(parser->parseProgram());

            std::vector<std::string> errors = parser->outputErrors();
            if (errors.size() != 0) {
                std::cout << "parser errors:\n";
                for (auto& it : errors) {
                    std::cout << "\t" + it + "\n";
                }
                std::cout << ">> ";
                continue;
            }
            if (mode == "[parser]") {
                std::cout << program->printString() << std::endl;
            }
            else if (mode == "[eval]") {
                std::shared_ptr<Object_t> obj = program->eval(ev);
                if (obj != nullptr) {
                    std::cout << obj->inspect() << std::endl;
                }
                else {
                    std::cout << "null" << std::endl;
                }
            }
            
            std::cout << ">> ";
        }
    }
}