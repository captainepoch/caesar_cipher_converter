#include "ccc.hpp"
#include <getopt.h>
#include <unistd.h>
#include <climits>
#include <iostream>

void print_usage_msg();
ccc::CCConfig parse_args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    ccc::CCConfig config = parse_args(argc, argv);

    ccc::CaesarCipher cc;
    return 0;
}

ccc::CCConfig parse_args(int argc, char *argv[])
{
    if (argc <= 1 || argv[0] == nullptr || argv[0][0] == '\0')
    {
        std::cout << "Empty arguments \n" << std::endl;
        print_usage_msg();
        exit(EXIT_FAILURE);
    }

    char *endptr = NULL;
    int shift = 0;

    for (;;)
    {
        switch (getopt(argc, argv, "s:h"))
        {
        case 's':
        {
            try
            {
                std::string arg = optarg;
                size_t idx = 0;
                shift = std::stoi(arg, &idx, 10);
                if (idx != arg.size())
                {
                    std::cerr << "Invalid argument for -s: '" << arg << "'\n";
                    exit(EXIT_FAILURE);
                }

                std::cout << "Value for -s: " << shift << "\n";
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Invalid argument for -s: '" << optarg << "'\n";
                exit(EXIT_FAILURE);
            }

            continue;
        }

        case '?':
        case 'h':
        default:
            print_usage_msg();
            exit(EXIT_SUCCESS);

        case -1:
            break;
        }

        break;
    }

    std::string text;
    for (int i = optind; i < argc; ++i)
    {
        if (i > optind)
        {
            text += ' ';
        }
        text += argv[i];
    }

    std::cout << "text = \"" << text << "\"\n";

    if (text.empty())
    {
        std::cout << "Text cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }

    return ccc::CCConfig{shift, text};
}

void print_usage_msg()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\tccc -s <shift value> <text to decode>" << std::endl;
}
