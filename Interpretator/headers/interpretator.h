#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "object.h"
#include "parser.h"
#include "symtab.h"
#include "function.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

class Interpreter {
public:
    Interpreter(std::vector<std::string> code, std::vector<std::string>* parametrs = nullptr, Object* arguments = nullptr);
    ~Interpreter();

    Object* ret;
    symtab symbolTable;
    parser codeParser;
  
    std::vector<std::string> code;
    Object* createObject(size_t&  index);
    Object* createObject(std::string value);
    void execute(std::vector<std::string>& tokens);
    void executeAssignment(const std::vector<std::string>& tokens, size_t& index);
    void executeIf(const std::vector<std::string>& tokens, size_t& index);
    void executeOtherwiseIf(const std::vector<std::string>& tokens, size_t& index);
    void executeOtherwise(const std::vector<std::string>& tokens, size_t& index);
    void executeDuring(const std::vector<std::string>& tokens, size_t& index);
    void executeLoop(const std::vector<std::string>& tokens, size_t& index);
    void executeBlock(const std::vector<std::string>& tokens, size_t& index);
    void runLine(const std::vector<std::string>& tokens);
    void defineFunction(const std::vector<std::string>& tokens, size_t& index);
    void callFunction(const std::string& functionName);
    Object* evaluateExpression(const std::vector<std::string>& tokens, size_t& index);
};



#endif // INTERPRETER_H
