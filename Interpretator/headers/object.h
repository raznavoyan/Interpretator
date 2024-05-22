#ifndef __OBJECT_H_
#define __OBJECT_H_

#include <string>
#include <stdexcept>
#include <initializer_list>
#include <vector>


//add copy assigments
//add cast opertaions from derivred to Object
struct Object 
{
    std::string name;
    void* value;

    Object(std::string n, void* v);
    virtual ~Object();

    virtual std::string __str__();

    virtual Object __add__(Object*);
    virtual Object __sub__(Object*);
    virtual Object __mod__(Object*);
    virtual Object __mul__(Object*);
    virtual Object __div__(Object*);

    virtual Object __or__(Object*);
    virtual Object __and__(Object*);
    virtual Object __den__(); // jxtum !

    virtual Object __bit_or__(Object*);
    virtual Object __bit_and__(Object*);
    virtual Object __xor__(Object*);
    virtual Object __bit_den__();
    virtual Object __left_shift__(Object*);
    virtual Object __right_shift__(Object*);

    virtual void __add_assign__(Object*);
    virtual void __sub_assign__(Object*);
    virtual void __mod_assign__(Object*);
    virtual void __mul_assign__(Object*);
    virtual void __div_assign__(Object*);
    virtual void __xor_assign__(Object*);
    virtual void __lshift_assign__(Object*);
    virtual void __rshift_assign__(Object*);

    virtual bool __more__(Object*);
    virtual bool __less__(Object*);
    virtual bool __more_equal__(Object*);
    virtual bool __less_equal__(Object*);
    virtual bool __equal__(Object*);
    virtual bool __not_equal__(Object*);

    virtual void __pop__(); // for string and array push is the same as add(use add)
    virtual int __size__();
    virtual Object __at__(int);
    /*
    virtual Object* __call__() 
    {
        throw std::invalid_argument(name + " does not have an overloaded call operator");
    }
    */
};

struct Int : public Object 
{
    Int();
    Int(Object* ptr);
    Int(int);
    ~Int();

    std::string __str__() override;
    Object __add__(Object* other) override;
    Object __sub__(Object* other) override;
    Object __mod__(Object* other) override;
    Object __mul__(Object* other) override;
    Object __div__(Object* other) override;

    Object __or__(Object* other) override;
    Object __and__(Object* other) override;
    Object __den__() override;

    Object __bit_or__(Object* other) override;
    Object __bit_and__(Object* other) override;
    Object __xor__(Object* other) override;
    Object __bit_den__() override;
    Object __left_shift__(Object* other) override;
    Object __right_shift__(Object* other) override;

    void __add_assign__(Object*) override;
    void __sub_assign__(Object*) override;
    void __mod_assign__(Object*) override;
    void __mul_assign__(Object*) override;
    void __div_assign__(Object*) override;
    void __xor_assign__(Object*) override;
    void __lshift_assign__(Object*) override;
    void __rshift_assign__(Object*) override;

    bool __more__(Object*) override;
    bool __less__(Object*) override;
    bool __more_equal__(Object*) override;
    bool __less_equal__(Object*) override;
    bool __equal__(Object*) override;
    bool __not_equal__(Object*) override;
};

struct Double : public Object 
{
    Double();
    Double(Object* ptr);
    Double(double);
    ~Double();

    std::string __str__() override;
    Object __add__(Object* other) override;
    Object __sub__(Object* other) override;
    Object __mul__(Object* other) override;
    Object __div__(Object* other) override;

    void __add_assign__(Object*) override;
    void __sub_assign__(Object*) override;
    void __mul_assign__(Object*) override;
    void __div_assign__(Object*) override;

    bool __more__(Object*) override;
    bool __less__(Object*) override;
    bool __more_equal__(Object*) override;
    bool __less_equal__(Object*) override;
    bool __equal__(Object*) override;
    bool __not_equal__(Object*) override;
};

struct Bool : public Object {
    Bool();
    Bool(Object* ptr);
    Bool(bool);
    ~Bool();

    std::string __str__() override ;
    Object __add__(Object* other) override;
    Object __sub__(Object* other) override;

    Object __or__(Object*) override;
    Object __and__(Object*) override;
    Object __den__() override;

    bool __more__(Object*) override;
    bool __less__(Object*) override;
    bool __more_equal__(Object*) override;
    bool __less_equal__(Object*) override;
    bool __equal__(Object*) override;
    bool __not_equal__(Object*) override;
};

struct String : public Object 
{
    String();
    String(Object* ptr);
    String(std::string);
    ~String();

    std::string __str__() override;
    Object __add__(Object* other) override;

    void __add_assign__(Object*) override;

    bool __more__(Object*) override;
    bool __less__(Object*) override;
    bool __more_equal__(Object*) override;
    bool __less_equal__(Object*) override;
    bool __equal__(Object*) override;
    bool __not_equal__(Object*) override;

    void __pop__() override;
    int __size__() override;
    Object __at__(int) override;
};

struct Array : public Object 
{
    Array();
    Array(Object* ptr);
    Array(std::initializer_list<Object*>);
    Array(const std::vector<Object*>&);
    ~Array();

    std::string __str__() override;

    Object __add__(Object* other) override;
    void __add_assign__(Object*) override;

    void __pop__() override;
    int __size__() override;
    Object __at__(int) override;
};

#endif