#include "board.h"
#include <algorithm>
#include <functional>
#include <sstream>

using namespace std;
using namespace dvonn;

namespace {
void resetStatus(pair<Stack, int> &b) { b.second = -1; }
void clearStacks(pair<Stack, int> &b) { b.first.clear(); }
unsigned int hasLessPieces(const pair<Stack, int> &s,
                           const pair<Stack, int> &t) {
  return s.first.height() < t.first.height();
}
} // namespace
const char *dvonn::nameOf(const Color p) {
  return p == White ? "White" : (p == Red ? "Red" : "Black");
}
//************************************************************
// Implementation of Piece
//************************************************************
Color Piece::color() const { return color_; }
bool Piece::isWhite() const { return color_ == White; }
bool Piece::isBlack() const { return color_ == Black; }
bool Piece::isRed() const { return color_ == Red; }
bool Piece::is(Color c) const { return color_ == c; }
Piece::Piece(Color c) : color_(c) {}
ostream &operator<<(ostream &s, const Piece &p) {
  static const char *v[nbColors] = {"R", "W", "B"};
  return s << v[p.color()];
}
//************************************************************
// Implementation of Stack
//************************************************************
unsigned int Stack::height() const { return size(); }
bool Stack::hasRed() const {
  return find_if(begin(), end(), mem_fun(&Piece::isRed)) != end();
}
bool Stack::hasPieces() const { return !empty(); }
const Piece *Stack::onTop() const { return hasPieces() ? back() : NULL; }
Stack::Stack() {}
ostream &operator<<(ostream &s, const Stack &t) {
  s << (t.hasRed() ? '*' : ' ') << t.size();
  if (t.hasPieces())
    s << (*t.onTop());
  else
    s << ' ';
  return s;
}
//************************************************************
// Implementation of Board
//************************************************************
unsigned int Board::nbSpacesMaxOnRow() { return 11; }
unsigned int Board::nbRows() { return 5; }
bool Board::isValid(Coord c) {
  const int yL = nbRows() / 2;
  const int yR = nbRows() - nbRows() / 2;
  const int nC = nbRows() % 2;
  return ((c.y() >= 0 && c.y() < yL && c.x() >= 0 &&
           c.x() <= static_cast<int>(nbSpacesMaxOnRow()) - (yL - c.y()) - nC) ||
          (nC == 1 && c.y() == yL && c.x() >= 0 &&
           c.x() < static_cast<int>(nbSpacesMaxOnRow())) ||
          (c.y() >= yR && c.y() < static_cast<int>(nbRows()) &&
           c.x() >= c.y() - yR + nC &&
           c.x() < static_cast<int>(nbSpacesMaxOnRow())));
}
bool Board::areAligned(Coord c0, Coord c1) {
  const int dx = c1.x() - c0.x();
  const int dy = c1.y() - c0.y();
  return dx == 0 || dy == 0 || dx == dy;
}
unsigned int Board::distance(Coord c0, Coord c1) {
  const int dx = c1.x() - c0.x();
  const int dy = c1.y() - c0.y();
  return max(abs(dx), abs(dy));
}
unsigned int Board::nbPieces(Color c) {
  switch (c) {
  case Red:
    return 3;
  case White:
    return 23;
  case Black:
    return 23;
  }
  return 0;
}
Board::Board() : spaces_(nbSpacesMaxOnRow() * nbRows()) {
  reinit();
  for (unsigned int n = 0; n < nbColors; ++n) {
    Color c = static_cast<Color>(n);
    for (unsigned int i = 0; i < nbPieces(c); ++i) {
      pieces_.push_back(Piece(c));
    }
  }
}
void Board::reinit() {
  unplaced_[0] = stack<const Piece *>();
  unplaced_[1] = stack<const Piece *>();
  unplaced_[2] = stack<const Piece *>();
  for (deque<Piece>::const_iterator iter = pieces_.begin();
       iter != pieces_.end(); ++iter) {
    unplaced_[iter->color()].push(&(*iter));
  }
  for_each(spaces_.begin(), spaces_.end(), resetStatus);
  for_each(spaces_.begin(), spaces_.end(), clearStacks);
  redSpaces_.clear();
}
Board::~Board() {}
unsigned int Board::coord2idx(Coord c) {
  return c.x() + c.y() * nbSpacesMaxOnRow();
}
Board::Coord Board::idx2coord(unsigned int n) {
  return Coord(n % nbSpacesMaxOnRow(), n / nbSpacesMaxOnRow());
}
Board::ConstStackHandle Board::stackAt(Coord c) const {
  return ConstStackHandle(c, &(spaces_[coord2idx(c)]));
}
Board::ConstStackHandle Board::stackAt(int x, int y) const {
  return stackAt(Coord(x, y));
}
Board::ConstStackIterator Board::stacks_begin() const {
  return ConstStackIterator(Coord(0, 0), &(spaces_[0]), this);
}
Board::ConstStackIterator Board::stacks_end() const {
  return ConstStackIterator(Coord(-1, -1), NULL, this);
}
string Board::prettyPrinted(const char *prefix) const {
  ostringstream os;
  const int yL = nbRows() / 2;
  const int yR = nbRows() - nbRows() / 2;
  const int nC = nbRows() % 2;
  for (int y = nbRows() - 1; y >= yR; --y) {
    os << prefix << string(2 * (y - yR + nC), ' ');
    for (int x = y - yR + nC; x < static_cast<int>(nbSpacesMaxOnRow()); ++x) {
      os << (*stackAt(Coord(x, y))) << ' ';
    }
    os << '\n';
  }
  if (nC == 1) {
    os << prefix;
    for (int x = 0; x < static_cast<int>(nbSpacesMaxOnRow()); ++x) {
      os << (*stackAt(Coord(x, yL))) << ' ';
    }
    os << endl;
  }
  for (int y = yL - 1; y >= 0; --y) {
    os << prefix << string(2 * (yL - y + nC - 1), ' ');
    for (int x = 0; x <= static_cast<int>(nbSpacesMaxOnRow()) - (yL - y) - nC;
         ++x) {
      os << (*stackAt(Coord(x, y))) << ' ';
    }
    os << '\n';
  }
  return os.str();
}
unsigned int Board::nbUnplacedPieces(Color c) const {
  return unplaced_[c].size();
}
const Piece *Board::getUnplacedPiece(Color c) const {
  if (unplaced_[c].empty())
    return NULL;
  return unplaced_[c].top();
}
/*!
 * You can place only a piece at a valid coord, and a piece that is
 * unplaced. So you must first get it with getUnplacedPiece(). Once it is
 * placed, calling this function again with the same piece will do nothing.
 */
