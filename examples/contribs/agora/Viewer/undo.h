#ifndef UNDO_H
#define UNDO_H

// To get QT_VERSION
#include "qglobal.h"
#include <iostream>

#if QT_VERSION < 0x040000
#include <qvaluevector.h>
#else
#include <qstringlist.h>
#endif

class Undo {
public:
  Undo() { clear(); }

  void clear();
  void addState(const QString &s);

  QString undoState();
  QString redoState();

  bool isEmpty() const { return state_.isEmpty(); };

  int nbMoves() const { return index_; };

  friend std::ostream &operator<<(std::ostream &out, const Undo &u);
  friend std::istream &operator>>(std::istream &in, Undo &u);

private:
#if QT_VERSION < 0x040000
  QValueVector<QString> state_;
#else
  QStringList state_;
#endif
  int index_, maxIndex_;
};

#endif // UNDO_H
