#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "object.h"
#include "parser.h"
#include "symtab.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <memory>

class Interpreter {
public:
    Interpreter();
    ~Interpreter();

    void interpret(const std::string& code);

private:
    symtab symbolTable;
    parser codeParser;
    //std::vector<std::vector<std::string>> parsed_code;
    std::vector<std::string> parsed_code;
    //std::unordered_set<Object*> objs;//?????
    //std::unordered_map<std::string, Object*> vars;
    //exsit in symtab
    std::chrono::steady_clock::time_point last;
  
    void execute(const std::vector<std::string>& tokens);
    void executeAssignment(const std::vector<std::string>& tokens, size_t& index);
    void executeIf(const std::vector<std::string>& tokens, size_t& index);
    void executeOtherwiseIf(const std::vector<std::string>& tokens, size_t& index);
    void executeDuring(const std::vector<std::string>& tokens, size_t& index);
    void executeLoop(const std::vector<std::string>& tokens, size_t& index);
    void executeBlock(const std::vector<std::string>& tokens, size_t& index);
    void runLine(const std::vector<std::string>& tokens);
    Object* evaluateExpression(const std::vector<std::string>& tokens, size_t& index);
    //Object* getObject(const std::string& token);
    //Object* parseValue(const std::string& token);
    //bool validVariableName(const std::string& var);

};

#endif // INTERPRETER_H