#include "interpretator.h"

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "headers.h"
#include "parser.h"
#include "symtab.h"

Interpreter::Interpreter(std::vector<std::string> code,
                         std::vector<std::string>* parameters,
                         Object* arguments)
    : ret{new Object("Null_object", nullptr)}, iout("oute.r") {
    std::cout << "Interpreter initialized." << std::endl;
    if (parameters) {
        for (int i = 0; i < parameters->size(); ++i) {
            symbolTable.setVal(parameters->at(i), arguments->__at__(i));
        }
    }
    execute(code, 0);
}

Interpreter::~Interpreter() {
    if (iout.is_open()) {
        iout.close();
    }
    std::cout << "Interpreter destroyed." << std::endl;
}

void Interpreter::execute(std::vector<std::string>& tokens, size_t index) {
    while (index < tokens.size()) {
        const std::string& token = tokens[index];
        std::cout << "\"" << token << "\" in execute function" << std::endl;
        // if (token == "|" || token == "}") {
        //     ++this->index;
        //     continue;
        // }
        if (parser::isAKeyword(token)) {
            if (token == "def") {
                defineFunction(tokens, index);
            } else if (token == "if") {
                ++index;
                executeIf(tokens, index);
            } else if (token == "during") {
                ++index;
                executeDuring(tokens, index);
            } else if (token == "loop") {
                ++index;
                std::cout << "\t called" << std::endl;
                executeLoop(tokens, index);
            } else if (token == "disp") {
                index += 2;
                print(evaluateExpression(tokens, index));
                ++index;
            } else {
                throw std::runtime_error("Unknown operation");
            }
            // } else if(functionTable.count(token)){
            //     callFunction(tokens, index);
        } else if (parser::isVariableName(token)) {
            // if(parser::isAMath(tokens[this->index + 1])){
            //     return evaluateExpression(tokens, index);
            // }else
            if (parser::isAssignment(tokens[index + 1])) {
                executeAssignment(tokens, index);
                // std::cout << tokens[index] << tokens[index + 1] << std::endl;
            } else {
                throw std::runtime_error("wrong operation expected assignment");
            }
        } else if (token == "{") {
            executeBlock(tokens, index);
        } else {
            // std::cout << tokens[index] << tokens[index + 1] << std::endl;
            throw std::runtime_error("wrong operation no such function");
        }
        std::cout << "execute ended" << std::endl;
    }
}

void Interpreter::executeAssignment(const std::vector<std::string>& tokens,
                                    size_t& index) {
    std::cout << "Assignment called" << std::endl;
    std::string variableName = tokens[index];
    std::cout << variableName << std::endl;
    ++index;
    if (tokens[index] == "=") {
        ++index;
        Object* value = evaluateExpression(tokens, index);
        symbolTable.setVal(variableName, value);
    } else {
        if (!symbolTable.are(variableName)) {
            throw std::runtime_error("INTERPRETER:: no such variable " +
                                     variableName);
        }
        ++index;
        Object* value = evaluateExpression(tokens, index);

        if (tokens[index - 1] == "+=") {
            symbolTable.getVal(variableName)->__add_assign__(value);
        } else if (tokens[index - 1] == "-=") {
            symbolTable.getVal(variableName)->__sub_assign__(value);
        } else if (tokens[index - 1] == "*=") {
            symbolTable.getVal(variableName)->__mul_assign__(value);
        } else if (tokens[index - 1] == "/=") {
            symbolTable.getVal(variableName)->__div_assign__(value);
        } else if (tokens[index - 1] == ">>=") {
            symbolTable.getVal(variableName)->__rshift_assign__(value);
        } else if (tokens[index - 1] == "<<=") {
            symbolTable.getVal(variableName)->__lshift_assign__(value);
        } else if (tokens[index - 1] == "%=") {
            symbolTable.getVal(variableName)->__mod_assign__(value);
        } else if (tokens[index - 1] == "^=") {
            symbolTable.getVal(variableName)->__xor_assign__(value);
        }
    }

    // std::cout << tokens[index] << std::endl;
    std::cout << "executeAssignment ended" << std::endl;
}

