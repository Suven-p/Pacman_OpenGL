#include <project/sprite.h>

#include <utility>

Sprite::Sprite() {
    position = std::make_pair(-1.0F, -1.0F);
}
void Sprite::setPosition(std::pair<float, float> newPosition) {
    position = newPosition;
}
std::pair<float, float> Sprite::getPosition() const {
    return position;
}
void Sprite::setTexture(std::string newTexture) {
    texture = std::move(newTexture);
}
std::string Sprite::getTexture() const {
    return texture;
}
Sprite::~Sprite() = default;

void Sprite::reset() {}
