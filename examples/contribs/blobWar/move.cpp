#include "move.h"
#include "board.h"
#include "qstringlist.h"
#include "qregexp.h"

Move::Move(const QPoint& s, const QPoint& e)
  : start(s), end(e)
{}

Move::Move(const Board* const b, int s, int e)
{
  start = b->pointFromInt(s);
  end   = b->pointFromInt(e);
}

Move::Move(const QString text)
{
#if QT_VERSION < 0x040000
  QStringList list = QStringList::split(QRegExp("\\D"), text);
#else
  QStringList list = text.split(QRegExp("\\D"), QString::SkipEmptyParts);
#endif

  start = QPoint(list[0].toInt(), list[1].toInt());
  end = QPoint(list[2].toInt(), list[3].toInt());
}

bool Move::isValid(const Board* const b) const
{
  return (b->isValid(start) &&
	  b->isValid(end) &&
	  abs(start.x()-end.x()) <= 2 &&
	  abs(start.y()-end.y()) <= 2 &&
	  start != end &&
	  b->stateOf(start)==Board::blueColor(b->bluePlays()) &&
	  b->stateOf(end)==Board::EMPTY);
}

bool Move::close() const
{
  QPoint delta = start - end;
  return (abs(delta.x()) < 2) && (abs(delta.y())<2);
}

int Move::numberOfNewPieces(const Board& b) const
{
  int res = 0;

  for (int i=-1; i<=1; ++i)
    for (int j=-1; j<=1; ++j)
      {
	const QPoint p(end.x()+i, end.y()+j);
	if (b.isValid(p) && b.stateOf(p) == Board::blueColor(!b.bluePlays()))
	  res++;
      }

  if (close())
    res++;
  
  return res;
}

std::ostream& operator<<(std::ostream& out, const Move& m)
{
  out << "(" << m.start.x() << "," << m.start.y() << ") -> (" << m.end.x() << ","<< m.end.y() << ")" << std::endl;
  return out;
}

