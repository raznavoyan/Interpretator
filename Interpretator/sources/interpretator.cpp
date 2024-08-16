#include "interpretator.h"
#include "parser.h"
#include "symtab.h"
#include <unistd.h>
#include <iostream>
#include <sstream>


Interpreter::Interpreter(std::vector<std::string> code, std::vector<std::string>* parameters, Object* arguments)
    :ret{new Object("Null_object", nullptr)}
{
    std::cout << "Interpreter initialized." << std::endl;
    if(parameters)
    {
        for(int i = 0; i < parameters->size(); ++i)
        {
            symbolTable.setVal(parameters->at(i), arguments->__at__(i));
        }
    }
    execute(code);
}

Interpreter::~Interpreter() 
{
    std::cout << "Interpreter destroyed." << std::endl;
}

void Interpreter::execute(std::vector<std::string>& tokens) {
    while (this->index < tokens.size()) {
        const std::string& token = tokens[this->index];
        std::cout << token << " " << tokens[index + 1] << "\tin execute function" << std::endl;
        if (token == "|" || token == "}") {
            ++this->index;
            continue;
        }
        if (parser::isAKeyword(token)) {
            if (token == "def") {
                defineFunction(tokens, this->index);
            } else if (token == "if") {
                ++this->index;
                executeIf(tokens, this->index);
            } else if (token == "during") {
                ++this->index;
                executeDuring(tokens, this->index);
            } else if (token == "loop") {
                ++this->index;
                std::cout << "\t called" << std::endl;
                executeLoop(tokens, this->index);
            } else if (token == "disp"){
                index += 2;
                print(evaluateExpression(tokens, index));
            } else {
                throw std::runtime_error("Unknown operation");
            }
        } else if(functionTable.count(token)){
            callFunction(tokens, index);
        }else if (parser::isVariableName(token)) {
            if (!parser::isAssignment(tokens[this->index + 1])) {
                // std::cout << tokens[index] << tokens[index + 1] << std::endl; 
                throw std::runtime_error("wrong operation expected assignment");
            }
            executeAssignment(tokens, this->index);
        } else if(token == "{"){
            executeBlock(tokens, index);
        } else {
            // std::cout << tokens[index] << tokens[index + 1] << std::endl;
            throw std::runtime_error("wrong operation no such function");
        }
        std::cout << "New execute" << std::endl;
    }
}

void Interpreter::executeAssignment(const std::vector<std::string>& tokens, size_t& index) 
{
    std::cout << "Assignment called" << std::endl;
    std::string variableName = tokens[index];
    ++index;
    if(tokens[index] == "=")
    {
        ++index;
        Object* value = evaluateExpression(tokens, index);
        symbolTable.setVal(variableName, value);
    } else 
    if(tokens[index] == "+=")
        {
            if(!symbolTable.are(variableName))
            {
                throw std::runtime_error("INTERPRETATOR:: no such variable " + __LINE__);
            }
            ++index;
            Object* value = evaluateExpression(tokens, index);
            symbolTable.getVal(variableName)->__add_assign__(value);
        } else if(tokens[index] == "-=")
        {
            if(!symbolTable.are(variableName))
            {
                throw std::runtime_error("INTERPRETATOR:: no such variable " + __LINE__);
            }
            ++index;
            Object* value = evaluateExpression(tokens, index);
            symbolTable.getVal(variableName)->__sub_assign__(value);
        } else if(tokens[index] == "*=")
        {
            if(!symbolTable.are(variableName))
            {
                throw std::runtime_error("INTERPRETATOR:: no such variable " + __LINE__);
            }
            ++index;
            Object* value = evaluateExpression(tokens, index);
            symbolTable.getVal(variableName)->__mul_assign__(value);
        } else if(tokens[index] == "/=")
        {
            if(!symbolTable.are(variableName))
            {
                throw std::runtime_error("INTERPRETATOR:: no such variable " + __LINE__);
            }
            ++index;
            Object* value = evaluateExpression(tokens, index);
            symbolTable.getVal(variableName)->__div_assign__(value);
        } else if(tokens[index] == ">>=")
        {
            if(!symbolTable.are(variableName))
            {
                throw std::runtime_error("INTERPRETATOR:: no such variable " + __LINE__);
            }
            ++index;
            Object* value = evaluateExpression(tokens, index);
            symbolTable.getVal(variableName)->__rshift_assign__(value);
        } else if(tokens[index] == "<<=")
        {
            if(!symbolTable.are(variableName))
            {
                throw std::runtime_error("INTERPRETATOR:: no such variable " + __LINE__);
            }
            ++index;
            Object* value = evaluateExpression(tokens, index);
            symbolTable.getVal(variableName)->__lshift_assign__(value);
        } else if(tokens[index] == "%=")
        {
            if(!symbolTable.are(variableName))
            {
                throw std::runtime_error("INTERPRETATOR:: no such variable " + __LINE__);
            }
            ++index;
            Object* value = evaluateExpression(tokens, index);
            symbolTable.getVal(variableName)->__mod_assign__(value);
        } else if(tokens[index] == "^=")
        {
            if(!symbolTable.are(variableName))
            {
                throw std::runtime_error("INTERPRETATOR:: no such variable " + __LINE__);
            }
            ++index;
            Object* value = evaluateExpression(tokens, index);
            symbolTable.getVal(variableName)->__xor_assign__(value);
        }

    // std::cout << tokens[index] << std::endl;
    std::cout << "executeAssignment ended" << std::endl;
}

