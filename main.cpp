#include "lib\application.cpp"

int main() {
  sf::RenderWindow window(sf::VideoMode(1920,1080), "Puzzle", sf::Style::Fullscreen);

  hello_screen(window);

  return 0;
}
