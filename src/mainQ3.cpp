#include "Grid.cpp"
#include "minisat/Solver.hpp"
#include <cstddef>
#include <iostream>

void print_grid(Grid grid) {
  std::vector<int> x = grid.getX(), y = grid.getY();
  for (size_t i = 0; i < x.size(); ++i) {
    std::cout << i + 1 << '\t' << x[i] << '\t' << y[i] << std::endl;
  }
}

int main() {
  Grid grid;
  grid.init_from_stdin();

  print_grid(grid);
}
