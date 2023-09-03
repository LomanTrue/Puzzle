#include <iostream>

#include "puzzle.cpp"

int app(sf::RenderWindow& window) {
  std::string pr_dir = "D:\\Projects\\Puzzle";

  sf::Texture puzzle_texture;
  if (!puzzle_texture.loadFromFile(pr_dir + "\\pictures\\6.png")) {
    return 1;
  }

  puzzle puzzle1(6, 6, puzzle_texture, sf::Vector2f(100, 100));

  window.clear();
  puzzle1.draw(window);
  window.display();

  sf::sleep(sf::seconds(2));

  puzzle1.shuffle();

  while (window.isOpen()) {
    sf::Event event;

    if (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        piece* temp_piece = puzzle1.isMouseInPiece();
        if (temp_piece != nullptr) {
          sf::Vector2f mouse_in_piece_cords = sf::Vector2f(sf::Mouse::getPosition()) - temp_piece->getPosition();

          while (true) {
            if (window.pollEvent(event) && event.type == sf::Event::MouseButtonReleased) {
              break;
            }

            puzzle1.connectPieces(*temp_piece);
            puzzle1.setPosition(*temp_piece, sf::Vector2f(sf::Mouse::getPosition()) - mouse_in_piece_cords);
            window.clear();
            puzzle1.draw(window);
            window.display();
          }
        }
      }
    }

    window.clear();
    puzzle1.draw(window);
    window.display();
  }

  return 0;
}

/*
  sf::Texture texture1;
  if (!texture1.loadFromFile(pr_dir + "\\pictures\\3.png")) {
    return 1;
  }
  sf::Texture texture2;
  if (!texture2.loadFromFile(pr_dir + "\\pictures\\2.png")) {
    return 1;
  }

  int textures_count = 2;

  sf::Texture textures[textures_count];
  textures[0] = texture1;
  textures[1] = texture2;

  int cur_i = 0;
  int x = 0;
  int y = 0;
  bool x_add = true;
  bool y_add = true;

  sf::Sprite sprite(texture1);

  sf::Vector2u size = texture1.getSize();

  sf::Text text;
  sf::Font font;

  if (!font.loadFromFile(pr_dir + "\\fonts\\1.otf")) {
    return 1;
  }

  text.setFont(font);
  text.setString("Sprite x: " + std::to_string(x) + " Sprite y: " + std::to_string(y));
  text.setCharacterSize(30);
  text.setFillColor(sf::Color::Red);
  sf::Vector2u a(200, 100);
  piece b;

  window.draw(sprite);
  window.draw(text);

  window.display();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonReleased) {

      }

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Key::Right) {
          cur_i = (cur_i + 1) % textures_count;
        } else if (event.key.code == sf::Keyboard::Key::Left) {
          if (cur_i == 0) {
            cur_i = 1;
          } else {
            cur_i = 0;
          }
        } else if (event.key.code == sf::Keyboard::Key::Escape) {
          window.close();
        }

        sprite.setTexture(textures[cur_i]);
      }
    }
  }

  return 0;

*/