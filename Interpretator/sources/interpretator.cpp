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
        std::cout << token << std::endl;
        if (token == "|") {
            ++this->index;
            continue;
        }

        if (parser::isAKeyword(token)) {
            if (token == "def") {
                defineFunction(tokens, this->index);
            } else if (token == "if") {
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
            } else if (token == "disp"){
                print(evaluateExpression(tokens,index));
            }else{
                throw std::runtime_error("Unknown operation");
            }
        } else if(functionTable.count(token)){
            callFunction(tokens, index);
        }else if (parser::isVariableName(token)) {
            if (parser::isAssignment(tokens[this->index + 1])) {
                throw std::runtime_error("wrong operation expected assignment");
            }
            executeAssignment(tokens, this->index);
        } else {
            throw std::runtime_error("wrong operation no such function");
        }
        ++this->index;
    }
}

void Interpreter::executeAssignment(const std::vector<std::string>& tokens, size_t& index) 
{
    std::cout << "executeAssignment called" << std::endl;
    std::string variableName = tokens[index];
    //index += 2; 
    //Object* value = evaluateExpression(tokens, index);

    ++index;
    if(tokens[index] == "=")
    {
        ++index;
        Object* value = evaluateExpression(tokens, index);
        symbolTable.setVal(variableName, value);
    } else {
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
    }
    std::cout << "executeAssignment ended" << std::endl;
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
        std::cout << (start->__equal__(end))->__str__() << std::endl;
        while ((start->__equal__(end))->__str__() == "false") {
                ///////////////////////////////////////////
            std::cout << "for sycl:\t" << startName << '\t' << start->__str__() << std::endl;
                //////////////////////////////////////////
                executeBlock(tokens, index); 
            index = blockStart;
            start = start->__add__(step);
        }

        while (tokens[index] != "}") {

                ///////////////////////////////////////////
                //////////////////////////////////////////
            ++index;
        }
        // ++index; 
    }
    symbolTable.popSpace();
    std::cout << "for ended successfully" << std::endl;
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

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}


void Interpreter::addBrecets(parser::toks& expression){
    std::cout << "addBrecets" << std::endl;
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
        result.push_back(std::string(1, operators.top()));
        operators.pop();
    }

    expression = result;
    std::cout << std::endl;
}

Object* Interpreter::evaluateSubExpression(parser::toks& expression){
    std::cout << "evaluateSubExpression called" << std::endl; 
    if (expression.empty()) {
        // Error: Unexpected end of expression
        return nullptr;
    }

    Object* tmp = nullptr;
    while (!expression.empty()) {
        // Handle unary operators
        if (expression[0] == "-") {
            // Negation
            expression.erase(expression.begin());
            tmp = evaluateSubExpression(expression);
            if (tmp) {
                tmp = tmp->__neg__();
            }
        } else if (expression[0] == "~") {
            // Bitwise complement
            expression.erase(expression.begin());
            tmp = evaluateSubExpression(expression);
            if (tmp) {
                tmp = tmp->__complement__();
            }
        } else if (expression[0] == "!") {
            // Logical negation
            expression.erase(expression.begin());
            tmp = evaluateSubExpression(expression);
            if (tmp) {
                tmp = tmp->__logical_not__();
            }
        } else {
            // Handle binary operators
            std::string op = expression[0];
            expression.erase(expression.begin());

            Object* left = evaluateSubExpression(expression);
            if (!left) {
                return nullptr;
            }

            // Perform binary operation based on the operator
            if (expression[1] == "+") {
                tmp = tmp->__add__(left);
            } else if (expression[index - 1] == "-") {
                tmp = tmp->__sub__(left);
            } else if (expression[index - 1] == "*") {
                tmp = tmp->__mul__(left);
            } else if (expression[index - 1] == "/") {
                tmp = tmp->__div__(left);
            } else if (expression[index - 1] == "%") {
                tmp = tmp->__mod__(left);
            } else if (expression[index - 1] == "|") {
                tmp = tmp->__or__(left);
            } else if (expression[index - 1] == "&") {
                tmp = tmp->__and__(left);
            } else if (expression[index - 1] == "~") {
                tmp = tmp->__complement__();
            } else if (expression[index - 1] == "||") {
                tmp = tmp->__or__(left);
            } else if (expression[index - 1] == "&&") {
                tmp = tmp->__and__(left);
            } else if (expression[index - 1] == "^") {
                tmp = tmp->__xor__(left);
            } else if (expression[index - 1] == "<<") {
                tmp = tmp->__left_shift__(left);
            } else if (expression[index - 1] == ">>") {
                tmp = tmp->__right_shift__(left);
            } else if (expression[index - 1] == ">") {
                tmp = tmp->__more__(left);
            } else if (expression[index - 1] == "<") {
                tmp = tmp->__less__(left);
            } else if (expression[index - 1] == ">=") {
                tmp = tmp->__more_equal__(left);
            } else if (expression[index - 1] == "<=") {
                tmp = tmp->__less_equal__(left);
            } else if (expression[index - 1] == "==") {
                tmp = tmp->__equal__(left);
            } else if (expression[index - 1] == "!=") {
                tmp = tmp->__not_equal__(left);
            } else {
                // Error: Unsupported binary operator
                std::cerr << "Error: Unsupported operator '" << op << "'" << std::endl;
                return nullptr;
            }
            // Ensure to handle errors for unsupported operators
            /*
            else {
                std::cerr << "Error: Unsupported operator '" << op << "'" << std::endl;
                return nullptr;
            }
            */
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
    parser::toks expretion;
    while(index < tokens.size() && tokens[index] != "|"){expretion.push_back(tokens[index]);++index;}
    ++index;
    addBrecets(expretion);
    for(auto& it : expretion){std::cout << it;}
    std::cout << std::endl;
    Object* tmp = evaluateSubExpression(expretion);
    // Check if the token represents a known type
    
    // Store the result in the symbol table
    if(tmp != nullptr)
        symbolTable.setVal(("tmp" + std::to_string(index)), tmp);

    do{++index;}while(tokens[index] != "|");

    std::cout << "evaluateExpression ended" << std::endl;
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
}


void Interpreter::defineFunction(const std::vector<std::string>& tokens, size_t& index) {
    if (tokens[index] != "def") {
        throw std::runtime_error("Expected 'def' for function definition");
    }
    ++index;

    if (index >= tokens.size() || !parser::isVariableName(tokens[index])) {
        throw std::runtime_error("Expected function name");
    }

    std::string functionName = tokens[index];
    ++index;

    if (index >= tokens.size() || tokens[index] != "()") {
        throw std::runtime_error("Expected '()' after function name");
    }
    ++index;

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

    //functionTable[functionName] = Function(body);
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