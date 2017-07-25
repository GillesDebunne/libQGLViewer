#ifndef DVONNWINDOWIMPL_H
#define DVONNWINDOWIMPL_H

#include <qglobal.h>

#if QT_VERSION >= 0x040000
#include "ui_dvonnwindow.h"
class DvonnWindow : public QMainWindow, public Ui::DvonnWindow {
public:
  DvonnWindow() { setupUi(this); }
};
#else
#if QT_VERSION >= 0x030000
#include "dvonnwindow.Qt3.h"
#else
#error "No designer .ui file available for Qt 2"
#endif
#endif
#include "game.h"

class QTextBrowser;
class QLabel;
class QTimer;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of DvonnWindowImpl
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DvonnWindowImpl : public DvonnWindow {
  Q_OBJECT
public:
  DvonnWindowImpl(dvonn::Game *g);
public Q_SLOTS:
  void load();
  void save();
  void saveAs();
  void print();

  void newGame();
  void undo();
  void redo();

  void help();
  void about();

  void randomlyFinishPlacement();
  void randomlyMoveStack();
  void play(dvonn::Game::Placement);
  void play(dvonn::Game::Move);

protected:
  QString labelFor(dvonn::Player);
  QString labelFor(dvonn::Phase);
  void updateStatusBar();
protected Q_SLOTS:
  void animateScore();

private:
  dvonn::Game *game_;
  QTextBrowser *rulesBrowser_;
  QLabel *currentPlayerLB_;
  QLabel *currentPhaseLB_;
  QTimer *startAnimScoreTimer_;
};

#endif // DVONNWINDOWIMPL_H
