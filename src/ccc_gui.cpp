#include "ccc.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>

int execute_gui()
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

    SDL_Window *window =
        SDL_CreateWindow("Caesar Cipher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 220, SDL_WINDOW_OPENGL);
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

    // Encoded/Decoded text
    std::string unencoded_text = "";
    std::string encoded_text = "";
    // Shift value
    int shift = 0;
    // Encode/Decode mode
    bool is_encode = true;
    // Encode/Decode button
    bool has_clicked = false;

    // Caesar Cipher Config
    ccc::CCConfig config{shift, unencoded_text, ccc::encode};
    ccc::CaesarCipher caesar_cipher{&config};

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
        ImGui::SetNextWindowSize(io.DisplaySize);

        ImGui::Begin(
            "##caesar_input",
            nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoCollapse);

        ImGui::BeginDisabled(!is_encode);

        ImGui::Text("Unencoded text");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputTextMultiline(
            "##unencoded",
            (char *)unencoded_text.c_str(),
            unencoded_text.capacity() + 1,
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 5),
            ImGuiInputTextFlags_CallbackResize,
            InputTextCallback,
            &unencoded_text);

        ImGui::EndDisabled();

        ImGui::Spacing();

        ImGui::BeginDisabled(is_encode);

        ImGui::Text("Encoded text");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputTextMultiline(
            "##encoded",
            (char *)encoded_text.c_str(),
            encoded_text.capacity() + 1,
            ImVec2(-1.0f, ImGui::GetTextLineHeight() * 5),
            ImGuiInputTextFlags_CallbackResize,
            InputTextCallback,
            &encoded_text);

        ImGui::EndDisabled();

        ImGui::Spacing();

        if (ImGui::DragInt("Caesar Shift", &shift, 1.0f, 0, 25, "Shift: %d"))
        {
            config.set_shift_value(shift);
        }

        ImGui::SameLine();

        ImGui::Checkbox("Encode", &is_encode);
        if (is_encode)
        {
            config.set_mode(ccc::decode);
        }
        else
        {
            config.set_mode(ccc::encode);
        }
        ImGui::SameLine();

        if (ImGui::Button("Execute"))
        {
            if (is_encode)
            {
                config.set_text(unencoded_text);
                encoded_text = caesar_cipher.get_caesar_text();
            }
            else
            {
                config.set_text(encoded_text);
                unencoded_text = caesar_cipher.get_caesar_text();
            }
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
}