void Interpreter::executeIf(const std::vector<std::string>& tokens, size_t& index) 
{
    // std::cout << "execute if start" << std::endl;
    if (tokens[index] != ":") {
        throw std::runtime_error("expected : after if");
    }
    ++index;
    symbolTable.pushSpace();
    Object* condition = evaluateExpression(tokens, index);
    if (*static_cast<bool*>(condition->value)) {
        const size_t Start = index; 
        executeBlock(tokens, index);
        index = Start;
        while (tokens[index] != "}" && index < tokens.size()) {
            std::cout << tokens[index] << " ";
            ++index;
        }
        std::cout << std::endl;
        ++index;

        while (tokens[index] == "otherwise" && index < tokens.size()) {
            while (tokens[index] != "}" && index < tokens.size()) {
                ++index;
            }
            ++index;
        }
        
    } else {
        while (tokens[index] != "}" && index < tokens.size()) {
            ++index;
        }
        ++index;
        if (tokens[index] == "otherwise") {
            ++index;
            if(tokens[index] == "if"){
                ++index;
                executeIf(tokens, index);
            }else{
                const size_t Start = index; 
                executeBlock(tokens, index);
                index = Start;
                while(tokens[index] != "}"){++index;}
            }
        }
    }
    symbolTable.popSpace();
    // std::cout << "end if" << std::endl;
}

void Interpreter::executeOtherwise(const std::vector<std::string>& tokens, size_t& index) 
{
    if (tokens[index] != "{") {
        throw std::runtime_error("expected { after otherwise");
    }
    executeBlock(tokens, index);
}

