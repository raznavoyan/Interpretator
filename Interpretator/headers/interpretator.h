#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "object.h"
#include "parser.h"
#include "symtab.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

class Interpreter {
public:
    Interpreter(parser::toks code);
    ~Interpreter();

    //void interpret(const std::string& code);

private:
    symtab symbolTable;
    std::vector<std::string> parsed_code;
  
    void execute(const std::vector<std::string>& tokens);
    void executeAssignment(const std::vector<std::string>& tokens, size_t& index);
    void executeIf(const std::vector<std::string>& tokens, size_t& index);
    void executeOtherwiseIf(const std::vector<std::string>& tokens, size_t& index);
    void executeDuring(const std::vector<std::string>& tokens, size_t& index);
    void executeLoop(const std::vector<std::string>& tokens, size_t& index);
    void executeBlock(const std::vector<std::string>& tokens, size_t& index);
    void runLine(const std::vector<std::string>& tokens);
    Object* evaluateExpression(const std::vector<std::string>& tokens, size_t& index);
};

#endif // INTERPRETER_H
