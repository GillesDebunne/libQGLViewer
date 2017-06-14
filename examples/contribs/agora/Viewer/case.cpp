#include "case.h"
#include <QGLViewer/vec.h>
#include <qgl.h>

using namespace qglviewer;

/////////////
// Drawing //
/////////////
static const float thickness = 0.2f;
static const float pieceSize = 0.8f;
static const float pieceShift = (1.0 - pieceSize) / 2.0;

// static qglviewer::Vec posOf(const QPoint& p) { return
// qglviewer::Vec(p.x()+0.5,p.y()+0.5,0.0); };

void Case::draw(const QPoint &pos) const {
  drawBasis(pos);
  drawPieces(pos);
}

void Case::drawSquare(float size) {
  static const float eps = 0.002f;
  glDisable(GL_LIGHTING);
  glBegin(GL_LINE_LOOP);
  glLineWidth(3);
  glVertex3f(-eps, -eps, thickness + eps);
  glVertex3f(size + eps, -eps, thickness + eps);
  glVertex3f(size + eps, size + eps, thickness + eps);
  glVertex3f(-eps, size + eps, thickness + eps);
  glEnd();
  glEnable(GL_LIGHTING);
}

void Case::drawTop(const QPoint &pos) const {
  glEnable(GL_BLEND);
  glPushMatrix();
  glTranslatef(pos.x(), pos.y(), altitude_ * thickness + 0.002f);

  glBegin(GL_QUADS);
  glColor4f(0.9f, 0.2f, 0.2f, 0.7f);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex2f(0.0, 0.0);
  glVertex2f(1.0, 0.0);
  glVertex2f(1.0, 1.0);
  glVertex2f(0.0, 1.0);
  glEnd();

  glPopMatrix();
  glDisable(GL_BLEND);
}

void Case::drawTopPiece(const QPoint &pos) const {
  glEnable(GL_BLEND);
  glPushMatrix();
  const float delta = pieceShift / 2.0f;
  glTranslatef(pos.x() + pieceShift - delta, pos.y() + pieceShift - delta,
               (altitude_ + nbBottom() + nbTop() - 1) * thickness);

  glColor4f(0.5, 0.5, 0.9f, 0.7f);

  Case::drawParallelepiped(pieceSize + 2 * delta, thickness + delta);

  glPopMatrix();
  glDisable(GL_BLEND);
}

void Case::drawTopPieceTop(const QPoint &pos) const {
  glEnable(GL_BLEND);
  glPushMatrix();
  glTranslatef(pos.x() + pieceShift, pos.y() + pieceShift,
               (altitude_ + nbBottom() + nbTop()) * thickness + 0.02f);

  glBegin(GL_QUADS);
  glColor4f(0.2f, 0.7f, 0.2f, 0.5f);
  glNormal3f(0.0, 0.0, 1.0);
  glVertex2f(0.0, 0.0);
  glVertex2f(pieceSize, 0.0);
  glVertex2f(pieceSize, pieceSize);
  glVertex2f(0.0, pieceSize);
  glEnd();

  glPopMatrix();
  glDisable(GL_BLEND);
}

void Case::drawPieces(const QPoint &pos) const {
  for (int i = 0; i < nbBottom(); i++)
    drawPiece(pos, i, !topIsBlack());
  for (int i = nbBottom(); i < nbBottom() + nbTop(); i++)
    drawPiece(pos, i, topIsBlack());
}

void Case::drawBasis(const QPoint &pos) const {
  glPushMatrix();
  glTranslatef(pos.x(), pos.y(), 0.0);

  glEnable(GL_TEXTURE_2D);
  glColor3f(0.5, 0.5, 0.5);
  Case::drawParallelepiped(1.0, altitude_ * thickness);
  glDisable(GL_TEXTURE_2D);

  glColor3f(0.2f, 0.2f, 0.2f);

  for (int i = 0; i < altitude_; i++) {
    Case::drawSquare(1.0);
    glTranslatef(0.0, 0.0, thickness);
  }

  glPopMatrix();
}

void Case::drawPiece(const QPoint &pos, int altitude, bool black) const {
  glPushMatrix();
  glTranslatef(pos.x() + pieceShift, pos.y() + pieceShift,
               (altitude_ + altitude) * thickness + 0.001f);

  if (black)
    glColor3f(0.2f, 0.2f, 0.2f);
  else
    glColor3f(0.6f, 0.6f, 0.6f);

  Case::drawParallelepiped(pieceSize, thickness);
  Case::drawSquare(pieceSize);

  glPopMatrix();
}

