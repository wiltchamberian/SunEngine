#include "TestScript.h"
#include "Eval.h"
#include "Try.h"
#include <cassert>
#include <iostream>
#include <chrono>

namespace Sun {

    SCRIPT_API void runAllTestCases() {
        Main();
        auto t1 = std::chrono::high_resolution_clock::now();
        testLexer();
        testDefStatements();
        testReturnStatements();
        testIdentifierExpression();
        testBooleanExpression();
        testIfExpression();
        testIfElseExpression();
        testIntegerExpression();
        testParsingPrefixExpression();
        testParsingInfixExpressions();
        testOperatorPrecedenceParsing();

        testFunctionLiteralParsing();
        testFunctionParameterParsing();

        testCallExpressionParsing();

        testEvalIntegerExpression();
        testBangOperator();

        testEvalIfElseExpressions();
        testEvalReturnStatements();

        testErrorHandling();
        testEvalDefStatements();
        testEvalFunctionObject();
        testFunctionApplication();
        testClosures();

        testMyMain();

        auto t2 = std::chrono::high_resolution_clock::now();
        int milli = std::chrono::duration<double, std::milli>(t2 - t1).count();

        std::printf("test : %d ms,Test End!\n",milli);


    }

    // 这是导出函数的一个示例。
    SCRIPT_API void testLexer(void)
    {
        Lexer lexer("int five = 5;\
            int ten = 10;\
        int add = fn(x, y) {\
            x + y;\
        };\
        int result = add(five, ten);\
        !-/*5;\
        5 < 10 > 5;\
        if (5 < 10) {\
            return true;\
        }\
        else {\
            return false;\
        }\
        10 == 10; \
        10 != 9;"
        );

        std::vector<SToken> expected = {
            {T_INT, "int"},
            {T_IDENT, "five"},
            {T_ASSIGN, "="},
            {T_INTEGER, "5"},
            {T_SEMICOLON, ";"},
            {T_INT, "int"},
            {T_IDENT, "ten"},
            {T_ASSIGN, "="},
            {T_INTEGER, "10"},
            {T_SEMICOLON, ";"},
            {T_INT, "int"},
            {T_IDENT, "add"},
            {T_ASSIGN, "="},
            {T_FUNCTION, "fn"},
            {T_LPAREN, "("},
            {T_IDENT, "x"},
            {T_COMMA, ","},
            {T_IDENT, "y"},
            {T_RPAREN, ")"},
            {T_LBRACE, "{"},
            {T_IDENT, "x"},
            {T_PLUS, "+"},
            {T_IDENT, "y"},
            {T_SEMICOLON, ";"},
            {T_RBRACE, "}"},
            {T_SEMICOLON, ";"},
            {T_INT, "int"},
            {T_IDENT, "result"},
            {T_ASSIGN, "="},
            {T_IDENT, "add"},
            {T_LPAREN, "("},
            {T_IDENT, "five"},
            {T_COMMA, ","},
            {T_IDENT, "ten"},
            {T_RPAREN, ")"},
            {T_SEMICOLON, ";"},
            {T_BANG,"!"},
            {T_MINUS,"-"},
            {T_DIV,"/"},
            {T_MUL,"*"},
            {T_INTEGER,"5"},
            {T_SEMICOLON, ";"},
            {T_INTEGER, "5"},
            {T_LT, "<"},
            {T_INTEGER, "10"},
            {T_GT, ">"},
            {T_INTEGER, "5"},
            {T_SEMICOLON, ";"},
            {T_IF, "if"},
            {T_LPAREN,"("},
            {T_INTEGER,"5"},
            {T_LT ,"<"},
            {T_INTEGER,"10"},
            {T_RPAREN,")"},
            {T_LBRACE,"{"},
            {T_RETURN,"return"},
            {T_TRUE,"true" },
            {T_SEMICOLON,";"},
            {T_RBRACE,"}"},
            {T_ELSE,"else"},
            {T_LBRACE,"{"},
            {T_RETURN,"return"},
            {T_FALSE,"false"},
            {T_SEMICOLON,";"},
            {T_RBRACE,"}"},
            {T_INTEGER, "10"},
            {T_EQ, "=="},
            {T_INTEGER, "10"},
            {T_SEMICOLON,";"},
            {T_INTEGER, "10"},
            {T_NOT_EQ, "!="},
            {T_INTEGER, "9"},
            {T_SEMICOLON,";"},
            {T_EOF, ""},
        };

        for (int i = 0; i < expected.size(); ++i) {
            SToken tk = lexer.nextToken();
            if (tk.type != expected[i].type) {
                assert(false);
            }
            if (tk.literal != expected[i].literal) {
                assert(false);
            }
        }

        SToken tk = lexer.nextToken();

        return;
    }

