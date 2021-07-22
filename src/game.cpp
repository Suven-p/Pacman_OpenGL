#include <memory>
#include <project/game.h>
#include <project/helpers.h>
#include <project/resourceManager.h>
#include <project/map.h>
#include <project/ghost.h>
#include <project/pacman.h>

Game::Game()
{
    ResourceManager::LoadShader("shaders/shader.vs", "shaders/shader.fs", nullptr, "mainShader");
    ResourceManager::LoadTexture("resources/map/pacman_map.png", true, "baseMap");
    ResourceManager::LoadTexture("resources/blinky.png", true, "blinky");
    ResourceManager::LoadTexture("resources/inky.png", true, "inky");
    ResourceManager::LoadTexture("resources/pacman.png", true, "pacman");
    ResourceManager::LoadSprite("baseMap", std::make_shared<Map>());
    ResourceManager::LoadSprite("blinky", std::make_shared<Ghost>("blinky"));
    ResourceManager::LoadSprite("inky", std::make_shared<Ghost>("inky"));
    ResourceManager::LoadSprite("pacman", std::make_shared<Pacman>());
    ResourceManager::GetSprite("blinky")->setPosition(std::make_pair(10, 29));
    ResourceManager::GetSprite("inky")->setPosition(std::make_pair(2, 1));
    ResourceManager::GetSprite("pacman")->setPosition(std::make_pair(13.5, 23));
}

double Game::baseSpeed = 0.01;
Game *Game::instance = nullptr;
std::vector<bool> Game::key_states(256, false);
double Game::lastRedraw = 0;
double Game::deltaTime = 0;
std::unordered_map<int, int> Game::special_key_map =
    {
        {0, int(DIRECTION::down)},
        {0, int(DIRECTION::up)},
        {0, int(DIRECTION::left)},
        {0, int(DIRECTION::right)}};
std::vector<bool> Game::special_key_states(Game::special_key_map.size(), false);

Game *Game::getInstance()
{
    if (!instance)
    {
        instance = new Game();
    }
    return instance;
}

void Game::render()
{
    double currentTime = (glfwGetTime() * 1000.0);
    deltaTime = std::min(currentTime - lastRedraw, 25.0);
    lastRedraw = currentTime;

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto baseMapPtr = std::dynamic_pointer_cast<Map>(ResourceManager::GetSprite("baseMap"));
    baseMapPtr->draw("mainShader");
    baseMapPtr->drawGridLines("mainShader");
    ResourceManager::GetSprite("blinky")->draw("mainShader");
    ResourceManager::GetSprite("inky")->draw("mainShader");
    ResourceManager::GetSprite("pacman")->draw("mainShader");
    baseMapPtr->drawObstacles("mainShader");
}

void Game::key_down(unsigned char key, int x, int y)
{
    Game::key_states[key] = true;
}

void Game::key_up(unsigned char key, int x, int y)
{
    Game::key_states[key] = false;
}

void Game::special_key_down(int key, int x, int y)
{
    Game::special_key_states[Game::special_key_map[key]] = true;
}

void Game::special_key_up(int key, int x, int y)
{
    Game::special_key_states[Game::special_key_map[key]] = false;
}

double Game::getSpeed()
{
    return Game::baseSpeed;
}

void Game::setSpeed(double newSpeed)
{
    Game::baseSpeed = newSpeed;
}

double Game::getTime()
{
    return Game::deltaTime;
}
