#include <memory>
#include <project/game.h>
#include <project/helpers.h>
#include <project/resourceManager.h>
#include <project/map.h>
#include <project/ghost.h>
#include <project/pacman.h>
#include <iostream>
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
    ResourceManager::GetSprite("pacman")->setPosition(std::make_pair(13.5, 23));//13.5,23
}

float Game::baseSpeed = 0.01;
Game *Game::instance = nullptr;
std::vector<bool> Game::key_states(256, false);
unsigned long long Game::lastRedraw = 0;
unsigned long long Game::deltaTime = 0;
std::unordered_map<int, int> Game::special_key_map =
    {
        {GLUT_KEY_DOWN, int(DIRECTION::down)},
        {GLUT_KEY_UP, int(DIRECTION::up)},
        {GLUT_KEY_LEFT, int(DIRECTION::left)},
        {GLUT_KEY_LEFT, int(DIRECTION::right)}};
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
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = currentTime - lastRedraw;
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
    glutSwapBuffers();
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
    
    getPacmanPtr()->setDirection(DIRECTION(Game::special_key_map[key]));
}

void Game::special_key_up(int key, int x, int y)
{
    Game::special_key_states[Game::special_key_map[key]] = false;
}



float Game::getSpeed()
{
    return Game::baseSpeed;
}

void Game::setSpeed(float newSpeed)
{
    Game::baseSpeed = newSpeed;
}

unsigned long long Game::getTime()
{
    return Game::deltaTime;
}
