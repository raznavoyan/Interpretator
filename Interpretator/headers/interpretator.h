#ifndef ___INTERPRETATOR__
#define ___INTERPRETATOR__


#include "object.h"
#include "symtab.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <chrono>

struct Function : Object {
  
};

struct Interpreter
{   
    std::vector<std::vector<std::string>> parsed_code;

    std::unordered_set<Object*> objs;


    Object* eval(std::string) {
    
    }

    Object* exec(Array) {
    
    }

    bool validVariableName(std::string var) {
    
    }

    void garbageColector() {
      for (auto ptr : objs) {
        if (ptr->count <= 0) {
          delete ptr;
          objs.remove(ptr);
        }
      }
    }

    void run() {
      for (auto& line : parsed_code) {
          auto cur = time();
          if (cur - last > 20second) {
            garbageColector();
            last = cur;
          }


      
        if (line.size() >= 3 && line[1] == "=") {
          if (validVariable(line[0])) {
            vars[line[0]] = exec();
          }
        }
      }
    }
};




#endif