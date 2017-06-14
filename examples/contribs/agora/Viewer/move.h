#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <qpoint.h>

class Board;

class Move {
public:
  Move(){};
  Move(const QPoint &s, const QPoint &e, bool under);
  Move(const Board *const b, int s, int e, bool under);
  Move(const QString text);

  bool isValid(const Board *const b) const;
  void updateBoard(const Board *b) const;

  friend std::ostream &operator<<(std::ostream &out, const Move &m);
  friend std::istream &operator>>(std::istream &in, Move &m);

  const QPoint &start() const { return start_; }
  const QPoint &end() const { return end_; }
  bool goesUnder() const { return under_; }

private:
  QPoint start_, end_;
  bool under_;
};

#endif // MOVE_H
