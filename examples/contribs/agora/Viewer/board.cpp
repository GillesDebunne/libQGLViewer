#include "board.h"

#include <qgl.h>
#include <qmessagebox.h>

void Board::draw() const {
  for (int i = 0; i < sizeX_; ++i)
    for (int j = 0; j < sizeY_; ++j) {
      QPoint p(i, j);
      caseAt(p).draw(p);
    }
}

void Board::drawPossibleDestinations(int piece, bool select) const {
  glEnable(GL_BLEND);

  const QPoint sp = pointFromInt(piece);

  for (int i = -1; i <= 1; ++i)
    for (int j = -1; j <= 1; ++j) {
      const QPoint p(sp.x() + i, sp.y() + j);
      bool moveValid = Move(sp, p, false).isValid(this);
      bool moveUnderValid = Move(sp, p, true).isValid(this);
      if (moveValid || moveUnderValid) {
        if (select) {
          glPushName((intFromPoint(p)));
          caseAt(p).drawTop(p);
          caseAt(p).drawPieces(p);
          glPopName();
        } else {
          if (moveValid)
            caseAt(p).drawTopPieceTop(p);
          if (moveUnderValid)
            caseAt(p).drawTop(p);
        }
      }
    }
  glDisable(GL_BLEND);
}

void Board::drawSelectedPiece(int piece) const {
  const QPoint p = pointFromInt(piece);
  caseAt(p).drawTopPiece(p);
}

void Board::drawSelectablePieces() const {
  for (int i = 0; i < sizeX_; ++i)
    for (int j = 0; j < sizeY_; ++j) {
      QPoint p(i, j);
      if (canBeSelected(p)) {
        glPushName(intFromPoint(p));
        caseAt(p).drawPieces(p);
        glPopName();
      }
    }
}

bool Board::canBeSelected(const QPoint &p) const {
  return (isValid(p) && (caseAt(p).topIsBlack() == blackPlays()) &&
          (pieceCanMove(p)));
}

bool Board::isValid(const QPoint &p) const {
  return (p.x() >= 0) && (p.y() >= 0) && (p.x() < sizeX_) && (p.y() < sizeY_);
}

void Board::play(const Move &m) {
  if (!m.isValid(this)) {
    QMessageBox::critical(NULL, "Invalid move", "Invalid move");
    return;
  }

  m.updateBoard(this);
  nbMovesLeft_--;

  undo_.addState(stateString());
  blackPlays_ = !blackPlays_;
}

bool Board::gameIsOver() const {
  if (nbMovesLeft_ == 0)
    return true;

  for (int i = 0; i < sizeX_; ++i)
    for (int j = 0; j < sizeY_; ++j) {
      const QPoint p(i, j);
      if (pieceCanMove(p))
        return false;
    }

  return true;
}

bool Board::pieceCanMove(const QPoint &p) const {
  for (int i = -1; i <= 1; ++i)
    for (int j = -1; j <= 1; ++j) {
      const QPoint q(p.x() + i, p.y() + j);
      Move move(p, q, true);
      if (move.isValid(this))
        return true;
      move = Move(p, q, false);
      if (move.isValid(this))
        return true;
    }
  return false;
}

QString Board::stateString() const {
  QString res = "";

  for (int i = 0; i < sizeX_; ++i)
    for (int j = 0; j < sizeY_; ++j) {
      res.append(QString::number((int)caseAt(QPoint(i, j))));
      res.append((j == (sizeY_ - 1)) ? "\n" : " ");
    }

  return res;
}

void Board::initFromStateString(const QString &s) {
  QStringList values = s.split(QRegExp("\\D+"), QString::SkipEmptyParts);
  for (int i = 0; i < sizeX_; ++i)
    for (int j = 0; j < sizeY_; ++j)
      board_[i][j].initFrom(values[i * sizeY_ + j].toInt());
}

std::ostream &operator<<(std::ostream &out, const Board &b) {
  out << b.sizeX_ << " " << b.sizeY_ << std::endl;

#if QT_VERSION < 0x040000
  out << b.stateString().ascii() << std::endl;
#else
  out << b.stateString().toLatin1().constData() << std::endl;
#endif

  out << b.undo_;

  return out;
}

std::istream &operator>>(std::istream &in, Board &b) {
  int sx, sy;
  in >> sx >> sy;
  b.resize(sx, sy);

  int c;
  QString s;
  for (int i = 0; i < sx; ++i)
    for (int j = 0; j < sy; ++j) {
      in >> c;
      // QMessageBox::warning(NULL ,"Unreadable board file",
      // QString::number(c));
      s.append(QString("%1 ").arg(c));
    }

  b.initFromStateString(s);

  in >> b.undo_;

  if (b.undo_.isEmpty())
    b.undo_.addState(s);

  b.blackPlays_ = (b.undo_.nbMoves() % 2 == 0);

  b.nbMovesLeft_ = 50;

  return in;
}

void Board::resize(int sizeX, int sizeY) {
  if (board_) {
    for (int i = 0; i < sizeX_; ++i)
      delete[] board_[i];
    delete[] board_;
  }

  sizeX_ = sizeX;
  sizeY_ = sizeY;

  board_ = new Case *[sizeX_];
  for (int i = 0; i < sizeX_; ++i)
    board_[i] = new Case[sizeY_];
}

bool Board::undo() {
  QString state = undo_.undoState();
  if (state.isEmpty())
    return false;
  else
    initFromStateString(state);
  blackPlays_ = !blackPlays_;
  return true;
}

bool Board::redo() {
  QString state = undo_.redoState();
  if (state.isEmpty())
    return false;
  else
    initFromStateString(state);
  blackPlays_ = !blackPlays_;
  return true;
}

QString Board::statusMessage() const {
  int nbBlack = 0;
  int nbWhite = 0;

  for (int i = 0; i < sizeX_; ++i)
    for (int j = 0; j < sizeY_; ++j) {
      const Case c = caseAt(QPoint(i, j));
      nbBlack += c.topIsBlack() ? c.nbTop() : c.nbBottom();
      nbWhite += !c.topIsBlack() ? c.nbTop() : c.nbBottom();
    }

  const QString score = "Black: " + QString::number(nbBlack) +
                        " - White: " + QString::number(nbWhite) + " - " +
                        QString::number(nbMovesLeft_) + " move" +
                        (nbMovesLeft_ > 1 ? "s" : "") + " left";

  if (gameIsOver())
    if (nbBlack == nbWhite)
      return "Deuce !\n" + score;
    else
      return QString("The ") + ((nbBlack > nbWhite) ? "Black" : "White") +
             " won !\n" + score;
  else
    return score + " - " + (blackPlays_ ? "Black" : "White") + " plays";
}

Move Board::randomMove(bool black) const {
  for (int i = 0; i < sizeX_; ++i)
    for (int j = 0; j < sizeY_; ++j) {
      const QPoint p(i, j);
      if (caseAt(p).topIsBlack() == black) {
        for (int ii = -1; ii <= 1; ++ii)
          for (int jj = -1; jj <= 1; ++jj) {
            const QPoint q(i + ii, j + jj);
            Move move(p, q, false);
            if (move.isValid(this))
              return move;
            move = Move(p, q, true);
            if (move.isValid(this))
              return move;
          }
      }
    }
  return Move();
}
