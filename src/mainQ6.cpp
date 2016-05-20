#include "Grid3D.cpp"
#include "minisat/Solver.hpp"
#include <cstddef>
#include <iostream>

int main(int argc, char const *argv[]) {
  Grid3D g;
  g.init_from_stdin();
  size_t K = g.getK(), N = g.getN(), M = g.getM(), H = g.getH();

  Solver s;
  vec<Lit> lits;

  // 4 dimensions : numéro du rectangle et coordonnées en largeur et longueur et
  // hauteur
  Var prop[K][N][M][H];

  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        for (size_t c = 0; c < H; c++) {
          prop[k][a][b][c] = s.newVar();
        }
      }
    }
  }

  // Unicité : rectangle utilisé pas plus d'une fois
  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        for (size_t c = 0; c < H; c++) {
          for (size_t k2 = 0; k2 < K; k2++) {
            if (k != k2) {
              s.addBinary(~Lit(prop[k][a][b][c]), ~Lit(prop[k2][a][b][c]));
            }
          }
          for (size_t a2 = 0; a2 < N; a2++) {
            if (a != a2) {
              s.addBinary(~Lit(prop[k][a][b][c]), ~Lit(prop[k][a2][b][c]));
            }
          }
          for (size_t b2 = 0; b2 < M; b2++) {
            if (b != b2) {
              s.addBinary(~Lit(prop[k][a][b][c]), ~Lit(prop[k][a][b2][c]));
            }
          }
          for (size_t c2 = 0; c2 < H; c2++) {
            if (c != c2) {
              s.addBinary(~Lit(prop[k][a][b][c]), ~Lit(prop[k][a][b][c2]));
            }
          }
        }
      }
    }
  }

  // existence : tous les rectangles ajoutés
  for (size_t k = 0; k < K; k++) {
    lits.clear();
    for (size_t a = 0; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        for (size_t c = 0; c < H; c++) {
          lits.push(Lit(prop[k][a][b][c]));
        }
      }
    }
    s.addClause(lits);
  }

  // chevauchement : les rectangles ne se chevauchent pas
  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        for (size_t c = 0; c < H; c++) {
          if (a + g.getX(k) - 1 < N && b + g.getY(k) - 1 < M &&
              c + g.getZ(k) - 1 < H) {
            for (size_t da = 0; da < g.getX(k); da++) {
              for (size_t db = 0; db < g.getY(k); db++) {
                for (size_t dc = 0; dc < g.getZ(k); dc++) {
                  for (size_t k2 = 0; k2 < K; k2++) {
                    if (!(da == 0 && db == 0 && dc == 0)) {
                      s.addBinary(~Lit(prop[k][a][b][c]),
                                  ~Lit(prop[k2][a + da][b + db][c + dc]));
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  // dans les limites : les rectangles restent à l'intérieur du grand
  for (size_t k = 0; k < K; k++) {
    for (size_t a = N - g.getX(k) + 1; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        for (size_t c = 0; c < H; c++) {
          s.addUnit(~Lit(prop[k][a][b][c]));
        }
      }
    }
    for (size_t a = 0; a < N; a++) {
      for (size_t b = M - g.getY(k); b < M; b++) {
        for (size_t c = 0; c < H; c++) {
          s.addUnit(~Lit(prop[k][a][b][c]));
        }
      }
    }
    for (size_t a = 0; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        for (size_t c = H - g.getZ(k) - 1; c < H; c++) {
          s.addUnit(~Lit(prop[k][a][b][c]));
        }
      }
    }
  }

  s.solve();

  if (s.okay()) {
    std::cout << "SAT" << std::endl;
    for (size_t k = 0; k < K; k++) {
      for (size_t n = 0; n < N; n++) {
        for (size_t m = 0; m < M; m++) {
          for (size_t h = 0; h < H; h++) {
            if (s.model[prop[k][n][m][h]] == l_True) {
              std::cout << k + 1 << '\t' << n << '\t' << m << '\t' << h
                        << std::endl;
            }
          }
        }
      }
    }
  } else {
    std::cout << "UNSAT" << std::endl;
  }

  return 0;
}
