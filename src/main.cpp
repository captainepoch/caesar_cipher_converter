#include "ccc.hpp"
#include <climits>
#include <cstdlib>
#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>

void print_usage_msg();
ccc::CCConfig parse_args(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // OpenGL 3.2 Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    const int window_size = 720;

    SDL_Window *window = SDL_CreateWindow(
        "Caesar Cipher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_size, window_size, SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    // Enable VSync
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init("#version 150");

    // InputTextMultiline variables
    std::string unencoded_text = "";
    std::string encoded_text = "";
    // Encode/Decode mode
    bool is_decode = false;
    // Encode/Decode button
    bool has_clicked = false;

    // Callback to resize the text buffer automatically
    auto InputTextCallback = [](ImGuiInputTextCallbackData *data) -> int
    {
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            std::string *str = (std::string *)data->UserData;
            str->resize(data->BufTextLen);
            data->Buf = (char *)str->c_str();
        }
        return 0;
    };

    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
            {
                done = true;
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(window_size, window_size));

        ImGui::Begin(
            "##caesar_input",
            nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse);

        ImGui::Text("Unencoded text");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputTextMultiline(
            "##unencoded",
            (char *)unencoded_text.c_str(),
            unencoded_text.capacity() + 1,
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 5),
            ImGuiInputTextFlags_CallbackResize,
            InputTextCallback,
            (void *)&unencoded_text);

        ImGui::Text("Encoded text");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputTextMultiline(
            "##encoded",
            (char *)encoded_text.c_str(),
            encoded_text.capacity() + 1,
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 5),
            ImGuiInputTextFlags_CallbackResize,
            InputTextCallback,
            (void *)&encoded_text);

        ImGui::Checkbox("Decode", &is_decode);
        ImGui::SameLine();
        if (ImGui::Button("Execute"))
        {
            has_clicked = !has_clicked;
        }
        if (has_clicked)
        {
            ImGui::SameLine();
            ImGui::Text("Thanks for clicking me!");
        }

        ImGui::End();

        ImGui::Render();

        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    /*ccc::CCConfig config = parse_args(argc, argv);

    ccc::CaesarCipher caesar_cipher{config};
    std::cout << caesar_cipher.get_caesar_text() << std::endl;

    return 0;*/
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

void print_usage_msg()
{
    std::cout << "Usage:" << std::endl;
    std::cout << "\tccc -s <shift value> -m <encode (e) or decode (d)> <text to decode>" << std::endl;
}
