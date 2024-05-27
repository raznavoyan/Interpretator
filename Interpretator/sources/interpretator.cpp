#include "interpretator.h"
#include "parser.h"
#include "symtab.h"
#include <unistd.h>
#include <iostream>
#include <sstream>

Interpreter::Interpreter(std::vector<std::string> code)
{
    std::cout << "Interpreter initialized." << std::endl;
    execute(code);
}

Interpreter::~Interpreter() 
{
    std::cout << "Interpreter destroyed." << std::endl;
}

void Interpreter::execute(const std::vector<std::string>& tokens) {
    while (this->index < tokens.size()) {
        const std::string& token = tokens[this->index];
        if(token == "|")
        {
            ++this->index;
        }
        if(parser::isAKeyword(token))
        {
            std::cout << token;
            if (token == "if") {
                ++this->index;
                executeIf(tokens, this->index);
                if (this->index < tokens.size() && tokens[this->index] == "otherwise if") {
                    ++this->index;
                    executeOtherwiseIf(tokens, this->index);
                }
                if (this->index < tokens.size() && tokens[this->index] == "otherwise") {
                    ++this->index;
                    executeOtherwise(tokens, this->index);
                }
            } else if (token == "during") {
                ++this->index;
                executeDuring(tokens, this->index);
            } else if (token == "loop") {
                ++this->index;
                std::cout << "\t called" << std::endl;
                executeLoop(tokens, this->index);
            }else 
            {
                throw std::runtime_error("wrong operation");
            }
        }else // if it is not a keyword it should be a variable's name
        {
            if(parser::isVariableName(token))
            {
                if(tokens[this->index + 1] != "=")
                {
                    throw std::runtime_error("wrong operation");
                }
                executeAssignment(tokens, this->index);
            } else 
            {
                throw std::runtime_error("wrong operation");
            }
        }
        ++this->index;

    }
}

