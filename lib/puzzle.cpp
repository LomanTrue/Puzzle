#include "puzzle.h"

sf::Vector2i Random() {
  int a = std::rand() % 1820;
  int b = std::rand() % 960;

  return sf::Vector2i(a,b);
}

bool InRange(int x, int left, int right) {
  return (x >= left) && (x <= right);
}

bool InRange(float x, float left, float right) {
  return (x >= left) && (x <= right);
}

void piece::setPosition(sf::Vector2f pos_) {
  sprite.setPosition(pos_);
}

void piece::setPosition(sf::Vector2i pos_) {
  sprite.setPosition(sf::Vector2f(pos_));
}

sf::Vector2f piece::getPosition() {
  return sprite.getPosition();
}

void piece::setTexture(sf::Texture texture_, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
  sprite.setTexture(texture_);
  sprite.setTextureRect(sf::IntRect(x, y, width, height));
}

sf::Sprite piece::draw() {
  return sprite;
}

bool piece::inPiece() {
  if (InRange(static_cast<float>(sf::Mouse::getPosition().x), sprite.getPosition().x, sprite.getPosition().x + sprite.getTextureRect().width) &&
      InRange(static_cast<float>(sf::Mouse::getPosition().y), sprite.getPosition().y, sprite.getPosition().y + sprite.getTextureRect().height)) {
    return true;
  } else {
    return false;
  }
}

uint16_t piece::getIndex() {
  return index;
}

sf::Vector2i piece::getPositionInPuzzleMatrix() {
  return sf::Vector2i(row,col);
}

sf::Vector2i piece::getSize() {
  return sf::Vector2i(width, height);
}

void puzzle::draw(sf::RenderWindow& window) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      window.draw(pieces[i][j].draw());
    }
  }
}

piece* puzzle::isMouseInPiece() {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (pieces[i][j].inPiece()) {
        return &(pieces[i][j]);
      }
    }
  }

  return nullptr;
}

void puzzle::shuffle() {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      pieces[i][j].setPosition(Random());
    }
  }
}

void puzzle::setPosition(piece& piece_, sf::Vector2f pos_) {
  uint16_t union_piece = dsu_of_pieces.find_set(piece_.getIndex());

  uint16_t count = dsu_of_pieces.getSize(union_piece);
  if (count == 1) {
    piece_.setPosition(pos_);
  } else {
    sf::Vector2f dpos = pos_ - piece_.getPosition();
    for (uint16_t i = 0; i < width * height; i++) {
      if (dsu_of_pieces.find_set(i) == union_piece) {
        uint16_t next_piece_row = i / width;
        uint16_t next_piece_col = i % width;
        piece& next_piece = pieces[next_piece_row][next_piece_col];
        next_piece.setPosition(next_piece.getPosition() + dpos);
        count--;

        if (count == 0) {
          break;
        }
      }
    }
  }
}

void puzzle::connectPieces(piece& piece_) {
  uint16_t index = piece_.getIndex();
  uint8_t row = piece_.getPositionInPuzzleMatrix().x;
  uint8_t col = piece_.getPositionInPuzzleMatrix().y;

  if (col != 0) {
    if (dsu_of_pieces.find_set(index) != dsu_of_pieces.find_set(index - 1) &&
        InRange(piece_.getPosition().x,
                pieces[row][col - 1].getPosition().x + pieces[row][col - 1].getSize().x - 5,
                pieces[row][col - 1].getPosition().x + pieces[row][col - 1].getSize().x + 5) &&
        InRange(piece_.getPosition().y,
                pieces[row][col - 1].getPosition().y - 5,
                pieces[row][col - 1].getPosition().y + 5)) {
      this->setPosition(pieces[row][col - 1], sf::Vector2f(piece_.getPosition().x - piece_.getSize().x,
                                                                  piece_.getPosition().y));
      dsu_of_pieces.union_sets(index, index - 1);
    }
  }

  if (col != width - 1) {
    if (dsu_of_pieces.find_set(index) != dsu_of_pieces.find_set(index + 1) &&
        InRange(piece_.getPosition().x,
                pieces[row][col + 1].getPosition().x - pieces[row][col + 1].getSize().x - 5,
                pieces[row][col + 1].getPosition().x - pieces[row][col + 1].getSize().x + 5) &&
        InRange(piece_.getPosition().y,
                pieces[row][col + 1].getPosition().y - 5,
                pieces[row][col + 1].getPosition().y + 5)) {
      this->setPosition(pieces[row][col + 1], sf::Vector2f(piece_.getPosition().x + piece_.getSize().x,
                                                                    piece_.getPosition().y));
      dsu_of_pieces.union_sets(index, index + 1);
    }
  }

  if (row != 0) {
    if (dsu_of_pieces.find_set(index) != dsu_of_pieces.find_set(index - static_cast<uint16_t>(width)) &&
        InRange(piece_.getPosition().x,
                pieces[row - 1][col].getPosition().x - 5,
                pieces[row - 1][col].getPosition().x + 5) &&
        InRange(piece_.getPosition().y,
                pieces[row - 1][col].getPosition().y + pieces[row - 1][col].getSize().y - 5,
                pieces[row - 1][col].getPosition().y + pieces[row - 1][col].getSize().y + 5)) {
      this->setPosition(pieces[row - 1][col], sf::Vector2f(piece_.getPosition().x,
                                                                    piece_.getPosition().y - piece_.getSize().y));
      dsu_of_pieces.union_sets(index, index - static_cast<uint16_t>(width));
    }
  }

  if (row != height - 1) {
    if (dsu_of_pieces.find_set(index) != dsu_of_pieces.find_set(index + static_cast<uint16_t>(width)) &&
        InRange(piece_.getPosition().x,
                pieces[row + 1][col].getPosition().x - 5,
                pieces[row + 1][col].getPosition().x + 5) &&
        InRange(piece_.getPosition().y,
                pieces[row + 1][col].getPosition().y - pieces[row + 1][col].getSize().y - 5,
                pieces[row + 1][col].getPosition().y - pieces[row + 1][col].getSize().y + 5)) {
      this->setPosition(pieces[row + 1][col], sf::Vector2f(piece_.getPosition().x,
                                                                    piece_.getPosition().y + piece_.getSize().y));
      dsu_of_pieces.union_sets(index, index + static_cast<uint16_t>(width));
    }
  }
}