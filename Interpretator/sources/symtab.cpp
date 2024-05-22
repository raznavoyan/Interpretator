#include "symtab.h"

symtab::symtab() {
  scopeStack.push_back(std::unordered_map<std::string, Object*>());
}

bool symtab::are(const std::string& name) {
  for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
    if (it->find(name) != it->end()) {
      return true;
    }
  }
  return false;
}

bool symtab::setVal(const std::string& name, Object* value) {
  scopeStack.back()[name] = value;
  return true;
}

Object* symtab::getVal(const std::string& name) {
  for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it) {
    if (it->find(name) != it->end()) {
      return it->at(name);
    }
  }
  return nullptr;
}

void symtab::pushSpace() {
  scopeStack.push_back(std::unordered_map<std::string, Object*>());
  ++level;
}

void symtab::popSpace() {
  if (level > 0) {
    scopeStack.pop_back();
    --level;
  }
}
