#ifndef __OBJECT_CPP_
#define __OBJECT_CPP_


#include "../headers/object.h"
#include <string>
#include <stdexcept>
#include <vector>

//////// OBJECT ////////////////////////
Object::Object(std::string n, void* v)
    : name{n}, value{v}, count{1}
{}

Object::Object(Object&& rhv)
    : name{rhv.name}, value{rhv.value}, count{1}
{
    rhv.value = nullptr;
}

Object::Object(const Object& rhv)
    : name{rhv.name}, value{nullptr}, count{1}
{
    if(rhv.name == "Int")
    {
        int* p = new int{*static_cast<int*>(rhv.value)};
        value = reinterpret_cast<void*>(p);
        p = nullptr;
    } else if(rhv.name == "double")
    {
        double* p = new double{*static_cast<double*>(rhv.value)};
        value = reinterpret_cast<void*>(p);
        p = nullptr;
    } else if(rhv.name == "bool")
    {
        bool* p = new bool{*static_cast<bool*>(rhv.value)};
        value = reinterpret_cast<void*>(p);
        p = nullptr;
    } else if(rhv.name == "string")
    {
        std::string* p = new std::string{*static_cast<std::string*>(rhv.value)};
        value = reinterpret_cast<void*>(p);
        p = nullptr;
    } else if(rhv.name == "array")
    {
        std::vector<Object*>* p = new std::vector<Object*>{*static_cast<std::vector<Object*>*>(rhv.value)};
        value = reinterpret_cast<void*>(p);
        p = nullptr;
    } else
    {
        throw std::runtime_error("can't execute this opereation");
    }
}

const Object& Object::operator=(const Object& rhv)
{
    if (this != &rhv)
    {
        clear();
        name = rhv.name;
        if (rhv.value)
        {
            if (name == "Int")
                value = new int(*static_cast<int*>(rhv.value));
            else if (name == "double")
                value = new double(*static_cast<double*>(rhv.value));
            else if (name == "bool")
                value = new bool(*static_cast<bool*>(rhv.value));
            else if (name == "string")
                value = new std::string(*static_cast<std::string*>(rhv.value));
            else if (name == "array")
            {
                auto& rhvVec = *static_cast<std::vector<Object*>*>(rhv.value);
                auto newVec = new std::vector<Object*>();
                for (auto& obj : rhvVec)
                {
                    newVec->push_back(obj->clone());
                }
                value = newVec;
            }
            else
            {
                throw std::runtime_error("Unsupported type");
            } 
        }
    }
    return *this;
}

const Object& Object::operator=(Object&& rhv)
{
    if (this != &rhv)
    {
        clear();

        name = std::move(rhv.name);
        value = rhv.value;
        rhv.value = nullptr;
    }
    return *this;
}

Object* Object::clone() 
{
    throw std::runtime_error("can't exectute this operation");
}

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

