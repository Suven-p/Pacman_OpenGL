#pragma once

#include <project/common.h>
#include <project/sprite.h>
#include <string>
#include <vector>
#include "project/helpers.h"

class PauseMenu : public Sprite {
    // vao for: darkening, white outline, actual menu
    GLuint vao[3]{}, vbo[4]{}, ebo{};
    std::vector<std::string> options;
    std::vector<std::string>::size_type selectedOption;
    int keyboardCallbackID;

   public:
    PauseMenu();
    ~PauseMenu() override;
    void draw(std::string shader) override;
    void show_options();
    void handleKeyboardInput(int key);
    void executeFunction();
};

class MainMenu : public Sprite {
    GLuint vao, vbo[2], ebo;

   public:
    MainMenu();
    void draw(std::string shader) override;
};
