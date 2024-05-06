#ifndef __PARSER__
#define __PARSER__

#include<string>
#include<vector>

class parser
{
private:
    std::vector<std::string> tokenTree;

public:
    std::vector<std::string> tokenise(std::string line);

};


#endif