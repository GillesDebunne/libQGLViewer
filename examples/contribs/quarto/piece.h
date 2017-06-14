#ifndef PIECE_H
#define PIECE_H

#include <iostream>
#include <math.h>
#include <qgl.h>

/*
 * Classe decrivant une piece
 */
class Piece {
private:
  int id;
  bool selected;
  bool fenetre;
  bool couleur;
  bool taille;
  bool forme;
  bool trou;
  double x_pos, y_pos;
  GLfloat amb_diff[3];
  GLfloat specular[3];
  GLfloat shininess;
  GLuint texture;

  void drawRectangle();
  void drawCylinder();
  void drawBoite();
  void drawBoule();

public:
  Piece(int, bool, bool, bool, bool, double, double);
  ~Piece() {}
  void paint(bool);
  void setSelected(bool s) { selected = s; }
  void setFenetre(bool f) { fenetre = f; }
  void setPos(double x, double y) {
    x_pos = x;
    y_pos = y;
  }
  void setTexture(GLuint t) { texture = t; }
  bool getTaille() { return taille; }
  bool getCouleur() { return couleur; }
  bool getForme() { return forme; }
  bool getTrou() { return trou; }
};

/*
 * Ensemble des pieces du jeu
 */
class SetOfPiece {
private:
  Piece *tab[16];
  int selected;

public:
  SetOfPiece();
  ~SetOfPiece() {
    for (int i = 0; i < 16; i++)
      delete (tab[i]);
  }
  void init();
  void paint(bool);
  void setSelected(int);
  void setTexture(GLuint);
  void placeSelectedPiece(int);
  Piece *getPiece() { return tab[selected]; }
};

#endif // PIECE_H
