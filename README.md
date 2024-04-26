Integral Data Types:
int: Represents a 4-byte integer.
double: Represents an 8-byte floating-point number.
string: Represents an 8-byte string (assuming it's stored similar to std::string in C++).
char: Represents a 1-byte character.
Variable Storage:
All variables will be stored using void* in a symbol table map, suggesting a generic pointer-based storage mechanism.
Variable Declaration and Initialization:
Variables can be declared and initialized with values of different types, allowing dynamic typing or type inference.
Example:
\\
val a = 9
a = "hello"
\\
This implies that a starts as an integer (9) and is later reassigned to a string ("hello"), indicating support for dynamic typing.
Variable Naming Rules:
Variable names (NAME) cannot be keywords.
Names must contain only Latin characters and numbers.
Names must start with a Latin character.
Line End Mark:
End of a line is marked with '' (an empty character?).
Operators:
Basic arithmetic operators (+, -, *, /).
Array access operator ([]).
Keywords:
It's unclear if all correct keywords are defined, but keywords typically include reserved words like val (for variable declaration), data type names (int, double, string, char), and potentially control flow keywords (if, else, while, for, etc.).
Comments and Questions:
It seems you're seeking validation or feedback on the correctness and completeness of your language specifications.
You might need to clarify certain aspects, such as the intended behavior of line ending markers ('') and additional keywords or operators.
To proceed with implementing this custom interpreted language for your project, you'll need to:

Define a grammar and syntax based on the outlined specifications.
Develop a parser and interpreter that can handle variable declarations, assignments, expressions, and control flow based on the defined rules.
Test your interpreter with sample programs to ensure it behaves as expected according to your language specifications.
If you have specific questions or need assistance with any part of this project, feel free to ask for further clarification or guidance!
