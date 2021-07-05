#include <project/game.h>
#include <project/helpers.h>
#include <project/resourceManager.h>
#include <project/map.h>

Game::Game()
{
    ResourceManager::LoadShader("./shaders/shader.vs", "./shaders/shader.fs", nullptr, "mainShader");
    ResourceManager::LoadTexture("./resources/map/pacman_map.png", true, "baseMap");
    baseMap = Map();
}

Game *Game::instance = nullptr;
std::vector<bool> Game::key_states(256, false);
std::unordered_map<int, int> Game::special_key_map =
    {
        {GLUT_KEY_DOWN, int(DIRECTION::down)},
        {GLUT_KEY_UP, int(DIRECTION::up)},
        {GLUT_KEY_LEFT, int(DIRECTION::left)},
        {GLUT_KEY_LEFT, int(DIRECTION::right)}};
std::vector<bool> Game::special_key_states(Game::special_key_map.size(), false);
constexpr int Game::numRows, Game::numCols;

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
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    baseMap.draw("mainShader");
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
}

void Game::special_key_up(int key, int x, int y)
{
    Game::special_key_states[Game::special_key_map[key]] = false;
}
