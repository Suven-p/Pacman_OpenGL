#pragma once

#include <project/common.h>
#include <project/sprite.h>
#include <string>
#include <vector>
#include "project/helpers.h"

class BorderedMenu : public Sprite {
    // vao for: darkening, white outline, actual menu
    GLuint vao[3]{}, vbo[4]{}, ebo{};
    std::string title;
    bool isActive;
    std::vector<std::string> optionNames;
    std::map<std::string, std::function<void(void)>> optionCallbacks;
    std::vector<std::string>::size_type selectedOption;
    int keyboardCallbackID;

   public:
    BorderedMenu(std::string title,
                 std::vector<std::string> names,
                 std::map<std::string, std::function<void(void)>> callbacks);
    ~BorderedMenu() override;
    void draw(std::string shader) override;
    void show_options();
    void handleKeyboardInput(int key);
    void executeFunction();
    void activate(bool activate = true);
};

class MainMenu : public Sprite {
    GLuint vao, vbo[2], ebo;

   public:
    MainMenu();
    void draw(std::string shader) override;
};

class ReadyScreen : public Sprite {
   public:
    void draw(std::string shader) override;
};
