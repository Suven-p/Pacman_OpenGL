#include <project/sprite.h>

void Sprite::setTexture(std::string newTexture) { texture = newTexture; }
std::string Sprite::getTexture() const { return texture; }