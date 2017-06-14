#include "jeu.h"

bool Jeu::needDrawing(int i) { return tab[i].empty; }

void Jeu::placePiece(int select, Piece *p) {
  // on verifie que la piece n'ai pas été déja placée autre part
  for (int i = 0; i < 16; i++)
    if (tab[i].piece == p) {
      if (i == select)
        return; // la piece a déja été placée au bon endroit
      // si oui, on l'enlève
      tab[i].empty = true;
      tab[i].piece = NULL;
    }
  // on place la piece
  tab[select].empty = false;
  tab[select].piece = p;
}

// Vrai ssi les 4 pieces ont unbe caractéristique commune
bool Jeu::caracCommune(Piece *pieces[4]) {
  for (int i = 0; i < 4; ++i)
    if (!pieces[i])
      return false;

  // sinon on teste les caracteristiques communes
  // la taille
  if (pieces[0]->getTaille() == pieces[1]->getTaille() &&
      pieces[0]->getTaille() == pieces[2]->getTaille() &&
      pieces[0]->getTaille() == pieces[3]->getTaille())
    return true;
  // la couleur
  if (pieces[0]->getCouleur() == pieces[1]->getCouleur() &&
      pieces[0]->getCouleur() == pieces[2]->getCouleur() &&
      pieces[0]->getCouleur() == pieces[3]->getCouleur())
    return true;
  // la forme
  if (pieces[0]->getForme() == pieces[1]->getForme() &&
      pieces[0]->getForme() == pieces[2]->getForme() &&
      pieces[0]->getForme() == pieces[3]->getForme())
    return true;
  // le trou
  if (pieces[0]->getTrou() == pieces[1]->getTrou() &&
      pieces[0]->getTrou() == pieces[2]->getTrou() &&
      pieces[0]->getTrou() == pieces[3]->getTrou())
    return true;

  return false;
}

// Analyse si un alignement a été effectué
bool Jeu::analyze() {
  Piece *pieces[4];

  // on teste les lignes //
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; ++i)
      pieces[i] = tab[j * 4 + i].piece;
    if (caracCommune(pieces))
      return true;
  }

  // on teste les colonnes //
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; ++i)
      pieces[i] = tab[4 * i + j].piece;
    if (caracCommune(pieces))
      return true;
  }

  // on teste les diagonales //
  for (int i = 0; i < 4; ++i)
    pieces[i] = tab[5 * i].piece;
  if (caracCommune(pieces))
    return true;

  for (int i = 0; i < 4; ++i)
    pieces[i] = tab[3 * (i + 1)].piece;
  if (caracCommune(pieces))
    return true;

  return false;
}
