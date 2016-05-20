#include "Grid.cpp"
#include "minisat/Solver.hpp"
#include <cstddef>
#include <iostream>

int main(int argc, char const *argv[]) {
  Grid g;
  g.init_from_stdin();
  size_t K = g.getK(), N = g.getN(), M = g.getM();

  Solver s;
  vec<Lit> lits;

  // 3 dimensions : numéro du rectangle et coordonnées en largeur et longueur
  Var prop[K][N][M];

  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        prop[k][a][b] = s.newVar();
      }
    }
  }

  // Unicité : rectangle utilisé pas plus d'une fois
  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        for (size_t k2 = 0; k2 < K; k2++) {
          if (k != k2) {
            s.addBinary(~Lit(prop[k][a][b]), ~Lit(prop[k2][a][b]));
          }
        }
        for (size_t a2 = 0; a2 < N; a2++) {
          if (a != a2) {
            s.addBinary(~Lit(prop[k][a][b]), ~Lit(prop[k][a2][b]));
          }
        }
        for (size_t b2 = 0; b2 < M; b2++) {
          if (b != b2) {
            s.addBinary(~Lit(prop[k][a][b]), ~Lit(prop[k][a][b2]));
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
        lits.push(Lit(prop[k][a][b]));
      }
    }
    s.addClause(lits);
  }

  // chevauchement : les rectangles ne se chevauchent pas
  for (size_t k = 0; k < K; k++) {
    for (size_t a = 0; a < N; a++) {
      for (size_t b = 0; b < M; b++) {
        if (a + g.getX(k) - 1 < N && b + g.getY(k) - 1 < M) {
          for (size_t da = 0; da < g.getX(k); da++) {
            for (size_t db = 0; db < g.getY(k); db++) {
              for (size_t k2 = 0; k2 < K; k2++) {
                if (!(da == 0 && db == 0)) {
                  s.addBinary(~Lit(prop[k][a][b]),
                              ~Lit(prop[k2][a + da][b + db]));
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
        s.addUnit(~Lit(prop[k][a][b]));
      }
    }
    for (size_t a = 0; a < N - g.getX(k) + 1; a++) {
      for (size_t b = M - g.getY(k); b < M; b++) {
        s.addUnit(~Lit(prop[k][a][b]));
      }
    }
  }

  s.solve();

  if (s.okay()) {
    std::cout << "SAT" << std::endl;
    for (size_t k = 0; k < K; k++) {
      for (size_t n = 0; n < N; n++) {
        for (size_t m = 0; m < M; m++) {
          if (s.model[prop[k][n][m]] == l_True) {
            std::cout << k + 1 << '\t' << m << '\t' << n << std::endl;
          }
        }
      }
    }
  } else {
    std::cout << "UNSAT" << std::endl;
  }

  return 0;
}
