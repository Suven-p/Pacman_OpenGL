#include <project/game.h>
#include <project/gameLogic.h>
#include <project/ghost.h>
#include <project/helpers.h>
#include <project/map.h>
#include <project/pacman.h>
#include <project/pellet.h>
#include <project/resourceManager.h>
#include <project/text_renderer.h>
#include <spdlog/spdlog.h>
#include <memory>
#include <project/menu.hpp>
#include "GLFW/glfw3.h"

double Game::baseSpeed = 0.01;
Timer Game::redrawTimer = Timer();
double Game::lastRedraw = 0;
GameState Game::state;
int Game::callbackCounter = 0;
std::map<int, std::function<void(int)>> Game::keyboardCallbacks;

auto callback = [](auto key) {
    if (key == 4) {
        Game::getState().invertPaused();
    }
};

// clang-format off
std::unordered_map<int, int> Game::key_map = {
    {GLFW_KEY_DOWN, int(DIRECTION::down)},
    {GLFW_KEY_UP, int(DIRECTION::up)},
    {GLFW_KEY_LEFT, int(DIRECTION::left)},
    {GLFW_KEY_RIGHT, int(DIRECTION::right)},
    {GLFW_KEY_ESCAPE, ESCKEY},
    {GLFW_KEY_W, int('w')},
    {GLFW_KEY_A, int('a')},
    {GLFW_KEY_S, int('s')},
    {GLFW_KEY_D, int('d')},
    {GLFW_KEY_ENTER, ENTERKEY}
};
// clang-format on

Game::Game() {
    ResourceManager::LoadShader("shaders/shader.vs", "shaders/shader.fs", nullptr, "mainShader");
    ResourceManager::LoadShader("shaders/text.vs", "shaders/text.fs", nullptr, "textShader");

    ResourceManager::LoadTexture("resources/map/pacman_map.png", true, "baseMap");
    ResourceManager::LoadTexture("resources/blinky.png", true, "blinky");
    ResourceManager::LoadTexture("resources/pinky.png", true, "pinky");
    ResourceManager::LoadTexture("resources/inky.png", true, "inky");
    ResourceManager::LoadTexture("resources/clyde.png", true, "clyde");
    ResourceManager::LoadTexture("resources/frightened.png", true, "frightened");
    ResourceManager::LoadTexture("resources/pacman.png", true, "pacman");
    ResourceManager::LoadTexture("resources/pellet.png", true, "pellet");
    ResourceManager::LoadTexture("resources/power_pellet.png", true, "power_pellet");
    ResourceManager::LoadTexture("resources/cherry.png", true, "cherry");
    ResourceManager::LoadTexture("resources/eyes/eyes_right.png", true, "eyesRight");
    ResourceManager::LoadTexture("resources/eyes/eyes_left.png", true, "eyesLeft");
    ResourceManager::LoadTexture("resources/eyes/eyes_up.png", true, "eyesUp");
    ResourceManager::LoadTexture("resources/eyes/eyes_down.png", true, "eyesDown");

    ResourceManager::LoadSprite("baseMap", std::make_shared<Map>());
    ResourceManager::LoadSprite("blinky", std::make_shared<Ghost>("blinky"));
    ResourceManager::LoadSprite("inky", std::make_shared<Ghost>("inky"));
    ResourceManager::LoadSprite("pinky", std::make_shared<Ghost>("pinky"));
    ResourceManager::LoadSprite("clyde", std::make_shared<Ghost>("clyde"));
    ResourceManager::LoadSprite("pacman", std::make_shared<Pacman>());
    ResourceManager::LoadSprite("pellet", std::make_shared<Pellet>());
    ResourceManager::LoadSprite("base", std::make_shared<GameLogic>());

    ResourceManager::LoadSprite("pauseMenu", std::make_shared<PauseMenu>());

    ResourceManager::GetSprite("pacman")->setPosition(std::make_pair(13.5, 23));

    Game::registerKeyboardCallback(callback);
}

Game& Game::initialize() {
    static Game game_obj;
    return game_obj;
}

void Game::render() {
    glClearColor(0.1F, 0.1F, 0.1F, 0.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    redrawTimer.start();
    // IMPORTANT: order of draw calls is important
    // especially when depth testing is disabled.
    // Objects drawn later appear above objects drawn
    // before.
    auto baseMapPtr = std::dynamic_pointer_cast<Map>(ResourceManager::GetSprite("baseMap"));
    baseMapPtr->draw("mainShader");
    // baseMapPtr->drawGridLines("mainShader");
    ResourceManager::GetSprite("base")->draw("mainShader");
    ResourceManager::GetSprite("pellet")->draw("mainShader");
    ResourceManager::GetSprite("pacman")->draw("mainShader");
    ResourceManager::GetSprite("clyde")->draw("mainShader");
    ResourceManager::GetSprite("inky")->draw("mainShader");
    ResourceManager::GetSprite("pinky")->draw("mainShader");
    ResourceManager::GetSprite("blinky")->draw("mainShader");
    ResourceManager::GetSprite("pauseMenu")->draw("mainShader");

    lastRedraw = redrawTimer.timeElapsed();
}

int Game::registerKeyboardCallback(const std::function<void(int)>& function) {
    keyboardCallbacks[callbackCounter] = function;
    return callbackCounter++;
}

void Game::unregisterKeyboardCallback(const int& id) {
    keyboardCallbacks.erase(id);
}

void Game::key_down(int key) {
    for (const auto& [id, func] : keyboardCallbacks) {
        func(key);
    }
}

double Game::getSpeed() {
    return Game::baseSpeed;
}

void Game::setSpeed(double newSpeed) {
    Game::baseSpeed = newSpeed;
}

double Game::getTime() {
    return std::min(redrawTimer.timeElapsed() - lastRedraw, 25.0);
}

GameState& Game::getState() {
    return state;
}