void Board::place(const Piece *p, Coord c) {
  if (p && p == getUnplacedPiece(p->color()) && isValid(c)) {
    spaces_[coord2idx(c)].first.push_back(p);
    unplaced_[p->color()].pop();
    if (p->color() == Red) {
      redSpaces_[p] = c;
    }
  }
}
/*!
 * A move is made only if src and dst are valid.
 */
Board::Ghosts Board::move(Coord src, Coord dst, bool killDeads) {
  Ghosts ghosts;
  if (isValid(src) && isValid(dst)) {
    Stack &s = spaces_[coord2idx(src)].first;
    Stack &d = spaces_[coord2idx(dst)].first;
    // If the src destination was containing a red
    Stack::const_iterator fter =
        find_if(s.begin(), s.end(), mem_fun(&Piece::isRed));
    if (fter != s.end()) {
      redSpaces_[*fter] = dst;
    }
    // Move the pieces from src to dst
    d.insert(d.end(), s.begin(), s.end());
    s.clear();
    updateStatus(ghosts, killDeads);
  }
  return ghosts;
}
void Board::updateStatus(Ghosts &ghosts, bool killDeads) {
  // We now need to update the status of the cases
  for_each(spaces_.begin(), spaces_.end(), resetStatus);
  stack<Coord> toVisit;
  // Start from the reds...
  for (map<const Piece *, Coord>::const_iterator iter = redSpaces_.begin();
       iter != redSpaces_.end(); ++iter) {
    int i = coord2idx(iter->second);
    spaces_[i].second = i;
    toVisit.push(iter->second);
  }
  /// ...and propagate.
  while (!toVisit.empty()) {
    Coord c = toVisit.top();
    toVisit.pop();
    int l = spaces_[coord2idx(c)].second;
    Coord n[6] = {Coord(c.x() - 1, c.y()),     Coord(c.x() + 1, c.y()),
                  Coord(c.x(), c.y() - 1),     Coord(c.x(), c.y() + 1),
                  Coord(c.x() - 1, c.y() - 1), Coord(c.x() + 1, c.y() + 1)};
    for (unsigned int i = 0; i < 6; ++i) {
      if (!isValid(n[i]))
        continue;
      unsigned int p = coord2idx(n[i]);
      Space &s = spaces_[p];
      if (s.second == -1 && s.first.hasPieces()) {
        s.second = l;
        toVisit.push(n[i]);
      }
    }
  }
  // Now get rid of the dead
  if (killDeads) {
    unsigned int i = 0;
    for (vector<Space>::iterator iter = spaces_.begin(); iter != spaces_.end();
         ++iter, ++i) {
      if (iter->second == -1 && iter->first.hasPieces()) {
        ghosts.push_back(Ghost(idx2coord(i), iter->first));
        iter->first.clear();
      }
    }
  }
}
bool Board::isFree(const Board::ConstStackHandle &h) const {
  Coord c = h.stackCoord();

  if (!isValid(c) || !stackAt(c)->hasPieces())
    return false;

  Coord c0 = Coord(c.x() + 1, c.y());
  Coord c1 = Coord(c.x() - 1, c.y());
  Coord c2 = Coord(c.x(), c.y() + 1);
  Coord c3 = Coord(c.x(), c.y() - 1);
  Coord c4 = Coord(c.x() + 1, c.y() + 1);
  Coord c5 = Coord(c.x() - 1, c.y() - 1);
  return ((!isValid(c0) || !stackAt(c0)->hasPieces()) ||
          (!isValid(c1) || !stackAt(c1)->hasPieces()) ||
          (!isValid(c2) || !stackAt(c2)->hasPieces()) ||
          (!isValid(c3) || !stackAt(c3)->hasPieces()) ||
          (!isValid(c4) || !stackAt(c4)->hasPieces()) ||
          (!isValid(c5) || !stackAt(c5)->hasPieces()));
}
unsigned int Board::heightMax() const {
  return max_element(spaces_.begin(), spaces_.end(), hasLessPieces)
      ->first.height();
}
Board::State Board::state() const {
  State s;
  s.spaces_ = spaces_;
  s.redSpaces_ = redSpaces_;
  s.unplaced_[0] = unplaced_[0];
  s.unplaced_[1] = unplaced_[1];
  s.unplaced_[2] = unplaced_[2];
  return s;
}
void Board::restore(State s) {
  spaces_ = s.spaces_;
  redSpaces_ = s.redSpaces_;
  unplaced_[0] = s.unplaced_[0];
  unplaced_[1] = s.unplaced_[1];
  unplaced_[2] = s.unplaced_[2];
}
//************************************************************
// Implementation of Board::Coord
//************************************************************
Board::Coord::Coord(int x, int y) : x_(x), y_(y) {}
int Board::Coord::x() const { return x_; }
int Board::Coord::y() const { return y_; }
bool Board::Coord::operator==(const Board::Coord other) const {
  return x_ == other.x_ && y_ == other.y_;
}
bool Board::Coord::operator!=(const Board::Coord other) const {
  return !operator==(other);
}
bool Board::Coord::operator<(const Board::Coord other) const {
  return x_ < other.x_ || (x_ == other.x_ && y_ < other.y_);
}
ostream &operator<<(ostream &s, const Board::Coord c) {
  return s << '[' << ((char)(c.x() + (int)'A')) << c.y() << ']';
}
//************************************************************
// Implementation of Board::ConstStackHandle
//************************************************************
Board::ConstStackHandle::~ConstStackHandle() {}
Board::ConstStackHandle::operator bool() const { return space_ != NULL; }
const Stack *Board::ConstStackHandle::operator->() const {
  return &(space_->first);
}
const Stack &Board::ConstStackHandle::operator*() const {
  return space_->first;
}
bool Board::ConstStackHandle::operator==(const ConstStackHandle &other) const {
  return (space_ == other.space_ && coord_ == other.coord_);
}
bool Board::ConstStackHandle::operator!=(const ConstStackHandle &other) const {
  return !operator==(other);
}
Board::Coord Board::ConstStackHandle::stackCoord() const { return coord_; }
int Board::ConstStackHandle::stackStatus() const { return space_->second; }
Board::ConstStackHandle::ConstStackHandle() : coord_(-1, -1), space_(NULL) {}
Board::ConstStackHandle::ConstStackHandle(Coord c, const Space *s)
    : coord_(c), space_(s) {}
