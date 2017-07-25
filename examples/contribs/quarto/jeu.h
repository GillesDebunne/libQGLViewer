#ifndef JEU_H
#define JEU_H

#include "piece.h"

struct cell {
  bool empty;
  Piece *piece;
};

class Jeu {
protected:
  struct cell tab[16];

public:
  Jeu() {}
  ~Jeu() {}

  void init() {
    for (int i = 0; i < 16; i++) {
      tab[i].empty = true;
      tab[i].piece = NULL;
    }
  }
  bool needDrawing(int);
  void placePiece(int, Piece *);
  bool caracCommune(Piece *pieces[4]);
  bool analyze();
};

#endif // JEU_H
