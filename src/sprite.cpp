#include <project/sprite.h>

Sprite::Sprite() {
    position = std::make_pair(-1.0f, -1.0f);
}
void Sprite::setPosition(std::pair<float, float> newPosition) {
    position = newPosition;
}
std::pair<float, float> Sprite::getPosition() const {
    return position;
}
void Sprite::setTexture(std::string newTexture) {
    texture = newTexture;
}
std::string Sprite::getTexture() const {
    return texture;
}
Sprite::~Sprite() = default;
