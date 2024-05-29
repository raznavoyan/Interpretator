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
    static std::string typeOf(std::string arg);
    static toks tokenize(const std::string& input);
    static bool isCustomDelimiter(const std::string& c);
    static bool isAKeyword(const std::string& tok);
    static bool isAMath(const std::string& tok);
    static bool isVariableName(const std::string& name);
    static bool isNumber(const std::string& str);
    static bool isBinaryOperator(const std::string& token);
};

#endif