    void checkParserErrors(Parser* par) {
        std::vector<std::string> errors = par->outputErrors();
        if (errors.empty())return;

        errorf("parser has %d errors\n", errors.size());
        for (std::string& str : errors) {
            errorf("parser error:%s\n", str.c_str());
        }
    }

    SCRIPT_API Program* prepareProgram(const std::string& str) {
        Lexer* lex = new Lexer(str);
        Parser* parser = new Parser(lex);
        Program* pro = parser->parseProgram();
        checkParserErrors(parser);
        delete lex;
        delete parser;
        return pro;
    }

    SCRIPT_API void testDefStatements() {
        struct Tmp {
            std::string input;
            std::string expectedIdentifier;
            Variant expectedValue;
        };
        std::vector<Tmp> tests = {
            {"int x= 5;","x",5},
            {"int y = true;","y",true},
            {"int foobar = y;","foobar",std::string("y")}
        };
        for (int i = 0; i < tests.size(); ++i) {
            Program* pro = prepareProgram(tests[i].input);
            if (pro->statements.size() != 1) {
                errorf("program.Statements does not contain 1 statements. got=%d", pro->statements.size());
            }
            Statement* stmt = pro->statements[0];
            if (!testDefStatement(stmt, tests[i].expectedIdentifier)) {
                return;
            }
            DefStatement* let = dynamic_cast<DefStatement*>(stmt);
            if (let == nullptr) {
                errorf("statement is not defstatement\n");
                return;
            }
            if (!testLiteralExpression(let->express, tests[i].expectedValue)) {
                return;
            }
        }
    }

    bool testDefStatement(Statement* st, const std::string& name) {
        if (!SToken::isDef(st->tk.type)/*st->tokenLiteral() != "let"*/) {
            assert(false);
            return false;
        }

        DefStatement* stmt = dynamic_cast<DefStatement*>(st);
        if (!stmt) {
            assert(false);
            return false;
        }

        if (stmt->ident->value != name) {
            //assert(false);
            errorf("letstatement ident value not equal name!\n");
            return false;
        }

        if (stmt->ident->tokenLiteral() != name) {
            errorf("letstatement ident tokenliteral not equal name!\n");
            //assert(false);
            return false;
        }

        return true;
    }

    SCRIPT_API void testReturnStatements() {
        std::string input = "return 5;\
        return 10;\
        return 993322;\
        ";

        Program* pro = prepareProgram(input);

        if (pro->statements.size() != 3) {
            //assert(false);
            errorf("program.Statements does not contain 3 statements. got=%d", pro->statements.size());
        }

        for (int i = 0; i < pro->statements.size(); ++i) {
            ReturnStatement* rt = dynamic_cast<ReturnStatement*>(pro->statements[i]);
            if (rt == nullptr) {
                errorf("stmt not *ast.returnStatement. got=%T", rt);
                continue;
            }
            if (rt->tokenLiteral() != "return") {
                errorf("returnStmt.TokenLiteral not 'return', got %s",
                    rt->tokenLiteral().c_str());
            }
        }

    }

    void testString() {
        Program* program = new Program();
        DefStatement* let = new DefStatement(SToken(T_INT, "int"));
        program->statements.push_back(let);
        let->ident = new Identifier(SToken(T_IDENT), "myVar");
        let->ident->value = "myVar";
        let->express = new Identifier(SToken(T_IDENT, "anotherVar"), "anotherVar");

        if (program->printString() != "let myVar =  anotherVar;") {
            errorf("program.String() wrong.got = %s", program->printString());
        }
    }

    void testIdentifierExpression() {
        std::string input = "foobar;";

        Program* pro = prepareProgram(input);

        if (pro->statements.size() != 1) {
            //assert(false);
            errorf("program has not enough statements. got=%d\n", pro->statements.size());
            return;
        }

        ExpressionStatement* st = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
        if (st == nullptr) {
            errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n",
                pro->statements[0]);
            return;
        }

