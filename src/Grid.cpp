#include <iostream>
#include <string>
#include <vector>

class Grid {
private:
  int k, m, n;
  std::vector<int> x, y;

public:
  Grid() = default;
  ~Grid() = default;

  void init_from_stdin() {
    int i = 0, addX, addY;
    std::cin >> k;
    std::cin >> m;
    std::cin >> n;
    while (i < k) {
      std::cin >> i >> addX >> addY;
      x.push_back(addX);
      y.push_back(addY);
    }
  }

  std::vector<int> const &getX() const { return x; }

  std::vector<int> const &getY() const { return y; }

  int const getK() const { return k; }

  int const getM() const { return m; }

  int const getN() const { return n; }
};
