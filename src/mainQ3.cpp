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
  size_t M = grid.getM(), N = grid.getN(), K = grid.getK();
  Var prop[M][N][K];

  Solver s;

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
    vec<Lit> lits;
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
          for (size_t e = 0; e < M - grid.getX(k) - 1; e++) {
            for (size_t f = 0; f < N - grid.getY(k) - 1; f++) {
              if (k != l) {
                s.addBinary(~Lit(prop[a][b][k]), ~Lit(prop[a + e][b + f][l]));
              }
            }
          }
        }
      }
    }
  }

  s.solve();

  if (s.okay()) {
    int i = 1;
    for (size_t m = 0; m < M; m++) {
      for (size_t n = 0; n < N; n++) {
        for (size_t k = 0; k < K; k++) {
          if (s.model[prop[m][n][k]] == l_True) {
            std::cout << i++ << '\t' << m << '\t' << n << std::endl;
          }
        }
      }
    }
  } else {
    std::cout << "Flute alors" << std::endl;
  }

  return 0;
}
