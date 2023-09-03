#include "puzzle.h"

sf::Vector2i RandomPos() {
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
  if (InRange(static_cast<float>(sf::Mouse::getPosition().x), sprite.getPosition().x - width / 2, sprite.getPosition().x + width / 2) &&
      InRange(static_cast<float>(sf::Mouse::getPosition().y), sprite.getPosition().y - height / 2, sprite.getPosition().y + height / 2)) {
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

uint8_t piece::getRotation() {
  return rot;
}

void piece::setRotation(uint8_t rot_) {
  sprite.rotate(90 * (rot_ - rot));
  rot = rot_;
}

void piece::incRotation() {
  setRotation((rot + 1) % 4);
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
      pieces[i][j].setPosition(RandomPos());
      if (isEnableRotation()) {
        pieces[i][j].setRotation(std::rand() % 4);
      }
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

bool puzzle::isOnePieceInUnion(piece& piece_) {
  return dsu_of_pieces.getSize(piece_.getIndex()) == 1;
}

bool puzzle::isEnableRotation() {
  return enableRotation;
}

std::vector<sf::Vector2i> puzzle::getPiecesInUnion(piece& piece_) {
  uint16_t union_size = dsu_of_pieces.getSize(piece_.getIndex());
  std::vector<sf::Vector2i> pieces_in_union;

  bool visited_pieces[height][width];
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      visited_pieces[i][j] = false;
    }
  }

  std::queue<sf::Vector2i> queue;
  queue.push(piece_.getPositionInPuzzleMatrix());

  while (!queue.empty()) {
    sf::Vector2i cur_pos = queue.front();
    pieces_in_union.push_back(cur_pos);
    queue.pop();
    visited_pieces[cur_pos.x][cur_pos.y] = true;

    uint16_t index = cur_pos.x * width + cur_pos.y;

    if (cur_pos.y != 0 && !visited_pieces[cur_pos.x][cur_pos.y - 1] && dsu_of_pieces.find_set(index) == dsu_of_pieces.find_set(index - 1)) {
      queue.push(sf::Vector2i(cur_pos.x, cur_pos.y - 1));
    }
    if (cur_pos.y != width - 1 && !visited_pieces[cur_pos.x][cur_pos.y + 1] && dsu_of_pieces.find_set(index) == dsu_of_pieces.find_set(index + 1)) {
      queue.push(sf::Vector2i(cur_pos.x, cur_pos.y + 1));
    }
    if (cur_pos.x != 0 && !visited_pieces[cur_pos.x - 1][cur_pos.y] && dsu_of_pieces.find_set(index) == dsu_of_pieces.find_set(index - width)) {
      queue.push(sf::Vector2i(cur_pos.x - 1, cur_pos.y));
    }
    if (cur_pos.x != height - 1 && !visited_pieces[cur_pos.x + 1][cur_pos.y] && dsu_of_pieces.find_set(index) == dsu_of_pieces.find_set(index + width)) {
      queue.push(sf::Vector2i(cur_pos.x + 1, cur_pos.y));
    }
  }

  return pieces_in_union;
}

void puzzle::connectPieces(piece& piece_) {
  std::vector<sf::Vector2i> pieces_in_union = getPiecesInUnion(piece_);

  for (int i = 0; i < dsu_of_pieces.getSize(piece_.getIndex()); i++) {
    piece& cur_piece = pieces[pieces_in_union[i].x][pieces_in_union[i].y];
    uint16_t index = cur_piece.getIndex();
    uint8_t row = cur_piece.getPositionInPuzzleMatrix().x;
    uint8_t col = cur_piece.getPositionInPuzzleMatrix().y;

    if (col != 0) {
      if (dsu_of_pieces.find_set(index) != dsu_of_pieces.find_set(index - 1) &&
          cur_piece.getRotation() == 0 && pieces[row][col - 1].getRotation() == 0 &&
          InRange(cur_piece.getPosition().x,
                  pieces[row][col - 1].getPosition().x + pieces[row][col - 1].getSize().x - 5,
                  pieces[row][col - 1].getPosition().x + pieces[row][col - 1].getSize().x + 5) &&
          InRange(cur_piece.getPosition().y,
                  pieces[row][col - 1].getPosition().y - 5,
                  pieces[row][col - 1].getPosition().y + 5)) {
        this->setPosition(pieces[row][col - 1], sf::Vector2f(cur_piece.getPosition().x - cur_piece.getSize().x,
                                                             cur_piece.getPosition().y));
        dsu_of_pieces.union_sets(index, index - 1);

        return;
      }
    }

    if (col != width - 1) {
      if (dsu_of_pieces.find_set(index) != dsu_of_pieces.find_set(index + 1) &&
          cur_piece.getRotation() == 0 && pieces[row][col + 1].getRotation() == 0 &&
          InRange(cur_piece.getPosition().x,
                  pieces[row][col + 1].getPosition().x - pieces[row][col + 1].getSize().x - 5,
                  pieces[row][col + 1].getPosition().x - pieces[row][col + 1].getSize().x + 5) &&
          InRange(cur_piece.getPosition().y,
                  pieces[row][col + 1].getPosition().y - 5,
                  pieces[row][col + 1].getPosition().y + 5)) {
        this->setPosition(pieces[row][col + 1], sf::Vector2f(cur_piece.getPosition().x + cur_piece.getSize().x,
                                                             cur_piece.getPosition().y));
        dsu_of_pieces.union_sets(index, index + 1);

        return;
      }
    }

    if (row != 0) {
      if (dsu_of_pieces.find_set(index) != dsu_of_pieces.find_set(index - static_cast<uint16_t>(width)) &&
          cur_piece.getRotation() == 0 && pieces[row - 1][col].getRotation() == 0 &&
          InRange(cur_piece.getPosition().x,
                  pieces[row - 1][col].getPosition().x - 5,
                  pieces[row - 1][col].getPosition().x + 5) &&
          InRange(cur_piece.getPosition().y,
                  pieces[row - 1][col].getPosition().y + pieces[row - 1][col].getSize().y - 5,
                  pieces[row - 1][col].getPosition().y + pieces[row - 1][col].getSize().y + 5)) {
        this->setPosition(pieces[row - 1][col], sf::Vector2f(cur_piece.getPosition().x,
                                                             cur_piece.getPosition().y - cur_piece.getSize().y));
        dsu_of_pieces.union_sets(index, index - static_cast<uint16_t>(width));

        return;
      }
    }

    if (row != height - 1) {
      if (dsu_of_pieces.find_set(index) != dsu_of_pieces.find_set(index + static_cast<uint16_t>(width)) &&
          cur_piece.getRotation() == 0 && pieces[row + 1][col].getRotation() == 0 &&
          InRange(cur_piece.getPosition().x,
                  pieces[row + 1][col].getPosition().x - 5,
                  pieces[row + 1][col].getPosition().x + 5) &&
          InRange(cur_piece.getPosition().y,
                  pieces[row + 1][col].getPosition().y - pieces[row + 1][col].getSize().y - 5,
                  pieces[row + 1][col].getPosition().y - pieces[row + 1][col].getSize().y + 5)) {
        this->setPosition(pieces[row + 1][col], sf::Vector2f(cur_piece.getPosition().x,
                                                             cur_piece.getPosition().y + cur_piece.getSize().y));
        dsu_of_pieces.union_sets(index, index + static_cast<uint16_t>(width));

        return;
      }
    }
  }
}

bool puzzle::isPuzzleSolved() {
  if (dsu_of_pieces.getSize(0) == size) {
    return true;
  } else {
    return false;
  }
}