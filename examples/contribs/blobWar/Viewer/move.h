#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <qpoint.h>

class Board;

class Move {
public:
  Move(){};
  Move(const QPoint &s, const QPoint &e);
  Move(const Board *const b, int s, int e);
  Move(const QString text);

  bool isValid(const Board *const b) const;
  bool isClose() const;

  int numberOfNewPieces(const Board &b) const;

  friend std::ostream &operator<<(std::ostream &out, const Move &m);

  const QPoint &start() const { return start_; }
  const QPoint &end() const { return end_; }

private:
  QPoint start_, end_;
};

#endif // MOVE_H
