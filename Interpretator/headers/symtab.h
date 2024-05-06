#ifndef __DATA__TYPES__
#define __DATA__TYPES__

#include<string>
#include<unordered_map>
#include<vector>

class symtab
{
private:
    std::vector<std::unordered_map<std::string, void*>> map;
    
    int levl = 0;

public:
    bool  are(std::string& name);
    bool  setNewVal(std::string& name, std::string& value);
    void* getVal(std::string& name);

public:
    void pushSpace();
    void popSpace();

};

#endif
