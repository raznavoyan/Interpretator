#ifndef __DATA__TYPES__
#define __DATA__TYPES__

#include <string>
#include <unordered_map>
#include <vector>
#include "object.h"
#include "parser.h"




struct point
{
    point(int a, int b): start(a), end(b){}
    int start;
    int end;
};

class symtab {
private:
    
    std::vector<std::unordered_map<std::string, Object*>> scopeStack;
    std::unordered_map<std::string, point> functions;
    int level = 0;
public:
    symtab();
    ~symtab();
    
    void setFunction(std::string name, int start, int end);
    point* getFunction(std::string name);

    void clearLevel();
    bool are(const std::string& name);
    Object* setVal(const std::string& name, Object* value);
    Object* getVal(const std::string& name);
    void pushSpace();
    void popSpace();
};

#endif