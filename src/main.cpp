#include <iostream>
#include <sstream>

std::string hispanicize(const std::string &input)
{
    std::stringstream ss;
    size_t start_sentence = 0;
    bool search_start = true;

    for (size_t i = 0; i < input.size(); i++)
    {
        char c = input[i];
        if (c == '.' || c == '?' || c == '!')
        {
            if (!search_start)
            {
                if (c == '?')
                {
                    ss << "¿";
                }
                else if (c == '!')
                {
                    ss << "¡";
                }
                ss << input.substr(start_sentence, i - start_sentence + 1);
            }
            search_start = true;
        }
        else if ((c == ' ' || c == '\n' || c == '\r' || c == '\t') && search_start)
        {
            start_sentence++;
            ss << c;
        }
        else if (search_start)
        {
            search_start = false;
            start_sentence = i;
        }
    }

    return ss.str();
}

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        std::cout << hispanicize(argv[1]) << std::endl;
    }
    else if(argc == 1)
    {
        std::stringstream buffer;
        buffer << std::cin.rdbuf();
        std::cout << hispanicize(buffer.str()) << std::endl;
    }
    else
    {
        std::cout << "Wrong number of arguments, provide either the text as first argument or zero to use the standart input." << std::endl;
        std::cout << "Usage: " << argv[0] << " (<text>)" << std::endl;
    }
}