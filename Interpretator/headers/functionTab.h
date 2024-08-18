#ifndef __FUNCTION__TAB__
#define __FUNCTION__TAB__

#include "headers.h"

class functionTab
{
private:
    std::unordered_map<std::string, Object*> functionTable;
public:
    functionTab();
    ~functionTab();
};





#endif