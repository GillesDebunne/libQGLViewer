#ifndef BOARD_H
#define BOARD_H

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
  void drawShadows() const;
  void drawSelectedPiece(int piece) const;
  void drawPossibleDestinations(int piece, bool select = false) const;
  void drawSelectablePieces() const;

  void play(const Move &m);

  QSize size() const { return QSize(sizeX_, sizeY_); };

  QString statusMessage() const;

  bool canBeSelected(int i);

  bool isValid(const QPoint &p) const;

  bool gameIsOver() const;
  bool bluePlays() const { return bluePlays_; };

  bool undo();
  bool redo();

#if QT_VERSION < 0x040000
  QValueVector<Move> possibleMoves(bool bluePlays) const;
#else
  QVector<Move> possibleMoves(bool bluePlays) const;
#endif
  Move bestMoveNumberOfNewPieces() const;

  enum State { EMPTY, RED, BLUE, HOLE };
  State stateOf(const QPoint p) const { return (State)board_[p.x()][p.y()]; };

  QString stateString() const;
  void initFromStateString(const QString &s);

  // Used for animation
  void doNotDrawPiece(const QPoint &p) { board_[p.x()][p.y()] = Board::EMPTY; };
  void doDrawPiece(const QPoint &p) {
    board_[p.x()][p.y()] = blueColor(bluePlays_);
  };
  void drawFlippingPieces(const QPoint &p, bool flip) const;

  static State blueColor(bool blue) { return blue ? Board::BLUE : Board::RED; };
  static void drawPiece(bool blue);

  // Input-Output
  friend std::ostream &operator<<(std::ostream &out, const Board &p);
  friend std::istream &operator>>(std::istream &in, Board &b);

private:
  void resize(int sizeX, int sizeY);
  bool pieceCanMove(const QPoint &p) const;
  void setStateOf(const QPoint &p, State s);

  int intFromPoint(const QPoint &p) const { return p.x() * sizeY_ + p.y(); };
  QPoint pointFromInt(int i) const { return QPoint(i / sizeY_, i % sizeY_); };

  static void drawBlock(int i, int j);
  static void drawSquare(const QPoint &p);
  static void drawPiece(const QPoint &p, bool blue);
  static void drawShadow(const QPoint &p);

  int sizeX_, sizeY_;
  int **board_;
  int nbBlue_, nbRed_;
  bool bluePlays_;
  Undo undo_;
};

#endif // BOARD_H