void Interpreter::executeAssignment(const std::vector<std::string>& tokens, size_t& index) 
{
    std::string variableName = tokens[index - 2];
    index += 1; 
    Object* value = createObject(index);
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
    //Ex: during : c == true {
    
    if(tokens[index] != ":")
    {
        throw std::runtime_error("expected ':' after during");
    }
    ++index;
    size_t conditionStart = index; 
    Object* condition = evaluateExpression(tokens, index);
    while(tokens[index++] != ":");
    // ++index;
    std::cout << tokens[index] << std::endl;

    if (tokens[index] == "{") {
        while (*static_cast<bool*>(condition->value)) {
            size_t blockStart = index;
            executeBlock(tokens, index);
            index = blockStart; 
            Object* condition = evaluateExpression(tokens, conditionStart);
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
    std::cout << "loop started" << std::endl;
   

    //Ex: loop : a = 5 -> 10{}
    symbolTable.pushSpace();
    std::cout << "push space" << std::endl;
    if(tokens[index] != ":")
    {
        std::runtime_error("expected : after loop");
    }
    
    ++index;
    if(!parser::isVariableName(tokens[index]))
    {
        std::runtime_error("not a variable name");
    }

    std::string startName = tokens[index];
    ++index;
    if(tokens[index] != "=")
    {
        std::runtime_error("expected =");
    }
    ++index;
    Object* start = nullptr;
    if(parser::isNumber(tokens[index])){
        start = createObject(tokens[index]);
        symbolTable.setVal(startName, start);
    }else if (symbolTable.are(tokens[index])) {
        *start = *symbolTable.getVal(tokens[index]);
        symbolTable.setVal(startName, start);
    }else{
        std::runtime_error("expected: value");
    }
    std::cout << "start: " << start << std::endl;
    ++index;
    if(tokens[index] != "->")
    {
        throw std::runtime_error("expected ->");
    }
    ++index;

    Object* end = nullptr;
    if(parser::isNumber(tokens[index])){
        end = createObject(tokens[index]);
        symbolTable.setVal("end" + std::to_string(index), end);
    }else if (symbolTable.are(tokens[index])) {
        end = symbolTable.getVal(tokens[index]);
    }else {
        std::runtime_error("expected: value");
    }
    std::cout << "end: " << end << std::endl;

    ++index;
    Object* step = nullptr;
    if(tokens[index] == ","){
        if(parser::isNumber(tokens[index])){
            step = createObject(tokens[index]);
            symbolTable.setVal("step" + std::to_string(index), step);
        }else if (symbolTable.are(tokens[index])) {
            step = symbolTable.getVal(tokens[index]);
        }else{
            std::runtime_error("expected: value");
        }
    }else{
        step = createObject("1");
        symbolTable.setVal("step" + std::to_string(index), step);
    }
    std::cout << "start loop" << std::endl;
    if (tokens[index] == "{") {
        ++index;
        size_t blockStart = index;
        std::cout << start->__str__() << '\t' << end->__str__() << std::endl;
        std::cout << (start->__equal__(end)).__str__() << std::endl;
        while ((start->__equal__(end)).__str__() == "true") {
                ///////////////////////////////////////////
            std::cout << "for sycl" << std::endl;
                //////////////////////////////////////////
                executeBlock(tokens, index); 
            index = blockStart;
            *start = start->__add__(step);
        }

        while (tokens[index] != "}") {

                ///////////////////////////////////////////
                std::cout << "for" << std::endl;
                //////////////////////////////////////////
            ++index;
        }
        ++index; 
    }
    symbolTable.popSpace();
}

void Interpreter::executeBlock(const std::vector<std::string>& tokens, size_t& index) 
{   
    symbolTable.pushSpace();
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
    symbolTable.popSpace();
}
/*
void Interpreter::runLine(const std::vector<std::string>& tokens) 
{
    execute(tokens);
}
*/

Object* Interpreter::evaluateExpression(const std::vector<std::string>& tokens, size_t& index) {
    // Handle base cases:
    if (index >= tokens.size()) {
        // Error: Unexpected end of expression
        return nullptr;
    }
    Object* tmp = nullptr;
    if(parser::typeOf(tokens[index]) != "undefine"){
        tmp = createObject(index);
        symbolTable.setVal(("tmp" + std::to_string(index)), tmp);
    }else if (symbolTable.are(tokens[index])) {
        tmp = symbolTable.getVal(tokens[index]);
    }else if (parser::isBinaryOperator(tokens[index])) {
        // Validate operand availability and types
        if (index + 1 >= tokens.size()) {
            // Error: Missing or invalid operand
            return nullptr;
        }

        Object* left = evaluateExpression(tokens, ++index); // Recursively evaluate left operand
        if (!left) {
            return nullptr;
        }
        
        if (tokens[index - 1] == "+") {
            *tmp = tmp->__add__(left);
        } else if (tokens[index - 1] == "-") {
            *tmp = tmp->__sub__(left);
        } else if (tokens[index - 1] == "*") {
            *tmp = tmp->__mul__(left);
        } else if (tokens[index - 1] == "/") {
            *tmp = tmp->__div__(left);
        } else if (tokens[index - 1] == "%") {
            *tmp = tmp->__mod__(left);
        } else if (tokens[index - 1] == "|") {
            *tmp = tmp->__or__(left);
        } else if (tokens[index - 1] == "&") {
            *tmp = tmp->__and__(left);
        } else if (tokens[index - 1] == "~") {
            *tmp = tmp->__den__();
        } else if (tokens[index - 1] == "||") {
            *tmp = tmp->__or__(left);
        } else if (tokens[index - 1] == "&&") {
            *tmp = tmp->__and__(left);
        } else if (tokens[index - 1] == "^") {
            *tmp = tmp->__xor__(left);
        } else if (tokens[index - 1] == "<<") {
            *tmp = tmp->__left_shift__(left);
        } else if (tokens[index - 1] == ">>") {
            *tmp = tmp->__right_shift__(left);
        } else if (tokens[index - 1] == "+=") {
            tmp->__add_assign__(left);
        } else if (tokens[index - 1] == "-=") {
            tmp->__sub_assign__(left);
        } else if (tokens[index - 1] == "*=") {
            tmp->__mul_assign__(left);
        } else if (tokens[index - 1] == "/=") {
            tmp->__div_assign__(left);
        } else if (tokens[index - 1] == "%=") {
            tmp->__mod_assign__(left);
        } else if (tokens[index - 1] == "^=") {
            tmp->__xor_assign__(left);
        } else if (tokens[index - 1] == "<<=") {
            tmp->__lshift_assign__(left);
        } else if (tokens[index - 1] == ">>=") {
            tmp->__rshift_assign__(left);
        } else if (tokens[index - 1] == ">") {
            *tmp = tmp->__more__(left);
        } else if (tokens[index - 1] == "<") {
            *tmp = tmp->__less__(left);
        } else if (tokens[index - 1] == ">=") {
            *tmp = tmp->__more_equal__(left);
        } else if (tokens[index - 1] == "<=") {
            *tmp = tmp->__less_equal__(left);
        } else if (tokens[index - 1] == "==") {
            *tmp = tmp->__equal__(left);
        } else if (tokens[index - 1] == "!=") {
            *tmp = tmp->__not_equal__(left);
        } else {
            // Error: Unsupported binary operator
            return nullptr;
        }
    }

    if(tmp != nullptr)
    symbolTable.setVal(("tmp" + std::to_string(index)), tmp);
    return tmp;
}

// Helper functions for evaluating expressions (implement these as needed):
Object* Interpreter::createObject(size_t& index) {
    if (index >= code.size()) {
        return nullptr;
        //throw std::out_of_range("Index out of bounds in createObject");
    }

    std::string value = code[index];
    std::string valtype = parser::typeOf(value); // Assuming parser class provides type information

    Object* tmp = nullptr;
    char t = static_cast<char>(valtype[0]);
    
    switch (t) {
        case 'i': // INT
            try {
                tmp = new Int(std::stoi(value));
            } catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid integer value: " + value);
            }
            break;
        case 'd': // DOUBLE
            try {
                tmp = new Double(std::stod(value));
            } catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid double value: " + value);
            }
            break;
        case 'b': // BOOL
            {
                bool val = (value == "true" ? true : false);
                tmp = new Bool(val);
            }
            break;
        case 's': // STRING
            tmp = new String(value);
            break;
        case 'a':
            break;
        default:
            throw std::out_of_range("No such type of variable: " + value);
    }

    if (t == 'a') {
        // ARRAY
        // Handle array creation using tokens from the code vector:
        if (index + 1 >= code.size() || code[index + 1] != "(") {
            throw std::runtime_error("Invalid array syntax: missing opening parenthesis");
        }

        ++index;
        std::vector<Object*> elements;

        // Recursively parse array elements until closing parenthesis:
        while (index < code.size() && code[index] != "]") {
            elements.push_back(createObject(index));
            ++index;
        }

        if (index >= code.size() || code[index] != "]") {
            throw std::runtime_error("Invalid array syntax: missing closing parenthesis");
        }
        ++index;
        tmp = new Array(elements);
    }

    return tmp;
}

