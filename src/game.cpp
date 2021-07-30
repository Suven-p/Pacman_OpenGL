#include <project/game.h>
#include <project/ghost.h>
#include <project/helpers.h>
#include <project/map.h>
#include <project/pacman.h>
#include <project/resourceManager.h>
#include <spdlog/spdlog.h>
#include <memory>

Game::Game() {
    ResourceManager::LoadShader(
        "shaders/shader.vs", "shaders/shader.fs", nullptr, "mainShader");
    ResourceManager::LoadTexture(
        "resources/map/pacman_map.png", true, "baseMap");
    ResourceManager::LoadTexture("resources/blinky.png", true, "blinky");
    ResourceManager::LoadTexture("resources/pinky.png", true, "pinky");
    ResourceManager::LoadTexture("resources/inky.png", true, "inky");
    ResourceManager::LoadTexture("resources/clyde.png", true, "clyde");
    ResourceManager::LoadTexture("resources/pacman.png", true, "pacman");
    ResourceManager::LoadTexture("resources/pellet.png", true, "pellet");
    ResourceManager::LoadTexture(
        "resources/eyes/eyes_right.png", true, "eyesRight");
    ResourceManager::LoadTexture(
        "resources/eyes/eyes_left.png", true, "eyesLeft");
    ResourceManager::LoadTexture("resources/eyes/eyes_up.png", true, "eyesUp");
    ResourceManager::LoadTexture(
        "resources/eyes/eyes_down.png", true, "eyesDown");
    ResourceManager::LoadSprite("baseMap", std::make_shared<Map>());
    ResourceManager::LoadSprite("blinky", std::make_shared<Ghost>("blinky"));
    ResourceManager::LoadSprite("inky", std::make_shared<Ghost>("inky"));
    ResourceManager::LoadSprite("pinky", std::make_shared<Ghost>("pinky"));
    ResourceManager::LoadSprite("clyde", std::make_shared<Ghost>("clyde"));
    ResourceManager::LoadSprite("pacman", std::make_shared<Pacman>());
    ResourceManager::GetSprite("blinky")->setPosition(std::make_pair(10, 29));
    ResourceManager::GetSprite("pinky")->setPosition(std::make_pair(10, 29));
    ResourceManager::GetSprite("clyde")->setPosition(std::make_pair(10, 29));
    ResourceManager::GetSprite("inky")->setPosition(std::make_pair(2, 1));
    ResourceManager::GetSprite("pacman")->setPosition(std::make_pair(13.5, 23));
}

double Game::baseSpeed = 0.01;
Game* Game::instance = nullptr;
std::vector<bool> Game::key_states(256, false);
double Game::lastRedraw = 0;
double Game::deltaTime = 0;
std::unordered_map<int, int> Game::special_key_map = {
    {GLFW_KEY_DOWN, int(DIRECTION::down)},
    {GLFW_KEY_UP, int(DIRECTION::up)},
    {GLFW_KEY_LEFT, int(DIRECTION::left)},
    {GLFW_KEY_RIGHT, int(DIRECTION::right)}};
std::vector<bool> Game::special_key_states(Game::special_key_map.size(), false);

Game* Game::getInstance() {
    if (!instance) {
        instance = new Game();
    }
    return instance;
}

void Game::render() {
    double currentTime = (glfwGetTime() * 1000.0);
    deltaTime = std::min(currentTime - lastRedraw, 25.0);
    lastRedraw = currentTime;

    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // IMPORTANT: order of draw calls is important
    // especially when depth testing is disabled.
    // Objects drawn later appear above objects drawn
    // before.
    auto baseMapPtr =
        std::dynamic_pointer_cast<Map>(ResourceManager::GetSprite("baseMap"));
    baseMapPtr->draw("mainShader");
    baseMapPtr->drawGridLines("mainShader");
    ResourceManager::GetSprite("pacman")->draw("mainShader");
    ResourceManager::GetSprite("clyde")->draw("mainShader");
    ResourceManager::GetSprite("inky")->draw("mainShader");
    ResourceManager::GetSprite("pinky")->draw("mainShader");
    ResourceManager::GetSprite("blinky")->draw("mainShader");
    baseMapPtr->drawObstacles("mainShader");
}

void Game::key_down(unsigned char key, int x, int y) {
    Game::key_states[key] = true;
}

void Game::key_up(unsigned char key, int x, int y) {
    Game::key_states[key] = false;
}

void Game::special_key_down(int key, int x, int y) {
    if (Game::special_key_map.count(key)) {
        Game::special_key_states[Game::special_key_map[key]] = true;
        getPacmanPtr()->setNextDirection(DIRECTION(Game::special_key_map[key]));
    }
    spdlog::trace("Switching direction to {}",
                  toString(DIRECTION(Game::special_key_map[key])));
}

void Game::special_key_up(int key, int x, int y) {
    if (Game::special_key_map.count(key)) {
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
    return Game::deltaTime;
}
