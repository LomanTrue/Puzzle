#include <iostream>
#include <cstdint>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class button {
 private:
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  sf::Sprite sprite;
  sf::Text text;
  sf::Font text_font;

 public:
  button(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, std::string text_) {
    x = x_;
    y = y_;
    width = width_;
    height = height_;
    sprite.setPosition(x_, y_);
    text.setString(text_);
    text.setPosition(x_,y_);

    if (!text_font.loadFromFile("D:\\Projects\\Puzzle\\fonts\\3.ttf")) {
      std::cout << "asfasfa";
    }
    text.setFont(text_font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Red);
  }


  button(uint16_t x_, uint16_t y_, uint16_t width_, uint16_t height_, sf::Text text_) {
    x = x_;
    y = y_;
    width = width_;
    height = height_;
    sprite.setPosition(x_, y_);
    text = text_;
    text.setPosition(x_, y_);

    sf::Font text_font;
    if (!text_font.loadFromFile("D:\\Projects\\Puzzle\\fonts\\3.ttf")) {
      std::cout << "asfasfa";
      return;
    }
    text.setFont(text_font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Red);
  }

  void setPosition(sf::Vector2f pos_);
  void setPosition(sf::Vector2i pos_);
  sf::Vector2f getPosition();
  void setString(std::string& text_string);
  bool inButton(sf::Vector2i mouse_pos);
  void draw(sf::RenderWindow& window);
};
