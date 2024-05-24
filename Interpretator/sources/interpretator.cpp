
#include "../headers/interpretator.h"
#include "../headers/parser.h"
#include "../headers/symtab.h"

#include <iostream>
#include <sstream>

Interpreter::Interpreter(parser::toks& code)
{
    std::cout << "Interpreter initialized." << std::endl;
    execute(code);
}

Interpreter::~Interpreter() 
{
    std::cout << "Interpreter destroyed." << std::endl;
}

// void Interpreter::interpret(const std::string& code) {
//     parsed_code = codeParser.tokenize(code);
//     if (codeParser.isValid(parsed_code)) {
//         for (const auto& line : parsed_code) {
//             std::istringstream iss(line);
//             std::vector<std::string> tokens;
//             std::string token;
//             while (iss >> token) {
//                 tokens.push_back(token);
//             }
//             execute(tokens);
//         }
//     } else {
//         std::cerr << "Error: Invalid code." << std::endl;
//     }

void Interpreter::execute(const std::vector<std::string>& tokens) {
    size_t index = 0;
    while (index < tokens.size()) {
        const std::string& token = tokens[index];
        if(token == "|")
        {
            ++index;
        }
        if(parser::isAKeyword(token))
        {
            if (token == "if") {
                index++;
                executeIf(tokens, index);
            } else if (token == "otherwise") {
                executeOtherwiseIf(tokens, index);
            } else if (token == "during") {
                ++index;
                executeDuring(tokens, index);
            } else if (token == "loop") {
                ++index;
                executeLoop(tokens, index);
            }else 
            {
                throw std::runtime_error("wrong operation");
            }
        }else // if it is not a keyword it should be a variable's name
        {
            if(parser::isVariableName(token))
            {
                if(tokens[index + 1] != "=")
                {
                    throw std::runtime_error("wrong operation");
                }
                executeAssignment(tokens, index);
            } else 
            {
                throw std::runtime_error("wrong operation");
            }
        }
        ++index;

    }

    // while (index < tokens.size()) {
    //     const std::string& token = tokens[index];
    //     if (token == "if") {
    //         executeIf(tokens, index);
    //     } else if (token == "otherwise") {
    //         executeOtherwiseIf(tokens, index);
    //     } else if (token == "during") {
    //         executeDuring(tokens, index);
    //     } else if (token == "loop") {
    //         executeLoop(tokens, index);
    //     } else if (token == "=") {
    //         executeAssignment(tokens, index);
    //     } else {
    //         ++index;
    //     }
    // }
}

void Interpreter::executeAssignment(const std::vector<std::string>& tokens, size_t& index) 
{
    std::string variableName = tokens[index - 2];
    index += 1; 
    Object* value = evaluateExpression(tokens, index);
    symbolTable.setVal(variableName, value);
}

void Interpreter::executeIf(const std::vector<std::string>& tokens, size_t& index) 
{
    if(tokens[index] != ":")
    {
        throw std::runtime_error("expected : after if");
    }
    ++index; 

    Object* condition = evaluateExpression(tokens, index);
    if (*static_cast<bool*>(condition->value)) {
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
        if(tokens[++index] != ":")
        {
            throw std::runtime_error("expected : after if");
        }
        ++index;
        Object* condition = evaluateExpression(tokens, index);
        if (*static_cast<bool*>(condition->value)) {
            executeBlock(tokens, index);
        } else {
            while (tokens[index] != "}") {
                ++index;
            }
            ++index; 
        }
    } else 
    {
        executeOtherwise(tokens, index);
    }
}

void Interpreter::executeOtherwise(const std::vector<std::string>& tokens, size_t& index) 
{ 
    if(tokens[index] != "{")
    {
        throw std::runtime_error("expected { after otherwise");
    }
    ++index;
    executeBlock(tokens, index);
}

void Interpreter::executeDuring(const std::vector<std::string>& tokens, size_t& index) 
{
    if(tokens[index] != ":")
    {
        throw std::runtime_error("expected ':' after during");
    }
    ++index;
    size_t conditionStart = index; 
    Object* condition = evaluateExpression(tokens, index);
    if (tokens[index] == "{") {
        ++index;
        while (*static_cast<bool*>(condition->value)) {
            Object* condition = evaluateExpression(tokens, conditionStart);
            size_t blockStart = index;
            executeBlock(tokens, index);
            index = blockStart; 
        }
        while (tokens[index] != "}") {
            ++index;
        }
        ++index; 
    } else
    {
        throw std::runtime_error("expected '{' after during");
    }
}

void Interpreter::executeLoop(const std::vector<std::string>& tokens, size_t& index) 
{
    if(tokens[index] != ":")
    {
        std::runtime_error("expected : after loop");
    }
    ++index;
    if(!parser::isVariableName(tokens[index]))
    {
        std::runtime_error("not a variable name");
    }
    std::string variableName = tokens[index];
    ++index;
    if(tokens[index] != "=")
    {
        std::runtime_error("expected =");
    }
    ++index; 
    executeAssignment(tokens, index);

    if(tokens[index] != "->")
    {
        throw std::runtime_error("expected ->");
    }    
    ++index;
    Object* end = evaluateExpression(tokens, index);
    Object* step = (tokens[index] == ",") ? evaluateExpression(tokens, index) : new Int(1);
    while (tokens[index++] != "{");

    if (tokens[index] == "{") {
        ++index; 
        if (*static_cast<bool*>((step->__equal__(new Int(1))).value)) {
            for (Object* i = start; i->__less_equal__(end); i = i->__add__(new Int(1))) {
                symbolTable.setVal(variableName, i);
                size_t blockStart = index;
                executeBlock(tokens, index);
                index = blockStart; 
            }
        } else {
            for (Object* i = start; i->__more_equal__(end); i = i->__sub__(new Int(1))) {
                symbolTable.setVal(variableName, i);
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

void Interpreter::executeBlock(const std::vector<std::string>& tokens, size_t& index) 
{
    if (tokens[index] == "{") {
        ++index;
        while (tokens[index] != "}") {
            std::vector<std::string> subTokens;
            while (tokens[index] != "|" && tokens[index] != "}") {
                subTokens.push_back(tokens[index++]);
            }
            if (tokens[index] == "|") ++index;
            execute(subTokens);
        }
        ++index; 
    }
}
/*
void Interpreter::runLine(const std::vector<std::string>& tokens) 
{
    execute(tokens);
}
*/

Object* Interpreter::evaluateExpression(const std::vector<std::string>& tokens, size_t& index)
{

    Object* tmp = new Object;










    symbolTable.setVal("__tmp__", tmp);
    return tmp;
}
/*
Object* Interpreter::getObject(const std::string& token) {
    Object* obj = symbolTable.getVal(token);
    if (!obj) {
        std::cerr << "Error: Undefined variable " << token << std::endl;
        exit(EXIT_FAILURE);
    }
    return obj;
}

Object* Interpreter::parseValue(const std::string& token) {
    if (codeParser.isNumber(token)) {
        return new Int(std::stoi(token));
    } else if (token == "true") {
        return new Bool(true);
    } else if (token == "false") {
        return new Bool(false);
    } else {
        return symbolTable.getVal(tokens[index++]);
    }
}

bool Interpreter::isVariableName(const std::string& var) {
    return !var.empty() && isalpha(var[0]);
}
*/