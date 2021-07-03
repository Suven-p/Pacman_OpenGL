#ifndef GAME_HPP_GUARD
#define GAME_HPP_GUARD

#include <vector>
#include <unordered_map>
#include <project/common.h>

class Game
{
    Game();
    static Game *instance;
    static std::vector<bool> key_states;
    static std::unordered_map<int, int> special_key_map;
    static std::vector<bool> special_key_states;

public:
    static constexpr int numRows = 36, numCols = 28;

    static Game *getInstance();
    void init();
    void render();
    static void reshape(int w, int h);
    static void key_down(unsigned char key, int x, int y);
    static void key_up(unsigned char key, int x, int y);
    static void special_key_down(int key, int x, int y);
    static void special_key_up(int key, int x, int y);

    unsigned int map_vao;
};

#endif
