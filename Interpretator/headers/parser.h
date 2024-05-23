#ifndef __PARSER__
#define __PARSER__

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <regex>
#include "object.h"

class parser {
public:
    using toks = std::vector<std::string>;
    char typeOf(std::string arg); //string->s,bool->b,...
    toks tokenize(const std::string& input);
    bool isCustomDelimiter(char c);
    bool isAKeyword(const std::string& tok);
    bool isAMath(const std::string& tok);
    bool isVariableName(const std::string& name);
    bool isNumber(const std::string& str);
public:
};

#endif
