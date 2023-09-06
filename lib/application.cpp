#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "puzzle.cpp"

int app(sf::RenderWindow& window) {
  std::string pr_dir = "D:\\Projects\\Puzzle";

  sf::Texture puzzle_texture;
  if (!puzzle_texture.loadFromFile(pr_dir + "\\pictures\\3.png")) {
    return 1;
  }

  puzzle puzzle1(3, 3, puzzle_texture, sf::Vector2f(150, 150), true);

  sf::Clock timer;
  sf::Time time;

  sf::Font font;
  if (font.loadFromFile(pr_dir + "\\fonts\\3.ttf"));
  sf::Text timer_text;
  timer_text.setFont(font);
  timer_text.setCharacterSize(30);
  timer_text.setFillColor(sf::Color::Red);
  timer_text.setPosition(0,0);
  timer_text.setString("Time: 0.000000");

  window.clear();
  window.draw(timer_text);
  puzzle1.draw(window);
  window.display();

  sf::sleep(sf::seconds(2));

  puzzle1.shuffle();
  timer.restart();

  sf::Vector2i mouse_pos;
  bool is_paused = false;
  float time_on_pause = 0;
  float buffer_time = 0;
  while (window.isOpen()) {
    sf::Event event;
    if (event.mouseMove.x != 0 && event.mouseMove.x != 1) {
      mouse_pos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    }

    if (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Key::Escape) {
          window.close();
        }

        if (event.key.code == sf::Keyboard::Key::P) {
          if (is_paused) {
            is_paused = false;
            time_on_pause += buffer_time;
            buffer_time = 0;
            timer.restart();
          } else {
            buffer_time = timer.getElapsedTime().asSeconds();
            is_paused = true;
          }
        }
      }

      if (event.type == sf::Event::MouseButtonPressed && !is_paused) {
        if (event.mouseMove.x != 0 && event.mouseMove.x != 1) {
          mouse_pos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          piece* temp_piece = puzzle1.isMouseInPiece(mouse_pos);
          if (temp_piece != nullptr) {
            sf::Vector2f mouse_in_piece_cords;
            mouse_in_piece_cords.x = static_cast<float>(mouse_pos.x) - temp_piece->getPosition().x;
            mouse_in_piece_cords.y = static_cast<float>(mouse_pos.y) - temp_piece->getPosition().y;

            while (true) {
              if (window.pollEvent(event) && event.type == sf::Event::MouseButtonReleased) {
                break;
              }

              if (event.mouseMove.x != 0 && event.mouseMove.x != 1) {
                mouse_pos = sf::Vector2i (event.mouseMove.x, event.mouseMove.y);
              }
              puzzle1.connectPieces(*temp_piece);

              sf::Vector2f piece_new_pos;
              piece_new_pos.x = static_cast<float>(mouse_pos.x) - mouse_in_piece_cords.x;
              piece_new_pos.y = static_cast<float>(mouse_pos.y) - mouse_in_piece_cords.y;
              puzzle1.setPosition(*temp_piece, piece_new_pos);

              window.clear();
              puzzle1.draw(window);
              time = timer.getElapsedTime();
              timer_text.setString("Time: " + std::to_string(time.asSeconds() + time_on_pause));
              window.draw(timer_text);
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
        } else if (puzzle1.isEnableRotation() && sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
          piece* temp_piece = puzzle1.isMouseInPiece(mouse_pos);
          if (temp_piece != nullptr && puzzle1.isOnePieceInUnion(*temp_piece)) {
            temp_piece->incRotation();
          }
        }
      }
    }

    window.clear();
    if (!is_paused) {
      time = timer.getElapsedTime();
    }
    puzzle1.draw(window);
    timer_text.setString("Time: " + std::to_string(time.asSeconds() + time_on_pause));
    window.draw(timer_text);
    window.display();
  }

  return 0;
}
