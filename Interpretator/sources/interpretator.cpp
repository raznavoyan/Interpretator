#include "../headers/interpreter.h"
#include <iostream>
#include <sstream>

Interpreter::Interpreter() : 
    last(std::chrono::steady_clock::now()) 
{
    symbolTable = symtab();
    codeParser = parser();
    std::cout << "Interpreter initialized." << std::endl;
}

Interpreter::~Interpreter() 
{
    for (auto& pair : symbolTable.getAll()) {
        delete pair.second;
    }
    std::cout << "Interpreter destroyed." << std::endl;
}

void Interpreter::interpret(const std::string& code) {
    parsed_code = codeParser.tokenize(code);
    if (codeParser.isValid(parsed_code)) {
        for (const auto& line : parsed_code) {
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::string token;
            while (iss >> token) {
                tokens.push_back(token);
            }
            execute(tokens);
        }
    } else {
        std::cerr << "Error: Invalid code." << std::endl;
    }
}

void Interpreter::execute(const std::vector<std::string>& tokens) {
    size_t index = 0;
    while (index < tokens.size()) {
        const std::string& token = tokens[index];
        if (token == "if") {
            executeIf(tokens, index);
        } else if (token == "otherwise") {
            executeOtherwiseIf(tokens, index);
        } else if (token == "during") {
            executeDuring(tokens, index);
        } else if (token == "loop") {
            executeLoop(tokens, index);
        } else if (token == "=") {
            executeAssignment(tokens, index);
        } else {
            ++index;
        }
    }
}

void Interpreter::executeAssignment(const std::vector<std::string>& tokens, size_t& index) {
    std::string variableName = tokens[index - 1];
    index += 1; 
    Object* value = evaluateExpression(tokens, index);
    symbolTable.setNewVal(variableName, value);
}

void Interpreter::executeIf(const std::vector<std::string>& tokens, size_t& index) {
    ++index; 
    Object* condition = evaluateExpression(tokens, index);
    if (condition->__equal__(new Bool(true))) {
        executeBlock(tokens, index);
    } else {
        while (tokens[index] != "}") {
            ++index;
        }
        ++index; 
    }
}

void Interpreter::executeOtherwiseIf(const std::vector<std::string>& tokens, size_t& index) {
    ++index; 
    if (tokens[index] == "if") {
        ++index;
        Object* condition = evaluateExpression(tokens, index);
        if (condition->__equal__(new Bool(true))) {
            executeBlock(tokens, index);
        } else {
            while (tokens[index] != "}") {
                ++index;
            }
            ++index; 
        }
    }
}

void Interpreter::executeDuring(const std::vector<std::string>& tokens, size_t& index) {
    ++index; 
    Object* condition = evaluateExpression(tokens, index);
    if (tokens[index] == "{") {
        ++index;
        while (condition->__equal__(new Bool(true))) {
            size_t blockStart = index;
            executeBlock(tokens, index);
            index = blockStart; 
        }
        while (tokens[index] != "}") {
            ++index;
        }
        ++index; 
    }
}

void Interpreter::executeLoop(const std::vector<std::string>& tokens, size_t& index) {
    ++index; 
    std::string variableName = tokens[index];
    index += 2; 
    Object* start = evaluateExpression(tokens, index);
    Object* end = evaluateExpression(tokens, index);
    Object* step = new Int(1);

    if (tokens[index] == ":") {
        index += 1;
        step = evaluateExpression(tokens, index);
    }

    if (tokens[index] == "{") {
        ++index; 
        if (step->__equal__(new Int(1))) {
            for (Object* i = start; i->__less_equal__(end); i = i->__add__(new Int(1))) {
                symbolTable.setNewVal(variableName, i);
                size_t blockStart = index;
                executeBlock(tokens, index);
                index = blockStart; 
            }
        } else {
            for (Object* i = start; i->__more_equal__(end); i = i->__sub__(new Int(1))) {
                symbolTable.setNewVal(variableName, i);
                size_t blockStart = index;
                executeBlock(tokens, index);
                index = blockStart; 
            }
        }
        while (tokens[index] != "}") {
            ++index;
        }
        ++index; 
    }
}

void Interpreter::executeBlock(const std::vector<std::string>& tokens, size_t& index) {
    if (tokens[index] == "{") {
        ++index;
        while (tokens[index] != "}") {
            std::vector<std::string> subTokens;
            while (tokens[index] != "|" && tokens[index] != "}") {
                subTokens.push_back(tokens[index++]);
            }
            if (tokens[index] == "|") ++index;
            runLine(subTokens);
        }
        ++index; 
    }
}

void Interpreter::runLine(const std::vector<std::string>& tokens) {
    if (tokens.size() >= 3 && tokens[1] == "=") {
        if (validVariableName(tokens[0])) {
            symbolTable.setNewVal(tokens[0], evaluateExpression(tokens, 2));
        }
    } else {
        execute(tokens);
    }
}

Object* Interpreter::evaluateExpression(const std::vector<std::string>& tokens, size_t& index) {
    if (codeParser.isNumber(tokens[index])) {
        return new Int(std::stoi(tokens[index++]));
    } else if (tokens[index] == "true") {
        ++index;
        return new Bool(true);
    } else if (tokens[index] == "false") {
        ++index;
        return new Bool(false);
    } else {
        return symbolTable.getVal(tokens[index++]);
    }
}

bool Interpreter::validVariableName(const std::string& var) {
    return !var.empty() && isalpha(var[0]);
}

