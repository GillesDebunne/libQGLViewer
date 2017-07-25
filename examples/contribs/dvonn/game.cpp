#include "game.h"
#include <algorithm>
#include <fstream>

using namespace std;
using namespace dvonn;

Color dvonn::colorOf(Player p) { return static_cast<Color>(p + 1); }
Player dvonn::player(Color c) /* throw (range_error)*/
{
  if (c == Red)
    throw range_error("No Red Player");
  return static_cast<Player>(c - 1);
}
QString dvonn::nameOf(const Player p) {
  return p == WhitePlayer ? "White" : "Black";
}
//************************************************************
// Implementation of Game
//************************************************************
Game::Game() : fileName_("savedGame.ago") { reinit(); }
void Game::reinit() {
  phase_ = RedPlacementPhase;
  player_ = WhitePlayer;
  board_.reinit();
  ghosts_.clear();
  score_[WhitePlayer] = -1;
  score_[BlackPlayer] = -1;
  time_ = 0;
  historyStates_.clear();
  historyPlayers_.clear();
  historyPhases_.clear();
  historyStates_.push_back(board_.state());
  historyPlayers_.push_back(theOnePlaying());
  historyPhases_.push_back(phase());
}
bool Game::isOver() const { return phase_ == GameOverPhase; }
int Game::score(Player p) const { return score_[p]; }
Game::~Game() {}
const Board &Game::board() const { return board_; }
Player Game::theOnePlaying() const { return player_; }
Phase Game::phase() const { return phase_; }
bool Game::isLegalPlacement(const Placement p) const {
  return board_.getUnplacedPiece(p.color) &&
         !board_.stackAt(p.dst)->hasPieces();
}
bool Game::isLegalMove(const Move m) const {
  if (Board::isValid(m.src) && Board::isValid(m.dst)) {
    Board::ConstStackHandle srcH = board_.stackAt(m.src);
    Board::ConstStackHandle dstH = board_.stackAt(m.dst);
    // Test the src is free
    return srcH->onTop()->color() == colorOf(theOnePlaying()) &&
           board_.isFree(srcH) && srcH->hasPieces() && dstH->hasPieces() &&
           Board::areAligned(m.src, m.dst) &&
           Board::distance(m.src, m.dst) == srcH->height();
  }
  return false;
}
bool Game::doPlacement(Placement p) {
  if ((phase_ == RedPlacementPhase && p.color == Red) ||
      (phase_ == PiecePlacementPhase && p.color == colorOf(theOnePlaying()))) {
    if (isLegalPlacement(p)) {
      board_.place(board_.getUnplacedPiece(p.color), p.dst);

      if (phase_ == RedPlacementPhase && board_.nbUnplacedPieces(Red) == 0) {
        phase_ = PiecePlacementPhase;
      } else if (board_.nbUnplacedPieces(White) == 0 &&
                 board_.nbUnplacedPieces(Black) == 0) {
        phase_ = MovePhase;
      }
      switchPlayers(player_);
      updateHistory();
      return true;
    }
  }
  return false;
}
bool Game::doMove(const Move m) {
  switch (phase_) {
  case RedPlacementPhase:
  case PiecePlacementPhase:
    return false;
  case MovePhase:
    if (!isLegalMove(m))
      return false;
    ghosts_[m] = board_.move(m.src, m.dst, true);
    switchPlayers(player_);
    updateHistory();
    break;
  case GameOverPhase:
    (void)board_.move(m.src, m.dst, false);
    break;
  }
  return true;
}
const Board::Ghosts *Game::killedBy(const Move m) const {
  map<Move, Board::Ghosts>::const_iterator fter = ghosts_.find(m);
  return (fter == ghosts_.end()) ? NULL : &(fter->second);
}
deque<Board::ConstStackHandle>
Game::possibleDestinations(const Board::ConstStackHandle &ha) const {
  deque<Board::ConstStackHandle> r;
  if (ha->hasPieces()) {
    unsigned int h = ha->height();
    Board::Coord s = ha.stackCoord();
    Board::Coord d[6] = {
        Board::Coord(s.x() - h, s.y()),     Board::Coord(s.x() + h, s.y()),
        Board::Coord(s.x(), s.y() - h),     Board::Coord(s.x(), s.y() + h),
        Board::Coord(s.x() - h, s.y() - h), Board::Coord(s.x() + h, s.y() + h)};
    for (unsigned int i = 0; i < 6; ++i) {
      if (Board::isValid(d[i]) && board_.stackAt(d[i])->hasPieces()) {
        r.push_back(board_.stackAt(d[i]));
      }
    }
  }
  return r;
}
void Game::switchPlayers(Player p) {
  Player n = (p == WhitePlayer) ? BlackPlayer : WhitePlayer;
  if (phase_ != MovePhase) {
    player_ = n;
    return;
  }
  Move m;
  if (getRandomMove(n, m)) {
    player_ = n;
    return;
  }
  if (getRandomMove(p, m)) {
    player_ = p;
    return;
  }
  phase_ = GameOverPhase;
  // Run thru all the stacks and cumulate heights
  score_[WhitePlayer] = 0;
  score_[BlackPlayer] = 0;
  for (Board::ConstStackIterator iter = board_.stacks_begin(),
                                 istop = board_.stacks_end();
       iter != istop; ++iter) {
    if (iter->hasPieces()) {
      if (iter->onTop()->color() != Red) {
        score_[player(iter->onTop()->color())] += iter->height();
      }
    }
  }
}
void Game::randomlyFinishPlacement() {
  deque<Color> s(board_.nbUnplacedPieces(Red), Red);
  deque<Color> w(board_.nbUnplacedPieces(White), White);
  deque<Color> b(board_.nbUnplacedPieces(Black), Black);
  s.insert(s.end(), w.begin(), w.end());
  s.insert(s.end(), b.begin(), b.end());

  random_shuffle(s.begin(), s.end());
  deque<Color>::const_iterator pter = s.begin();
  for (Board::ConstStackIterator iter = board_.stacks_begin(),
                                 istop = board_.stacks_end();
       iter != istop; ++iter) {
    if (!(*iter).hasPieces()) {
      board_.place(board_.getUnplacedPiece(*pter++), iter.stackCoord());
    }
  }
  phase_ = MovePhase;
  player_ = WhitePlayer;
  updateHistory();
}
bool Game::getRandomMove(Player p, Game::Move &m) const {
  deque<Move> moves;
  // Traverse all the cases...
  for (Board::ConstStackIterator iter = board_.stacks_begin(),
                                 istop = board_.stacks_end();
       iter != istop; ++iter) {
    // ... to find a free non empty stack controlled by the player
    if (iter->hasPieces() && iter->onTop()->color() == colorOf(p) &&
        board_.isFree(iter)) {
      const deque<Board::ConstStackHandle> poss = possibleDestinations(iter);
      for (deque<Board::ConstStackHandle>::const_iterator jter = poss.begin();
           jter != poss.end(); ++jter) {
        moves.push_back(Move(iter.stackCoord(), jter->stackCoord()));
      }
    }
  }
  if (!moves.empty()) {
    random_shuffle(moves.begin(), moves.end());
    m = moves.front();
    return true;
  }
  return false;
}
QString Game::fileName() const { return fileName_; }
bool Game::load(const QString &fileName) {
  fileName_ = fileName;
#if QT_VERSION < 0x040000
  ifstream f(fileName_.latin1());
#else
  ifstream f(fileName_.toLatin1().constData());
#endif
  if (!f.good())
    return false;
  // TODO: load here
  f.close();

  return true;
}
bool Game::save() {
#if QT_VERSION < 0x040000
  std::ofstream f(fileName_.latin1());
#else
  std::ofstream f(fileName_.toLatin1().constData());
#endif
  if (!f.good())
    return false;
  f.close();
  return true;
}
bool Game::saveAs(const QString &fileName) {
  fileName_ = fileName;
  return save();
}
void Game::updateHistory() {
  ++time_;
  if (time_ >= historyStates_.size()) // cannot be more than size() actually
  {
    historyStates_.push_back(board_.state());
    historyPlayers_.push_back(theOnePlaying());
    historyPhases_.push_back(phase());
  } else {
    historyStates_[time_] = board_.state();
    historyPlayers_[time_] = theOnePlaying();
    historyPhases_[time_] = phase();
  }
  knownTime_ = 0;
}
bool Game::canUndo() const { return time_ > 0; }
bool Game::canRedo() const { return time_ < knownTime_; }
void Game::undo() {
  if (canUndo()) {
    if (knownTime_ == 0)
      knownTime_ = time_;
    --time_;
    board_.restore(historyStates_[time_]);
    player_ = historyPlayers_[time_];
    phase_ = historyPhases_[time_];
  }
}
void Game::redo() {
  if (canRedo()) {
    ++time_;
    board_.restore(historyStates_[time_]);
    player_ = historyPlayers_[time_];
    phase_ = historyPhases_[time_];
  }
}
//************************************************************
// Implementation of Game::Placement
//************************************************************
Game::Placement::Placement(Color c, Board::Coord d) : color(c), dst(d) {}
//************************************************************
// Implementation of Game::Move
//************************************************************
Game::Move::Move(Board::Coord s, Board::Coord d) : src(s), dst(d) {}
bool Game::Move::operator<(const Move other) const {
  return src < other.src || (src == other.src && dst < other.dst);
}
ostream &operator<<(ostream &s, const Game::Placement p) {
  return s << nameOf(p.color) << " ->" << p.dst;
}
ostream &operator<<(ostream &s, const Game::Move m) {
  return s << m.src << "->" << m.dst;
}
