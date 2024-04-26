#include <iostream>
#include <fstream>
#include <string>




int
main(int argc, char* argv[])
{
    std::istream* input;  // Pointer to input stream (either ifstream or std::cin)

    // Check if a filename is provided as a command-line argument
    if (argc > 1) {
        // Open the file specified in the command-line argument
        std::ifstream* fileInput = new std::ifstream(argv[1]);
        if (!fileInput->is_open()) {
            std::cerr << "Error opening file: " << argv[1] << '\n';
            delete fileInput;
            return 1;
        }
        input = fileInput;  // Use ifstream for input
    } else {
        // Use std::cin for input
        input = &std::cin;
    }

    

    // Read input line by line from the selected input stream
    std::string line;
    while (std::getline(*input, line)) {
        std::cout << "Read: " << line << '\n';
    }

    // Clean up: close file stream if opened
    if (argc > 1) {
        static_cast<std::ifstream*>(input)->close();
        delete input;  // Clean up dynamically allocated ifstream
    }

    return 0;
}