void Interpreter::executeIf(const std::vector<std::string>& tokens,
                            size_t& index) {
    std::cout << "execute if start" << std::endl;
    if (tokens[index] != ":") {
        throw std::runtime_error("expected : after if");
    }
    ++index;
    symbolTable.pushSpace();
    parser::toks expr;
    while (tokens[index] != ":") {
        expr.push_back(tokens[index]);
        ++index;
        if (tokens[index] == "{") {
            throw std::runtime_error("forget :");
        }
    }
    ++index;
    size_t condIndex = 0;

    Object* condition = evaluateExpression(expr, condIndex);
    if (condition->__str__() == "true") {
        std::cout << "condition is true" << std::endl;
        // for (int ind = 0; ind < 5; ++ind) {
        //     iout << tokens[index + ind] << ' ';
        // }
        // iout << std ::endl;
        //iout << index << std::endl;
        executeBlock(tokens, index);
        //iout << index << std::endl;

        while (tokens[index] == "otherwise" && index < tokens.size()) {
            while (tokens[index] != "}" && index < tokens.size()) {
                ++index;
            }
            ++index;
        }
    } else {
        std::cout << "condition is false" << std::endl;
        while (tokens[index] != "}" && index < tokens.size()) {
            ++index;
        }
        ++index;
        
        if (tokens[index] == "otherwise") {
            ++index;
            if (tokens[index] == "if") {
                ++index;
                executeIf(tokens, index);
            } else {
                executeBlock(tokens, index);
            }
        }
    }
    std::cout << "end if" << std::endl;
}

void Interpreter::executeDuring(const std::vector<std::string>& tokens,
                                size_t& index) {
    // Ex: during : c == true {
    symbolTable.pushSpace();
    std::cout << "during stsrted" << std::endl;
    if (tokens[index] != ":") {
        throw std::runtime_error("expected ':' after during");
    }
    ++index;
    const size_t conditionStart = index;
    Object* condition = evaluateExpression(tokens, index);

    // std::cout << tokens[index] << std::endl;
    if (tokens[index] == "{") {
        while (*static_cast<bool*>(condition->value)) {
            executeBlock(tokens, index);
            index = conditionStart;
            condition = evaluateExpression(tokens, index);
            ++index;
        }

    } else {
        throw std::runtime_error("expected '{' after during");
    }
    index = conditionStart;
    while (tokens[index] != "}") {
        ++index;
    }
    ++index;
    std::cout << "during ended" << std::endl;
    symbolTable.popSpace();
}

