#ifndef __OBJECT_H_
#define __OBJECT_H_

#include <string>
#include <stdexcept>
#include <initializer_list>
#include <vector>

struct Object 
{
    std::string name;
    void* value;
    int count;

    Object(std::string n, void* v);
    Object(Object&&);
    Object(const Object&);
    virtual ~Object();

    const Object& operator=(const Object&);
    const Object& operator=(Object&&);

    virtual std::string __str__();

    virtual Object* __add__(Object*);
    virtual Object* __sub__(Object*);
    virtual Object* __mod__(Object*);
    virtual Object* __mul__(Object*);
    virtual Object* __div__(Object*);
    virtual Object* __neg__();

    virtual Object* __or__(Object*);
    virtual Object* __and__(Object*);
    virtual Object* __logical_not__(); // jxtum !

    virtual Object* __bit_or__(Object*);
    virtual Object* __bit_and__(Object*);
    virtual Object* __xor__(Object*);
    virtual Object* __complement__();
    virtual Object* __left_shift__(Object*);
    virtual Object* __right_shift__(Object*);

    virtual void __add_assign__(Object*);
    virtual void __sub_assign__(Object*);
    virtual void __mod_assign__(Object*);
    virtual void __mul_assign__(Object*);
    virtual void __div_assign__(Object*);
    virtual void __xor_assign__(Object*);
    virtual void __lshift_assign__(Object*);
    virtual void __rshift_assign__(Object*);

    virtual Object* __more__(Object*);
    virtual Object* __less__(Object*);
    virtual Object* __more_equal__(Object*);
    virtual Object* __less_equal__(Object*);
    virtual Object* __equal__(Object*);
    virtual Object* __not_equal__(Object*);

    virtual void __pop__(); // for string and array push is the same as add(use add)
    virtual int __size__(); // for arrays and strings
    virtual Object* __at__(int); // for arrays and strings

    virtual Object* __call__(Object*); // for function

    private:
    void clear();
    virtual Object* clone();
};

struct Function : public Object
{
    std::vector<std::string>* arg_names;

    public:
    Function(std::vector<std::string>*, std::vector<std::string>*);

    std::string __str__() override;
    Object* __call__(Object*) override;
};

struct Int : public Object 
{
    Int();
    Int(Object* ptr);
    Int(int);
    ~Int();

    std::string __str__() override;
    Object* __add__(Object* other) override;
    Object* __sub__(Object* other) override;
    Object* __mod__(Object* other) override;
    Object* __mul__(Object* other) override;
    Object* __div__(Object* other) override;
    Object* __neg__() override;

    Object* __or__(Object* other) override;
    Object* __and__(Object* other) override;
    Object* __logical_not__() override;

    Object* __bit_or__(Object* other) override;
    Object* __bit_and__(Object* other) override;
    Object* __xor__(Object* other) override;
    Object* __complement__() override;
    Object* __left_shift__(Object* other) override;
    Object* __right_shift__(Object* other) override;

    void __add_assign__(Object*) override;
    void __sub_assign__(Object*) override;
    void __mod_assign__(Object*) override;
    void __mul_assign__(Object*) override;
    void __div_assign__(Object*) override;
    void __xor_assign__(Object*) override;
    void __lshift_assign__(Object*) override;
    void __rshift_assign__(Object*) override;

    Object* __more__(Object*) override;
    Object* __less__(Object*) override;
    Object* __more_equal__(Object*) override;
    Object* __less_equal__(Object*) override;
    Object* __equal__(Object*) override;
    Object* __not_equal__(Object*) override;

    private:
    Object* clone() override;
};

struct Double : public Object 
{
    Double();
    Double(Object* ptr);
    Double(double);
    ~Double();

    std::string __str__() override;
    Object* __add__(Object* other) override;
    Object* __sub__(Object* other) override;
    Object* __mul__(Object* other) override;
    Object* __div__(Object* other) override;
    Object* __neg__() override;

    void __add_assign__(Object*) override;
    void __sub_assign__(Object*) override;
    void __mul_assign__(Object*) override;
    void __div_assign__(Object*) override;

    Object* __more__(Object*) override;
    Object* __less__(Object*) override;
    Object* __more_equal__(Object*) override;
    Object* __less_equal__(Object*) override;
    Object* __equal__(Object*) override;
    Object* __not_equal__(Object*) override;

    private:
    Object* clone() override;
};

struct Bool : public Object {
    Bool();
    Bool(Object* ptr);
    Bool(bool);
    ~Bool();

    std::string __str__() override ;
    Object* __add__(Object* other) override;
    Object* __sub__(Object* other) override;

    Object* __or__(Object*) override;
    Object* __and__(Object*) override;
    Object* __logical_not__() override;

    Object* __more__(Object*) override;
    Object* __less__(Object*) override;
    Object* __more_equal__(Object*) override;
    Object* __less_equal__(Object*) override;
    Object* __equal__(Object*) override;
    Object* __not_equal__(Object*) override;

    private:
    Object* clone() override;
};

struct String : public Object 
{
    String();
    String(Object* ptr);
    String(std::string);
    ~String();

    std::string __str__() override;
    Object* __add__(Object* other) override;

    void __add_assign__(Object*) override;

    Object* __more__(Object*) override;
    Object* __less__(Object*) override;
    Object* __more_equal__(Object*) override;
    Object* __less_equal__(Object*) override;
    Object* __equal__(Object*) override;
    Object* __not_equal__(Object*) override;

    void __pop__() override;
    int __size__() override;
    Object* __at__(int) override;

    private:
    Object* clone() override;
};

struct Array : public Object 
{
    Array();
    Array(Object* ptr);
    Array(std::initializer_list<Object*>);
    Array(const std::vector<Object*>&);
    ~Array();

    std::string __str__() override;

    Object* __add__(Object* other) override;
    void __add_assign__(Object*) override;

    void __pop__() override;
    int __size__() override;
    Object* __at__(int) override;

    private:
    Object* clone() override;
};

#endif