Object* Interpreter::createObject(std::string value) {
    std::string valtype = parser::typeOf(value); // Assuming parser class provides type information

    Object* tmp = nullptr;
    char t = static_cast<char>(valtype[0]);
    
    switch (t) {
        case 'i': // INT
            try {
                tmp = new Int(std::stoi(value));
                std::cout << "New Int: " << tmp << std::endl;
            } catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid integer value: " + value);
            }
            break;
        case 'd': // DOUBLE
            try {
                tmp = new Double(std::stod(value));
            } catch (const std::invalid_argument& e) {
                throw std::runtime_error("Invalid double value: " + value);
            }
            break;
        case 'b': // BOOL
            {
                bool val = (value == "true" ? true : false);
                tmp = new Bool(val);
            }
            break;
        case 's': // STRING
            tmp = new String(value);
            break;
        case 'a':
            break;
        default:
            throw std::out_of_range("No such type of variable: " + value);
    }

    return tmp; 
    //         throw std::runtime_error("Invalid array syntax: missing closing parenthesis");
    //     }
    //     ++this->index;
    //     tmp = new Array(elements);
    // }
       // if (t == 'a') {
    //     // ARRAY
    //     // Handle array creation using tokens from the code vector:
    //     if (this->index + 1 >= code.size() || code[this->index + 1] != "(") {
    //         throw std::runtime_error("Invalid array syntax: missing opening parenthesis");
    //     }

    //     ++this->index;
    //     std::vector<Object*> elements;

    //     // Recursively parse array elements until closing parenthesis:
    //     while (this->index < code.size() && code[this->index] != "]") {
    //         elements.push_back(createObject(this->index));
    //         ++this->index;
    //     }

    //     if (this->index >= code.size() || code[this->index] != "]") {
    //         throw std::runtime_error("Invalid array syntax: missing closing parenthesis");
    //     }
    //     ++this->index;
    //     tmp = new Array(elements);
    //     // Recursively parse array elements until closing parenthesis:
    //     while (this->index < code.size() && code[this->index] != "]") {
    //         elements.push_back(createObject(this->index));
    //         ++this->index;
    //     }

    //     if (this->index >= code.size() || code[this->index] != "]") {
    //         throw std::runtime_error("Invalid array syntax: missing closing parenthesis");
    //     }
    //     ++this->index;
    //     tmp = new Array(elements);
    // }// ARRAY
    //     // Handle array creation using tokens from the code vector:
    //     if (this->index + 1 >= code.size() || code[this->index + 1] != "(") {
    //         throw std::runtime_error("Invalid array syntax: missing opening parenthesis");
    //     }

    //     ++this->index;
    //     std::vector<Object*> elements;

    //     // Recursively parse array elements until closing parenthesis:
    //     while (this->index < code.size() && code[this->index] != "]") {
    //         elements.push_back(createObject(this->index));
    //         ++this->index;
    //     }

    //     if (this->index >= code.size() || code[this->index] != "]") {
    //         throw std::runtime_error("Invalid array syntax: missing closing parenthesis");
    //     }
    //     ++this->index;
    //     tmp = new Array(elements);
    //     // Recursively parse array elements until closing parenthesis:
    //     while (this->index < code.size() && code[this->index] != "]") {
    //         elements.push_back(createObject(this->index));
    //         ++this->index;
    //     }

    //     if (this->index >= code.size() || code[this->index] != "]") {
}

