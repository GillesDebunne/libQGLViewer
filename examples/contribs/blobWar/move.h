#ifndef MOVE_H
#define MOVE_H

#include <qpoint.h>
#include <iostream>

class Board;

class Move
{
  friend class Board;
  
public:
  Move() {};
  Move(const QPoint& s, const QPoint& e);
  Move(const Board* const b, int s, int e);
  Move(const QString text);

  bool isValid(const Board* const b) const;  
  bool close() const;

  int numberOfNewPieces(const Board& b) const;

  friend std::ostream& operator<<(std::ostream& out, const Move& m);

private:
  QPoint start, end;
};

#endif // MOVE_H
