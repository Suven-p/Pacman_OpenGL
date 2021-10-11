#ifndef RESOURCE_MANAGER_HPP_GUARD
#define RESOURCE_MANAGER_HPP_GUARD

#include <map>
#include <set>
#include <memory>
#include <string>

#include <project/common.h>
#include <project/shader.h>
#include <project/sprite.h>
#include <project/texture.h>
#include <spdlog/spdlog.h>
#include <project/gameState.hpp>
#include <project/MainWindow.hpp>

/**
 * @brief Manage all game resources.
 * Stores all the shaders, textures and sprites in string indexed map.
 * Stored objects can be accessed from any class or function since the methods
 * are static.
 */
class ResourceManager {
   public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string, std::shared_ptr<Sprite>> Sprites;
    static std::shared_ptr<MainWindow> mainWindow;

    static std::shared_ptr<MainWindow> getMainWindow();

    /**
     * @brief Compile and link shaders.
     *
     * @param vShaderFile Path to vertex shader file.
     * @param fShaderFile Path to fragment shader file.
     * @param gShaderFile Path to geometric shader file.
     * @param name Index for the stored shader.
     * @return Shader
     */
    static Shader LoadShader(const char* vShaderFile,
                             const char* fShaderFile,
                             const char* gShaderFile,
                             const std::string& name);

    /**
     * @brief Get the Shader object.
     *
     * @param name Index for the shader to get.
     * @return Shader
     */
    static Shader GetShader(const std::string& name);

    /**
     * @brief Generate and configure a texture.
     *
     * @param file Path to image file.
     * @param alpha Presence of alpha channel in image.
     * @param name Index for the generated texture.
     * @return Texture2D
     */
    static Texture2D LoadTexture(const char* file, bool alpha, const std::string& name);

    /**
     * @brief Get the Texture object.
     *
     * @param name Index for the texture to get.
     * @return Texture2D
     */
    static Texture2D GetTexture(const std::string& name);

    /**
     * @brief Store a pointer to Sprite.
     *
     * @param name Index for the generated sprite.
     * @param sprite Pointer to the generated sprite.
     * @return std::shared_ptr<Sprite>
     */
    static std::shared_ptr<Sprite> LoadSprite(const std::string& name, Sprite* sprite);

    /**
     * @brief Store a pointer to Sprite.
     *
     * @param name Index for the generated sprite.
     * @param sprite shared_ptr to the generated sprite.
     * @return std::shared_ptr<Sprite>
     */
    static std::shared_ptr<Sprite> LoadSprite(const std::string& name,
                                              std::shared_ptr<Sprite> sprite);

    /**
     * @brief Get the Sprite object.
     *
     * @param name Index for the stored sprite.
     * @return std::shared_ptr<Sprite>
     */
    template <typename T = Sprite>
    static std::shared_ptr<T> GetSprite(const std::string& name);

    /**
     * @brief Clear all stored textures and shaders.
     *
     */
    static void Clear();
    static std::string resolvePath(const std::string& toResolve);
    static void resetSprites(std::set<std::string> excluded = {});

    ResourceManager() = delete;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(const ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&&) = delete;

   private:
    static std::shared_ptr<spdlog::logger> getLogger();
    static Shader loadShaderFromFile(const char* vShaderFile,
                                     const char* fShaderFile,
                                     const char* gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const char* file, bool alpha);
};

template <typename T>
std::shared_ptr<T> ResourceManager::GetSprite(const std::string& name) {
    auto ptr = Sprites.at(name);
    return std::dynamic_pointer_cast<T>(ptr);
}

#endif