Object Object::__neg__()
{
    throw std::runtime_error("This operation is not supported for this object");
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

void Object::__pop__()
{
    throw std::runtime_error("This operation is not supported for this objects");
}

int Object::__size__()
{
    throw std::runtime_error("This operation is not supported for this objects");
}

Object Object::__at__(int)
{
    throw std::runtime_error("This operation is not supported for this objects");
}

void Object::clear()
{
    if (name == "array" && value)
    {
        auto& elements = *static_cast<std::vector<Object*>*>(value);
        for (auto obj : elements)
        {
            delete obj;
        }
        delete static_cast<std::vector<Object*>*>(value);
    }
    else if (name == "Int")
    {
        delete static_cast<int*>(value);
    }
    else if (name == "double")
    {
        delete static_cast<double*>(value);
    }
    else if (name == "bool")
    {
        delete static_cast<bool*>(value);
    }
    else if (name == "string")
    {
        delete static_cast<std::string*>(value);
    }
   
    value = nullptr;
}

Object::~Object()
{}
///////////////////////////////////////


////////////////// INT /////////////////
Int::Int() 
    : Object("int", new int{0}) 
{}

Int::Int(int val) 
    : Object("int", new int{val}) 
{}

Int::~Int()
{
    delete (static_cast<int*>(value));
}

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

Object Int::__neg__()
{
    Int ans = -*static_cast<int*>(value);
    return ans;
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
    if(other->name == "int" || other->name == "bool" || other->name == "double")
    {
        *(static_cast<int*>(value)) += *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__sub_assign__(Object* other)
{
    if(other->name == "int" || other->name == "bool" || other->name == "double")
    {
        *(static_cast<int*>(value)) -= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__div_assign__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        *(static_cast<int*>(value)) /= *(static_cast<int*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Int::__mul_assign__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        *(static_cast<int*>(value)) *= *(static_cast<int*>(other->value));
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

Object* Int::clone()
{
    return new Int(*static_cast<int*>(value));
}
/////////////////////////////////////////////




/////////// DOUBLE ////////////////////////////
Double::Double() 
    : Object("double", new double{}) 
{}

Double::Double(double val) 
    : Object("double", new double{val}) 
{}

Double::~Double()
{
    delete (static_cast<double*>(value));
}

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

Object Double::__neg__()
{
    Double ans = -*static_cast<double*>(value);
    return ans;
}

void Double::__add_assign__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        *(static_cast<double*>(value)) += *(static_cast<double*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Double::__sub_assign__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        *(static_cast<double*>(value)) -= *(static_cast<double*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Double::__div_assign__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        *(static_cast<double*>(value)) /= *(static_cast<double*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

void Double::__mul_assign__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        *(static_cast<double*>(value)) *= *(static_cast<double*>(other->value));
        return;
    }
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Double::__more__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        return *(static_cast<double*>(value)) > *(static_cast<double*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Double::__less__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        return *(static_cast<double*>(value)) < *(static_cast<double*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Double::__more_equal__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        return *(static_cast<double*>(value)) >= *(static_cast<double*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Double::__less_equal__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        return *(static_cast<double*>(value)) <= *(static_cast<double*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Double::__equal__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        return *(static_cast<double*>(value)) == *(static_cast<double*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Double::__not_equal__(Object* other)
{
    if(other->name == "int" || other->name == "double")
    {
        return *(static_cast<double*>(value)) != *(static_cast<double*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

Object* Double::clone()
{
    return new Double(*static_cast<double*>(value));
}
///////////////////////////////////////////////




///////////// BOOL /////////////////////////
Bool::Bool() 
    : Object("bool", new bool{}) 
{}

Bool::Bool(bool val) 
    : Object("bool", new bool{val}) 
{}

Bool::~Bool()
{
    delete (static_cast<bool*>(value));
}

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

Object Bool::__or__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        bool res = *(static_cast<bool*>(value)) || *(static_cast<bool*>((*other).value));
        Bool new_obj;
        *static_cast<bool*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Bool::__and__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        bool res = *(static_cast<bool*>(value)) && *(static_cast<bool*>((*other).value));
        Bool new_obj;
        *static_cast<bool*>(new_obj.value) = res;
        return new_obj;
    }
    throw std::invalid_argument("Conversion from this type to int is not supported.");
}

Object Bool::__den__()
{
    bool res = !*(static_cast<bool*>(value));
    Bool new_obj;
    *static_cast<bool*>(new_obj.value) = res;
    return new_obj;
}

bool Bool::__more__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        return *(static_cast<int*>(value)) > *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Bool::__less__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        return *(static_cast<int*>(value)) < *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Bool::__more_equal__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        return *(static_cast<int*>(value)) >= *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Bool::__less_equal__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        return *(static_cast<int*>(value)) <= *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Bool::__equal__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        return *(static_cast<int*>(value)) == *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool Bool::__not_equal__(Object* other)
{
    if(other->name == "int" || other->name == "bool")
    {
        return *(static_cast<int*>(value)) != *(static_cast<int*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

Object* Bool::clone()
{
    return new Bool(*static_cast<bool*>(value));
}
/////////////////////////////////////////////



////////////////////// STRING /////////////
String::String() 
    : Object("string", new std::string) 
{}

String::String(std::string val) 
    : Object("string", new std::string{val}) 
{}

String::~String()
{
    delete (static_cast<std::string*>(value));
}

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

void String::__add_assign__(Object* other)
{
    *(static_cast<std::string*>(value)) += other->__str__();
}

bool String::__more__(Object* other)
{
    if(other->name == "string")
    {
        return *(static_cast<std::string*>(value)) > *(static_cast<std::string*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool String::__less__(Object* other)
{
    if(other->name == "string")
    {
        return *(static_cast<std::string*>(value)) < *(static_cast<std::string*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool String::__more_equal__(Object* other)
{
    if(other->name == "string")
    {
        return *(static_cast<std::string*>(value)) >= *(static_cast<std::string*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool String::__less_equal__(Object* other)
{
    if(other->name == "string")
    {
        return *(static_cast<std::string*>(value)) <= *(static_cast<std::string*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool String::__equal__(Object* other)
{
    if(other->name == "string")
    {
        return *(static_cast<std::string*>(value)) == *(static_cast<std::string*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

bool String::__not_equal__(Object* other)
{
    if(other->name == "string")
    {
        return *(static_cast<std::string*>(value)) != *(static_cast<std::string*>(other->value));
    } 
    throw std::runtime_error("This operation is not supported for this objects");
}

void String::__pop__()
{
    (static_cast<std::string*>(value))->pop_back();
}

int String::__size__()
{
    return (static_cast<std::string*>(value))->size();
}

Object String::__at__(int index)
{
    char ch = (static_cast<std::string*>(value))->at(index);
    std::string new_string(1, ch);
    String ans = new_string;
    return ans;
}

Object* String::clone()
{
    return new String(*static_cast<std::string*>(value));
}
/////////////////////////////////////////////////////



///////////// Array /////////////////////////////
Array::Array() 
    : Object("array", new std::vector<Object*>) 
{}

Array::~Array() {
    auto& elements = *static_cast<std::vector<Object*>*>(value);
    for (auto obj : elements) {
        delete obj;
    }
    delete static_cast<std::vector<Object*>*>(value);
}

Array::Array(std::initializer_list<Object*> init)
    : Object("array", new std::vector<Object*>()) 
{
    auto& elements = *static_cast<std::vector<Object*>*>(value);
    for (auto obj : init) {
        if (obj) {
            elements.push_back(new Object(*obj));
        } else {
            elements.push_back(nullptr);
        }
    }
}

Array::Array(const std::vector<Object*>& vec) 
    : Object("array", new std::vector<Object*>()) 
{
    std::vector<Object*>* elements = static_cast<std::vector<Object*>*>(value);
    for (Object* obj : vec) {
        if (obj) {
            elements->push_back(new Object(*obj));
        } else {
            elements->push_back(nullptr);
        }
    }
}


Array::Array(Object* ptr) 
    : Object("array", nullptr)
{
    if(ptr->name == "array")
    {
        std::vector<Object*>* vec_ptr = new std::vector<Object*>(ptr->__size__(), nullptr);
        for(int i = 0; i < ptr->__size__(); ++i)
        {
            if((ptr->__at__(i)).name == "int")
            {
                (*vec_ptr)[i] = new Int{*static_cast<int*>((ptr->__at__(i)).value)};
            } else if((ptr->__at__(i)).name == "bool")
            {
                (*vec_ptr)[i] = new Bool{*static_cast<bool*>((ptr->__at__(i)).value)};
            } else if((ptr->__at__(i)).name == "double")
            {
                (*vec_ptr)[i] = new Double{*static_cast<double*>((ptr->__at__(i)).value)};
            } else if((ptr->__at__(i)).name == "string")
            {
                (*vec_ptr)[i] = new String{*static_cast<std::string*>((ptr->__at__(i)).value)};
            } else if((ptr->__at__(i)).name == "array")
            {
                (*vec_ptr)[i] = new Array{static_cast<Object*>((ptr->__at__(i)).value)};
            } else 
            {
                throw std::runtime_error("wrong argument");
            }
        }
        value = reinterpret_cast<void*>(vec_ptr);
        vec_ptr = nullptr;
    } else if(ptr->name == "int" || ptr->name == "bool" || ptr->name == "double" || ptr->name == "string")
    {
        std::vector<Object*>* vec_ptr = new std::vector<Object*>(1, ptr);
        value = reinterpret_cast<void*>(vec_ptr);
        vec_ptr = nullptr;
    } else
    {
        throw std::runtime_error("wrong argument");
    }
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

Object Array:: __add__(Object* other) 
{
    if(other->name == "array")
    {
        std::vector<Object*>* new_vector = new std::vector<Object*>;
        std::vector<Object*>* this_vector = static_cast<std::vector<Object*>*>(value);
        std::vector<Object*>* other_vector = static_cast<std::vector<Object*>*>(other->value);

        new_vector->reserve(this_vector->size() + other_vector->size());
        for (auto obj : *this_vector) {
            if(obj->name == "int")
            {
                new_vector->push_back(new Int(obj));
            } else if (obj->name == "bool")
            {
                new_vector->push_back(new Bool(obj));
            } else if (obj->name == "double")
            {
                new_vector->push_back(new Double(obj));
            } else if (obj->name == "string")
            {
                new_vector->push_back(new String(obj));
            } else if (obj->name == "array")
            {
                new_vector->push_back(new Array(obj));
            } else 
            {
                throw std::runtime_error("wrong argument");
            }
        }
        for (auto obj : *other_vector) {
            if(obj->name == "int")
            {
                new_vector->push_back(new Int(obj));
            } else if (obj->name == "bool")
            {
                new_vector->push_back(new Bool(obj));
            } else if (obj->name == "double")
            {
                new_vector->push_back(new Double(obj));
            } else if (obj->name == "string")
            {
                new_vector->push_back(new String(obj));
            } else if (obj->name == "array")
            {
                new_vector->push_back(new Array(obj));
            } else 
            {
                throw std::runtime_error("wrong argument");
            }
        }
        return Array(*new_vector);
    }
    if (other->name == "int" || other->name == "double" || other->name == "bool" || other->name == "string")
    {
        std::vector<Object*>* new_vector = new std::vector<Object*>;
        std::vector<Object*>* this_vector = static_cast<std::vector<Object*>*>(value);

        new_vector->reserve(this_vector->size() + 1);
        for (auto obj : *this_vector) {
            if(obj->name == "int")
            {
                new_vector->push_back(new Int(obj));
            } else if (obj->name == "bool")
            {
                new_vector->push_back(new Bool(obj));
            } else if (obj->name == "double")
            {
                new_vector->push_back(new Double(obj));
            } else if (obj->name == "string")
            {
                new_vector->push_back(new String(obj));
            } else if (obj->name == "array")
            {
                new_vector->push_back(new Array(obj));
            } else 
            {
                throw std::runtime_error("wrong argument");
            }
        }
        if(other->name == "int")
        {
            new_vector->push_back(new Int(other));
        } else if (other->name == "bool")
        {
            new_vector->push_back(new Bool(other));
        } else if (other->name == "double")
        {
            new_vector->push_back(new Double(other));
        } else if (other->name == "string")
        {
            new_vector->push_back(new String(other));
        } else if (other->name == "array")
        {
            new_vector->push_back(new Array(other));
        } else 
        {
            throw std::runtime_error("wrong argument");
        }
        return Array(*new_vector);
    }
    throw std::runtime_error("wrong argument");
}

void Array:: __add_assign__(Object* other) 
{
    if(other->name == "array")
    {
        std::vector<Object*>* this_vector = static_cast<std::vector<Object*>*>(value);
        std::vector<Object*>* other_vector = static_cast<std::vector<Object*>*>(other->value);

        this_vector->reserve(this_vector->size() + other_vector->size());
        for (auto obj : *other_vector) {
            if(obj->name == "int")
            {
                this_vector->push_back(new Int(obj));
            } else if (obj->name == "bool")
            {
                this_vector->push_back(new Bool(obj));
            } else if (obj->name == "double")
            {
                this_vector->push_back(new Double(obj));
            } else if (obj->name == "string")
            {
                this_vector->push_back(new String(obj));
            } else if (obj->name == "array")
            {
                this_vector->push_back(new Array(obj));
            } else 
            {
                throw std::runtime_error("wrong argument");
            }
        }
    }
    if (other->name == "int" || other->name == "double" || other->name == "bool" || other->name == "string")
    {
        std::vector<Object*>* this_vector = static_cast<std::vector<Object*>*>(value);

        this_vector->reserve(this_vector->size() + 1);
        if(other->name == "int")
        {
            this_vector->push_back(new Int(other));
        } else if (other->name == "bool")
        {
            this_vector->push_back(new Bool(other));
        } else if (other->name == "double")
        {
            this_vector->push_back(new Double(other));
        } else if (other->name == "string")
        {
            this_vector->push_back(new String(other));
        } else if (other->name == "array")
        {
            this_vector->push_back(new Array(other));
        } else 
        {
            throw std::runtime_error("wrong argument");
        }
    }
    throw std::runtime_error("wrong argument");
}

void Array:: __pop__() {
    std::vector<Object*>* this_vector = static_cast<std::vector<Object*>*>(value);
    if (this_vector->empty()) 
    {
        throw std::out_of_range("Cannot pop from an empty array");
    }
    delete this_vector->back();
    this_vector->pop_back();
}


int Array::__size__()
{
    std::vector<Object*>* vec = static_cast<std::vector<Object*>*>(value);
    int ans = vec->size();
    vec = nullptr;
    return ans;
}

Object Array::__at__(int index)
{
    std::vector<Object*>* vec = static_cast<std::vector<Object*>*>(value);
    Object ans = *(vec->at(index));
    vec = nullptr;
    return ans;
}

Object* Array::clone()
{
    return new Array(this);
}
//////////////////////////////////////////

#endif