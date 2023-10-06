#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "puzzle.cpp"
#include "button.cpp"

int death(sf::RenderWindow& window) {
  std::string anim_dir = "D:\\Projects\\Puzzle\\animations\\freddy\\Withered Freddy Jumpscare_0";

  sf::Texture anim_texture;
  sf::Sprite anim_sprite;

  for (int i = 39; i <= 72; i++) {
    if (!anim_texture.loadFromFile(anim_dir + std::to_string(i) + ".jpg")) {
      return 1;
    }
    anim_sprite.setPosition(0,0);
    anim_sprite.setTexture(anim_texture);
    anim_sprite.setScale(static_cast<float>(window.getSize().x) / static_cast<float>(anim_sprite.getTexture()->getSize().x),
                       static_cast<float>(window.getSize().y) / static_cast<float>(anim_sprite.getTexture()->getSize().y));
    window.clear();
    window.draw(anim_sprite);
    window.display();
    sf::sleep(sf::milliseconds(60));
  }

  return 0;
}

int puzzle_screen(sf::RenderWindow& window, uint16_t num_image) {
  std::string pr_dir = "D:\\Projects\\Puzzle";

  sf::Texture puzzle_texture;
  if (!puzzle_texture.loadFromFile(pr_dir + "\\pictures\\" + std::to_string(num_image) + ".png")) {
    if (!puzzle_texture.loadFromFile(pr_dir + "\\pictures\\" + std::to_string(num_image) + ".jpg")) {
      return 1;
    }
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
          break;
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
              timer_text.setString("Time: " + std::to_string(60 - time.asSeconds() + time_on_pause));
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
    if ((time.asSeconds() + time_on_pause) >= 60 && !is_paused) {
      death(window);
      break;
    }
    timer_text.setString("Time: " + std::to_string(60 - time.asSeconds() + time_on_pause));
    window.draw(timer_text);
    window.display();
  }

  return 0;
}

int levels_screen(sf::RenderWindow& window) {
  sf::Texture puzzle_texture;
  uint16_t cur_image = 0;
  uint16_t max_image = 8;
  sf::Sprite* puzzle_sprite = new sf::Sprite;

  if (!puzzle_texture.loadFromFile("D:\\Projects\\Puzzle\\pictures\\" + std::to_string(cur_image + 1) + ".jpg")) {
    if (!puzzle_texture.loadFromFile("D:\\Projects\\Puzzle\\pictures\\" + std::to_string(cur_image + 1) + ".png")) {
      return 1;
    }
  }

  puzzle_sprite->setTexture(puzzle_texture);
  puzzle_sprite->setOrigin(puzzle_texture.getSize().x / 2, puzzle_texture.getSize().y / 2);
  puzzle_sprite->setPosition(window.getSize().x / 2, window.getSize().y / 2);
  //puzzle_sprite->setScale(static_cast<float>(window.getSize().x) / static_cast<float>(puzzle_texture->getSize().x) / 3,
                         //static_cast<float>(window.getSize().y) / static_cast<float>(puzzle_texture->getSize().y) / 3);

  while (window.isOpen()) {
    sf::Event event;

    if (window.pollEvent(event)) {
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Right) {
          cur_image = (cur_image + 1) % max_image;

          if (!puzzle_texture.loadFromFile("D:\\Projects\\Puzzle\\pictures\\" + std::to_string(cur_image + 1) + ".jpg")) {
            if (!puzzle_texture.loadFromFile("D:\\Projects\\Puzzle\\pictures\\" + std::to_string(cur_image + 1) + ".png")) {
              return 1;
            }
          }

          sf::Sprite* temp_sprite = new sf::Sprite;
          delete puzzle_sprite;
          puzzle_sprite = temp_sprite;
          puzzle_sprite->setTexture(puzzle_texture);
          puzzle_sprite->setOrigin(puzzle_texture.getSize().x / 2, puzzle_texture.getSize().y / 2);
          puzzle_sprite->setPosition(window.getSize().x / 2, window.getSize().y / 2);
          //puzzle_sprite->setScale(static_cast<float>(window.getSize().x) / static_cast<float>(puzzle_texture->getSize().x) / 2,
                                 //static_cast<float>(window.getSize().y) / static_cast<float>(puzzle_texture->getSize().y) / 2);
        }

        if (event.key.code == sf::Keyboard::Left) {
          if (cur_image == 0) {
            cur_image = max_image - 1;
          } else {
            cur_image--;
          }

          if (!puzzle_texture.loadFromFile("D:\\Projects\\Puzzle\\pictures\\" + std::to_string(cur_image + 1) + ".jpg")) {
            if (!puzzle_texture.loadFromFile("D:\\Projects\\Puzzle\\pictures\\" + std::to_string(cur_image + 1) + ".png")) {
              return 1;
            }
          }

          sf::Sprite* temp_sprite = new sf::Sprite;
          delete puzzle_sprite;
          puzzle_sprite = temp_sprite;
          puzzle_sprite->setTexture(puzzle_texture);
          puzzle_sprite->setOrigin(puzzle_texture.getSize().x / 2, puzzle_texture.getSize().y / 2);
          puzzle_sprite->setPosition(window.getSize().x / 2, window.getSize().y / 2);
          //puzzle_sprite->setScale(static_cast<float>(window.getSize().x) / static_cast<float>(puzzle_texture->getSize().x) / 2,
                                 //static_cast<float>(window.getSize().y) / static_cast<float>(puzzle_texture->getSize().y) / 2);
        }

        if (event.key.code == sf::Keyboard::Enter) {
          puzzle_screen(window, cur_image + 1);
        }

        if (event.key.code == sf::Keyboard::Escape) {
          break;
        }
      }
    }

    window.clear();
    window.draw(*puzzle_sprite);
    window.display();
  }

  return 0;
}

int hello_screen(sf::RenderWindow& window) {
  std::string pr_dir = "D:\\Projects\\Puzzle";

  button to_level_b(30, 30, 200, 50, "To levels"); //button 'To levels'
  button exit_b(30, 100, 200, 50, "Exit"); //button 'Exit'
  sf::Vector2i mouse_pos;

  sf::Texture bg_texture;
  if (!bg_texture.loadFromFile(pr_dir + "\\pictures\\bg.jpg")) {
    return 1;
  }
  sf::Sprite bg_sprite(bg_texture);
  bg_sprite.setPosition(0,0);
  bg_sprite.setScale(static_cast<float>(window.getSize().x) / static_cast<float>(bg_sprite.getTexture()->getSize().x),
                     static_cast<float>(window.getSize().y) / static_cast<float>(bg_sprite.getTexture()->getSize().y));
  while (window.isOpen()) {
    sf::Event event;

    if (event.mouseMove.x != 0 && event.mouseMove.x != 1) {
      mouse_pos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
    }

    if (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          if (event.mouseMove.x != 0 && event.mouseMove.x != 1) {
            mouse_pos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
          }

          if (to_level_b.inButton(mouse_pos)) {
            levels_screen(window);
          }

          if (exit_b.inButton(mouse_pos)) {
            window.close();
            break;
          }
        }
      }
    }

    window.clear();
    window.draw(bg_sprite);
    to_level_b.draw(window);
    exit_b.draw(window);
    window.display();
  }

  return 0;
}
