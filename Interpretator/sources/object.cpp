#ifndef __OBJECT_CPP_
#define __OBJECT_CPP_


#include "../headers/object.h"
#include <string>
#include <stdexcept>
#include <vector>

//////// OBJECT ////////////////////////
Object::Object(std::string n, void* v)
    : name{n}, value{v}, cnt{1}
{}

std::string Object::__str__() 
{
    return "Object";
}

Object Object::__add__(Object*)
{
    throw std::runtime_error("Addition not supported for this objects");
}

Object Object::__sub__(Object*)
{
    throw std::runtime_error("Subtruction not supported for this objects");
}

Object Object::__mod__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__mul__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__div__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__or__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__and__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__den__()
{
    throw std::runtime_error("This operation is not supported for this object");
}

Object Object::__bit_and__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__bit_or__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__bit_den__()
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__xor__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__left_shift__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__right_shift__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__add_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__sub_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__div_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__mul_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__mod_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__or_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__and_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__xor_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__lshift_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__rshift_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::__den_assign__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Object::__more__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Object::__less__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Object::__more_equal__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Object::__less_equal__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Object::__equal__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Object::__not_equal__(Object*)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object::~Object()
{}
///////////////////////////////////////


////////////////// INT /////////////////
Int::Int() 
    : Object("int", new int{0}) 
{}

Int::Int(Object* ptr)
    : Object("int", new int{0}) 
{
    if(ptr->name == "int")
    {
        *(static_cast<int*>(value)) = *(static_cast<int*>(ptr->value));
    } else if(ptr->name == "bool")
    {
        *(static_cast<int*>(value)) = *(static_cast<bool*>(ptr->value));
    } else if(ptr->name == "string")
    {
        *(static_cast<int*>(value)) = std::stoi(*(static_cast<std::string*>(ptr->value)));
    } else if(ptr->name == "double")
    {
        *(static_cast<int*>(value)) = *(static_cast<double*>(ptr->value));
    } else 
    {
        throw std::invalid_argument("Conversion from this type to int is not supported.");
    }
}

std::string Int::__str__()
{
    return std::to_string(*(static_cast<int*>(value)));
}

