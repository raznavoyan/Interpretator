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
    // Define Function specific members and methods here if needed
};

struct Interpreter {
    std::vector<std::vector<std::string>> parsed_code;
    std::unordered_set<Object*> objs;
    std::unordered_map<std::string, Object*> vars;
    std::chrono::steady_clock::time_point last;

    Interpreter() : last(std::chrono::steady_clock::now()) {}

    Object* eval(std::string expr) {
        // Implement your evaluation logic here
        return nullptr;
    }

    Object* exec(Array array) {
        // Implement your execution logic here
        return nullptr;
    }

    bool validVariableName(const std::string& var) {
        // Implement your variable name validation logic here
        return true;
    }

    void garbageCollector() {
        for (auto it = objs.begin(); it != objs.end();) {
            if ((*it)->cnt <= 0) {
                delete *it;
                it = objs.erase(it);
            } else {
                ++it;
            }
        }
    }

    void run() {
      for (auto& line : parsed_code) {
            auto cur = std::chrono::steady_clock::now();
            if (std::chrono::duration_cast<std::chrono::seconds>(cur - last).count() > 20) {
                garbageCollector();
            last = cur;
          }

        if (line.size() >= 3 && line[1] == "=") {
                if (validVariableName(line[0])) {
                    vars[line[0]] = exec(Array());
          }
        }
      }
    }
};

#endif