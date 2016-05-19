#include "Grid3D.cpp"
#include "minisat/Solver.hpp"
#include <cstddef>
#include <iostream>

int main() {
  Grid3D grid;
  grid.init_from_stdin();
  size_t M = grid.getM(), N = grid.getN(), H = grid.getH(), K = grid.getK();
  Var prop[M][N][H][K];
  vec<Lit> lits;

  Solver s;

  // Initialize prop vector
  for (size_t i = 0; i < M; ++i) {
    for (size_t j = 0; j < N; j++) {
      for (size_t h = 0; h < H; h++) {
        for (size_t k = 0; k < K; k++) {
          prop[i][j][h][k] = s.newVar();
        }
      }
    }
  }

  // First constraint: all rectangles used and inside the grid
  for (size_t k = 0; k < K; ++k) {
    lits.clear();
    for (size_t a = 0; a < M - grid.getX(k); a++) {
      for (size_t b = 0; b < N - grid.getY(k); b++) {
        for (size_t c = 0; c < H - grid.getZ(k); c++) {
          lits.push(Lit(prop[a][b][c][k]));
        }
      }
    }
    s.addClause(lits);
  }

  // Second constraint: no overlapping
  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < M - grid.getX(k); a++) {
      for (size_t b = 0; b < N - grid.getY(k); b++) {
        for (size_t c = 0; c < H - grid.getZ(k); c++) {
          for (size_t l = 0; l < K; l++) {
            if (k != l) {
              for (size_t e = a; e < a + grid.getX(k); e++) {
                for (size_t f = b; f < b + grid.getY(k); f++) {
                  for (size_t g = c; g < c + grid.getZ(k); g++) {
                    s.addBinary(~Lit(prop[a][b][c][k]), ~Lit(prop[e][f][g][l]));
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  // Third constraint: cannnot float
  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < M; a++) {
      for (size_t b = 0; b < N; b++) {
        for (size_t c = 1; c < H; c++) {
          for (size_t l = 0; l < K; l++) {
            if (k != l) {
              s.addBinary(~Lit(prop[a][b][c][k]), Lit(prop[a][b][c - 1][l]));
            }
          }
        }
      }
    }
  }

  s.solve();

  if (s.okay()) {
    for (size_t k = 0; k < K; k++) {
      for (size_t m = 0; m < M; m++) {
        for (size_t n = 0; n < N; n++) {
          for (size_t h = 0; h < H; h++) {
            if (s.model[prop[m][n][h][k]] == l_True) {
              std::cout << k + 1 << '\t' << m << '\t' << n << '\t' << h
                        << std::endl;
            }
          }
        }
      }
    }
  } else {
    std::cout << "Flute alors" << std::endl;
  }

  return 0;
}
