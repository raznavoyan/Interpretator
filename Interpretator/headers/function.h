#ifndef __FUNCTION__H__
#define __FUNCTION__H__

#include "headers.h"

class function
{
private:
    std::vector<std::string> argTypes;
    std::vector<std::string> code;   
public:
    function(std::vector<std::string> code);
    ~function();
    Object* call(Array* args);
};

#endif
