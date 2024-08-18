#include "symtab.h"

symtab::symtab() {
    scopeStack.push_back(std::unordered_map<std::string, Object*>());
}

symtab::~symtab() {
    std::cout << "deleting symtab" << std::endl;
    while(this->level > 0) {
        this->popSpace();
    }
    std::cout << "ended" << std::endl;
}

void symtab::clearLevel() {
    if (level > 0) {
        for (auto& pair : scopeStack.back()) {
            delete pair.second;
        }
        scopeStack.back().clear();
    }
}

bool symtab::are(const std::string& name) {
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
        if (it->find(name) != it->end()) {
            return true;
        }
    }
    return false;
}

Object* symtab::setVal(const std::string& varName, Object* value) {
    auto& currentScope = scopeStack.back();
    auto it = currentScope.find(varName);

    if (it != currentScope.end()) {
        if(it->second->name == value->name){
            it->second->value = value->value;
            return it->second;
        }
        delete it->second;
    }

    currentScope[varName] = value;
    return currentScope[varName];
}

Object* symtab::getVal(const std::string& name) {
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second;
        }
    }
    return nullptr;
}

void symtab::pushSpace() {
    scopeStack.push_back(std::unordered_map<std::string, Object*>());
    ++level;
}

void symtab::popSpace() {
    if (level > 0) {
        Object* tmp = nullptr;
        for (auto& pair : scopeStack.back()) {
            //if(pair.second->count == 0){
                delete pair.second;
            //}
        }
        scopeStack.pop_back();
        --level;
    }
}

Object* symtab::createObject(std::string val, std::string name){
    Object* tmp = newObject(val);
    setVal(name, tmp);
    return tmp;
}

Object* symtab::createObject(std::string val) {
    Object* tmp = newObject(val);
    setVal((std::to_string(++tmpcount)), tmp);
    return tmp; 
}

Object *symtab::newObject(std::string value)
{ 
    std::string valtype = parser::typeOf(value); // Assuming parser class provides type information

    Object* tmp = nullptr;
    char t = static_cast<char>(valtype[0]);
    
    switch (t) {
        case 'i': // INT
            try {
                tmp = new Int(std::stoi(value));
                // std::cout << "New Int: " << tmp << std::endl;
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
            throw "array not redady now";
            break;
        default:
            throw std::out_of_range("No such type of variable: " + value);
    }

    return tmp;
}
