#include "symtab.h"

symtab::symtab() {
    scopeStack.push_back(std::unordered_map<std::string, Object*>());
}

symtab::~symtab() {
    for (auto& scope : scopeStack) {
        for (auto& pair : scope) {
            delete pair.second;
        }
    }
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

Object* symtab::makeObject(const std::string& valName, std::string value){
    std::string valtype = parser::typeOf(value);
    if(tmp != nullptr){
        if(tmp->name != valtype){
            delete tmp;
            tmp = nullptr;
            char t = valtype[0];
            switch (t)
            {
            case 'i'://INT
                tmp = new Int(std::stoi(value));
                break;
            case 'd'://DOUBLE
                tmp = new Double(std::stod(value));
                break;
            case 'b'://BOOL
                bool val = (value == "true" ? true : false);
                tmp = new Bool(val);
                break;
            case 's'://String
                tmp = new String(value);
                break;
            // case 'a'://Array
            //     tmp = new Int();
            //     break;
            
            default:
                throw std::out_of_range("No such type of variable\n");
                break;
            }
        }
    }

    //Object* obj = new Object(value);  // Assuming Object can be constructed from std::string
    return ;//setVal(name, obj);
}

Object* symtab::setArr(const std::string& name, std::vector<std::string> vec){
    std::vector<std::string> 
}

Object* symtab::setVal(const std::string& name, std::string value) {
    
    Object* tmp = nullptr;
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            tmp = found->second;
            break;
        }
    }


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