void Board::ConstStackHandle::setCoord(Coord c) { coord_ = c; }
void Board::ConstStackHandle::setSpace(const Space *s) { space_ = s; }
const Board::Space *Board::ConstStackHandle::space() const { return space_; }
bool Board::ConstStackHandle::isNull() const { return !(*this); }
Board::ConstStackHandle Board::ConstStackHandle::null() {
  return ConstStackHandle();
}
ostream &operator<<(ostream &s, const dvonn::Board::ConstStackHandle &h) {
  if (h)
    return s << '(' << h.stackCoord() << " : " << (*h) << ')';

  return s << "null";
}
//************************************************************
// Implementation of Board::ConstStackIterator
//************************************************************
Board::ConstStackIterator::~ConstStackIterator() {}
bool Board::ConstStackIterator::
operator==(const ConstStackHandle &other) const {
  const ConstStackIterator *pother =
      dynamic_cast<const ConstStackIterator *>(&other);
  if (!pother)
    return ConstStackHandle::operator==(other);
  return operator==(*pother);
}
bool Board::ConstStackIterator::
operator!=(const ConstStackHandle &other) const {
  return !operator==(other);
}
/*!
 * To be equal, they must have same board, and same handle.
 */
bool Board::ConstStackIterator::
operator==(const ConstStackIterator &other) const {
  return (board_ == other.board_ && ConstStackHandle::operator==(other));
}
bool Board::ConstStackIterator::
operator!=(const ConstStackIterator &other) const {
  return !operator==(other);
}
Board::ConstStackIterator &Board::ConstStackIterator::operator++() {
  if (!operator bool())
    return *this;

  int x = stackCoord().x();
  int y = stackCoord().y();
  do {
    if (++x >= static_cast<int>(nbSpacesMaxOnRow())) {
      x = 0;
      if (++y >= static_cast<int>(nbRows())) {
        setSpace(NULL);
        setCoord(Coord(-1, -1));
        return *this;
      }
    }
  } while (!isValid(Coord(x, y)));

  setCoord(Coord(x, y));
  setSpace(&(board_->spaces_[Board::coord2idx(Coord(x, y))]));

  return *this;
}
/*!
 * Rk: the default ConstStackIterator is not equal to the stacks_end()
 * iterator of a Board.
 */
Board::ConstStackIterator::ConstStackIterator()
    : Board::ConstStackHandle(), board_(NULL) {}
Board::ConstStackIterator::ConstStackIterator(Coord c, const Space *s,
                                              const Board *b)
    : Board::ConstStackHandle(c, s), board_(b) {}
//************************************************************
// Implementation of Board::Ghost
//************************************************************
Board::Ghost::Ghost(Board::Coord c, const Stack &s)
    : coord(c), stack(s.begin(), s.end()) {}
