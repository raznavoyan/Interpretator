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

class parser {
public:
    using toks = std::vector<std::string>;

    // Function to check if a character is a custom delimiter
    bool isCustomDelimiter(char c) {
        static const std::unordered_set<char> delimiters = {
            '|', ':', ';', '{', '}', '=', '+', '-',
            '*', '/', '%', '!', '#', '^'
        };
        return delimiters.count(c) > 0;
    }

    // Function to tokenize a string based on custom delimiters and operators
    toks tokenize(const std::string& input) {
        toks tokens;
        std::istringstream iss(input);
        std::string line;
        while (std::getline(iss, line)) {
            size_t pos = 0;
            while (pos < line.length()) {
                if (isCustomDelimiter(line[pos])) {
                    // Check for multi-character operators like "==", "+=", "-="
                    if (pos + 1 < line.length() && isCustomDelimiter(line[pos + 1])) {
                        std::string op = line.substr(pos, 2);
                        tokens.push_back(op);
                        pos += 2; // Move past the operator
                    } else {
                        tokens.push_back(std::string(1, line[pos])); // Push single-character delimiter/operator
                        ++pos;
                    }
                } else if (!isspace(line[pos])) {
                    // Read alphanumeric tokens or literals until next delimiter or whitespace
                    size_t start = pos;
                    while (pos < line.length() && !isCustomDelimiter(line[pos]) && !isspace(line[pos])) {
                        ++pos;
                    }
                    tokens.push_back(line.substr(start, pos - start));
                } else {
                    ++pos;
                }
            }
        }
        return tokens;
    }
    bool isAKeyword(const std::string& tok) {
        static const std::unordered_set<std::string> keywords = {
            "if", "otherwise", "true", "false", "loop",
            "during", "."
        };
        return keywords.count(tok) > 0;
    }

    bool isAMath(const std::string& tok) {
        static const std::unordered_set<std::string> mathOps = {
            "+", "-", "/", "!", "%", "*", "--", "==", "++", "->",
            ">", "<", "<=", ">=", "+=", "-=", "/=", "%="
        };
        return mathOps.count(tok) > 0;
    }

    bool isVariableName(const std::string& name) {
        std::regex variable_name_regex(R"([a-zA-Z_][a-zA-Z0-9_]*)");
        return std::regex_match(name, variable_name_regex);
    }

    bool isNumber(const std::string& str) {
        std::regex number_regex(R"(\d+)");
        return std::regex_match(str, number_regex);
    }

    bool isValid(const toks& tokens) {
        std::vector<std::string> stack;

        for (size_t i = 0; i < tokens.size(); ++i) {
            const std::string& token = tokens[i];

            if (token == "if" || token == "otherwise" || token == "during" || token == "loop") {
                stack.push_back(token);
            } else if (token == "{") {
                if (stack.empty()) {
                    return false;
                }
                stack.push_back(token);
            } else if (token == "}") {
                if (stack.empty() || stack.back() != "{") {
                    return false;
                }
                stack.pop_back(); // Remove '{'
                stack.pop_back(); // Remove corresponding control structure token
            } else if (token == "=") {
                if (i == 0 || i == tokens.size() - 1 || !isVariableName(tokens[i - 1]) || (!isVariableName(tokens[i + 1]) && !isNumber(tokens[i + 1]))) {
                    return false;
                }
            } else if (token == ";") {
                continue;
            } else if (!isVariableName(token) && !isNumber(token) && !isAKeyword(token) && !isAMath(token)) {
                return false;
            }
        }

        return stack.empty();
    }
};


#endif

