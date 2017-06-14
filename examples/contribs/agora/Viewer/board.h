#ifndef BOARD_H
#define BOARD_H

#include "case.h"
#include "move.h"
#include "undo.h"

#include <qsize.h>
#include <qstring.h>
#if QT_VERSION < 0x040000
#include <qvaluevector.h>
#else
#include <qvector.h>
#endif

class Board {
  friend class Move;

public:
  Board() : board_(NULL){};

  void draw() const;
  void drawSelectedPiece(int piece) const;
  void drawPossibleDestinations(int piece, bool select = false) const;
  void drawSelectablePieces() const;

  // Used for animation
  // void doNotDrawPiece(const QPoint& p) { board_[p.x()][p.y()] = Board::EMPTY;
  // };  void doDrawPiece(const QPoint& p) { board_[p.x()][p.y()] =
  // blackColor(blackPlays_); };  void drawFlippingPieces(const QPoint& p, bool
  // flip) const;

  void play(const Move &m);

  QSize size() const { return QSize(sizeX_, sizeY_); };

  QString statusMessage() const;

  bool canBeSelected(const QPoint &p) const;
  bool canBeSelected(int i) const { return canBeSelected(pointFromInt(i)); }

  bool isValid(const QPoint &p) const;

  bool gameIsOver() const;
  bool blackPlays() const { return blackPlays_; };
  int nbMovesLeft() const { return nbMovesLeft_; };

  bool undo();
  bool redo();

  Case &caseAt(const QPoint p) const { return board_[p.x()][p.y()]; };

  QString stateString() const;
  void initFromStateString(const QString &s);

  Move randomMove(bool black) const;

  // Input-Output
  friend std::ostream &operator<<(std::ostream &out, const Board &p);
  friend std::istream &operator>>(std::istream &in, Board &b);

private:
  void resize(int sizeX, int sizeY);
  bool pieceCanMove(const QPoint &p) const;

  int intFromPoint(const QPoint &p) const { return p.x() * sizeY_ + p.y(); };
  QPoint pointFromInt(int i) const { return QPoint(i / sizeY_, i % sizeY_); };

  int sizeX_, sizeY_;
  int nbMovesLeft_;
  Case **board_;
  bool blackPlays_;
  Undo undo_;
};

#endif // BOARD_H
