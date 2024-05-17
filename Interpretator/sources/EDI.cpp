#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

using toks = std::vector<std::string>;

// Function to check if a character is a custom delimiter
bool isCustomDelimiter(char c) {
    static const std::unordered_set<char> delimiters = {
        '|', ':', ';', '{', '}', '=', '+', '-',
         '*', '/', '%', '!', '#', '^'};
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
            } else if(!isspace(line[pos])){
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

bool isAKeyword(std::string tok){
    static const std::unordered_set<std::string> keywards = {
        "if" , "otherwise", "true", "false", "loop",
        "during" , "."
    };
    return keywards.count(tok) > 0;
}

bool isAMath(std::string tok){
    static const std::unordered_set<std::string> keywards = {
        "+" , "-" , "/", "!", "%", "*", "--","==","++", "->",
        ">" , "<" , "<=", ">=", "+=", "-=", "/=", "%="
    };
    return keywards.count(tok) > 0;
}

bool isValid(std::vector<std::vector<std::string>>& code){
    bool indicator = true;

    for(auto cur : code){
        
    }


    return indicator;
}


int main(int argc, char* argv[]) {
    // Check if a filename argument is provided
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // Get the filename from command-line argument
    std::string filename = argv[1];

    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    // Read file contents into a single string
    std::ostringstream oss;
    oss << file.rdbuf();
    std::string file_contents = oss.str();

    // Tokenize file contents based on custom delimiters and operators
    toks tokens = tokenize(file_contents);

    if(!isVaid(tokens)){
        std::cerr <<  "Error" << std::endl;
    }

    // Display tokens (for demonstration)
    std::cout << "Tokens:" << std::endl;
    for (const auto& token : tokens) {
        std::cout << token << std::endl;
    }

    return 0;
}