        testIdentify(st->expression, "foobar");
    }

    void testBooleanExpression() {
        std::string input = "true;";

        Program* pro = prepareProgram(input);

        if (pro->statements.size() != 1) {
            //assert(false);
            errorf("program has not enough statements. got=%d\n", pro->statements.size());
            return;
        }

        ExpressionStatement* st = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
        if (st == nullptr) {
            errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n",
                pro->statements[0]);
            return;
        }

        testBooleanLiteral(st->expression, true);
    }

    void testIfExpression() {
        std::string input = "if (x < y) { x }";
        Program* pro = prepareProgram(input);

        if (pro->statements.size() != 1) {
            //assert(false);
            errorf("program has not enough statements. got=%d\n", pro->statements.size());
            return;
        }

        ExpressionStatement* st = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
        if (st == nullptr) {
            errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n",
                pro->statements[0]);
            return;
        }

        IfExpression* exp = dynamic_cast<IfExpression*>(st->expression);
        if (exp == nullptr) {
            errorf("stmt.Expression is not ast.IfExpression. got=%T\n");
            return;
        }

        if (!testInfixExpression(exp->condition, std::string("x"), "<", std::string("y"))) {
            return;
        }

        if (exp->consequence->statements.size() != 1) {
            errorf("consequence is not 1 statements. got=%d\n",
                exp->consequence->statements.size());
        }

        ExpressionStatement* consequence = dynamic_cast<ExpressionStatement*>(exp->consequence->statements[0]);
        if (consequence == nullptr) {
            errorf("Statements[0] is not ast.ExpressionStatement. got=%T");
            return;
        }

        if (!testIdentify(consequence->expression, "x")) {
            return;
        }

        if (exp->alternative != nullptr) {
            errorf("exp.Alternative.Statements was not nil. got=%+v\n");
        }
    }

    void testIfElseExpression() {
        std::string input = "if (x < y) {x}else { x }";
        Program* pro = prepareProgram(input);

        if (pro == nullptr) {
            assert(false);
            std::cout << "ParseProgram() returned nil\n";
        }
        if (pro->statements.size() != 1) {
            //assert(false);
            errorf("program has not enough statements. got=%d\n", pro->statements.size());
            return;
        }

        ExpressionStatement* st = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
        if (st == nullptr) {
            errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n",
                pro->statements[0]);
            return;
        }

        IfExpression* exp = dynamic_cast<IfExpression*>(st->expression);
        if (exp == nullptr) {
            errorf("stmt.Expression is not ast.IfExpression. got=%T\n");
            return;
        }

        if (!testInfixExpression(exp->condition, std::string("x"), "<", std::string("y"))) {
            return;
        }

        if (exp->consequence->statements.size() != 1) {
            errorf("consequence is not 0 statements. got=%d\n",
                exp->consequence->statements.size());
        }

        if (exp->alternative == nullptr) {
            errorf("exp.Alternative.Statements got nil. got=%+v\n");
            return;
        }
        if (exp->alternative->statements.size() != 1) {
            errorf("exp.Alternative.Statements size should be %s,got %s\n", 1, exp->alternative->statements.size());
        }
        ExpressionStatement* alternative = dynamic_cast<ExpressionStatement*>(exp->alternative->statements[0]);
        if (alternative == nullptr) {
            errorf("Statements[0] is not ast.ExpressionStatement. got=%T\n");
            return;
        }
        if (!testIdentify(alternative->expression, "x")) {
            return;
        }
    }

    void testIntegerExpression(){
        std::string input = "5;";

        Program* pro = prepareProgram(input);

        if (pro == nullptr) {
            assert(false);
            std::cout << "ParseProgram() returned nil\n";
        }
        if (pro->statements.size() != 1) {
            //assert(false);
            errorf("program has not enough statements. got=%d\n", pro->statements.size());
            return;
        }
        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
        if (stmt == nullptr) {
            errorf("program.Statements[0] is not ast.ExpressionStatement\n");
            return;
        }
        
        testIntegerLiteral(stmt->expression, 5);
    }

    SCRIPT_API void testParsingPrefixExpression() {
        struct Tmp {
            std::string input;
            std::string op;
            Variant var;
        };
        std::vector<Tmp> prefixTest = {
            {"!5;","!",std::int64_t(5)},
            {"-15;","-",std::int64_t(15)},
            {"!true;","!",true},
            {"!false;","!",false},
        };
 

        for (auto& it : prefixTest) {
            Program* pro = prepareProgram(it.input);

            if (pro->statements.size() != 1) {
                errorf("program.Statements does not contain %d statements. got=%d\n", 1, pro->statements.size());
            }

            ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
            if (stmt == nullptr) {
                errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n");
                return;
            }

            PrefixExpression* exp = dynamic_cast<PrefixExpression*>(stmt->expression);
            if (exp == nullptr) {
                errorf("stmt is not ast.PrefixExpression. got=%T\n");
                return;
            }
            if (exp->op != it.op) {
                errorf("exp.Operator is not '%s' .got = %s\n", it.op, exp->op);
            }
            if (!testLiteralExpression(exp->right, it.var)) {
                return;
            }
        }
    }

    SCRIPT_API void testParsingInfixExpressions() {
        struct Tmp {
            std::string input;
            Variant leftValue;
            const char* op;
            Variant rightValue;
        };
        std::vector<Tmp> vec = {
            {"5 + 5;", 5, "+", 5},
            {"5 - 5;", 5, "-", 5},
            {"5 * 5;", 5, "*", 5},
            {"5 / 5;", 5, "/", 5},
            {"5 > 5;", 5, ">", 5},
            {"5 < 5;", 5, "<", 5},
            {"5 == 5;", 5, "==", 5},
            {"5 != 5;", 5, "!=", 5},
            {"true==true",true,"==",true},
            {"true!=false",true,"!=",false},
            {"false==false",false,"==",false},
        };
        for (auto& tt : vec) {
            Program* pro = prepareProgram(tt.input);

            if (pro->statements.size() != 1) {
                errorf("program.Statements does not contain %d statements. got=%d\n", 1, pro->statements.size());
            }

            ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
            if (stmt == nullptr) {
                errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n");
                return;
            }

            testInfixExpression(stmt->expression, tt.leftValue, tt.op, tt.rightValue);
#if 0
            InfixExpression* exp = dynamic_cast<InfixExpression*>(stmt->expression);
            if (exp == nullptr) {
                errorf("exp is not ast.InfixExpression. got=%T\n");
                return;
            }

            if (!testIntegerLiteral(exp->left, tt.leftValue)) {
                return;
            }
            if (exp->op != tt.op) {
                errorf("exp.Operator is not '%s'. got=%s\n",
                    tt.op, exp->op);
            }
            if (!testIntegerLiteral(exp->right, tt.rightValue)) {
                return;
            }
#endif
        }
    }

    SCRIPT_API void testOperatorPrecedenceParsing() {
        struct Tmp {
            std::string input;
            std::string expected;
        };

        std::vector<Tmp> vec = {
            {"1+2+3","((1 + 2) + 3)"},
            {"-a*b","((-a) * b)"},
            {"!-a","(!(-a))"},
            {"a+b+c","((a + b) + c)"},
            {"a+b-c","((a + b) - c)"},
            {"a*b*c","((a * b) * c)"},
            {"a*b/c","((a * b) / c)"},
            {"a+b/c","(a + (b / c))"},
            {"a+b*c+d/e-f","(((a + (b * c)) + (d / e)) - f)"},
            {"3+4;-5*5","(3 + 4)((-5) * 5)"},
            {"5<4!=3>4","((5 < 4) != (3 > 4))"},
            {"3+4*5==3*1+4*5","((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
            {"true","true"},
            {"false","false"},
            {"3>5==false","((3 > 5) == false)"},
            {"3<5==true","((3 < 5) == true)"},
            {"1+(2+3)+4","((1 + (2 + 3)) + 4)"},
            {"(5+5)*2","((5 + 5) * 2)"},
            {"2/(5+5)","(2 / (5 + 5))"},
            {"-(5+5)","(-(5 + 5))"},
            {"!(true == true)", "(!(true == true))"},
            {"a+add(b*c)+d","((a + add((b * c))) + d)"},
            {"add(a,b,1,2*3,4+5,add(6,7*8))","add(a, b, 1, (2 * 3), (4 + 5), add(6, (7 * 8)))"},
            {"add(a+b+c*d/f+g)","add((((a + b) + ((c * d) / f)) + g))"}
        };

        for (auto& tt : vec) {
            Program* pro = prepareProgram(tt.input);
            std::string actual = pro->printString();
            if (actual != tt.expected) {
                errorf("expected=%s, got=%s\n", tt.expected.c_str(), actual.c_str());
            }
        }
    }

    SCRIPT_API void testFunctionLiteralParsing() {
        std::string input = "fn(x, y) { x + y; }";
        Program* pro = prepareProgram(input);
        if (pro->statements.size() != 1) {
            errorf("program.Statements does not contain %d statements. got=%d\n", 1, pro->statements.size());
        }

        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
        if (stmt == nullptr) {
            errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n");
            return;
        }
                
        FunctionLiteral* function = dynamic_cast<FunctionLiteral*>(stmt->expression);
        if (function == nullptr) {
            errorf("stmt.Expression is not ast.FunctionLiteral. got=%T",
                stmt->expression);
            return;
        }

        if (function->parameters.size() != 2) {
            errorf("function literal parameters wrong. want 2 got=%d\n", function->parameters.size());
        }

        testLiteralExpression(function->parameters[0], std::string("x"));
        testLiteralExpression(function->parameters[1], std::string("y"));

        if (function->body==nullptr || function->body->statements.size() != 1) {
            errorf("function.Body.Statements has not 1 statements,got=%d\n", function->body->statements.size());
        }

        ExpressionStatement* bodyStmt = dynamic_cast<ExpressionStatement*>(function->body->statements[0]);
        if (bodyStmt == nullptr) {
            errorf("function body stmt is not ast.ExpressionStatement. got=%T",
                function->body->statements[0]);
            return;
        }
        testInfixExpression(bodyStmt->expression, std::string("x"), "+", std::string("y"));
    }

    SCRIPT_API void testFunctionParameterParsing() {
        struct Tmp {
            std::string input;
            std::vector<std::string> expectedParams;
        };
        std::vector<Tmp> tests = {
            {"fn(){};",{}},
            {"fn(x) {};",{"x"}},
            {"fn(x,y,z){};",{"x","y","z"}}
        };
        for (auto& tt : tests) {
            Program* pro = prepareProgram(tt.input);
            if (pro->statements.size() != 1) {
                errorf("program.Statements does not contain %d statements. got=%d\n", 1, pro->statements.size());
            }

            ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
            if (stmt == nullptr) {
                errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n");
                return;
            }
            
            FunctionLiteral* function = dynamic_cast<FunctionLiteral*>(stmt->expression);
            if (function == nullptr || function->parameters.size() != tt.expectedParams.size()) {
                errorf("length parameters wrong. want %d, got=%d\n",
                    tt.expectedParams.size(), function->parameters.size());
            }

            for (int i = 0; i < tt.expectedParams.size();++i) {
                testLiteralExpression(function->parameters[i], tt.expectedParams[i]);
            }
        }

    }

    SCRIPT_API void testCallExpressionParsing() {
        std::string input = "add(1, 2*3, 4+5);";
        Program* pro = prepareProgram(input);

        if (pro->statements.size() != 1) {
            errorf("program.Statements does not contain %d statements. got=%d\n", 1, pro->statements.size());
        }

        ExpressionStatement* stmt = dynamic_cast<ExpressionStatement*>(pro->statements[0]);
        if (stmt == nullptr) {
            errorf("program.Statements[0] is not ast.ExpressionStatement. got=%T\n");
            return;
        }

        CallExpression* exp = dynamic_cast<CallExpression*>(stmt->expression);
        if (exp == nullptr) {
            errorf("stmt.Expression is not ast.CallExpression. got=%T", stmt->expression);
            return;
        }

        if (!testIdentify(exp->function, std::string("add"))) {
            return;
        }

        if (exp->arguments.size() != 3) {
            errorf("wrong length of arguments. got=%d", exp->arguments.size());
        }

        testLiteralExpression(exp->arguments[0], 1);
        testInfixExpression(exp->arguments[1], 2, "*", 3);
        testInfixExpression(exp->arguments[2], 4, "+", 5);
    }

    SCRIPT_API bool testIntegerLiteral(Expression* il, std::int64_t value) {
        IntegerLiteral* integ = dynamic_cast<IntegerLiteral*>(il);
        if (integ == nullptr) {
            errorf("il not *ast.IntegerLiteral. got=%T\n", il);
            return false;
        }
        if (integ->value != value){
            errorf("integ.Value not %d. got=%d\n", value, integ->value);
            return false;
        }
        if (integ->tokenLiteral() != std::to_string(value)) {
            errorf("integ.TokenLiteral not %d. got=%s\n", value,
                integ->tokenLiteral().c_str());
            return false;
        }
        return true;
    }

    SCRIPT_API bool testBooleanLiteral(Expression* exp, bool value) {
        BooleanExp* ident = dynamic_cast<BooleanExp*>(exp);
        if (ident == nullptr) {
            errorf("exp not Identifier\n");
            return false;
        }
        if (ident->value != value) {
            errorf("ident.Value not %d. got=%d\n", value, ident->value);
            return false;
        }
        std::string bl = value ? "true" : "false";
        if (ident->tokenLiteral() != bl) {
            errorf("ident.TokenLiteral not %s. got = %s\n", bl.c_str(),
                ident->tokenLiteral().c_str());
            return false;
        }
        return true;


    }

    SCRIPT_API bool testIdentify(Expression* exp, const std::string& value) {
        Identifier* ident = dynamic_cast<Identifier*>(exp);
        if (ident == nullptr) {
            errorf("exp not Identifier\n");
            return false;
        }
        if (ident->value != value) {
            errorf("ident.Value not %s. got=%s\n", value.c_str(), ident->value.c_str());
            return false;
        }
        if (ident->tokenLiteral() != value) {
            errorf("ident.TokenLiteral not %s. got = %s\n", value.c_str(),
                ident->tokenLiteral().c_str());
            return false;
        }
        return true;
    }

    bool testLiteralExpression(Expression* exp, const Variant& var) {
        if (std::get_if<std::int64_t>(&var)) {
            return testIntegerLiteral(exp, std::get<std::int64_t>(var));
        }
        else if (std::get_if<int>(&var)) {
            return testIntegerLiteral(exp, std::get<int>(var));
        }
        else if (std::get_if<bool>(&var)) {
            return testBooleanLiteral(exp, std::get<bool>(var));
        }
        else if (std::get_if<std::string>(&var)) {
            return testIdentify(exp, std::get<std::string>(var));
        }
        return false;
    }

    bool testInfixExpression(Expression* exp, const Variant& left, const char* op, const Variant& right) {
        InfixExpression* opExp = dynamic_cast<InfixExpression*>(exp);
        if (opExp == nullptr) {
            errorf("exp is not ast.OperatorExpression. got=%T(%s)\n", exp, exp);
            return false;
        }
        if (!testLiteralExpression(opExp->left, left)) {
            return false;
        }
        if (opExp->op != op) {
            errorf("exp.Operator is not '%s'. got=%s\n", op, opExp->op);
            return false;
        }
        if (!testLiteralExpression(opExp->right, right)) {
            return false;
        }
        return true;
    }

    SCRIPT_API void testEvalIntegerExpression() {
        struct Tmp {
            std::string input;
            std::int64_t expected;
        };

        std::vector<Tmp> tests = {
            {"5",5},
            {"10",10},
            {"-5",-5},
            {"-10",-10},
            {"5+5+5+5-10",10},
            {"2*2*2*2*2",32},
            {"-50+100+-50",0},
            {"5*2+10",20},
            {"5+2*10",25},
            {"20+2*-10",0},
            {"50/2*2+10",60},
            {"2*(5+10)",30},
            {"3*3*3+10",37},
            {"3*(3*3)+10",37},
            {"(5+10*2+15/3)*2+-10",50},
        };

        for (auto& tt : tests) {
            std::shared_ptr<Object_t> evaluated = testEval(tt.input);
            testIntegerObject(evaluated.get(), tt.expected);
        }
    }

    std::shared_ptr<Object_t> testEval(const std::string& input) {
        std::unique_ptr<EvalVisitor> visitor;
        std::unique_ptr<Environment> ev(new Environment);
        Lexer* lex = new Lexer(input);
        Parser* parser = new Parser(lex);
        Program* program = parser->parseProgram();
        std::shared_ptr<Object_t> obj = program->accept(visitor.get(),ev.get());
        delete program;
        delete parser;
        delete lex;
        return obj;
    }

    bool testIntegerObject(Object_t* obj, std::int64_t expected) {
        if (obj == nullptr) {
            errorf("object is not integer,is null!");
            return false;
        }
        bool ok = (obj->type() == INTEGER_OBJ);
        std::int64_t value = obj->intValue();
        if (!ok) {
            errorf("object is not integer.got %s\n", obj->type());
            return false;
        }
        if (value != expected) {
            errorf("object has wrong value. got=%d, want=%d",
                value, expected);
            return false;
        }
        return true;
    }

    void testEvalBooleanExpression() {
        struct Tmp {
            std::string input;
            bool expected;
        };

        std::vector<Tmp> tests = {
            {"true",true},
            {"false",false},
            {"1<2",true},
            {"1>2",false},
            {"1<1",false},
            {"1>1",false},
            {"1==1",true},
            {"1!=1",true},
            {"1==2",false},
            {"1!=2",true},
            {"true == true",true},
            {"false==false",true},
            {"true == false", false},
            {"true != false", true},
            {"false != true", true},
            {"(1 < 2) == true", true},
            {"(1 < 2) == false", false},
            {"(1 > 2) == true", false},
            {"(1 > 2) == false", true},
        };

        for (auto& tt : tests) {
            std::shared_ptr<Object_t> evaluated = testEval(tt.input);
            testBooleanObject(evaluated.get(), tt.expected);
        }

    }

    bool testBooleanObject(Object_t* obj, bool expected) {
        if (obj == nullptr) {
            errorf("object is null\n");
            return false;
        }
        bool ok = (obj->type() == BOOLEAN_OBJ);
        if (!ok) {
            errorf("object is not integer.got %s\n", obj->type());
            return false;
        }
        bool value = obj->boolValue();    
        if (value != expected) {
            errorf("object has wrong value. got=%d, want=%d",
                value, expected);
            return false;
        }
        return true;
    }

    void testBangOperator() {
        struct Tmp {
            std::string input;
            bool expected;
        };
        std::vector<Tmp> tests = {
            { "!true", false},
            {"!false", true},
            {"!5", false},
            {"!!true",true},
            {"!!false",false},
            {"!!5",true}
        };

        for (auto& tt : tests) {
            std::shared_ptr<Object_t> obj = testEval(tt.input);
            testBooleanObject(obj.get(), tt.expected);
        }
    }

    SCRIPT_API void testEvalIfElseExpressions() {
        struct Tmp {
            std::string input;
            Variant expected;
        };
        std::vector<Tmp> tests = {
            {"if (true) { 10 }", 10},
            {"if (false) { 10 }", (void*)nullptr},
            {"if (1) { 10 }", 10},
            {"if (1 < 2) { 10 }", 10},
            {"if (1 > 2) { 10 }", (void*)nullptr},
            {"if (1 > 2) { 10 } else { 20 }", 20},
            {"if (1 < 2) { 10 } else { 20 }", 10},
        };

        for (auto& tt : tests) {
            std::shared_ptr<Object_t> evaluated = testEval(tt.input);
            if (std::get_if<std::int64_t>(&tt.expected)){
                testIntegerObject(evaluated.get(), std::get<std::int64_t>(tt.expected));
            }
            else if (std::get_if<int>(&tt.expected)) {
                testIntegerObject(evaluated.get(), std::get<int>(tt.expected));
            }
            else {
                testNullObject(evaluated.get());
            }
        }
    }

    bool testNullObject(Object_t* obj) {
        if (obj != nullptr){
            errorf("object is not NULL. got=%T (%+v)", obj, obj);
            return false;
        }
        return true;
    }

    SCRIPT_API void testEvalReturnStatements() {
        struct Tmp {
            std::string input;
            std::int64_t expected;
        };

        std::vector<Tmp> tests = {
            {"return 10;", 10},
            {"return 10; 9;", 10},
            {"return 2*5;9;", 10},
            {"9; return 2*5; 9;", 10},
            {"if(10>1){\
               if(10>1){\
                 return 10;}\
               return 1;}" ,10}
        };

        for (auto& tt : tests) {
            std::shared_ptr<Object_t> evaluated = testEval(tt.input);
            testIntegerObject(evaluated.get(), tt.expected);
        }
    }

    SCRIPT_API void testErrorHandling() {
        struct Tmp {
            std::string input;
            std::string expected;
        };
        std::vector<Tmp> tests = {
            {"5+true;","type mismatch: INTEGER + BOOLEAN"},
            {"5 + true; 5;","type mismatch: INTEGER + BOOLEAN"},
            {
                "-true",
                "unknown operator: -BOOLEAN"
            },
            {
                "true + false;",
                "unknown operator: BOOLEAN + BOOLEAN"
            },
            {
                "5; true + false; 5",
                "unknown operator: BOOLEAN + BOOLEAN"
            },
            {
                "if (10 > 1) { true + false; }",
                "unknown operator: BOOLEAN + BOOLEAN"
            },
            {   "132\
                if (10 > 1) {\
                if (10 > 1) {\
                return true + false;\
                }\
                return 1;}"
                ,
                "unknown operator: BOOLEAN + BOOLEAN"
            },
            {
                "foobar","identifier not found: foobar"
            },
        };
        for (auto& tt : tests) {
            std::shared_ptr<Object_t> obj = testEval(tt.input);
            std::shared_ptr<Error> error = std::dynamic_pointer_cast<Error>(obj);
            if (error == nullptr) {
                errorf("no error object returned\n");
                continue;
            }
            //if (error->message != tt.expected) {
            //    errorf("wrong error message. expected=%s, got=%s",
            //        tt.expected.c_str(), error->message.c_str());
            //}
        }
    }

    void testEvalDefStatements() {
        struct Tmp {
            std::string input;
            std::int64_t expected;
        }; 
        std::vector<Tmp> tests = {
            {"int a = 5; a;", 5},
            { "int a = 5 * 5; a;", 25 },
            { "int a = 5; int b = a; b;", 5 },
            { "int a = 5; int b = a; int c = a + b + 5; c;", 15 },
        };
        for (auto& tt : tests) {
            testIntegerObject(testEval(tt.input).get(), tt.expected);
        }
    }

    SCRIPT_API void testEvalFunctionObject() {
        std::string input = "fn(x) { x + 2; };";
        std::shared_ptr<Object_t> evaluated = testEval(input);
        std::shared_ptr<Function> fn = std::dynamic_pointer_cast<Function>(evaluated);
        if(!fn){
            errorf("object is not Function. got=%T (%+v)\n", evaluated, evaluated);
            return;
        }
        if (fn->parameters.size() != 1) {
            errorf("function has wrong parameters. Parameters siz=%s\n",
                fn->parameters.size());
        }
        if (fn->parameters[0]->printString() != "x") {
            errorf("parameter is not 'x'. got=%s\n", fn->parameters[0]->printString());
        }
        std::string expectedBody = "(x + 2)";
        if (fn->body->printString() != expectedBody) {
            errorf("body is not %s. got=%s\n", expectedBody.c_str(), fn->body->printString().c_str());
        }

    }

    SCRIPT_API void testFunctionApplication() {
        struct Tmp {
            std::string input;
            std::int64_t expected;
        };
        std::vector<Tmp> tests = {
            {"int identity = fn(x) { x; }; identity(5);", 5},
            { "let identity = fn(x) { return x; }; identity(5);", 5 },
            { "let double = fn(x) { x * 2; }; double(5);", 10 },
            { "let add = fn(x, y) { x + y; }; add(5, 5);", 10 },
            { "let add = fn(x, y) { x + y; }; add(5 + 5, add(5, 5));", 20 },
            { "fn(x) { x; }(5)", 5 },
        };
        for(auto& tt : tests){
            testIntegerObject(testEval(tt.input).get(), tt.expected);
        }
    }

    SCRIPT_API void testClosures() {
        std::string input = "let newAdder = fn(x) {fn(y) { x + y };};\
        let addTwo = newAdder(2); \
        addTwo(2);";
        
        testIntegerObject(testEval(input).get(), 4);
    }

    SCRIPT_API void testMyMain() {
        std::string input = "\
        let plus = fn(x,y){ return x+y;};\
        let minus = fn(x,y){return x-y;};\
        int x = 3;\
        int y = x;\
        int k = plus(x, y); \
        {\
            return minus(x, y); \
        }\
        else {\
            ;\
        }\
        x = (x + y) * 3 + (x * plus(x, y));\
        return x;";

        Lexer* lex=new Lexer(input);
        Parser* parser = new Parser(lex);
        Program* pro = parser->parseProgram();
        std::cout << pro->printString() <<std::endl;
    }
}