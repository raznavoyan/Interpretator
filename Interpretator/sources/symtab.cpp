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

void symtab::setFunction(std::string name, int start, int end){
    auto it = functions.find(name);
    if(it != functions.end()){
        it->second = point(start, end);
    }

    functions[name] = point(start,end);
}

point* symtab::getFunction(std::string name){
    auto it = functions.find(name);
    if(it != functions.end()){
        return &it->second;
    }
    return nullptr;
}


