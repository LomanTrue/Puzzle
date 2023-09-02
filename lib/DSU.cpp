#include "DSU.h"

uint16_t DSU::find_set(uint16_t v) {
  if (v == parent[v]) {
    return v;
  }
  return parent[v] = find_set(parent[v]);
}

void DSU::union_sets(uint16_t a, uint16_t b) {
  a = find_set(a);
  b = find_set(b);
  if (a != b) {
    if (rank[a] < rank[b]) {
      parent[a] = b;
      rank[b] += a;
    } else {
      parent[b] = a;
      rank[a] += b;
    }
  }
}

uint16_t DSU::getSize(uint16_t v) {
  return rank[find_set(v)];
}