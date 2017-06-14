#include "dvonnviewer.h"
#include "drawer.h"
#include "game.h"
#include <fstream>
#include <math.h>
#include <qmessagebox.h>

#include <QGLViewer/manipulatedCameraFrame.h>

#include <QMouseEvent>

using namespace std;
using namespace qglviewer;
using namespace dvonn;

namespace {
int debugInterval;
const unsigned int nbAnimationSteps = 30;
class BoardConstraint : public Constraint {
public:
  virtual void constrainRotation(Quaternion &q, Frame *const fr) {
    const Vec up = fr->transformOf(Vec(0.0, 0.0, 1.0));
    Vec axis = q.axis();
    float angle = 2.0 * acos(q[3]);
    if (fabs(axis * up) > fabs(axis.x))
      axis.projectOnAxis(up);
    else {
      angle = (axis.x > 0.0) ? angle : -angle;
      axis.setValue(fabs(axis.x), 0.0, 0.0);
      const float currentAngle =
          asin(fr->inverseTransformOf(Vec(0.0, 0.0, -1.0)).z);
      if (currentAngle + angle > -0.2)
        angle = -0.2 - currentAngle; // Not too low
      if (currentAngle + angle < -M_PI / 2.0)
        angle = -M_PI / 2.0 - currentAngle; // Do not pass on the other side
    }
    q = Quaternion(axis, angle);
  }
};
} // namespace
//************************************************************
// Implementation of DvonnViewer
//************************************************************
#if QT_VERSION < 0x040000
DvonnViewer::DvonnViewer(QWidget *parent, const char *name)
    : QGLViewer(parent, name),
#else
DvonnViewer::DvonnViewer(QWidget *parent)
    : QGLViewer(parent),
