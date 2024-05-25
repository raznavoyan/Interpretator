#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include "parser.h"
#include "object.h"
#include "interpretator.h"
#include "symtab.h"

using toks = std::vector<std::string>;

int 
main(int argc, char* argv[]) 
{
    // // Check if a filename argument is provided
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    //     return 1;
    // }

    // // Get the filename from command-line argument
    // std::string filename = argv[1];

    // // // Open the file
    // std::ifstream file(filename);
    // if (!file.is_open()) {
    //     std::cerr << "Error: Could not open file " << filename << std::endl;
    //     return 1;
    // }

    // // Read file contents into a single string
    // std::ostringstream oss;
    // oss << file.rdbuf();
    // std::string file_contents = oss.str();
    // Tokenize file contents based on custom delimiters and operators
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::vector<std::string> tokens;
    std::string filename = argv[1];
    try {
        tokens = parser::tokenize(filename);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    for(auto& it : tokens){
        std::cout << it << std::endl;
    }
    Interpreter runInterpretator(tokens);


    std::cout << "End of program!" << std::endl;
    return 0;
}
