#ifndef __DATA__TYPES__
#define __DATA__TYPES__

#include <string>
#include <unordered_map>
#include <vector>
#include "object.h"
#include "parser.h"

class symtab {
private:
    unsigned long long tmpcount = 0;
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

public: // new objects

    Object* createObject(std::string value, std::string name);
    Object* createObject(std::string value);

private:
    Object* newObject(std::string val);

};
#endif