void Interpreter::executeLoop(const std::vector<std::string>& tokens,
                              size_t& index) {
    std::cout << "loop started" << std::endl;
    // Ex: loop : a = 5 -> 10{}
    symbolTable.pushSpace();
    std::cout << "push space" << std::endl;
    if (tokens[index] != ":") {
        std::runtime_error("expected : after loop");
    }

    ++index;
    if (!parser::isVariableName(tokens[index])) {
        std::runtime_error("not a variable name");
    }

    std::string startName = tokens[index];
    ++index;
    if (tokens[index] != "=") {
        std::runtime_error("expected =");
    }
    ++index;
    Object* start = nullptr;
    if (parser::isNumber(tokens[index])) {
        start = symbolTable.createObject(tokens[index]);
        symbolTable.setVal(startName, start);
    } else if (symbolTable.are(tokens[index])) {
        *start = *symbolTable.getVal(tokens[index]);
        symbolTable.setVal(startName, start);
    } else {
        std::runtime_error("expected: value");
    }
    ++index;
    if (tokens[index] != "->") {
        throw std::runtime_error("expected ->");
    }
    ++index;

    Object* end = nullptr;
    if (parser::isNumber(tokens[index])) {
        end = symbolTable.createObject(tokens[index]);
        symbolTable.setVal("end" + std::to_string(index), end);
    } else if (symbolTable.are(tokens[index])) {
        end = symbolTable.getVal(tokens[index]);
    } else {
        std::runtime_error("expected: value");
    }

    ++index;
    Object* step = nullptr;
    if (tokens[index] == ",") {
        if (parser::isNumber(tokens[index])) {
            step = symbolTable.createObject(tokens[index]);
            symbolTable.setVal("step" + std::to_string(index), step);
        } else if (symbolTable.are(tokens[index])) {
            step = symbolTable.getVal(tokens[index]);
        } else {
            std::runtime_error("expected: value");
        }
    } else {
        step = symbolTable.createObject("1");
        symbolTable.setVal("step" + std::to_string(index), step);
    }

    std::cout << "start: " << start << " " << start->__str__() << std::endl;
    std::cout << "end: " << end << " " << end->__str__() << std::endl;
    std::cout << "step: " << step << " " << step->__str__() << std::endl;

    std::cout << "start loop " << index << " " << tokens.size() << std::endl;
    std::cout << "with token " << tokens[index] << std::endl;
    if (tokens.size() < index && tokens[index] == "{") {
        std::cout << "checkpoint1" << std::endl;
        size_t blockStart = index;
        std::cout << "checkpoint1" << std::endl;
        std::cout << start->__str__() << '\t' << end->__str__() << std::endl;
        std::cout << (start->__equal__(end))->__str__() << std::endl;
        std::cout << "checkpoint2" << std::endl;

        while ((start->__equal__(end))->__str__() == "false") {
            ///////////////////////////////////////////
            std::cout << "for sycl:\t" << startName << '\t' << start->__str__()
                      << std::endl;
            //////////////////////////////////////////
            executeBlock(tokens, index);
            index = blockStart;
            start = start->__add__(step);
        }

        while (tokens[index] != "}") {
            ++index;
        }
        // ++index;
    }
    symbolTable.popSpace();
    std::cout << "loop ended successfully" << std::endl;
}

void Interpreter::executeBlock(const std::vector<std::string>& tokens,
                               size_t& index) {
    std::cout << "execute block sarted" << std::endl;

    if (tokens[index] != "{") {
        throw std::runtime_error("sintax error\n");
    }
    ++index;
    // New scope
    symbolTable.pushSpace();

    std::vector<std::string> subTokens;
    while (tokens[index] != "}") {
        subTokens.push_back(tokens[index]);
        ++index;
    }
    execute(subTokens, 0);

    symbolTable.popSpace();
    if (tokens[index] != "}") {
        throw std::runtime_error("expected '}' in the end of block");
    }
    ++index;
    std::cout << "execute block ended" << std::endl;
}

int Interpreter::precedence(std::string op) {
    if (op == "+" || op == "-") return 2;
    if (op == "*" || op == "/" || op == "%") return 3;
    if (op == "==" || op == "!=" || op == "<=" || op == ">=") return 4;
    if (op == "<" || op == ">") return 5;
    if (op == "&&") return 6;
    if (op == "||") return 7;
    if (op == "&") return 8;
    if (op == "|") return 9;
    if (op == "^") return 10;
    if (op == "~") return 11;
    return 1;
    return 0;
}
void Interpreter::addBrecets(std::vector<std::string>& expression) {
    std::vector<std::string> result;
    std::stack<std::string> operators;

    for (const std::string& token : expression) {
        if (parser::isVariableName(token) || parser::isNumber(token)) {
            result.push_back(token);
        } else if (parser::isAKeyword(token)) {
            // Handle keywords if needed (this implementation assumes keywords
            // are handled separately)
            result.push_back(token);
        } else if (parser::isOperator(token[0])) {
            while (!operators.empty() &&
                   precedence(operators.top()) >= precedence(token) &&
                   operators.top() != "(") {
                result.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        } else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                result.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty() && operators.top() == "(") {
                operators.pop();  // Remove the '('
            } else {
                throw std::runtime_error("Unmatched parentheses");
            }
        } else if (parser::isCustomDelimiter(token)) {
            // Handle custom delimiters if necessary
            result.push_back(token);
        } else {
            throw std::runtime_error("Unknown token: " + token);
        }
    }

    // Pop remaining operators
    while (!operators.empty()) {
        std::string top = operators.top();
        if (top != "(") {  // Ignore remaining '('
            result.push_back(top);
        }
        operators.pop();
    }

    // Update expression
    expression = result;
}

