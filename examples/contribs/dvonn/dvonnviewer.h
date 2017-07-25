#ifndef DVONNVIEWER_H
#define DVONNVIEWER_H

#include "game.h"

#include <QGLViewer/qglviewer.h>
#include <list>

class QTimer;

namespace dvonn {
class Game;
class Drawer;
} // namespace dvonn

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of DvonnViewer
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DvonnViewer : public QGLViewer {
  Q_OBJECT

public:
#if QT_VERSION < 0x040000
  DvonnViewer(QWidget *parent = NULL, const char *name = 0);
#else
  DvonnViewer(QWidget *parent = NULL);
#endif
  ~DvonnViewer();
  void setGame(dvonn::Game *g);
  void fadeOut(const dvonn::Board::Ghosts *g);
  void animateMove(dvonn::Game::Move m);
  void stopAllAnimations();
Q_SIGNALS:
  void requested(dvonn::Game::Placement);
  void requested(dvonn::Game::Move);
public Q_SLOTS:
  void toggleTexture(bool);
  void toggleLight(bool);
  void toggleShowPossible(bool);
  void toggleShowStatus(bool);
  void toggleShowLabels(bool);
  void toggleShowAnimation(bool);
  void toggleDragToPlay(bool);
  void animateScore();
protected Q_SLOTS:
  void advanceFadeOut();
  void advanceAnimateMove();
  void advanceAnimateScore();

protected:
  virtual void init();
  void initOpenGL();
  void initSpotLight();
  void initViewer();
  virtual void draw();
  void drawAllPieces(bool pick = false);
  void drawAllSpaces(bool pick = false);
  virtual void drawWithNames();
  virtual void postSelection(const QPoint &point);
  virtual QString helpString() const;

  virtual void keyPressEvent(QKeyEvent *e);
  virtual void mousePressEvent(QMouseEvent *e);
  virtual void mouseMoveEvent(QMouseEvent *e);
  virtual void mouseReleaseEvent(QMouseEvent *e);
  void commitDstPicked();

private:
  dvonn::Game *game_;
  dvonn::Drawer *drawer_;

  int selectionMode_;
  bool piecePicked_;
  dvonn::Board::ConstStackHandle dstPicked_;
  dvonn::Board::ConstStackHandle srcPicked_;
  bool showPossDest_;
  bool showStatus_;
  bool showLabels_;
  bool useLight_;
  std::deque<dvonn::Board::ConstStackHandle> possDests_;
  bool dragToPlay_;
  const dvonn::Board::Ghosts *fadeGhosts_;
  QTimer *fadeTimer_;
  float fadeAlpha_;
  QTimer *animateTimer_;
  float animateT_;
  dvonn::Game::Move animateMove_;
  bool showAnimation_;
  QTimer *scoreTimer_;
  float scoreT_;
  dvonn::Game::Move scoreMove_;
};
#endif // DVONNVIEWER_H
