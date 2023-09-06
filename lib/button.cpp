#include "button.h"

bool InRange2(int x, int left, int right) {
  return (x >= left) && (x <= right);
}

bool InRange2(float x, float left, float right) {
  return (x >= left) && (x <= right);
}

void button::setPosition(sf::Vector2f pos_) {
  x = static_cast<int>(pos_.x);
  y = static_cast<int>(pos_.y);
  text.setPosition(pos_);
}

void button::setPosition(sf::Vector2i pos_) {
  x = pos_.x;
  y = pos_.y;
  text.setPosition(sf::Vector2f(x,y));
}

sf::Vector2f button::getPosition() {
  return text.getPosition();
}

void button::setString(std::string& text_string) {
  text.setString(text_string);
}

bool button::inButton(sf::Vector2i mouse_pos) {
  if (InRange2(mouse_pos.x, x - width, x + width) &&
      InRange2(mouse_pos.y, y - height, y + height)) {
    return true;
  } else {
    return false;
  }
}

void button::draw(sf::RenderWindow& window) {
  window.draw(sprite);
  window.draw(text);
}