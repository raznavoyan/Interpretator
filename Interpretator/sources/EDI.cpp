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
#include <unistd.h>

using toks = std::vector<std::string>;

int main(int argc, char* argv[]) 
{
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
    // for(auto& it : tokens){
    //     std::cout << it << std::endl;
    // }
    Interpreter runInterpretator(tokens);

    
    std::cout << "End of program!" << std::endl;
    return 0;
}