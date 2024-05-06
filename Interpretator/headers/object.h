#ifndef __OBJECT__
#define __OBJECT__
#include <string>

class object
{
private:
    /* data */
    std::string name;
    void* data;
    int refcount = 0;

public:
    object(/* args */);
    ~object();

public:
    virtual std::string ots() = 0;
};

object::object(/* args */)
{}

object::~object()
{}

#endif
