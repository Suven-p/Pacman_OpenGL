#ifndef RESOURCE_MANAGER_HPP_GUARD
#define RESOURCE_MANAGER_HPP_GUARD

#include <map>
#include <string>

#include <glad/glad.h>

#include <project/texture.h>
#include <project/shader.h>
#include <project/sprite.h>
#include <memory>

// An interface to access objects from another class
class ResourceManager
{
public:
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;
    static std::map<std::string, std::shared_ptr<Sprite>> Sprites;

    static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, const std::string &name);
    static Shader GetShader(const std::string &name);

    static Texture2D LoadTexture(const char *file, bool alpha, const std::string &name);
    static Texture2D GetTexture(const std::string &name);

    static std::shared_ptr<Sprite> LoadSprite(const std::string &name, Sprite *sprite);
    static std::shared_ptr<Sprite> LoadSprite(const std::string &name, std::shared_ptr<Sprite> sprite);
    static std::shared_ptr<Sprite> GetSprite(const std::string name);

    static void Clear();

private:
    ResourceManager() {}
    static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);
    static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif
