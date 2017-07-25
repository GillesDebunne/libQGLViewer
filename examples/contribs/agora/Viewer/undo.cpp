#include "undo.h"
#include <qstring.h>

void Undo::clear() {
  index_ = 0;
  maxIndex_ = 0;
  state_.clear();
}

void Undo::addState(const QString &s) {
  if (index_ < int(state_.size()))
    state_[index_] = s;
  else
    state_.append(s);

  index_++;
  maxIndex_ = index_;
}

QString Undo::undoState() {
  if (index_ > 1) {
    index_--;
    return state_[index_ - 1];
  } else
    return QString();
}

QString Undo::redoState() {
  if (index_ < maxIndex_) {
    index_++;
    return state_[index_ - 1];
  } else
    return QString();
}

std::ostream &operator<<(std::ostream &out, const Undo &u) {
  out << std::endl << u.index_ << ' ' << u.maxIndex_ << std::endl;

  for (int i = 0; i < u.maxIndex_; ++i)
#if QT_VERSION < 0x040000
    out << u.state_[i].ascii() << std::endl;
#else
    out << u.state_[i].toLatin1().constData() << std::endl;
#endif

  return out;
}

std::istream &operator>>(std::istream &in, Undo &u) {
  u.state_.clear();
  u.index_ = u.maxIndex_ = 0;
  in >> u.index_ >> u.maxIndex_;

  char str[10000];
  for (int i = 0; i < u.maxIndex_; ++i) {
    in >> str;
    u.state_.append(str);
  }

  return in;
}