void Interpreter::executeDuring(const std::vector<std::string>& tokens, size_t& index) 
{
    //Ex: during : c == true {
    symbolTable.pushSpace();
    std::cout << "during stsrted" << std::endl;
    if(tokens[index] != ":")
    {
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
        start = symbolTable.createObject(tokens[index]);
        symbolTable.setVal(startName, start);
    }else if (symbolTable.are(tokens[index])) {
        *start = *symbolTable.getVal(tokens[index]);
        symbolTable.setVal(startName, start);
    }else{
        std::runtime_error("expected: value");
    }
    ++index;
    if(tokens[index] != "->")
    {
        throw std::runtime_error("expected ->");
    }
    ++index;

    Object* end = nullptr;
    if(parser::isNumber(tokens[index])){
        end = symbolTable.createObject(tokens[index]);
        symbolTable.setVal("end" + std::to_string(index), end);
    }else if (symbolTable.are(tokens[index])) {
        end = symbolTable.getVal(tokens[index]);
    }else {
        std::runtime_error("expected: value");
    }

    ++index;
    Object* step = nullptr;
    if(tokens[index] == ","){
        if(parser::isNumber(tokens[index])){
            step = symbolTable.createObject(tokens[index]);
            symbolTable.setVal("step" + std::to_string(index), step);
        }else if (symbolTable.are(tokens[index])) {
            step = symbolTable.getVal(tokens[index]);
        }else{
            std::runtime_error("expected: value");
        }
    }else{
        step = symbolTable.createObject("1");
        symbolTable.setVal("step" + std::to_string(index), step);
    }

    std::cout << "start: " << start << " " << start->__str__() << std::endl;
    std::cout << "end: "   << end   << " " << end->__str__()   << std::endl;
    std::cout << "step: "  << step  << " " << step->__str__()  << std::endl;

    std::cout << "start loop "<< index << " " << tokens.size() << std::endl;
    std::cout << "with token "<< tokens[index] << std::endl;
    if (tokens.size() < index && tokens[index] == "{") {
        std::cout << "checkpoint1" << std::endl;
        size_t blockStart = index;
        std::cout << "checkpoint1" << std::endl;
        std::cout << start->__str__() << '\t' << end->__str__() << std::endl;
        std::cout << (start->__equal__(end))->__str__() << std::endl;
        std::cout << "checkpoint2" << std::endl;

        while ((start->__equal__(end))->__str__() == "false") {
                ///////////////////////////////////////////
            std::cout << "for sycl:\t" << startName << '\t' << start->__str__() << std::endl;
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

void Interpreter::executeBlock(const std::vector<std::string>& tokens, size_t& index) 
{   
    std::cout << "execute block" << std::endl;
    symbolTable.pushSpace();
    if (tokens[index] == "{") {
        ++index;
    }
    std::vector<std::string> subTokens;
    while (tokens[index] != "}") {
        subTokens.push_back(tokens[index]);
        ++index;
    }
    index = 0;
    execute(subTokens);
    symbolTable.popSpace();
    std::cout << "execute block ended" << std::endl;
}

bool isOperator(char c) {
    std::string operators = "+-/*!%*--==++-><<=+="
                            "-=/=%=>>=<<&&|||&>>=<<=~^"
                            "^=*!=";
    return operators.find(c) != std::string::npos;
}

int precedence(char op ) {
    switch(op) {
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
        case '%':
            return 3;
        case '!':
        case '=':
            return 4;
        case '<':
        case '>':
            return 5;
        case '&':
            return 6;
        case '|':
            return 7;
        case '^':
            return 8;
        case '~':
            return 9;
        default:
            return 1; // Default precedence for other operators
    }
}

void Interpreter::addBrecets(parser::toks& expression){
    // std::cout << "addBrecets" << std::endl;
    std::vector<std::string> result;
    std::stack<char> operators;

    for (const std::string& token : expression) {
        if (token.size() == 1 && isOperator(token[0])) {
            while (!operators.empty() && precedence(operators.top()) >= precedence(token[0]) && operators.top() != '(') {
                result.push_back(std::string(1, operators.top()));
                operators.pop();
            }
            operators.push(token[0]);
        } else if (token == "(") {
            operators.push('(');
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != '(') {
                result.push_back(std::string(1, operators.top()));
                operators.pop();
            }
            operators.pop(); // Remove the '('
        } else { // Operand
            result.push_back(token);
        }
    }

    while (!operators.empty()) {
        std::string stmp = std::string(1, operators.top());
        if(stmp != "(" && stmp != ")"){
            result.push_back(stmp);
        }
        operators.pop();
    }

    for(auto& it : expression){std::cout << it;}
    std::cout << std::endl;
    // std::cout << "adding brecets ended" << std::endl;
    expression = result;
}

Object *Interpreter::evaluateSubExpression(Object *left, Object *right, std::string op ){
    std::cout << "evaluateSubExpression called" << std::endl;
    std::cout << left->__str__() << "  " << op << "  " << right->__str__()  << std::endl;
    Object* tmp = nullptr;
    if (right == nullptr) {
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
    }else {
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

// newArray funcion-i hamar petqa vor evaluateExpression-y minchev storaket exaci value-n veradarcni


Object* Interpreter::evaluateExpression(const std::vector<std::string>& tokens, size_t& index) {
    std::cout << "evaluateExpression started" << std::endl;
    // Handle base cases:
    if (index >= tokens.size()) {
        // Error: Unexpected end of expression
        return nullptr;
    }
    parser::toks expression;
    while(index < tokens.size() && tokens[index] != "|" && tokens[index] != ")" && tokens[index] != ":"){
        if(tokens[index] != "{"){
            expression.push_back(tokens[index]);
        }
        ++index;
    }
    ++index;
    addBrecets(expression);
    for(auto it: expression){
        std::cout << it << ' ' ;
    }
    std::cout << std::endl;
    Object* tmp = nullptr;
    std::vector<Object*> vals;
    size_t i = 0;
    // if(expression[0] == "-" || expression[0] == "+"){
    //     ++i;
    //     vals.push_back(evaluateSubExpression(createObject(expression[i]), nullptr, expression[0]));
    //     ++i;
    //     // std::cout << "i = " << i << std::endl;
    // }
    for(auto& expr : expression) {
        if(parser::isAMath(expr)){
            if(vals.size() < 2){
                throw std::runtime_error("invalit expretion");
            }
            Object* arg2 = vals.back();
            vals.pop_back();
            Object* arg1 = vals.back();
            vals.pop_back();
            vals.push_back(evaluateSubExpression(arg1,arg2, expr));

            for(auto it : vals){
                std::cout << it->__str__() << ' ';
            }
            std::cout << std::endl;

        }else if(symbolTable.are(expr)){
            vals.push_back(symbolTable.getVal(expr));
            std::cout << vals.back()->__str__() << std::endl;

        }else if(parser::isNumber(expr) || expr == "true" || expr == "false"){
            std::cout << "adding new object by value:  " << expr << std::endl;
            vals.push_back(symbolTable.createObject(expr));

            for(auto it : vals){
                std::cout << it->__str__() << ' ';
            }
            std::cout << std::endl;
        }else{
            std::cout << expression[i] << std::endl;
            throw std::runtime_error("unnown token");
        }
    }
    std::cout << vals.size() << std::endl;
    if(vals.size() != 1){
        throw std::runtime_error("wrong expretion1");
    }
    Object* newtmp = vals.back();
    vals.pop_back();
    //do{++index;}while(tokens[index] != "|" && tokens[index] != ":" && !(tokens[index] == ")" && tokens[index + 1] == "{"));
    // std::cout << "evaluateExpression ended  " << tokens[index] << std::endl;
    return newtmp;
}


void Interpreter::defineFunction(const std::vector<std::string>& tokens, size_t& index) {
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
    while(tokens[index] != ")")
    {
        if(!parser::isVariableName(tokens[index]))
        {
            throw std::runtime_error("not valid name for a parameter " + __LINE__);
        }
        arg_names.push_back(tokens[index]);
        ++index;
        if(tokens[index] == ",")
        {
            ++index;
        } else if(tokens[index] != ")")
        {
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

void Interpreter::callFunction(const std::vector<std::string>& tokens, size_t& index) 
{
    std::cout << "callFunction called" << std::endl;
    std::string functionName = tokens[index];
    ++index;
    //Object* value = evaluateExpression(tokens, index);
    if(tokens[index]!= "(")
    {
        throw std::runtime_error("expected ( after function name " + __LINE__);
    }
    ++index;
    Array* arguments = newArray(tokens, index);
    functionTable[functionName]->__call__(arguments);
}

void Interpreter::print(Object *arg)
{
    std::cout << "print is called" << std::endl;
    std::cout << arg->__str__() << std::endl;
    ++index;
}

Array* Interpreter::newArray(const std::vector<std::string>& tokens, size_t& index)
{
    Array* obj = new Array;
    while(tokens[index] != ")")
    {
        Object* tmp = evaluateExpression(tokens, index);
        obj->__add_assign__(tmp);
        ++index;
        if(tokens[index] != "," && tokens[index] != ")")
        {
            throw std::runtime_error("expected , or ) after an argument " + __LINE__);
        }
        if(tokens[index] == ",")
        {
            ++index;
        }
    }
    return obj;
}
