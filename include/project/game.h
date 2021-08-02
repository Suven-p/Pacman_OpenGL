#ifndef GAME_HPP_GUARD
#define GAME_HPP_GUARD

#include <project/common.h>
#include <project/map.h>
#include <memory>
#include <project/timer.hpp>
#include <unordered_map>
#include <vector>

/**
 * @brief Main class to co-oridinate the different components.
 * Initializes all the shaders, textures, and sprites.
 * Provides render function to draw all game objects.
 */
class Game {
    Game();
    static std::shared_ptr<Game> instance;
    static std::vector<bool> key_states;
    static std::unordered_map<int, int> special_key_map;
    static std::vector<bool> special_key_states;
    static double baseSpeed;
    static Timer redrawTimer;
    static double lastRedraw;

   public:
    /**
     * @brief Get the current instance.
     * Get the current instance if it exists and generate one if it doesn't exist.
     * This is used to enforce singleton pattern.
     * @return Game*
     */
    static std::shared_ptr<Game> getInstance();

    /** @brief Draw function to be called for every render.
     */
    static void render();

    /**
     * @brief Callback function for key pressed.
     *
     * @param key Ascii code for the key pressed.
     * @param x X coordinate for mouse position.
     * @param y Y coordinate for Mouse position.
     */
    static void key_down(unsigned char key, int x, int y);
    /**
     * @brief Callback function for key released.
     *
     * @param key Ascii code for the key released.
     * @param x X coordinate for mouse position.
     * @param y Y coordinate for Mouse position.
     */
    static void key_up(unsigned char key, int x, int y);
    /**
     * @brief Callback function for special key pressed.
     *
     * @param key Value for the key pressed. Value is defined in macros.
     * @param x X coordinate for mouse position.
     * @param y Y coordinate for Mouse position.
     */
    static void special_key_down(int key, int x, int y);
    /**
     * @brief Callback function for special key released.
     *
     * @param key Value for the key released. Value is defined in macros.
     * @param x X coordinate for mouse position.
     * @param y Y coordinate for Mouse position.
     */
    static void special_key_up(int key, int x, int y);
    /**
     * @brief Get the value of base speed
     *
     */
    static double getSpeed();
    /**
     * @brief Set the value of base speed
     *
     * @param newSpeed Value of new speed
     */
    static void setSpeed(double newSpeed);
    /**
     * @brief Get the value of delta time
     *
     * @return unsigned long long
     */
    static double getTime();
};

#endif
