#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

class Grid {
private:
  size_t k, m, n;
  std::vector<int> x, y;

public:
  Grid() = default;
  explicit Grid(size_t i) {
    k = i;
    for (int j = 1; j <= i; ++j) {
      x.push_back(j);
      y.push_back(j);
    }
  }
  ~Grid() = default;

  void init_from_stdin() {
    int i = 0, addX, addY;
    std::cin >> k;
    std::cin >> n;
    std::cin >> m;
    while (i < k) {
      std::cin >> i >> addX >> addY;
      x.push_back(addX);
      y.push_back(addY);
    }
  }

  std::vector<int> const &getX() const { return x; }

  std::vector<int> const &getY() const { return y; }

  int const getX(size_t i) const { return x[i]; }

  int const getY(size_t j) const { return y[j]; }

  size_t const getK() const { return k; }

  size_t const getM() const { return m; }

  size_t const getN() const { return n; }

  void toSquare(size_t s) {
    m = s;
    n = s;
  }

  int maxX() const { return *std::max_element(std::begin(x), std::end(x)); }

  int maxY() const { return *std::max_element(std::begin(y), std::end(y)); }
};
