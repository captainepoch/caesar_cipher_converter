#include "ccc_utils.hpp"
#include <iostream>
#include <getopt.h>

void print_usage_msg()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\tccc -s <shift value> -m <encode (e) or decode (d)> <text to decode>" << std::endl;
}

ccc::CCConfig parse_args(int argc, char *argv[])
{
    if (argc <= 1 || argv[0] == nullptr || argv[0][0] == '\0')
    {
        std::cerr << "Empty arguments \n" << std::endl;
        print_usage_msg();
        exit(EXIT_FAILURE);
    }

    int shift = -1;
    ccc::CCConfigModeEnum mode = ccc::none;

    int opt;
    while ((opt = getopt(argc, argv, "s:m:h")) != -1)
    {
        switch (opt)
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

                if (shift > 26)
                {
                    std::cerr << "Shift value bigger than the size of the alphabet: '" << arg << "'\n";
                    exit(EXIT_FAILURE);
                }
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Invalid argument for -s: '" << optarg << "'\n";
                exit(EXIT_FAILURE);
            }

            continue;
        }

        case 'm':
        {
            try
            {
                switch (optarg[0])
                {
                case 'e':
                    mode = ccc::encode;
                    break;
                case 'd':
                    mode = ccc::decode;
                    break;
                default:
                    std::cerr << "Invalid mode: '" << optarg << "'\n";
                    exit(EXIT_FAILURE);
                }
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Invalid argument for -m: '" << optarg << "'\n";
                exit(EXIT_FAILURE);
            }

            continue;
        }

        case 'h':
        case '?':
        default:
        {
            print_usage_msg();
            exit(EXIT_SUCCESS);
        }
        }
    }

    if (shift == -1)
    {
        std::cerr << "Error: -s parameter is required\n";
        exit(EXIT_FAILURE);
    }
    if (mode == ccc::none)
    {
        std::cerr << "Error: -m parameter is required\n";
        exit(EXIT_FAILURE);
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

    if (text.empty())
    {
        std::cerr << "Text cannot be empty" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Original text = \"" << text << "\"\n";

    return ccc::CCConfig{shift, text, mode};
}
