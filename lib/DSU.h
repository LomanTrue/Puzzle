#include <cstdint>

class DSU {
 private:
  uint16_t* parent;
  uint16_t parent_size;
  uint16_t* rank;
  uint16_t rank_size;

 public:
  DSU(uint32_t size) {
    parent = new uint16_t[size];
    rank = new uint16_t[size];
    parent_size = size;
    rank_size = size;

    for (uint32_t i = 0; i < size; i++) {
      parent[i] = i;
      rank[i] = 1;
    }
  }

  ~DSU() {
    delete parent;
    delete rank;
  }

  uint16_t find_set(uint16_t v);
  void union_sets(uint16_t a, uint16_t b);
  uint16_t getSize(uint16_t v);
};
