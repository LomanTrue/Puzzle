#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "puzzle.cpp"

int app(sf::RenderWindow& window) {
  std::string pr_dir = "D:\\Projects\\Puzzle";

  sf::Texture puzzle_texture;
  if (!puzzle_texture.loadFromFile(pr_dir + "\\pictures\\8.jpg")) {
    return 1;
  }

  puzzle puzzle1(6, 4, puzzle_texture, sf::Vector2f(150, 150));

  sf::Clock timer;
  sf::Time time;

  sf::Font font;
  if (font.loadFromFile(pr_dir + "\\fonts\\1.otf"));
  sf::Text timer_text;
  timer_text.setFont(font);
  timer_text.setCharacterSize(30);
  timer_text.setFillColor(sf::Color::Red);
  timer_text.setPosition(0,0);
  time = timer.restart();
  timer_text.setString("Time: 0.000000");

  window.clear();
  window.draw(timer_text);
  puzzle1.draw(window);
  window.display();

  sf::sleep(sf::seconds(2));

  puzzle1.shuffle();
  timer.restart();

  while (window.isOpen()) {
    sf::Event event;

    if (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
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
              time = timer.getElapsedTime();
              timer_text.setString("Time: " + std::to_string(time.asSeconds()));
              window.draw(timer_text);
              puzzle1.draw(window);
              window.display();

              if (puzzle1.isPuzzleSolved()) {
                /*
                sf::SoundBuffer sound_buffer;
                if (!sound_buffer.loadFromFile(pr_dir + "\\sounds\\applause.mp3")) {
                  return 1;
                }

                sf::Sound sound(sound_buffer);
                sound.play();
                */
                sf::sleep(sf::seconds(2));
                window.close();
                break;
              }
            }
          }
        } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
          piece* temp_piece = puzzle1.isMouseInPiece();
          if (temp_piece != nullptr && puzzle1.isOnePieceInUnion(*temp_piece)) {
            temp_piece->incRotation();
          }
        }
      }
    }

    window.clear();
    time = timer.getElapsedTime();
    timer_text.setString("Time: " + std::to_string(time.asSeconds()));
    window.draw(timer_text);
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