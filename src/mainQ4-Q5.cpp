#include "Grid.cpp"
#include "minisat/Solver.hpp"
#include <cstddef>
#include <iostream>

void run(Grid &grid, size_t size) {
  grid.toSquare(size);

  size_t M = grid.getM(), N = grid.getN(), K = grid.getK();
  Var prop[M][N][K];
  vec<Lit> lits;

  Solver s;

  std::cout << "Testing with size " << size << std::endl;

  // Initialize prop vector
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; j++) {
      for (size_t k = 0; k < K; k++) {
        prop[i][j][k] = s.newVar();
      }
    }
  }

  // First constraint: all rectangles used and inside the grid
  for (size_t k = 0; k < K; ++k) {
    lits.clear();
    for (size_t a = 0; a < M - grid.getX(k); a++) {
      for (size_t b = 0; b < N - grid.getY(k); b++) {
        lits.push(Lit(prop[a][b][k]));
      }
    }
    s.addClause(lits);
  }

  // Second constraint: no overlapping
  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < M - grid.getX(k); a++) {
      for (size_t b = 0; b < N - grid.getY(k); b++) {
        for (size_t l = 0; l < K; l++) {
          if (k != l) {
            for (size_t e = a; e < a + grid.getX(k); e++) {
              for (size_t f = b; f < b + grid.getY(k); f++) {
                s.addBinary(~Lit(prop[a][b][k]), Lit(prop[e][f][l]));
              }
            }
          }
        }
      }
    }
  }

  s.solve();

  if (s.okay()) {
    std::cout << "Minimal square: " << size << std::endl;
  } else {
    std::cout << "Failed with size " << size << std::endl;
    run(grid, size + 1);
  }
}

int main() {
  Grid grid;
  grid.init_from_stdin();
  int max_x = grid.maxX();
  int max_y = grid.maxY();

  run(grid, max_x < max_y ? max_y : max_x);
  return 0;
}
