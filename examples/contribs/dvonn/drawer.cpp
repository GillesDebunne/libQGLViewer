#include "drawer.h"
#include <QGLViewer/qglviewer.h>
#include <qimage.h>
#include <qmessagebox.h>
#include <qstringlist.h>

using namespace std;
using namespace dvonn;
using namespace qglviewer;

namespace {
static const float whiteColor[3] = {1.0f, 1.0f, 1.0f};
static const float blackColor[3] = {0.2f, 0.2f, 0.2f};
static const float redColor[3] = {1.0f, 0.0f, 0.0f};
static const float boardColor[3] = {1.0f, 1.0f, 1.0f};
static const float boardBorderColor[3] = {0.0f, 0.0f, 0.0f};
// Dimension (D=widht=height) of a case and of the inner border of the
// case (B)
const float caseD = 1.0f;
const float caseB = 0.2f;
// Dimension of a piece
const float pieceRMax = (caseD - 2.0f * caseB) / sqrtf(2.0f);
const float pieceRMin = pieceRMax / 3.0f;
const float pieceH = 0.14f;
const float pieceC = 0.25f; // curvature of normal on vring part
// Dimension of the label case
float hLabelW = caseD; // must be caseD
float hLabelH = caseD / 3.0f;
float vLabelW = caseD / 3.0f;
float vLabelH = caseD; // must be caseD
// Dimension of the board
const float boardB = 0.1f;
const float boardW =
    Board::nbSpacesMaxOnRow() * caseD + 2 * boardB + 2 * vLabelW;
const float boardH = Board::nbRows() * caseD + 2 * boardB + 2 * hLabelH;
const float poolB = 0.1f;
void tessAndTexture(float x0, float y0, float x2, float y2, float u0 = 0.0f,
                    float v0 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) {
  if (x0 > x2)
    swap(x0, x2);
  if (y0 > y2)
    swap(y0, y2);
  if (x2 - x0 > caseD) {
    float x1 = (x0 + x2) / 2.0f;
    float u1 = (u0 + u2) / 2.0f;
    tessAndTexture(x0, y0, x1, y2, u0, v0, u1, v2);
    tessAndTexture(x1, y0, x2, y2, u1, v0, u2, v2);
  } else if (y2 - y0 > caseD) {
    float y1 = (y0 + y2) / 2.0f;
    float v1 = (v0 + v2) / 2.0f;
    tessAndTexture(x0, y0, x2, y1, u0, v0, u2, v1);
    tessAndTexture(x0, y1, x2, y2, u0, v1, u2, v2);
  } else {
    glMultiTexCoord2f(GL_TEXTURE0, x0 / caseD, y0 / caseD);
    glMultiTexCoord2f(GL_TEXTURE1, u0, v0);
    glVertex2f(x0, y0);
    glMultiTexCoord2f(GL_TEXTURE0, x2 / caseD, y0 / caseD);
    glMultiTexCoord2f(GL_TEXTURE1, u2, v0);
    glVertex2f(x2, y0);
    glMultiTexCoord2f(GL_TEXTURE0, x2 / caseD, y2 / caseD);
    glMultiTexCoord2f(GL_TEXTURE1, u2, v2);
    glVertex2f(x2, y2);
    glMultiTexCoord2f(GL_TEXTURE0, x0 / caseD, y2 / caseD);
    glMultiTexCoord2f(GL_TEXTURE1, u0, v2);
    glVertex2f(x0, y2);
  }
}
const Vec normal[5] = {Vec(-1.0, 0.0, 0.0), Vec(0.0, 1.0, 0.0),
                       Vec(1.0, 0.0, 0.0), Vec(0.0, -1.0, 0.0),
                       Vec(-1.0, 0.0, 0.0)};
void drawHRing(const float rMin, const float rMax, const float height) {
  // TODO : tabulate the cos and sin tables
  const int nbSteps = 24;

  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_QUAD_STRIP);
  glVertex3f(rMin, 0.0, height);
  glVertex3f(rMax, 0.0, height);
  for (int i = 1; i <= nbSteps; ++i) {
    const float angle = i * 2.0 * M_PI / nbSteps;
    const float cosine = cos(angle);
    const float sine = sin(angle);
    glVertex3f(rMin * cosine, rMin * sine, height);
    glVertex3f(rMax * cosine, rMax * sine, height);
  }
  glEnd();
}
void drawVRing(const float hMin, const float hMax, const float r, bool in) {
  const int nbSteps = 24;
  const float normalSign = (in ? -1.0 : 1.0);
  float thMin = hMin;
  float thMax = hMax;
  if (in)
    swap(thMax, thMin);
  glBegin(GL_QUAD_STRIP);
  glNormal3f(normalSign, 0.0, 0.0);
  for (int i = 0; i <= nbSteps; ++i) {
    const float angle = i * 2.0 * M_PI / nbSteps;
    const float cosine = cos(angle);
    const float sine = sin(angle);
    glNormal3fv(Vec(normalSign * cosine, normalSign * sine, +pieceC).unit());
    glVertex3f(r * cosine, r * sine, thMax);
    glNormal3fv(Vec(normalSign * cosine, normalSign * sine, -pieceC).unit());
    glVertex3f(r * cosine, r * sine, thMin);
  }
  glEnd();
}
void drawPiece(const Color &p, float a = 1.0f) {
  static const float *colors[3] = {redColor, whiteColor, blackColor};
  const float *c = colors[static_cast<int>(p)];
  glColor4f(c[0] * a, c[1] * a, c[2] * a, a);
  drawHRing(pieceRMin, pieceRMax, pieceH);
  drawVRing(0.0f, pieceH, pieceRMax, false);
  drawVRing(0.0f, pieceH, pieceRMin, true);
}
void drawHLabel(unsigned int i) {
  glBegin(GL_QUADS);
  unsigned int d = Board::nbRows() / 2;
  if (i < Board::nbSpacesMaxOnRow() - d) // Bottom
  {
    float x = boardB + vLabelW + (d + 1) * (caseD / 2.0f) + i * hLabelW;
    float y = boardB;
    tessAndTexture(x, y, x + hLabelH, y + hLabelH, 0.0f, 0.0f, 1.0f, 1.0f);
    tessAndTexture(x + hLabelH, y, x + hLabelW, y + hLabelH, 2.0f, 2.0f, 2.0f,
                   2.0f);
  }
  if (i >= d) {
    float x = boardB + vLabelW - (d + 1) * (caseD / 2.0f) + i * hLabelW;
    float y = boardB + hLabelH + Board::nbRows() * caseD;
    tessAndTexture(x, y, x + hLabelW - hLabelH, y + hLabelH, 2.0f, 2.0f, 2.0f,
                   2.0f);
    tessAndTexture(x + hLabelW - hLabelH, y, x + hLabelW, y + hLabelH, 0.0f,
                   0.0f, 1.0f, 1.0f);
  }
  glEnd();
}
void drawVLabel(int i) {
  glBegin(GL_QUADS);
  int d = static_cast<int>(Board::nbRows() / 2);
  int e = abs(d - i);
  float shift = e * caseD / 2.0f;
  float x = boardB + shift;
  float y = boardB + hLabelH + i * vLabelH;
  tessAndTexture(x, y, x + vLabelW, y + vLabelH / 3.0f, 2.0f, 2.0f, 2.0f, 2.0f);
  tessAndTexture(x, y + vLabelH / 3.0f, x + vLabelW, y + 2.0f * vLabelH / 3.0f,
                 0.0f, 0.0f, 1.0f, 1.0f);
  tessAndTexture(x, y + 2.0f * vLabelH / 3.0f, x + vLabelW, y + vLabelH, 2.0f,
                 2.0f, 2.0f, 2.0f);
  x = boardB + shift + vLabelW + (Board::nbSpacesMaxOnRow() - e) * caseD;
  y = boardB + hLabelH + i * vLabelH;
  tessAndTexture(x, y, x + vLabelW, y + vLabelH / 3.0f, 2.0f, 2.0f, 2.0f, 2.0f);
  tessAndTexture(x, y + vLabelH / 3.0f, x + vLabelW, y + 2.0f * vLabelH / 3.0f,
                 0.0f, 0.0f, 1.0f, 1.0f);
  tessAndTexture(x, y + 2.0f * vLabelH / 3.0f, x + vLabelW, y + vLabelH, 2.0f,
                 2.0f, 2.0f, 2.0f);
  glEnd();
}
GLuint loadTexture(const QString fileName) {
  QImage img("images/" + fileName);
  if (img.isNull()) {
    QMessageBox::critical(NULL, "Image not found",
                          "Unable to load texture from " + fileName);
    exit(1); // TODO: be nicer!
  }
  // 1E-3 needed. Just try with width=128 and see !
  if ((img.width() != 1 << (int)(1 + log(img.width() - 1 + 1E-3) / log(2.0))) ||
      (img.height() !=
       1 << (int)(1 + log(img.height() - 1 + 1E-3) / log(2.0)))) {
    QMessageBox::critical(NULL, "Wrong image dimensions",
                          "Texture dimensions are not powers of 2 in " +
                              fileName);
    exit(1); // TODO: be nicer!
  }
  // cout << "Loaded "<<fileName<<endl;
  QImage glImg = QGLWidget::convertToGLFormat(img); // flipped 32bit RGBA
  // Create a texture object
  GLuint to;
  glGenTextures(1, &to);
  glBindTexture(GL_TEXTURE_2D, to);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, glImg.width(), glImg.height(),
                    GL_RGBA, GL_UNSIGNED_BYTE, glImg.bits());
  return to;
}
} // namespace
//************************************************************
// Implementation of Drawer
//************************************************************
Drawer::Drawer() : showTextures_(true) {}
void Drawer::toggleTexture(bool b) { showTextures_ = b; }
Drawer::~Drawer() {}
void Drawer::init() {
  static const QStringList texFilenames = QStringList() << "board.png"
                                                        << "case.png";
  for (QStringList::const_iterator it = texFilenames.begin();
       it != texFilenames.end(); ++it) {
    textures_[*it] = loadTexture(*it);
  }
  for (unsigned int i = 0; i < Board::nbSpacesMaxOnRow(); ++i) {
    static const char *letter = "ABCDEFGHIJK";
    hLabels_.push_back(loadTexture(QString("label%1.png").arg(letter[i])));
  }
  for (unsigned int i = 0; i < Board::nbRows(); ++i) {
    vLabels_.push_back(loadTexture(QString("label%1.png").arg(i + 1)));
  }
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glActiveTexture(GL_TEXTURE1);
  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
void Drawer::startTexture(const QString &t) const {
  glPushAttrib(GL_ENABLE_BIT);
  glActiveTexture(GL_TEXTURE0);
  map<QString, GLuint>::const_iterator fter = textures_.find(t);
  if (showTextures_ && fter != textures_.end()) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fter->second);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  } else {
    glDisable(GL_TEXTURE_2D);
  }
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
}
void Drawer::startTexture(const QString &t, GLuint to) const {
  glPushAttrib(GL_ENABLE_BIT);
  glActiveTexture(GL_TEXTURE0);
  map<QString, GLuint>::const_iterator fter = textures_.find(t);
  if (showTextures_ && fter != textures_.end()) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fter->second);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  } else {
    glDisable(GL_TEXTURE_2D);
  }
  glActiveTexture(GL_TEXTURE1);
  if (showTextures_) {
    glBindTexture(GL_TEXTURE_2D, to);
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  } else {
    glDisable(GL_TEXTURE_2D);
  }
}
void Drawer::startTexture() const {
  glPushAttrib(GL_ENABLE_BIT);
  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE1);
  glDisable(GL_TEXTURE_2D);
}
void Drawer::endTexture() const { glPopAttrib(); }
void translateTo(Board::Coord c, float h = 0.0f) {
  static const float shifts[5] = {1.0f, 0.5f, 0.0f, -0.5f, -1.0f};
  glTranslatef(boardB + vLabelW + c.x() * caseD + shifts[c.y()] * caseD,
               boardB + hLabelH + c.y() * caseD, h);
}
void Drawer::drawPieces(const Board::ConstStackHandle &s) const {
  startTexture();
  glPushMatrix();
  translateTo(s.stackCoord());
  glTranslatef(0.5f * caseD, 0.5f * caseD, 0.0f);
  for (Stack::const_iterator iter = s->begin(), istop = s->end(); iter != istop;
       ++iter) {
    drawPiece((*iter)->color());
    glTranslatef(0.0f, 0.0f, pieceH);
  }
  glPopMatrix();
  endTexture();
}
void Drawer::drawStatus(const Board::ConstStackHandle &s, QGLViewer *v) const {
  startTexture();
  glPushMatrix();
  translateTo(s.stackCoord(), s->height() * pieceH + pieceH / 2.0f);
  glColor3f(1.0f, 1.0f, 0.0f);
  v->renderText(0.5f * caseD, 0.5f * caseD, QString("%1").arg(s.stackStatus()),
                QFont());
  glPopMatrix();
  endTexture();
}
void Drawer::drawTransparentPiece(Color col, const Board::ConstStackHandle &c,
                                  float a) const {
  startTexture();
  glPushMatrix();
  translateTo(c.stackCoord(), c->height() * pieceH);
  glTranslatef(0.5f * caseD, 0.5f * caseD, 0.0f);
  drawPiece(col, a);
  glPopMatrix();
  endTexture();
}
void Drawer::drawTransparentPieces(Stack::const_iterator first,
                                   Stack::const_iterator last,
                                   const Board::Coord &c, float h,
                                   float a) const {
  startTexture();
  glPushMatrix();
  translateTo(c, h * pieceH);
  glTranslatef(0.5f * caseD, 0.5f * caseD, 0.0f);
  while (first != last) {
    drawPiece((*first++)->color(), a);
    glTranslatef(0.0f, 0.0f, pieceH);
  }
  glPopMatrix();
  endTexture();
}
void Drawer::draw(const Board::ConstStackHandle &c) const {
  glPushMatrix();
  translateTo(c.stackCoord());
  startTexture("case.png");
  glColor3fv(boardColor);
  glNormal3fv(boardUpVector());
  glBegin(GL_QUADS);
  tessAndTexture(0.0f, 0.0f, caseD, caseD);
  glEnd();
  endTexture();
  glPopMatrix();
}
void Drawer::drawComplement(bool showLabels) const {
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glNormal3fv(boardUpVector());
  glColor3fv(boardColor);
  for (unsigned int i = 0; i < Board::nbSpacesMaxOnRow(); ++i) {
    if (showLabels)
      startTexture("board.png", hLabels_[i]);
    else
      startTexture("board.png");
    drawHLabel(i);
    endTexture();
  }
  glColor3fv(boardColor);
  for (unsigned int i = 0; i < Board::nbRows(); ++i) {
    if (showLabels)
      startTexture("board.png", vLabels_[i]);
    else
      startTexture("board.png");
    drawVLabel(i);
    endTexture();
  }
  startTexture("board.png");
  glBegin(GL_QUADS);
  if (boardB > 0.0f) {
    glColor3fv(boardBorderColor);
    tessAndTexture(0.0f, 0.0f, boardW, boardB);
    tessAndTexture(0.0f, boardH - boardB, boardW, boardH);
    tessAndTexture(0.0f, boardB, boardB, boardH - boardB);
    tessAndTexture(boardW - boardB, boardB, boardW, boardH - boardB);
  }
  glColor3fv(boardColor);
  int d = Board::nbRows() / 2;
  tessAndTexture(boardB, boardB, boardB + vLabelW + (d + 1) * caseD / 2.0f,
                 boardB + hLabelH);
  tessAndTexture(boardB, boardH - boardB - hLabelH,
                 boardB + vLabelW + (d + 1) * caseD / 2.0f - hLabelW,
                 boardH - boardB);
  tessAndTexture(boardW - (boardB + vLabelW + (d + 1) * caseD / 2.0f) + hLabelW,
                 boardB, boardW - boardB, boardB + hLabelH);
  tessAndTexture(boardW - (boardB + vLabelW + (d + 1) * caseD / 2.0f),
                 boardH - boardB - hLabelH, boardW - boardB, boardH - boardB);
  for (int i = 0; i < d; ++i) {
    tessAndTexture(boardB, boardB + hLabelH + i * vLabelH,
                   boardB + (d - i) * caseD / 2.0f,
                   boardB + hLabelH + i * vLabelH + vLabelH);
    tessAndTexture(boardB, boardH - (boardB + hLabelH + i * vLabelH + vLabelH),
                   boardB + (d - i) * caseD / 2.0f,
                   boardH - (boardB + hLabelH + i * vLabelH));
    tessAndTexture(boardW - (boardB + (d - i) * caseD / 2.0f),
                   boardB + hLabelH + i * vLabelH, boardW - boardB,
                   boardB + hLabelH + i * vLabelH + vLabelH);
    tessAndTexture(boardW - (boardB + (d - i) * caseD / 2.0f),
                   boardH - (boardB + hLabelH + i * vLabelH + vLabelH),
                   boardW - boardB, boardH - (boardB + hLabelH + i * vLabelH));
  }
  glEnd();
  endTexture();
  glPopAttrib();
}
void Drawer::drawWhitePiecePools(const Board &b, bool lastTransparent) const {
  unsigned int d = Board::nbRows() / 2;
  unsigned int e = Board::nbSpacesMaxOnRow() - d;
  startTexture();
  // Draw the whites and its up to two reds
  unsigned int nbR = b.nbUnplacedPieces(Red);
  unsigned int nbW = b.nbUnplacedPieces(White);
  unsigned int nbRed4White = nbR / 2 + nbR % 2;
  for (unsigned int istop = nbW + nbRed4White, i = 0; i < istop; ++i) {
    const float x = boardB + vLabelW + (d + 1) * caseD / 2.0f + (i % e) * caseD;
    static const float y = -poolB - caseD / 2.0f;
    const float z = (i / e) * pieceH;
    glPushMatrix();
    glTranslatef(x, y, z);
    drawPiece((i < nbW) ? White : Red,
              (lastTransparent && i == nbW + nbRed4White - 1) ? 0.5f : 1.0f);
    glPopMatrix();
  }
  endTexture();
}
void Drawer::drawBlackPiecePools(const Board &b, bool lastTransparent) const {
  unsigned int d = Board::nbRows() / 2;
  unsigned int e = Board::nbSpacesMaxOnRow() - d;
  startTexture();
  // Draw the blacks and its up to one red
  unsigned int nbR = b.nbUnplacedPieces(Red);
  unsigned int nbB = b.nbUnplacedPieces(Black);
  unsigned int nbRed4Black = nbR / 2;
  for (unsigned int istop = nbB + nbRed4Black, i = 0; i < istop; ++i) {
    const float x =
        boardW - (boardB + vLabelW + (d + 1) * caseD / 2.0f + (i % e) * caseD);
    static const float y = boardH + poolB + caseD / 2.0f;
    const float z = (i / e) * pieceH;
    glPushMatrix();
    glTranslatef(x, y, z);
    drawPiece((i < nbB) ? Black : Red,
              (lastTransparent && i == nbB + nbRed4Black - 1) ? 0.4f : 1.0f);
    glPopMatrix();
  }
  endTexture();
}
void Drawer::highlight(const Board::ConstStackHandle &c) const {
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_LIGHTING);
  startTexture();
  glPushMatrix();
  translateTo(c.stackCoord(), 0.05f * pieceH + pieceH / 2.0f);
  glScalef(caseD, caseD, pieceH);
  // glutWireCube(1.0f);
  glPopMatrix();
  endTexture();
  glPopAttrib();
}
void Drawer::highlightPieces(const Board::ConstStackHandle &c) const {
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_LIGHTING);
  startTexture();
  glPushMatrix();
  translateTo(c.stackCoord(), 0.01f);
  glTranslatef(0.5f * caseD, 0.5f * caseD, 0.0f);
  glColor4f(0.0f, 0.0f, 0.3f, 0.3f);
  drawHRing(pieceRMin, 1.2f * pieceRMax, 0.0f);
  glPopMatrix();
  endTexture();
  glPopAttrib();
}
void Drawer::drawMove(const Board &b, const Game::Move m, float t) const {
  const float L = caseD * estimateDrawMoveLength(b, m);
  const float t0 =
      (b.heightMax() + 1 - b.stackAt(m.src)->height()) * pieceH / L;
  const float t1 =
      1.0f - (b.heightMax() + 1 - b.stackAt(m.dst)->height()) * pieceH / L;
  Board::ConstStackHandle src = b.stackAt(m.src);
  glPushMatrix();
  startTexture();
  if (t <= t0) {
    t /= t0;
    float srcH = src->height() * pieceH;
    float dstH = b.heightMax() * pieceH + pieceH;
    translateTo(m.src);
    glTranslatef(0.5f * caseD, 0.5f * caseD, (1 - t) * srcH + t * dstH);
    for (Stack::const_iterator iter = src->begin(), istop = src->end();
         iter != istop; ++iter) {
      drawPiece((*iter)->color());
      glTranslatef(0.0f, 0.0f, pieceH);
    }
  } else if (t <= t1) {
    t = (t - t0) / (t1 - t0);
    static const float shifts[5] = {1.0f, 0.5f, 0.0f, -0.5f, -1.0f};
    float srcX =
        boardB + vLabelW + m.src.x() * caseD + shifts[m.src.y()] * caseD;
    float srcY = boardB + hLabelH + m.src.y() * caseD;
    float dstX =
        boardB + vLabelW + m.dst.x() * caseD + shifts[m.dst.y()] * caseD;
    float dstY = boardB + hLabelH + m.dst.y() * caseD;
    glTranslatef((1.0f - t) * srcX + t * dstX + 0.5f * caseD,
                 (1.0f - t) * srcY + t * dstY + 0.5f * caseD,
                 b.heightMax() * pieceH + pieceH);
    for (Stack::const_iterator iter = src->begin(), istop = src->end();
         iter != istop; ++iter) {
      drawPiece((*iter)->color());
      glTranslatef(0.0f, 0.0f, pieceH);
    }
  } else {
    t = (t - t1) / (1.0f - t1);
    float srcH = b.heightMax() * pieceH + pieceH;
    float dstH = b.stackAt(m.dst)->height() * pieceH;
    translateTo(m.dst);
    glTranslatef(0.5f * caseD, 0.5f * caseD, (1 - t) * srcH + t * dstH);
    for (Stack::const_iterator iter = src->begin(), istop = src->end();
         iter != istop; ++iter) {
      drawPiece((*iter)->color());
      glTranslatef(0.0f, 0.0f, pieceH);
    }
  }
  endTexture();
  glPopMatrix();
}
float Drawer::estimateDrawMoveLength(const Board &b, const Game::Move m) const {
  static const float shifts[5] = {1.0f, 0.5f, 0.0f, -0.5f, -1.0f};
  float srcX = boardB + vLabelW + m.src.x() * caseD + shifts[m.src.y()] * caseD;
  float srcY = boardB + hLabelH + m.src.y() * caseD;
  float dstX = boardB + vLabelW + m.dst.x() * caseD + shifts[m.dst.y()] * caseD;
  float dstY = boardB + hLabelH + m.dst.y() * caseD;
  return (Vec(dstX - srcX, dstY - srcY, 0.0f).norm() +
          (b.heightMax() + 1 - b.stackAt(m.src)->height()) * pieceH +
          (b.heightMax() + 1 - b.stackAt(m.dst)->height()) * pieceH) /
         caseD;
}
Vec Drawer::boardCenter() const {
  return Vec(boardW / 2.0f, boardH / 2.0f, 0.0f);
}
Vec Drawer::boardUpVector() const { return Vec(0.0f, 0.0f, 1.0f); }
float Drawer::boardRadius() const {
  return Vec(boardW / 2.0f, boardH / 2.0f, 0.0).norm();
}
Vec Drawer::defaultEyePosition() const {
  return boardCenter() + boardRadius() * Vec(0.0, -1.7f, 1.0);
}