Object* Interpreter::evaluateSubExpression(Object* left, Object* right,
                                           std::string op) {
    std::cout << "evaluateSubExpression called" << std::endl;
    // std::cout << left->__str__() << " " << right << std::endl;
    // std::cout << left->__str__() << "  " << op << "  " << right->__str__() <<
    // std::endl;
    Object* tmp = nullptr;
    if (right == nullptr) {
        std::cout << left->__str__() << "  " << op << std::endl;
        if (op == "-") {
            // Negation
            if (left) {
                tmp = left->__neg__();
            }
        } else if (op == "~") {
            // Bitwise complement
            if (left) {
                tmp = left->__complement__();
            }
        } else if (op == "!") {
            if (left) {
                tmp = left->__logical_not__();
            }
        } else {
            throw std::runtime_error("no such a op");
        }
    } else {
        std::cout << left->__str__() << "  " << op << "  " << right->__str__()
                  << std::endl;
        if (op == "+") {
            tmp = left->__add__(right);
        } else if (op == "-") {
            tmp = left->__sub__(right);
        } else if (op == "*") {
            tmp = left->__mul__(right);
        } else if (op == "/") {
            tmp = left->__div__(right);
        } else if (op == "%") {
            tmp = left->__mod__(right);
        } else if (op == "|") {
            tmp = left->__or__(right);
        } else if (op == "&") {
            tmp = left->__and__(right);
        } else if (op == "~") {
            tmp = left->__complement__();
        } else if (op == "||") {
            tmp = left->__or__(right);
        } else if (op == "&&") {
            tmp = left->__and__(right);
        } else if (op == "^") {
            tmp = left->__xor__(right);
        } else if (op == "<<") {
            tmp = left->__left_shift__(right);
        } else if (op == ">>") {
            tmp = left->__right_shift__(right);
        } else if (op == ">") {
            tmp = left->__more__(right);
        } else if (op == "<") {
            tmp = left->__less__(right);
        } else if (op == ">=") {
            tmp = left->__more_equal__(right);
        } else if (op == "<=") {
            tmp = left->__less_equal__(right);
        } else if (op == "==") {
            tmp = left->__equal__(right);
        } else if (op == "!=") {
            tmp = left->__not_equal__(right);
        } else {
            // Error: Unsupported binary operator
            throw std::runtime_error("Error: Unsupported operator");
            return nullptr;
        }
    }
    std::cout << "evaluateSubExpression ended" << std::endl;
    return tmp;
}

// newArray funcion-i hamar petqa vor evaluateExpression-y minchev storaket
// exaci value-n veradarcni

Object* Interpreter::evaluateExpression(const std::vector<std::string>& tokens,
                                        size_t& index) {
    std::cout << "evaluateExpression started" << std::endl;
    // Handle base cases:
    if (index >= tokens.size()) {
        // Error: Unexpected end of expression
        return nullptr;
    }
    parser::toks expression;
    while (index < tokens.size() && tokens[index] != "|" &&
           tokens[index] != ")" && tokens[index] != ":") {
        if (tokens[index] != "{") {
            expression.push_back(tokens[index]);
        } else {
            throw std::runtime_error("sintax error");
        }
        ++index;
    }
    ++index;
    addBrecets(expression);

    /*
    std::cout << "mod ";
    std::cout << "expretion: ";
    for (auto it : expression) {
        std::cout << it << ' ';
    }
    std::cout << std::endl;
    */

    std::vector<Object*> vals;
    size_t i = 0;
    // if(expression[0] == "-" || expression[0] == "+"){
    //     ++i;
    //     vals.push_back(evaluateSubExpression(createObject(expression[i]),
    //     nullptr, expression[0]));
    //     ++i;
    //     // std::cout << "i = " << i << std::endl;
    // }
    for (auto& expr : expression) {
        std::cout << expr << std::endl;
        if (parser::isAMath(expr)) {
            if (vals.size() < 1) {
                throw std::runtime_error("invalit expretion(size <  2)");
            }

            Object* arg2 = nullptr;
            if (!parser::isAUnaryOp(expr)) {
                arg2 = vals.back();
                vals.pop_back();
            }
            Object* arg1 = vals.back();
            vals.pop_back();

            vals.push_back(evaluateSubExpression(arg1, arg2, expr));

        } else if (symbolTable.are(expr)) {
            vals.push_back(symbolTable.getVal(expr));
            // std::cout << vals.back()->__str__() << std::endl;

        } else if (parser::isNumber(expr) || expr == "true" ||
                   expr == "false") {
            std::cout << "adding new object by value:  " << expr << std::endl;
            vals.push_back(symbolTable.createObject(expr));

            for (auto it : vals) {
                std::cout << it->__str__() << ' ';
            }
            std::cout << std::endl;
        } else {
            std::cout << expression[i] << std::endl;
            throw std::runtime_error("unnown token");
        }
    }

    if (vals.size() != 1) {
        throw std::runtime_error("wrong expretion1");
    }

    return vals.back();
}