void Case::drawParallelepiped(float size, float height) {
  glBegin(GL_QUADS);
  // Top
  glNormal3f(0.0, 0.0, 1.0);
  glTexCoord2i(0, 0);
  glVertex3f(0.0, 0.0, height);
  glTexCoord2i(1, 0);
  glVertex3f(size, 0.0, height);
  glTexCoord2i(1, 1);
  glVertex3f(size, size, height);
  glTexCoord2i(0, 1);
  glVertex3f(0.0, size, height);

  // Sides
  glNormal3f(0.0, -1.0, 0.0);
  glTexCoord2i(0, 0);
  glVertex3f(0.0, 0.0, height);
  glTexCoord2i(1, 0);
  glVertex3f(0.0, 0.0, 0.0);
  glTexCoord2i(1, 1);
  glVertex3f(size, 0.0, 0.0);
  glTexCoord2i(0, 1);
  glVertex3f(size, 0.0, height);

  glNormal3f(1.0, 0.0, 0.0);
  glTexCoord2i(0, 0);
  glVertex3f(size, 0.0, height);
  glTexCoord2i(1, 0);
  glVertex3f(size, 0.0, 0.0);
  glTexCoord2i(1, 1);
  glVertex3f(size, size, 0.0);
  glTexCoord2i(0, 1);
  glVertex3f(size, size, height);

  glNormal3f(0.0, 1.0, 0.0);
  glTexCoord2i(0, 0);
  glVertex3f(size, size, height);
  glTexCoord2i(1, 0);
  glVertex3f(size, size, 0.0);
  glTexCoord2i(1, 1);
  glVertex3f(0.0, size, 0.0);
  glTexCoord2i(0, 1);
  glVertex3f(0.0, size, height);

  glNormal3f(-1.0, 0.0, 0.0);
  glTexCoord2i(0, 0);
  glVertex3f(0.0, size, height);
  glTexCoord2i(1, 0);
  glVertex3f(0.0, size, 0.0);
  glTexCoord2i(1, 1);
  glVertex3f(0.0, 0.0, 0.0);
  glTexCoord2i(0, 1);
  glVertex3f(0.0, 0.0, height);

  glEnd();
}

///////////////////
// Displacements //
///////////////////
void Case::removePiece() {
  nbTop_--;
  if ((nbTop_ == 0) && (nbBottom_ > 0)) {
    nbTop_ = nbBottom_;
    nbBottom_ = 0;
    topIsBlack_ = !topIsBlack_;
  }
  checkForRevolution();
}

void Case::addPiece(bool under, bool black) {
  if (under) {
    nbBottom_++;
    checkForRevolution();
  } else {
    nbTop_++;
    nbTop_ += nbBottom_;
    nbBottom_ = 0;
    topIsBlack_ = black;
  }
}

void Case::checkForRevolution() {
  if (nbBottom_ > nbTop_) {
    nbTop_ += nbBottom_;
    nbBottom_ = 0;
    topIsBlack_ = !topIsBlack_;
  }
}

///////////////////////////////
// Conversion from/to binary //
///////////////////////////////
void Case::initFrom(int binary) {
  altitude_ = binary & 7;
  topIsBlack_ = (binary >>= 3) & 1;
  nbTop_ = 0;
  nbBottom_ = 0;

  if (binary > 0) {
    while (!((binary >>= 1) & 1))
      ++nbBottom_;
    ++nbTop_;
    while ((binary >>= 1) & 1)
      ++nbTop_;
  }
}

Case::operator int() const {
  int binary(0);
  if (nbTop_ > 0) {
    for (int i = 0; i < nbTop_; ++i) {
      binary <<= 1;
      binary |= 1;
    }
    binary <<= nbBottom_;

    binary <<= 1;
    if (topIsBlack_)
      binary |= 1;

    binary <<= 3;
  }
  binary += altitude_;
  return binary;
}

std::ostream &operator<<(std::ostream &out, const Case &c) {
  return out << "[alt(" << c.altitude_ << "), col(" << c.topIsBlack_
             << "), top(" << c.nbTop_ << "), bot(" << c.nbBottom_ << ")]";
}

std::istream &operator>>(std::istream &in, Case &c) {
  int binary;
  in >> binary;
  c.initFrom(binary);
  return in;
}
