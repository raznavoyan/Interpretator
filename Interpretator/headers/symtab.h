#ifndef __DATA__TYPES__
#define __DATA__TYPES__

#include <string>
#include <unordered_map>
#include <vector>
#include "object.h"
#include "parser.h"

class symtab {
private:
    std::vector<std::unordered_map<std::string, Object*>> scopeStack;
    int level = 0;
    Object* makeObject(const std::string& valName, std::string value);
public:
    symtab();
    ~symtab();
    void clearLevel();
    bool are(const std::string& name);
    Object* setArr(const std::string& name, std::vector<std::string> vec);
    Object* setVal(const std::string& name, Object* value);
    Object* setVal(const std::string& name, std::string value);
    Object* getVal(const std::string& name);
    void pushSpace();
    void popSpace();
};

#endif