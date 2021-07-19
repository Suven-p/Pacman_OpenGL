#include <project/resourceManager.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <project/stb_image.h>

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, std::shared_ptr<Sprite>> ResourceManager::Sprites;

Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, const std::string &name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string &name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, const std::string &name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string &name)
{
    return Textures[name];
}

std::shared_ptr<Sprite> ResourceManager::LoadSprite(const std::string &name, Sprite *sprite)
{
    Sprites[name] = std::shared_ptr<Sprite>(sprite);
    return Sprites[name];
}

std::shared_ptr<Sprite> ResourceManager::LoadSprite(const std::string &name, std::shared_ptr<Sprite> sprite)
{
    Sprites[name] = sprite;
    return Sprites[name];
}

std::shared_ptr<Sprite> ResourceManager::GetSprite(const std::string name)
{
    return Sprites[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    auto vShaderFileResolved = resolvePath(vShaderFile);
    auto fShaderFileResolved = resolvePath(fShaderFile);
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFileResolved.c_str());
        std::ifstream fragmentShaderFile(fShaderFileResolved.c_str());
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            auto gShaderFileResolved = resolvePath(gShaderFile);
            std::ifstream geometryShaderFile(gShaderFileResolved.c_str());
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        spdlog::error("SHADER::Failed to read shader files.\n"
                      " Vertex Shader Path: {}\n"
                      " Fragment Shader Path: {}\n"
                      " Geometric Shadder Path: {}\n",
                      std::string(vShaderFile),
                      std::string(fShaderFile),
                      std::string(gShaderFile));
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *file, bool alpha)
{
    auto fileLocation = resolvePath(file);
    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(fileLocation.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        spdlog::error("TEXTURE::Failed to load texture. File location: {}", std::string(file));
    }
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
std::filesystem::path getExecutablePath()
{
    WCHAR buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer);
}
#elif defined(__linux__)
std::filesystem::path getExecutablePath()
{
    auto execPath = std::filesystem::read_symlink("/proc/self/exe");
    auto reqPath = execPath.parent_path();
    return reqPath;
}
#else
std::filesystem::path getExecutablePath()
{
    return "";
}
#endif

std::string ResourceManager::resolvePath(const std::string &toResolve)
{
    auto pathToResolve = std::filesystem::path(toResolve);

    auto executionPath = getExecutablePath();
    if (std::filesystem::exists(executionPath))
    {
        auto attemptPath = executionPath / pathToResolve;
        if (std::filesystem::exists(attemptPath))
        {
            std::string fullPath = attemptPath;
            spdlog::debug("Resolved {} to {}", toResolve, fullPath);
            return fullPath;
        }
        else
        {
            spdlog::debug("Received path to executable as: \"{}\""
                          "but could not find file \"{}\"",
                          executionPath.string(), toResolve);
        }
    }

    auto binaryPath = std::filesystem::path(BINARY_DIRECTORY);
    if (std::filesystem::exists(binaryPath))
    {
        auto attemptPath = binaryPath / pathToResolve;
        if (std::filesystem::exists(attemptPath))
        {
            std::string fullPath = attemptPath.c_str();
            spdlog::debug("Resolved {} to {}", toResolve, fullPath);
            return fullPath;
        }
        else
        {
            spdlog::debug("Received path to binary directory as: \"{}\""
                          "but could not find file \"{}\"",
                          BINARY_DIRECTORY, toResolve);
        }
    }
    else
    {
        spdlog::debug("Stored path to binary: \"{}\" is invalid.", BINARY_DIRECTORY);
    }

    auto currentPath = std::filesystem::current_path();
    auto attemptPath = currentPath / pathToResolve;
    if (std::filesystem::exists(attemptPath))
    {
        std::string fullPath = attemptPath.c_str();
        spdlog::debug("Resolved {} to {}", toResolve, fullPath);
        return fullPath;
    }
    else
    {
        spdlog::error("Could not find file: \"{}\"", toResolve);
    }

    return "";
}
