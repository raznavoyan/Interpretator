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
public:
    symtab();
    ~symtab();
    void clearLevel();
    bool are(const std::string& name);
    Object* setVal(const std::string& name, Object* value);
    Object* getVal(const std::string& name);
    void pushSpace();
    void popSpace();
};

#endif