#include "Grid.cpp"
#include "minisat/Solver.hpp"
#include <cstddef>
#include <iostream>

int main() {
  Grid grid;
  grid.init_from_stdin();
  size_t M = grid.getM(), N = grid.getN(), K = grid.getK();
  Var prop[M][N][K];
  vec<Lit> lits;

  std::cout << "M: " << M << ", N: " << N << std::endl;

  for (auto it = grid.getX().begin(); it < grid.getX().end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  for (auto it = grid.getY().begin(); it < grid.getY().end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  Solver s;

  // Initialize prop vector
  for (size_t i = 0; i < M; i++) {
    for (size_t j = 0; j < N; j++) {
      for (size_t k = 0; k < K; k++) {
        prop[i][j][k] = s.newVar();
      }
    }
  }

  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < M; a++) {
      for (size_t b = 0; b < N; b++) {
        for (size_t k2 = 0; k2 < K; k2++) {
          if (k != k2) {
            s.addBinary(~Lit(prop[a][b][k]), ~Lit(prop[a][b][k2]));
          }
        }
        for (size_t a2 = 0; a2 < M; a2++) {
          if (a != a2) {
            s.addBinary(~Lit(prop[a][b][k]), ~Lit(prop[a2][b][k]));
          }
        }
        for (size_t b2 = 0; b2 < N; b2++) {
          if (b != b2) {
            s.addBinary(~Lit(prop[a][b][k]), ~Lit(prop[a][b2][k]));
          }
        }
      }
    }
  }

  // First constraint: all rectangles used
  for (size_t k = 0; k < K; k++) {
    lits.clear();
    for (size_t a = 0; a < M - grid.getX(k) + 1; a++) {
      for (size_t b = 0; b < N - grid.getY(k) + 1; b++) {
        lits.push(Lit(prop[a][b][k]));
      }
    }
    s.addClause(lits);
  }

  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < M; a++) {
      for (size_t b = 0; b < N; b++) {
        if (a + grid.getX(k) - 1 < M && b + grid.getY(k) - 1 < N) {
          for (size_t da = 0; da < grid.getX(k); da++) {
            for (size_t db = 0; db < grid.getY(k); db++) {
              for (size_t k2 = 0; k2 < K; k2++) {
                if ((da == 0 && db == 0) || k == k2) {
                  continue;
                } else {
                  s.addBinary(~Lit(prop[a][b][k]),
                              ~Lit(prop[a + da][b + db][k2]));
                }
              }
            }
          }
        }
      }
    }
  }

  for (size_t k = 0; k < K; k++) {
    for (size_t a = M - grid.getX(k) + 1; a < M; a++) {
      for (size_t b = N - grid.getY(k) + 1; b < N; b++) {
        s.addUnit(~Lit(prop[a][b][k]));
      }
    }
  }

  //// Second constraint : no overlapping
  // for (size_t k = 0; k < K; k++) {
  //  for (size_t a = 0; a <= M - grid.getX(k); a++) {
  //    for (size_t b = 0; b <= N - grid.getY(k); b++) {
  //      for (size_t l = 0; l < K; l++) {
  //        if (k != l) {
  //          for (size_t e = a; e < a + grid.getX(k); e++) {
  //            for (size_t f = b; f < b + grid.getY(k); f++) {
  //              s.addBinary(~Lit(prop[a][b][k]), ~Lit(prop[e][f][l]));
  //            }
  //          }
  //        }
  //      }
  //    }
  //  }
  //}

  s.solve();

  if (s.okay()) {
    for (size_t k = 0; k < K; k++) {
      for (size_t m = 0; m < M; m++) {
        for (size_t n = 0; n < N; n++) {
          if (s.model[prop[m][n][k]] == l_True) {
            std::cout << k + 1 << '\t' << m << '\t' << n << std::endl;
          }
        }
      }
    }
  } else {
    std::cout << "Flute alors" << std::endl;
  }

  return 0;
}
