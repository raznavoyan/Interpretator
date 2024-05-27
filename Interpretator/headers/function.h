#ifndef __FUNCTION__H__
#define __FUNCTION__H__

#include "headers.h"
#include "object.h"

class Function
{
private:
    std::vector<std::string> argTypes;
    std::vector<std::string> code; 
    
public:
    Function(std::vector<std::string> code);
    ~Function();
    Object* call(Array* args);
};

#endif
