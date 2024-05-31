#include "parser.h"

std::string parser::typeOf(std::string arg){
    // Regular expressions for each type
    std::regex int_regex(R"(^-?\d+$)");
    std::regex bool_regex(R"(^true$|^false$)");
    std::regex double_regex(R"(^-?\d*\.\d+$)");
    std::regex string_regex(R"(^".*"$)");
    std::regex array_regex(R"(^\[.*\]$)");

    if (std::regex_match(arg, int_regex)) {
        return "i";
    } else if (std::regex_match(arg, bool_regex)) {
        return "b";
    } else if (std::regex_match(arg, double_regex)) {
        return "d";
    } else if (std::regex_match(arg, string_regex)) {
        return "s";
    } else if (std::regex_match(arg, array_regex)) {
        return "a";
    }
    return "non type";
}

bool parser::isAKeyword(const std::string& tok) {
    static const std::unordered_set<std::string> keywords = {
        "if", "otherwise", "loop", "during", "true", "false",
        "disp", "def", "return"
    };
    return keywords.count(tok) > 0;
}

bool parser::isBinaryOperator(const std::string& token) {
    static const std::unordered_set<std::string> binaryOps = {
        "+", "-", "*", "/", "%", "^"
    };

    static const std::unordered_set<std::string> comparisonOps = {
        "==", "!=", "<", ">", "<=", ">="
    };

    static const std::unordered_set<std::string> logicalOps = {
        "&&", "||"
    };

    return binaryOps.count(token) || comparisonOps.count(token) || logicalOps.count(token);
}

bool parser::isCustomDelimiter(const std::string& c) {
    static const std::unordered_set<std::string> delimiters = {
        "|", ":", ";", "{", "}", "=", "+",
        "-", "*", "/", "%", "!", "#", "^"
    };
    return delimiters.count(c) > 0;
}

std::vector<std::string> parser::tokenize(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> tokens;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        char c;

        while (stream.get(c)) {
            std::string currentToken;

            // Handle string literals
            if (c == '"') {
                currentToken += c;
                while (stream.get(c)) {
                    currentToken += c;
                    if (c == '"') break; // end of string literal
                }
                tokens.push_back(currentToken);
            }
            // Handle multi-character operators and delimiters
            else if (parser::isAMath(std::string(1, c)) || parser::isCustomDelimiter(std::string(1, c))) {
                currentToken += c;
                if (stream.peek() != EOF) {
                    char next = stream.peek();
                    std::string potentialOperator = currentToken + next;
                    if (parser::isAMath(potentialOperator)) {
                        stream.get(c); // consume the next character
                        currentToken += c;
                    }
                }
                tokens.push_back(currentToken);
            }
            // Handle alphabetic tokens
            else if (std::isalpha(c)) {
                currentToken += c;
                while (stream.get(c) && std::isalpha(c)) {
                    currentToken += c;
                }
                stream.unget(); // put the non-alphabetic character back to the stream
                tokens.push_back(currentToken);
            }
            // Handle numeric tokens
            else if (std::isdigit(c)) {
                currentToken += c;
                while (stream.get(c) && std::isdigit(c)) {
                    currentToken += c;
                }
                stream.unget(); // put the non-numeric character back to the stream
                tokens.push_back(currentToken);
            }
            // Handle whitespace
            else if (std::isspace(c)) {
                continue; // skip whitespace
            }
            // Handle any other single character
            else {
                tokens.push_back(std::string(1, c));
            }
        }
    }

    file.close();
   
    return tokens;
}



bool parser::isAMath(const std::string& tok) {
    static const std::unordered_set<std::string> mathOps = {
      "+", "-", "/", "!", "%", "*", "--", "==", "++", "->",
      ">", "<", "<=", ">=", "+=", "-=", "/=", "%=", ">>", "<<",
      "&&", "||", "|","&", ">>=", "<<=", "~", "^", "^=", "*=","!="
    };
    return mathOps.count(tok) > 0;
}

bool parser::isAssignment(const std::string& tok) {
    static const std::unordered_set<std::string> assignmentOps = {
      "+=", "-=", "/=", "%=",">>=", "<<=", "^=", "*="
    };
    return assignmentOps.count(tok) > 0;
}

bool parser::isVariableName(const std::string& name) {
  std::regex variable_name_regex(R"([a-zA-Z_][a-zA-Z0-9_]*)");
  return std::regex_match(name, variable_name_regex);
}

bool parser::isNumber(const std::string& str) {
  // std::regex number_regex(R"(\d+)");
  // return std::regex_match(str, number_regex);
  return true;
}
