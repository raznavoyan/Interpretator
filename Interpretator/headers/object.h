#ifndef __OBJECT__
#define __OBJECT__

#include "symtab.h"
#include <string>
#include <stdexcept>

struct Object {
    std::string name{};
    void* value{};
    int cnt{};
    Object(std::string n, void* v)
      : name{n}
      , value{v}
    {}

    virtual std::string __str__() {
        
        if(name == "bool"){
            if(*(bool*)value){
                return "true";
            }
            return "false";
        }
        if(name == "double")
        return "Object";
    }

    virtual Object* __add__(Object* rptr) {
        throw;
    }

    virtual Object* __call__() {
        throw std::invalid_argument(name + "not have an overloaded call operator");
    }

    virtual ~Object() {}
};

class Int : Object{
public:
    Int() 
      : Object("int", new int{0})
    {}

    Int(Object* ptr)
      : Object("int", new int{0})
    {}

    std::string __str__() {
      return "";
    }
};


/*
   if (ptr->name == "int") {
            *(int*)value = *(int*)ptr->value;
        }
        else if(ptr->name == "int") {
            *(int*)value = *(double*)ptr->value;
        }
        else if (ptr->name == "bool"){
            *(int*)value = *(bool*)ptr->value;
        }
        else if (ptr->name == "string") {
            *(int*)value = std::stoi(*(std::string*)ptr->value);
        }
        else if (ptr->name == "double"){

        }
        else {
          throw std::invalid_argument("No suche datatype");
        }
    
*/

/*
struct String : Object {
    String() 
      : Object("Tox", new std::string, 0)
    {}

    String(Object* ptr)
      : Object("Amboxj", new std::string, 0)
    {
        *(string*)value = ptr->__str__();   
    }

    string __str__() {
      return "";
    }
};
*/

/*
struct Array : Object {
    Array() 
      : Object("Zangvac", new string, 0)
    {}

    Array(Object* ptr)
      : Object("Zangvac", new string, 0)
    {
        throw;   
    }

    string __str__() {
      return "";
    }
};
*/
#endif