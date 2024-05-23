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

Object& symtab::setVal(const std::string& varName, std::string value) {
  auto it = scopeStack.back().find(varname);
  if (it != scopeStack.back().end()) {
    // Variable already exists in the current scope
    if (it->second->name() == value->name()) {
      // Types are the same, so just update the value
      it->second->setValue(value->getValue());
      return *it->second;
    } else {
      // Types are different, so decrement the reference count of the existing object and create a new object with the new type
      //\\//\\//\\//\\//it->second->decRefCount();
      return *value;
    }
  } else {
    // Variable does not exist in the current scope, so add it
    scopeStack.back()[varName] = value;
    return *value;
  }
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