#endif
      game_(NULL), selectionMode_(-1), piecePicked_(false),
      dstPicked_(Board::ConstStackHandle::null()),
      srcPicked_(Board::ConstStackHandle::null()), showPossDest_(true),
      showStatus_(false), showLabels_(false), useLight_(true),
      dragToPlay_(false), fadeGhosts_(NULL), animateT_(-1.0f),
      showAnimation_(true), scoreT_(-1.0f) {
  drawer_ = new Drawer;
  fadeTimer_ = new QTimer();
  connect(fadeTimer_, SIGNAL(timeout()), this, SLOT(advanceFadeOut()));
  animateTimer_ = new QTimer();
  connect(animateTimer_, SIGNAL(timeout()), this, SLOT(advanceAnimateMove()));
  scoreTimer_ = new QTimer();
  connect(scoreTimer_, SIGNAL(timeout()), this, SLOT(advanceAnimateScore()));
}
DvonnViewer::~DvonnViewer() {
  delete drawer_;
  delete fadeTimer_;
  delete animateTimer_;
  delete scoreTimer_;
}
void DvonnViewer::advanceFadeOut() {
  fadeAlpha_ -= 0.05f;
  if (fadeAlpha_ < 0.0f) {
    fadeTimer_->stop();
    fadeAlpha_ = 0.0f;
    fadeGhosts_ = NULL;
  }
  update();
}
void DvonnViewer::fadeOut(const Board::Ghosts *g) {
  if ((fadeGhosts_ = g) != NULL && !g->empty()) {
    fadeAlpha_ = 1.0f;
    fadeTimer_->start(30);
  }
}
void DvonnViewer::advanceAnimateMove() {
  animateT_ += 1.0f / nbAnimationSteps;
  if (animateT_ >= 1.0f) {
    animateTimer_->stop();
    animateT_ = -1.0f;
    Q_EMIT requested(animateMove_);
  }
  update();
}
void DvonnViewer::animateMove(Game::Move m) {
  if (animateT_ < 0.0f) {
    if (showAnimation_) {
      animateMove_ = m;
      animateT_ = 0.0f;
      static const float v = 1.0f / 250;
      const float d = drawer_->estimateDrawMoveLength(game_->board(), m);
      const float T = d / v;
      animateTimer_->start(static_cast<int>(T / nbAnimationSteps));
    } else {
      Q_EMIT requested(m);
    }
  }
}
void DvonnViewer::advanceAnimateScore() {
  scoreT_ += 1.0f / nbAnimationSteps;
  if (scoreT_ >= 1.0f) {
    scoreTimer_->stop();
    scoreT_ = -1.0f;
    Q_EMIT requested(scoreMove_);
  }
  update();
}
void DvonnViewer::animateScore() {
  if (scoreT_ < 0.0f) {
    // Stack will be moved to to positions at center of the board
    static const Board::Coord centers[2] = {
        Board::Coord(Board::nbSpacesMaxOnRow() / 2, Board::nbRows() / 2 + 1),
        Board::Coord(Board::nbSpacesMaxOnRow() / 2 + 1,
                     Board::nbRows() / 2 + 1)};
    // Search for a stack to move
    for (Board::ConstStackIterator iter = game_->board().stacks_begin(),
                                   istop = game_->board().stacks_end();
         iter != istop; ++iter) {
      if (iter.stackCoord() != centers[0] && iter.stackCoord() != centers[1] &&
          iter->hasPieces()) {
        Color c = iter->onTop()->color();
        if (c != Red) {
          scoreMove_ = Game::Move(iter.stackCoord(), centers[c - 1]);
          scoreT_ = 0.0f;
          static const float v = 1.0f / 250;
          const float d =
              drawer_->estimateDrawMoveLength(game_->board(), scoreMove_);
          const float T = d / v;
          scoreTimer_->start(debugInterval =
                                 static_cast<int>(T / nbAnimationSteps));
          return;
        }
      }
    }
  }
}
void DvonnViewer::stopAllAnimations() {
  fadeTimer_->stop();
  fadeAlpha_ = 0.0f;
  fadeGhosts_ = NULL;

  animateTimer_->stop();
  animateT_ = -1.0f;

  scoreTimer_->stop();
  scoreT_ = -1.0f;

  update();
}
void DvonnViewer::setGame(Game *g) { game_ = g; }
void DvonnViewer::toggleTexture(bool b) {
  drawer_->toggleTexture(b);
  update();
}
void DvonnViewer::toggleLight(bool b) {
  useLight_ = b;
  update();
}
void DvonnViewer::toggleShowPossible(bool b) {
  showPossDest_ = b;
  if (showPossDest_ && game_->phase() == MovePhase && !srcPicked_.isNull()) {
    possDests_ = game_->possibleDestinations(srcPicked_);
  }
  update();
}
void DvonnViewer::toggleShowStatus(bool b) {
  showStatus_ = b;
  update();
}
void DvonnViewer::toggleShowLabels(bool b) {
  showLabels_ = b;
  update();
}
void DvonnViewer::toggleShowAnimation(bool b) {
  showAnimation_ = b;
  update();
}
void DvonnViewer::toggleDragToPlay(bool b) {
  dragToPlay_ = b;
  update();
}
// I n i t i a l i z a t i o n   f u n c t i o n s //
void DvonnViewer::init() {
  initOpenGL();
  initSpotLight();
  initViewer();

  drawer_->init();
}
void DvonnViewer::initOpenGL() {
  glCullFace(GL_BACK);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void DvonnViewer::initSpotLight() {
  const GLfloat light_ambient[4] = {1.0, 1.0, 1.0, 1.0};
  const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
  const GLfloat light_diffuse[4] = {1.0, 1.0, 1.0, 1.0};

  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1f);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.3f);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3f);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

  const Vec pos = drawer_->boardCenter() +
                  drawer_->boardRadius() * drawer_->boardUpVector();
  const float posv[4] = {pos.x, pos.y, pos.z, 1.0f};
  const Vec dir = -drawer_->boardUpVector();
  const float dirv[4] = {dir.x, dir.y, dir.z, 1.0f};
  glLightfv(GL_LIGHT1, GL_POSITION, posv);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dirv);
}
void DvonnViewer::initViewer() {
  setSceneCenter(drawer_->boardCenter());
  setSceneRadius(drawer_->boardRadius());
  camera()->setUpVector(drawer_->boardUpVector());
  camera()->setPosition(drawer_->defaultEyePosition());
  camera()->lookAt(sceneCenter());
  // Limit camera rotation motion
  camera()->frame()->setConstraint(new BoardConstraint());

  clearMouseBindings();
  // Defines new bindings
  setMouseBindingDescription(Qt::NoModifier, Qt::LeftButton, "Moves stack");
  setMouseBinding(Qt::ControlModifier, Qt::LeftButton, CAMERA, ROTATE);
  setMouseBinding(Qt::NoModifier, Qt::RightButton, CAMERA, TRANSLATE);
  setMouseBinding(Qt::NoModifier, Qt::MidButton, ZOOM_TO_FIT, true);
  setMouseBinding(Qt::NoModifier, Qt::RightButton, ZOOM_TO_FIT, true);
  setMouseBinding(Qt::ControlModifier, Qt::LeftButton, RAP_FROM_PIXEL, true);
  setMouseBinding(Qt::ControlModifier, Qt::RightButton, RAP_IS_CENTER, true);

  setWheelBinding(Qt::NoModifier, CAMERA, ZOOM);

#if QT_VERSION >= 0x040000
  setWheelBinding(Qt::ControlModifier, FRAME, NO_MOUSE_ACTION);
#else
  setWheelBinding(Qt::ControlButton, FRAME, NO_MOUSE_ACTION);
#endif
}
void DvonnViewer::draw() {
  (useLight_ ? glEnable : glDisable)(GL_LIGHTING);
  glEnable(GL_LIGHT1);

  drawAllSpaces();
  drawer_->drawComplement(showLabels_);
  drawAllPieces();
  Player p = game_->theOnePlaying();
  Color c = game_->phase() == RedPlacementPhase ? Red : colorOf(p);
  drawer_->drawWhitePiecePools(game_->board(),
                               p == WhitePlayer && piecePicked_);
  drawer_->drawBlackPiecePools(game_->board(),
                               p == BlackPlayer && piecePicked_);
  if (piecePicked_ && !dstPicked_.isNull()) {
    drawer_->drawTransparentPiece(c, dstPicked_);
  }
  if (!piecePicked_ && !srcPicked_.isNull()) {
    drawer_->drawTransparentPieces(srcPicked_->begin(), srcPicked_->end(),
                                   srcPicked_.stackCoord(), 0.0f, 0.4f);
    if (showPossDest_) {
      glColor3f(1.0f, 1.0f, 0.0f);
      for (deque<Board::ConstStackHandle>::const_iterator iter =
               possDests_.begin();
           iter != possDests_.end(); ++iter) {
        drawer_->highlightPieces(*iter);
      }
    }
    if (!dstPicked_.isNull()) {
      drawer_->drawTransparentPieces(srcPicked_->begin(), srcPicked_->end(),
                                     dstPicked_.stackCoord(),
                                     dstPicked_->height(), 0.9f);
    }
  }
  // Ghosts
  if (fadeGhosts_) {
    for (Board::Ghosts::const_iterator iter = fadeGhosts_->begin();
         iter != fadeGhosts_->end(); ++iter) {
      drawer_->drawTransparentPieces(iter->stack.begin(), iter->stack.end(),
                                     iter->coord, 0.0f,
                                     fadeAlpha_ * fadeAlpha_);
    }
  }
  // Animated move
  if (animateT_ >= 0.0f) {
    drawer_->drawMove(game_->board(), animateMove_, animateT_);
  }
  if (scoreT_ >= 0.0f) {
    drawer_->drawMove(game_->board(), scoreMove_, scoreT_);
  }
}
void DvonnViewer::drawAllPieces(bool pick) {
  unsigned int name = 0;
  for (Board::ConstStackIterator iter = game_->board().stacks_begin(),
                                 istop = game_->board().stacks_end();
       iter != istop; ++iter) {
    if (pick)
      glPushName(name++);
    if (srcPicked_ != iter &&
        (animateT_ < 0.0f || iter.stackCoord() != animateMove_.src) &&
        (scoreT_ < 0.0f || iter.stackCoord() != scoreMove_.src)) {
      drawer_->drawPieces(iter);
    }
    if (pick)
      glPopName();
  }
  if (showStatus_ && !pick) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 1.0, 0.0f);
    for (Board::ConstStackIterator iter = game_->board().stacks_begin(),
                                   istop = game_->board().stacks_end();
         iter != istop; ++iter) {
      drawer_->drawStatus(iter, this);
    }
  }
  glPopAttrib();
}
void DvonnViewer::drawAllSpaces(bool pick) {
  unsigned int name = 0;
  for (Board::ConstStackIterator iter = game_->board().stacks_begin(),
                                 istop = game_->board().stacks_end();
       iter != istop; ++iter) {
    if (pick)
      glPushName(name++);
    drawer_->draw(iter);
    if (pick)
      glPopName();
  }
}
void DvonnViewer::drawWithNames() {
  if (game_->isOver())
    return;
  switch (selectionMode_) {
  case 1:
    glPushName(0);
    if (game_->theOnePlaying() == WhitePlayer) {
      drawer_->drawWhitePiecePools(game_->board(), false);
    } else {
      drawer_->drawBlackPiecePools(game_->board(), false);
    }
    glPopName();
    break;
  case 2:
  case 5:
    drawAllSpaces(true);
    break;
  case 3:
  case 4:
    drawAllPieces(true);
    break;
  default:
    cout << "No selection mode active!" << endl;
  }
}
void DvonnViewer::postSelection(const QPoint &) {
  switch (selectionMode_) {
  case 1:
    piecePicked_ = (selectedName() != -1);
    break;
  case 2:
    if (selectedName() != -1) {
      Board::ConstStackIterator iter = game_->board().stacks_begin();
      for (int i = 0; i < selectedName(); ++i)
        ++iter;
      dstPicked_ = iter;
      if (dstPicked_->hasPieces())
        dstPicked_ = Board::ConstStackHandle::null();
    } else {
      dstPicked_ = Board::ConstStackHandle::null();
    }
    break;
  case 3:
    if (selectedName() != -1) {
      Board::ConstStackIterator iter = game_->board().stacks_begin();
      for (int i = 0; i < selectedName(); ++i)
        ++iter;
      srcPicked_ = iter;
      if (!srcPicked_.isNull() &&
          srcPicked_->onTop()->color() != colorOf(game_->theOnePlaying())) {
        srcPicked_ = Board::ConstStackHandle::null();
      }
    } else {
      srcPicked_ = Board::ConstStackHandle::null();
    }
    break;
  case 4:
  case 5:
    if (selectedName() != -1) {
      Board::ConstStackIterator iter = game_->board().stacks_begin();
      for (int i = 0; i < selectedName(); ++i)
        ++iter;
      dstPicked_ = iter;
      if (dstPicked_ == srcPicked_) {
        dstPicked_ = Board::ConstStackHandle::null();
      }
      if (!dstPicked_.isNull() &&
          !game_->isLegalMove(
              Game::Move(srcPicked_.stackCoord(), dstPicked_.stackCoord()))) {
        dstPicked_ = Board::ConstStackHandle::null();
      }
    } else {
      dstPicked_ = Board::ConstStackHandle::null();
    }
    break;
  };
  selectionMode_ = -1;
}
void DvonnViewer::mousePressEvent(QMouseEvent *e) {
#if QT_VERSION >= 0x040000
  if (e->button() == Qt::LeftButton)
#else
  if (e->stateAfter() == Qt::LeftButton)
#endif
  {
    if (game_->phase() == RedPlacementPhase ||
        game_->phase() == PiecePlacementPhase) {
      if (dragToPlay_) {
        selectionMode_ = 1;
        select(e);
        if (!dstPicked_.isNull()) {
          piecePicked_ = true;
        }
      } else {
        piecePicked_ = true;
        ;
        Board::ConstStackHandle firstClickDstPicked = dstPicked_;
        selectionMode_ = 2;
        select(e);
        if (dstPicked_ == firstClickDstPicked) {
          commitDstPicked();
        } else if (dstPicked_.isNull()) {
          piecePicked_ = false;
        }
      }
      update();
    } else // phase == MovePhase
    {
      if (dragToPlay_ || (!dragToPlay_ && srcPicked_.isNull())) {
        selectionMode_ = 3;
        select(e);
        // Check that the picked src is free
        if (!srcPicked_.isNull() && !game_->board().isFree(srcPicked_)) {
          srcPicked_ = Board::ConstStackHandle::null();
        }
        // If asked, search for possible destinations
        if (showPossDest_ && !srcPicked_.isNull()) {
          possDests_ = game_->possibleDestinations(srcPicked_);
        }
        setMouseTracking(true);
      } else {
        selectionMode_ = 4;
        select(e);
        // Since selection in mode 4 can work only for case with pieces, we
        // try to select a case if no case is selected yet.
        if (dstPicked_.isNull()) {
          selectionMode_ = 5;
          select(e);
        }
        commitDstPicked();
      }
      update();
    }
  } else
    QGLViewer::mousePressEvent(e);
}
void DvonnViewer::mouseMoveEvent(QMouseEvent *e) {
#if QT_VERSION >= 0x040000
  if ((dragToPlay_ && e->button() == Qt::LeftButton) ||
#else
  if ((dragToPlay_ && e->stateAfter() == Qt::LeftButton) ||
#endif
      ((!dragToPlay_) && (!srcPicked_.isNull()) &&
       (!camera()->frame()->isManipulated()))) {
    if (game_->phase() == RedPlacementPhase ||
        game_->phase() == PiecePlacementPhase) {
      if (piecePicked_) {
        selectionMode_ = 2;
        select(e);
        update();
      }
    } else // phase == MovePhase
    {
      if (!srcPicked_.isNull()) {
        selectionMode_ = 4;
        select(e);
        // Since selection in mode 4 can work only for case with pieces, we
        // try to select a case if no case is selected yet.
        if (dstPicked_.isNull()) {
          selectionMode_ = 5;
          select(e);
        }
        update();
      }
    }
  } else
    QGLViewer::mouseMoveEvent(e);
}
void DvonnViewer::mouseReleaseEvent(QMouseEvent *e) {
#if QT_VERSION >= 0x040000
  if (e->button() == Qt::LeftButton)
#else
  if (e->state() == Qt::LeftButton)
#endif
  {
    if (dragToPlay_) {
      commitDstPicked();
    }
  } else
    QGLViewer::mouseReleaseEvent(e);
}
void DvonnViewer::commitDstPicked() {
  if (game_->phase() == RedPlacementPhase ||
      game_->phase() == PiecePlacementPhase) {
    if (piecePicked_ && !dstPicked_.isNull()) {
      Player p = game_->theOnePlaying();
      Q_EMIT requested(Game::Placement(
          game_->phase() == RedPlacementPhase ? Red : colorOf(p),
          dstPicked_.stackCoord()));
      update();
    }
  } else // phase == MovePhase
  {
    if (!srcPicked_.isNull() && !dstPicked_.isNull()) {
      Q_EMIT requested(
          Game::Move(srcPicked_.stackCoord(), dstPicked_.stackCoord()));
    }
  }
  piecePicked_ = false;
  dstPicked_ = Board::ConstStackHandle::null();
  srcPicked_ = Board::ConstStackHandle::null();
  setMouseTracking(false);

  update();
}
void DvonnViewer::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_D && scoreT_ > 0.0f) {
    if (scoreTimer_->isActive())
      scoreTimer_->stop();
    else
      scoreTimer_->start(debugInterval);
  } else if (e->key() == Qt::Key_T) {
    toggleShowStatus(!showStatus_);
  } else
    QGLViewer::keyPressEvent(e);
}
QString DvonnViewer::helpString() const {
  QString text("<h2>D v o n n</h2>");
  text += "See the <i>Help/Rules of Dvonn</i> menu for the rules of the "
          "game.<br><br>";
  text += "Use the mouse left button to play. Middle and right buttons move "
          "camera.";
  text += "Use <b>Ctrl+left</b> to rotate the camera. See the mouse tab for "
          "complete mouse bindings.";
  return text;
}
