#ifndef DVONNWINDOWIMPL_H
#define DVONNWINDOWIMPL_H

#include "dvonnwindow.h"
#include "game.h"

class QTextBrowser;
class QLabel;
class QTimer;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Interface of DvonnWindowImpl
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
class DvonnWindowImpl : public DvonnWindow
{
  Q_OBJECT
public:
  DvonnWindowImpl(dvonn::Game* g,QWidget* parent=NULL,const char* name="");
public slots:
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
protected slots:
  void animateScore();
private:
  dvonn::Game*  game_;
  QTextBrowser* rulesBrowser_;
  QLabel*       currentPlayerLB_;
  QLabel*       currentPhaseLB_;
  QTimer*       startAnimScoreTimer_;

};

#endif // DVONNWINDOWIMPL_H
