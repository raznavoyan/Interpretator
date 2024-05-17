#ifndef __DATA__TYPES__
#define __DATA__TYPES__

#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include "object.h"

class symtab {
private:
    std::stack<std::unordered_map<std::string, Object*>> scopeStack;
    int level = 0;

public:
    symtab() {
        // Initialize with a global scope
        scopeStack.push(std::unordered_map<std::string, Object*>());

    }
    bool are(const std::string& name) {
        for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
            if (it->find(name) != it->end()) {
                return true;
            }
        }
        return false;
    }
    bool setNewVal(const std::string& name, Object* value) {
        if (!scopeStack.empty()) {
            scopeStack.top()[name] = value;
            return true;
        }
        return false;
    }
    Object* getVal(const std::string& name) {
        for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
            auto found = it->find(name);
            if (found != it->end()) {
                return found->second;
            }
        }
        return nullptr;
    }

    void pushSpace() {
        scopeStack.push(std::unordered_map<std::string, Object*>());
        ++level;
    }
    void popSpace() {
        if (!scopeStack.empty()) {
            scopeStack.pop();
            --level;
        }
    }
};

#endif