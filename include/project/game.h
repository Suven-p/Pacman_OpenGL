#ifndef GAME_HPP_GUARD
#define GAME_HPP_GUARD

#include <project/common.h>
#include <project/map.h>
#include <memory>
#include <project/gameState.hpp>
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
    static Timer redrawTimer;
    static double lastRedraw;
    static GameState state;
    static int callbackCounter;
    static std::map<int, std::function<void(int)>> keyboardCallbacks;

   public:
    ~Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&&) = delete;

    static std::unordered_map<int, int> key_map;

    /**
     * @brief Get the State object
     *
     * @return GameState&
     */
    static GameState& getState();

    /**
     * @brief Get the current instance.
     * Get the current instance if it exists and generate one if it doesn't exist.
     * This is used to enforce singleton pattern.
     * @return Game*
     */
    static Game& initialize();

    /** @brief Draw function to be called for every render.
     */
    static void render();

    /**
     * @brief Register callback for keyboard event
     *
     * @param function Function to be called
     * @return int  Identifier to be used to unregister callback
     */
    static int registerKeyboardCallback(const std::function<void(int)>& function);

    /**
     * @brief Unregister previously registered keyboard callback
     *
     * @param id Id of callback returned by registerKeyboardCallback()
     */
    static void unregisterKeyboardCallback(const int& id);

    /**
     * @brief Callback function for key pressed.
     *
     * @param key code for key pressed
     */
    static void key_down(int key);

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

    static Timer& getTimer();

    static void reset();
};

#endif
