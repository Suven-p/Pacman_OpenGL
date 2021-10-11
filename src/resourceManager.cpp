#include <project/MainWindow.hpp>
#include <project/resourceManager.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, std::shared_ptr<Sprite>> ResourceManager::Sprites;
std::shared_ptr<MainWindow> ResourceManager::mainWindow;

std::shared_ptr<MainWindow> ResourceManager::getMainWindow() {
    if (!mainWindow) {
        mainWindow = std::make_shared<MainWindow>();
    }
    return mainWindow;
}

Shader ResourceManager::LoadShader(const char* vShaderFile,
                                   const char* fShaderFile,
                                   const char* gShaderFile,
                                   const std::string& name) {
    Shaders[name] = ResourceManager::loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string& name) {
    return Shaders.at(name);
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, const std::string& name) {
    Textures[name] = ResourceManager::loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string& name) {
    return Textures.at(name);
}

std::shared_ptr<Sprite> ResourceManager::LoadSprite(const std::string& name, Sprite* sprite) {
    Sprites[name] = std::shared_ptr<Sprite>(sprite);
    return Sprites[name];
}

std::shared_ptr<Sprite> ResourceManager::LoadSprite(const std::string& name,
                                                    std::shared_ptr<Sprite> sprite) {
    Sprites[name] = std::move(sprite);
    return Sprites[name];
}

void ResourceManager::Clear() {
    // (properly) delete all shaders
    for (const auto& iter : Shaders) {
        glDeleteProgram(iter.second.ID);
    }
    // (properly) delete all textures
    for (const auto& iter : Textures) {
        glDeleteTextures(1, &iter.second.ID);
    }
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile,
                                           const char* fShaderFile,
                                           const char* gShaderFile) {
    auto vShaderFileResolved = resolvePath(vShaderFile);
    auto fShaderFileResolved = resolvePath(fShaderFile);
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        // open files
        std::ifstream vertexShaderFile(vShaderFileResolved.c_str());
        std::ifstream fragmentShaderFile(fShaderFileResolved.c_str());
        std::stringstream vShaderStream;
        std::stringstream fShaderStream;
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
        if (gShaderFile != nullptr) {
            auto gShaderFileResolved = resolvePath(gShaderFile);
            std::ifstream geometryShaderFile(gShaderFileResolved.c_str());
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    } catch (std::exception e) {
        getLogger()->error("SHADER::Failed to read shader files.\n"
                           " Vertex Shader Path: {}\n"
                           " Fragment Shader Path: {}\n"
                           " Geometric Shadder Path: {}\n",
                           std::string(vShaderFile),
                           std::string(fShaderFile),
                           std::string(gShaderFile));
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
    auto fileLocation = resolvePath(file);
    // create texture object
    Texture2D texture;
    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width = 0;
    int height = 0;
    int nrChannels = 0;

    stbi_set_flip_vertically_on_load(int(true));
    unsigned char* data = stbi_load(fileLocation.c_str(), &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        getLogger()->error("Failed to load texture file: {}", file);
    }
    // now generate texture
    texture.Generate(width, height, data);  // NOLINT: values initialized by stbi_load
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
std::filesystem::path getExecutablePath() {
    WCHAR buffer[MAX_PATH];
    GetModuleFileNameW(NULL, buffer, MAX_PATH);
    auto execPath = std::filesystem::path(buffer);
    auto reqPath = execPath.parent_path();
    return reqPath;
}
#elif defined(__linux__)
std::filesystem::path getExecutablePath() {
    auto execPath = std::filesystem::read_symlink("/proc/self/exe");
    auto reqPath = execPath.parent_path();
    return reqPath;
}
#else
std::filesystem::path getExecutablePath() {
    return "";
}
#endif

std::string ResourceManager::resolvePath(const std::string& toResolve) {
    auto pathToResolve = std::filesystem::path(toResolve);

    // Try executablePath first
    auto executionPath = getExecutablePath();
    if (!std::filesystem::exists(executionPath)) {
        getLogger()->debug("Received path to executable as: \"{}\""
                           "but could not find file \"{}\"",
                           executionPath.string(),
                           toResolve);
    } else {
        auto attemptPath = executionPath / pathToResolve;
        if (std::filesystem::exists(attemptPath)) {
            std::string fullPath = attemptPath.string();
            getLogger()->trace("Resolved {} to {}", toResolve, fullPath);
            return fullPath;
        }
    }

    // Next try output directory stored by cmake during build
    auto binaryPath = std::filesystem::path(BINARY_DIRECTORY);
    if (std::filesystem::exists(binaryPath)) {
        auto attemptPath = binaryPath / pathToResolve;
        if (!std::filesystem::exists(attemptPath)) {
            getLogger()->debug("Received path to binary directory as: \"{}\""
                               "but could not find file \"{}\"",
                               BINARY_DIRECTORY,
                               toResolve);
        } else {
            std::string fullPath = attemptPath.string();
            spdlog::get("ResourceManager")->info("hello");
            getLogger()->trace("Resolved {} to {}", toResolve, fullPath);
            return fullPath;
        }
    } else {
        getLogger()->info("Stored path to binary: \"{}\" is invalid.", BINARY_DIRECTORY);
    }

    // On windows this path is location of exe when program is run by clicking exe
    // When running from terminal it is directory from which command is run
    auto currentPath = std::filesystem::current_path();
    auto attemptPath = currentPath / pathToResolve;
    if (!std::filesystem::exists(attemptPath)) {
        getLogger()->error("Could not find file: \"{}\"", toResolve);
    } else {
        std::string fullPath = attemptPath.string();
        getLogger()->trace("Resolved {} to {}", toResolve, fullPath);
        return fullPath;
    }

    return "";
}

std::shared_ptr<spdlog::logger> ResourceManager::getLogger() {
    static std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("ResourceManager");
    return logger;
}

void ResourceManager::resetSprites(std::set<std::string> excluded) {
    for (auto [key, ptr] : Sprites) {
        if (excluded.find(key) == excluded.end()) {
            ptr->reset();
        }
    }
}
