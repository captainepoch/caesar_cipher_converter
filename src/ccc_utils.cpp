#include "ccc_utils.hpp"
#include <iostream>
#include <memory>
#include "ccc.hpp"
#include <getopt.h>

void print_usage_msg()
{
    std::cout << "Usage (cli):" << std::endl;
    std::cout << "\tccc -s <shift value> -m <encode (e) or decode (d)> <text to encode/decode>" << std::endl;
    std::cout << "Usage (gui):" << std::endl;
    std::cout << "\tccc -g" << std::endl;
}

ccc::CCConfig parse_args(int argc, char *argv[], bool *is_gui_mode)
{
    if (argc <= 1 || argv[0] == nullptr || argv[0][0] == '\0')
    {
        std::cerr << "Error: Empty arguments \n" << std::endl;
        print_usage_msg();
        exit(EXIT_FAILURE);
    }

    int shift = -1;
    ccc::CCConfigModeEnum mode = ccc::none;

    int opt;
    while ((opt = getopt(argc, argv, "s:m:gh")) != -1)
    {
        switch (opt)
        {
        case 'g':
        {
            *is_gui_mode = true;
            break;
        }
        case 's':
        {
            try
            {
                std::string arg = optarg;
                size_t idx = 0;
                shift = std::stoi(arg, &idx, 10);
                if (idx != arg.size())
                {
                    std::cerr << "Error: Invalid argument for -s: '" << arg << "'\n";
                    exit(EXIT_FAILURE);
                }

                if (shift > 26)
                {
                    std::cerr << "Error: Shift value bigger than the size of the alphabet: '" << arg << "'\n";
                    exit(EXIT_FAILURE);
                }
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Error: Invalid argument for -s: '" << optarg << "'\n";
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
                    std::cerr << "Error: Invalid mode: '" << optarg << "'\n";
                    exit(EXIT_FAILURE);
                }
            }
            catch (const std::invalid_argument &)
            {
                std::cerr << "Error: Invalid argument for -m: '" << optarg << "'\n";
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

    std::string text;
    if (!(*is_gui_mode))
    {
        if (shift == -1)
        {
            std::cerr << "Error: Error: -s parameter is required\n";
            exit(EXIT_FAILURE);
        }
        if (mode == ccc::none)
        {
            std::cerr << "Error: Error: -m parameter is required\n";
            exit(EXIT_FAILURE);
        }

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
    }

    return ccc::CCConfig{shift, text, mode};
}