Object Int::__add__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        int res = *(static_cast<int*>(value)) + *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    if(other->name == "double")
    {
        double res = *(static_cast<int*>(value)) + *(static_cast<double*>((*other).value));
        Double new_obj;
        *static_cast<double*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__sub__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        int res = *(static_cast<int*>(value)) - *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    if(other->name == "double")
    {
        double res = *(static_cast<int*>(value)) - *(static_cast<double*>((*other).value));
        Double new_obj;
        *static_cast<double*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__mod__(Object* other)
{
    if(other->name == "int")
    {
        int res = *(static_cast<int*>(value)) % *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__mul__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        int res = *(static_cast<int*>(value)) * *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    if(other->name == "double")
    {
        double res = *(static_cast<int*>(value)) * *(static_cast<double*>((*other).value));
        Double new_obj;
        *static_cast<double*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__div__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        int res = *(static_cast<int*>(value)) / *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    if(other->name == "double")
    {
        double res = *(static_cast<int*>(value)) / *(static_cast<double*>((*other).value));
        Double new_obj;
        *static_cast<double*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__or__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        bool res = *(static_cast<int*>(value)) || *(static_cast<bool*>((*other).value));
        Bool new_obj;
        *static_cast<bool*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__and__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        bool res = *(static_cast<int*>(value)) && *(static_cast<bool*>((*other).value));
        Bool new_obj;
        *static_cast<bool*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__den__()
{
    bool res = !*(static_cast<bool*>(value));
    Bool new_obj;
    *static_cast<bool*>(new_obj.value) = res;
    return new_obj;
}

Object Int::__bit_and__(Object* other)
{
    if(other->name == "int")
    {
        int res = *(static_cast<int*>(value)) & *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__bit_or__(Object* other)
{
    if(other->name == "int")
    {
        int res = *(static_cast<int*>(value)) | *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__bit_den__()
{
    int res = ~*(static_cast<bool*>(value));
    Int new_obj;
    *static_cast<int*>(new_obj.value) = res;
    return new_obj;
}

Object Int::__xor__(Object* other)
{
    if(other->name == "int")
    {
        int res = *(static_cast<int*>(value)) ^ *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__left_shift__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        int res = *(static_cast<int*>(value)) << *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Int::__right_shift__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        int res = *(static_cast<int*>(value)) >> *(static_cast<int*>((*other).value));
        Int new_obj;
        *static_cast<int*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

void Int::__add_assign__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        *(static_cast<int*>(value)) += *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__sub_assign__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        *(static_cast<int*>(value)) -= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__div_assign__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        *(static_cast<int*>(value)) /= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__mul_assign__(Object* other)
{
    if(other->name == "int")
    {
        *(static_cast<int*>(value)) %= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__mod_assign__(Object* other)
{
    if(other->name == "int")
    {
        *(static_cast<int*>(value)) %= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__xor_assign__(Object* other)
{
    if(other->name == "int")
    {
        *(static_cast<int*>(value)) ^= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__lshift_assign__(Object* other)
{
    if(other->name == "int")
    {
        *(static_cast<int*>(value)) <<= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__rshift_assign__(Object* other)
{
    if(other->name == "int")
    {
        *(static_cast<int*>(value)) >>= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Int::__more__(Object* other)
{
    if(other->name == "int" || other->name == "bool" || other->name == "double")
    {
        return *(static_cast<int*>(value)) > *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Int::__less__(Object* other)
{
    if(other->name == "int" || other->name == "bool" || other->name == "double")
    {
        return *(static_cast<int*>(value)) < *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Int::__more_equal__(Object* other)
{
    if(other->name == "int" || other->name == "bool" || other->name == "double")
    {
        return *(static_cast<int*>(value)) >= *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Int::__less_equal__(Object* other)
{
    if(other->name == "int" || other->name == "bool" || other->name == "double")
    {
        return *(static_cast<int*>(value)) <= *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Int::__equal__(Object* other)
{
    if(other->name == "int" || other->name == "bool" || other->name == "double")
    {
        return *(static_cast<int*>(value)) == *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Int::__not_equal__(Object* other)
{
    if(other->name == "int" || other->name == "bool" || other->name == "double")
    {
        return *(static_cast<int*>(value)) != *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}
/////////////////////////////////////////////


/////////// DOUBLE ////////////////////////////
Double::Double() 
    : Object("double", new bool{}) 
{}

Double::Double(Object* ptr)
    : Object("double", new bool{}) 
{
    if(ptr->name == "int")
    {
        *(static_cast<double*>(value)) = *(static_cast<int*>(ptr->value));
    } else if(ptr->name == "double")
    {
        *(static_cast<double*>(value)) = *(static_cast<double*>(ptr->value));
    } else if(ptr->name == "string")
    {
        *(static_cast<double*>(value)) = std::stod(*(static_cast<std::string*>(ptr->value)));
    } else 
    {
        throw std::invalid_argument("Conversion from this type to double is not supported.");
    }  
}

std::string Double::__str__()
{
    return std::to_string(*(static_cast<double*>(value)));
}

Object Double::__add__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        int res = *(static_cast<double*>(value)) + *(static_cast<double*>((*other).value));
        Double new_obj;
        *static_cast<double*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to double is not supported.");
}

Object Double::__sub__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        double res = *(static_cast<double*>(value)) - *(static_cast<double*>((*other).value));
        Double new_obj;
        *static_cast<double*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to double is not supported.");
}

Object Double::__mul__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        double res = *(static_cast<double*>(value)) * *(static_cast<double*>((*other).value));
        Double new_obj;
        *static_cast<double*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to double is not supported.");
}

Object Double::__div__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        double res = *(static_cast<double*>(value)) * *(static_cast<double*>((*other).value));
        Double new_obj;
        *static_cast<double*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to double is not supported.");
}
///////////////////////////////////////////////

///////////// BOOL /////////////////////////
Bool::Bool() 
    : Object("bool", new bool{}) 
{}

Bool::Bool(Object* ptr)
    : Object("bool", new bool{}) 
{
    if(ptr->name == "int")
    {
        *(static_cast<bool*>(value)) = *(static_cast<int*>(ptr->value));
    } else if(ptr->name == "bool")
    {
        *(static_cast<bool*>(value)) = *(static_cast<bool*>(ptr->value));
    } else if(ptr->name == "string")
    {
        *(static_cast<bool*>(value)) = (*(static_cast<std::string*>(ptr->value)) == "true") ? true : false;
    } else 
    {
        throw std::invalid_argument("Conversion from this type to bool is not supported.");
    }  
}

std::string Bool::__str__() 
{
    return (*(static_cast<bool*>(value)) == true) ? "true" : "false";
}

Object Bool::__add__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        int res = *(static_cast<bool*>(value)) + *(static_cast<bool*>((*other).value));
        Bool new_obj;
        *static_cast<bool*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to bool is not supported.");
}

Object Bool::__sub__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        int res = *(static_cast<bool*>(value)) - *(static_cast<bool*>((*other).value));
        Bool new_obj;
        *static_cast<bool*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to bool is not supported.");
}

/////////////////////////////////////////////

////////////////////// STRING /////////////
String::String() 
    : Object("string", new std::string) 
{}

String::String(Object* ptr)
    : Object("string", new std::string) 
{
    *(static_cast<std::string*>(value)) = ptr->__str__();   
}

std::string String::__str__()
{
    return *(static_cast<std::string*>(value));
}

Object String::__add__(Object* other)
{
    std::string res = __str__() + other->__str__();
    String new_obj;
    *static_cast<std::string*>(new_obj.value) = res;
    return new_obj;
}

/////////////////////////////////////////////////////

///////////// Array /////////////////////////////
Array::Array() 
    : Object("array", new std::vector<Object*>) 
{}

Array::Array(Object* ptr)
    : Object("array", new std::vector<Object*>) 
{
    throw std::runtime_error("Array initialization from Object* not supported");
}

std::string Array::__str__() 
{
    std::string res = "{";
    std::vector<Object*> vec = *(reinterpret_cast<std::vector<Object*>*>(value));
    for(int i = 0; i < vec.size(); ++i)
    {
        Object* item = vec[i];
        if(i != vec.size() - 1)
        {
            res += (*item).__str__() + ", ";
        }
        else 
        {
            res+= (*item).__str__() + "}";
        }
    }
    return res;
}
//////////////////////////////////////////

#endif