void Interpreter::defineFunction(const std::vector<std::string>& tokens,
                                 size_t& index) {
    std::cout << "functoin def" << std::endl;
    if (tokens[index] != "def") {
        throw std::runtime_error("Expected 'def' for function definition");
    }
    ++index;

    if (index >= tokens.size() || !parser::isVariableName(tokens[index])) {
        throw std::runtime_error("Expected function name");
    }

    std::string functionName = tokens[index];
    ++index;

    if (index >= tokens.size() && tokens[index] != "(") {
        throw std::runtime_error("Expected '()' after function name");
    }
    ++index;
    std::vector<std::string> arg_names;
    while (tokens[index] != ")") {
        if (!parser::isVariableName(tokens[index])) {
            throw std::runtime_error("not valid name for a parameter " +
                                     __LINE__);
        }
        arg_names.push_back(tokens[index]);
        ++index;
        if (tokens[index] == ",") {
            ++index;
        } else if (tokens[index] != ")") {
            throw std::runtime_error("expected , " + __LINE__);
        }
    }
    ++index;
    std::cout << tokens[index] << std::endl;
    if (index >= tokens.size() || tokens[index] != "{") {
        throw std::runtime_error("Expected '{' to start function body");
    }
    ++index;

    std::vector<std::string> body;
    while (index < tokens.size() && tokens[index] != "}") {
        body.push_back(tokens[index]);
        ++index;
    }

    if (index >= tokens.size() || tokens[index] != "}") {
        throw std::runtime_error("Expected '}' to end function body");
    }
    ++index;

    Function* newFunction = new Function{&body, &arg_names};
    symbolTable.setVal(functionName, newFunction);
    std::cout << "end" << std::endl;
}

void Interpreter::callFunction(const std::vector<std::string>& tokens,
                               size_t& index) {
    throw "Functions not ready";
    // std::cout << "callFunction called" << std::endl;
    // std::string functionName = tokens[index];
    // ++index;
    // //Object* value = evaluateExpression(tokens, index);
    // if(tokens[index]!= "(")
    // {
    //     throw std::runtime_error("expected ( after function name " +
    //     __LINE__);
    // }
    // ++index;
    // Array* arguments = newArray(tokens, index);
    // functionTable[functionName]->__call__(arguments);
}

void Interpreter::print(Object* arg) {
    std::cout << "disp is called: ";
    // std::cout << arg->__str__() << std::endl;
    iout << arg->__str__() << std::endl;
}

Array* Interpreter::newArray(const std::vector<std::string>& tokens,
                             size_t& index) {
    Array* obj = new Array;
    while (tokens[index] != ")") {
        Object* tmp = evaluateExpression(tokens, index);
        obj->__add_assign__(tmp);
        ++index;
        if (tokens[index] != "," && tokens[index] != ")") {
            throw std::runtime_error("expected , or ) after an argument " +
                                     __LINE__);
        }
        if (tokens[index] == ",") {
            ++index;
        }
    }
    return obj;
}
