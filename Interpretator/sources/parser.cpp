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

bool parser::isCustomDelimiter(char c) {
  static const std::unordered_set<char> delimiters = {
      '|', ':', ';', '{', '}', '=', '+',
      '-', '*', '/', '%', '!', '#', '^'
  };
  return delimiters.count(c) > 0;
}

parser::toks parser::tokenize(const std::string& input) {
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

bool parser::isAKeyword(const std::string& tok) {
  static const std::unordered_set<std::string> keywords = {
      "if", "otherwise", "loop", "during"
  };
  return keywords.count(tok) > 0;
}

bool parser::isAMath(const std::string& tok) {
    static const std::unordered_set<std::string> mathOps = {
      "+", "-", "/", "!", "%", "*", "--", "==", "++", "->",
      ">", "<", "<=", ">=", "+=", "-=", "/=", "%=", ">>", "<<",
      "&&", "||", "|","&", ">>=", "<<=", "~", "^", "^=", "*=","!="
    };
    return mathOps.count(tok) > 0;
}

bool parser::isVariableName(const std::string& name) {
  std::regex variable_name_regex(R"([a-zA-Z_][a-zA-Z0-9_]*)");
  return std::regex_match(name, variable_name_regex);
}

bool parser::isNumber(const std::string& str) {
  std::regex number_regex(R"(\d+)");
  return std::regex_match(str, number_regex);
}
