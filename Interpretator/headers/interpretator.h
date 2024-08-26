#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include<fstream>
#include "headers.h"

    class Interpreter {
public:
    Interpreter(std::vector<std::string> code, std::vector<std::string>* parametrs = nullptr, Object* arguments = nullptr);
    ~Interpreter();

    std::ofstream iout;

    // functionTab fTab;
    Object* ret;
    symtab symbolTable;
    parser codeParser;

    std::vector<std::string> code;

    void execute(std::vector<std::string>& tokens, size_t index);
    void executeAssignment(const std::vector<std::string>& tokens, size_t& index);
    void executeIf(const std::vector<std::string>& tokens, size_t& index);
    void executeDuring(const std::vector<std::string>& tokens, size_t& index);
    void executeLoop(const std::vector<std::string>& tokens, size_t& index);
    void executeBlock(const std::vector<std::string>& tokens, size_t& index);
    void defineFunction(const std::vector<std::string>& tokens, size_t& index);
    void callFunction(const std::vector<std::string>& tokens, size_t& index);

    Object* evaluateExpression(const std::vector<std::string>& tokens,
                               size_t& index);
    Object* evaluateSubExpression(Object* r, Object* l, std::string d);

    void print(Object* arg); // disp
    void addBrecets(std::vector<std::string>& expression);

    Array* newArray(const std::vector<std::string>& tokens, size_t& index);

private:
    int precedence(std::string op);
};

#endif // INTERPRETER_H
