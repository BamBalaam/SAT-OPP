#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

class Grid3D {
private:
  size_t k, m, n, h;
  std::vector<int> x, y, z;

public:
  Grid3D() = default;
  ~Grid3D() = default;

  void init_from_stdin() {
    int i = 0, addX, addY, addZ;
    std::cin >> k;
    std::cin >> n;
    std::cin >> m;
    std::cin >> h;
    while (i < k) {
      std::cin >> i >> addY >> addX >> addZ;
      x.push_back(addX);
      y.push_back(addY);
      z.push_back(addZ);
    }
  }

  std::vector<int> const &getX() const { return x; }

  std::vector<int> const &getY() const { return y; }

  std::vector<int> const &getZ() const { return z; }

  int const getX(size_t i) const { return x[i]; }

  int const getY(size_t j) const { return x[j]; }

  int const getZ(size_t l) const { return z[l]; }

  size_t const getK() const { return k; }

  size_t const getM() const { return m; }

  size_t const getN() const { return n; }

  size_t const getH() const { return h; }
};
