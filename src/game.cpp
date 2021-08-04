#include <project/base.h>
#include <project/game.h>
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

std::vector<bool> Game::key_states(256, false);
double Game::baseSpeed = 0.01;
Timer Game::redrawTimer = Timer();
double Game::lastRedraw = 0;
GameState Game::state;

std::unordered_map<int, int> Game::special_key_map = {{GLFW_KEY_DOWN, int(DIRECTION::down)},
                                                      {GLFW_KEY_UP, int(DIRECTION::up)},
                                                      {GLFW_KEY_LEFT, int(DIRECTION::left)},
                                                      {GLFW_KEY_RIGHT, int(DIRECTION::right)},
                                                      {GLFW_KEY_ESCAPE, 4}};
std::vector<bool> Game::special_key_states(Game::special_key_map.size(), false);

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
    ResourceManager::LoadSprite("base", std::make_shared<Base>());
    ResourceManager::LoadSprite("menu", std::make_shared<Menu>());

    ResourceManager::GetSprite("pacman")->setPosition(std::make_pair(13.5, 23));
}

Game& Game::initialize() {
    static Game game_obj;
    return game_obj;
}

void Game::render() {
    glClearColor(0.2F, 0.2F, 0.2F, 0.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    if (state.isPaused()) {
        ResourceManager::GetSprite("menu")->draw("mainShader");
    }

    lastRedraw = redrawTimer.timeElapsed();
}

void Game::key_down(unsigned char key, int x, int y) {
    Game::key_states[key] = true;
}

void Game::key_up(unsigned char key, int x, int y) {
    Game::key_states[key] = false;
}

void Game::special_key_down(int key, int x, int y) {
    if (Game::special_key_map.count(key) > 0) {
        auto mapped_key = Game::special_key_map[key];
        special_key_states[mapped_key] = true;
        if (mapped_key < 4) {
            getPacmanPtr()->setNextDirection(DIRECTION(mapped_key));
        } else if (mapped_key == 4) {
            state.invertPaused();
        }
    }
}

void Game::special_key_up(int key, int x, int y) {
    if (Game::special_key_map.count(key) > 0) {
        Game::special_key_states[Game::special_key_map[key]] = false;
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
