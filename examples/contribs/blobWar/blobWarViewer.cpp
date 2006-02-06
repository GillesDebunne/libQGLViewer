#include "blobWarViewer.h"
#include "board.h"

#include <fstream>
#include <qmessagebox.h>
#include <qfileinfo.h>
#include <qfiledialog.h>
#include <qstatusbar.h>

#if QT_VERSION >= 0x040000
# include "ui_blobWarWindow.Qt4.h"
  class BlobWarWindow : public QMainWindow, public Ui::BlobWarWindow {};
#else
# include <qmainwindow.h>
#endif

using namespace std;
using namespace qglviewer;

static QString colorString[2] = { "Red", "Blue" };


class BoardConstraint : public Constraint
{
public:
  virtual void constrainRotation(Quaternion& q, Frame * const fr)
  {
    const Vec up = fr->transformOf(Vec(0.0, 0.0, 1.0));
    Vec axis = q.axis();
    float angle = 2.0*acos(q[3]);
    if (fabs(axis*up) > fabs(axis.x))
      axis.projectOnAxis(up);
    else
      {
	angle = (axis.x > 0.0) ? angle : -angle;
	axis.setValue(fabs(axis.x), 0.0, 0.0);
	const float currentAngle = asin(fr->inverseTransformOf(Vec(0.0, 0.0, -1.0)).z);
	if (currentAngle + angle > -0.2)
	  angle = -0.2 - currentAngle; // Not too low
	if (currentAngle + angle < -M_PI/2.0)
	  angle = -M_PI/2.0 - currentAngle; // Do not pass on the other side
      }
    q = Quaternion(axis, angle);
  }
};


#if QT_VERSION < 0x040000
BlobWarViewer::BlobWarViewer(QWidget* parent, const char* name)
  : QGLViewer(parent, name),
#else
BlobWarViewer::BlobWarViewer(QWidget* parent)
  : QGLViewer(parent),
#endif
    boardFileName_("4x4.bwb"), selectedPiece_(-1),
    displayPossibleMoves_(true), animatePlays_(false)
{  
  // QObject::connect(&kfi_[0], SIGNAL(interpolated()), this, SLOT(updateGL()));
  // QObject::connect(&kfi_[0], SIGNAL(endReached()), this, SLOT(simplePlay()));
  for (int i=0; i<2; ++i)
    connect(&(computerPlayer_[i]), SIGNAL(moveMade(QString, int)), this, SLOT(playComputerMove(QString, int)));

  // Red player is computer
  computerPlayer_[0].setProgramFileName("greedy");
  computerPlayer_[0].setIsActive(true);
}


// I n i t i a l i z a t i o n   f u n c t i o n s

void BlobWarViewer::init()
{
  initViewer();
  fitCameraToBoard();

  setMouseBinding(Qt::RightButton, CAMERA, ROTATE);
  setMouseBinding(Qt::LeftButton, SELECT);

#if QT_VERSION >= 0x040000
  // Signals and slots connections
  BlobWarWindow* bww = (BlobWarWindow*)(window());
  connect(bww->fileExitAction, SIGNAL(activated()), bww, SLOT(close()));
  connect(bww->fileOpenAction, SIGNAL(activated()), this, SLOT(load()));
  connect(bww->fileSaveAction, SIGNAL(activated()), this, SLOT(save()));
  connect(bww->fileSaveAsAction, SIGNAL(activated()), this, SLOT(saveAs()));
  connect(bww->gameNewGameAction, SIGNAL(activated()), this, SLOT(newGame()));
  connect(bww->gameUndoAction, SIGNAL(activated()), this, SLOT(undo()));
  connect(bww->gameRedoAction, SIGNAL(activated()), this, SLOT(redo()));
  
  connect(bww->gameBlueIsHumanAction, SIGNAL(toggled(bool)), this, SLOT(bluePlayerIsHuman(bool)));
  connect(bww->gameRedIsHumanAction, SIGNAL(toggled(bool)), this, SLOT(redPlayerIsHuman(bool)));
  
  connect(bww->gameBlueIsHumanAction, SIGNAL(toggled(bool)), bww->gameConfigureBluePlayerAction, SLOT(setDisabled(bool)));
  connect(bww->gameRedIsHumanAction, SIGNAL(toggled(bool)), bww->gameConfigureRedPlayerAction, SLOT(setDisabled(bool)));
  
  connect(bww->gameConfigureBluePlayerAction, SIGNAL(activated()), this, SLOT(configureBluePlayer()));
  connect(bww->gameConfigureRedPlayerAction, SIGNAL(activated()), this, SLOT(configureRedPlayer()));
  
  connect(bww->togglePossibleMoveAction, SIGNAL(toggled(bool)), this, SLOT(toggleDisplayPossibleMoves(bool)));
  connect(bww->toggleAnimationAction, SIGNAL(toggled(bool)), this, SLOT(toggleAnimation(bool)));
  
  connect(bww->helpAboutAction, SIGNAL(activated()), this, SLOT(about()));
  connect(bww->helpRulesAction, SIGNAL(activated()), this, SLOT(displayRules()));
#endif
}

void BlobWarViewer::initViewer()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  board_ = new Board();
  boardFileName_ = "Boards/4x4.bwb";
  newGame();
}

void BlobWarViewer::newGame()
{
  if (!QFileInfo(boardFileName_).isReadable())
    {
      QMessageBox::warning(NULL ,"Unreadable board file", "Unable to read board file ("+boardFileName_+").\nLoad another board.");
      return;
    }
  
#if QT_VERSION < 0x040000
  std::ifstream f(boardFileName_.latin1());
#else
  std::ifstream f(boardFileName_.toLatin1().constData());
#endif
  f >> *board_;
  f.close();
  
  // todo :
    // - delay avant undo
    // - regles
    // - animation

  selectedPiece_ = -1;

  playNextMove();
}

void BlobWarViewer::fitCameraToBoard()
{
  static BoardConstraint* boardConstraint = new BoardConstraint();
  
  // Free camera rotation motion
  camera()->frame()->setConstraint(NULL);

  setSceneCenter(Vec(board_->size().width()/2.0, board_->size().height()/2.0, 0.0));
  setSceneRadius(sceneCenter().norm());

  camera()->setUpVector(Vec(0.0, 0.0, 1.0));
  camera()->setPosition(Vec(2.0*sceneCenter().x, -1.5*sceneCenter().y, 0.8*(sceneCenter().x+sceneCenter().y)));
  camera()->lookAt(sceneCenter());
  showEntireScene();

  // Limit camera rotation motion
  camera()->frame()->setConstraint(boardConstraint);
}


// D r a w i n g  f u n c t i o n s
void BlobWarViewer::draw()
{
  board_->draw();

  if (selectedPiece_ >= 0)
    {
      board_->drawSelectedPiece(selectedPiece_);
      if (displayPossibleMoves_)
	board_->drawPossibleDestinations(selectedPiece_);
    }

  board_->drawShadows();
}

// G a m e   I n t e r f a c e
void BlobWarViewer::play(const Move& m)
{
  if (animatePlays_)
    animatePlay();
  else
    simplePlay(m);
}

void BlobWarViewer::simplePlay(const Move& m)
{
  board_->play(m);  
  updateGL();
  playNextMove();
}

void BlobWarViewer::playNextMove()
{
  static const QString stateFileName = "board.bwb";

  // Updates statusBar
#if QT_VERSION < 0x040000
  ((QMainWindow*)(((QWidget*)parent())->parent()))->statusBar()->message(board_->statusMessage());
#else
  ((QMainWindow*)(((QWidget*)parent())->parent()))->statusBar()->showMessage(board_->statusMessage());
#endif

  if (board_->gameIsOver())
    QMessageBox::information(this, "Game over", board_->statusMessage());
  else
    {
      // Save board state
#if QT_VERSION < 0x040000
      std::ofstream f(stateFileName.latin1());
#else
      std::ofstream f(stateFileName.toLatin1().constData());
#endif
      f << *board_;
      f.close();

      computerPlayer_[board_->bluePlays()].play(board_->bluePlays(), stateFileName);
    }
}

void BlobWarViewer::playComputerMove(QString move, int duration)
{
  QString message = colorString[board_->bluePlays()]+" program played %1 in %2 out of %3 seconds.";
  message = message.arg(move).arg(duration/1000.0, 0, 'f', 1).arg(computerPlayer_[board_->bluePlays()].allowedTime());
#if QT_VERSION < 0x040000
  qWarning(message.latin1());
#else
  qWarning(message.toLatin1().constData());
#endif
  Move m(move);
  if (m.isValid(board_))
    play(m);
  else
    QMessageBox::warning(this, "Wrong move", "The move "+move+" is not legal for "+colorString[board_->bluePlays()]);
}

void BlobWarViewer::animatePlay()
{
  /*
  float start = 0.0;
  float end = 0.8;
  // int hpops = higherPieceOnPosition(play_.depart());
  // int hpope = higherPieceOnPosition(play_.arrivee());
  // revoStart_.clear();
  // revoEnd_.clear();
  // swapArrival_.clear();

  if (hpops < 0)
    cerr << "Internal error" << endl;

  int nbKfi = 0;
  if (play_.dessus())
    {
      for (int i=0; i<16; ++i)
	if ((piece_[i].square == play_.arrivee()) && (piece_[i].isBlack != piece_[hpops].isBlack) && (piece_[hpops].isBlack != piece_[hpope].isBlack))
	  {
	    // piece_[i].isBlack = piece_[hpops].isBlack;
	    nbKfi++;
	    kfi_[nbKfi].deletePath();
	    kfi_[nbKfi].setFrame(&piece_[i].frame);
	    kfi_[nbKfi].addKeyFrame(piece_[i].frame, 0.0);
	    Frame midFrame(piece_[i].frame.position()+Vec(0.0, 0.0, 4*pieceHeight*(piece_[i].level+2)),
			   Quaternion(Vec(1.0, 0.0, 0.0), 2.0));
	    kfi_[nbKfi].addKeyFrame(midFrame, 0.3);
	    Frame endFrame(piece_[i].frame.position()+Vec(0.0, 0.0, pieceHeight),
			   Quaternion(Vec(1.0, 0.0, 0.0), M_PI));
	    kfi_[nbKfi].addKeyFrame(endFrame, 0.6);
	    start = 0.6;
	    swapArrival_.push_back(i);
	  }
    }
  else
    {
      for (int i=0; i<16; ++i)
	if (piece_[i].square == play_.arrivee())
	  {
	    nbKfi++;
	    kfi_[nbKfi].deletePath();
	    kfi_[nbKfi].setFrame(&piece_[i].frame);
	    kfi_[nbKfi].addKeyFrame(piece_[i].frame, 0.0);
	    Frame midFrame, endFrame;
	    if ((blobWar.ArriveeRevolution(play_)) && (piece_[i].isBlack != piece_[hpops].isBlack))
	      {
		midFrame = Frame(piece_[i].frame.position()+Vec(0.0, 0.0, 3.0*pieceHeight*(piece_[i].level+2)),
				 Quaternion(Vec(1.0, 0.0, 0.0), 2.0));
		endFrame = Frame(piece_[i].frame.position()+Vec(0.0, 0.0, 2.0*pieceHeight),
				 Quaternion(Vec(1.0, 0.0, 0.0), M_PI));
		revoEnd_.push_back(i);
		// piece_[i].isBlack = piece_[hpops].isBlack;
	      }
	    else
	      {
		midFrame.setPosition(piece_[i].frame.position()+Vec(0.0, 0.0, 3.0*pieceHeight*(piece_[i].level+2)));
		endFrame.setPosition(piece_[i].frame.position()+Vec(0.0, 0.0, pieceHeight));
	      }
	    kfi_[nbKfi].addKeyFrame(midFrame, 0.4);
	    kfi_[nbKfi].addKeyFrame(endFrame, 1.0);
	    end = 1.0;
	  }
    }

  // Revolution on starting case
  if (blobWar.DepartRevolution(play_))
    for (int i=0; i<16; ++i)
      if ((piece_[i].square == play_.depart()) && (i!=hpops) && (piece_[i].isBlack == piece_[hpops].isBlack))
	{
	  nbKfi++;
	  kfi_[nbKfi].deletePath();
	  kfi_[nbKfi].setFrame(&piece_[i].frame);
	  kfi_[nbKfi].addKeyFrame(piece_[i].frame, 0.0);
	  kfi_[nbKfi].addKeyFrame(piece_[i].frame, 0.6);
	  Frame midFrame(piece_[i].frame.position()+Vec(0.0, 0.0, 3.0*pieceHeight*(piece_[i].level+2)),
			 Quaternion(Vec(1.0, 0.0, 0.0), 2.0));
	  Frame endFrame(piece_[i].frame.position()+Vec(0.0, 0.0, pieceHeight),
			 Quaternion(Vec(1.0, 0.0, 0.0), M_PI));

	  // piece_[i].isBlack = !piece_[hpops].isBlack;
	  kfi_[nbKfi].addKeyFrame(midFrame, 0.9);
	  kfi_[nbKfi].addKeyFrame(endFrame, 1.5);
	  end = 1.5;
	  revoStart_.push_back(i);
	}

  if (!revoStart_.empty())   QTimer::singleShot(900 , this, SLOT(swapRevoStartColor()));
  if (!revoEnd_.empty())     QTimer::singleShot(400 , this, SLOT(swapRevoEndColor()));
  if (!swapArrival_.empty()) QTimer::singleShot(300 , this, SLOT(swapArrivalColor()));

  // Selected piece
  kfi_[0].deletePath();
  kfi_[0].setFrame(&piece_[hpops].frame);
  Frame arrival;
  if ((hpope >= 0) && (play_.dessus()))
    arrival.setPosition(piece_[hpope].frame.position() + Vec(0.0, 0.0, pieceHeight));
  else
    arrival.setPosition(casePosition[play_.arrivee()]);

  Vec mid(0.5 * (piece_[hpops].frame.position() + arrival.position()));
  mid.z = max(piece_[hpops].frame.position().z, arrival.position().z) + 2.0 * pieceHeight;
  const Frame intermediate(mid, Quaternion());

  kfi_[0].addKeyFrame(piece_[higherPieceOnPosition(play_.depart())].frame, 0.0);
  kfi_[0].addKeyFrame(piece_[higherPieceOnPosition(play_.depart())].frame, start);
  kfi_[0].addKeyFrame(intermediate, start+0.3);
  kfi_[0].addKeyFrame(arrival, start+0.8);
  kfi_[0].addKeyFrame(arrival, start+end);

  for (int i=1; i<=nbKfi; ++i)
    kfi_[i].startInterpolation();
  kfi_[0].startInterpolation();
  */
}




// M o v e   S e l e c t i o n
void BlobWarViewer::drawWithNames()
{
  // Selection enabled only when the computer program is not active
  if (!computerPlayer_[board_->bluePlays()].isActive())
    {
      if (selectedPiece_ >= 0)
	board_->drawPossibleDestinations(selectedPiece_, true);
  
      // Always drawned, so that a new selection can occur
      board_->drawSelectablePieces();
    }
}

void BlobWarViewer::postSelection(const QPoint&)
{
  if (selectedPiece_ >= 0)
    {
      if (selectedName() >= 0)
	if (selectedName() == selectedPiece_)
	    selectedPiece_ = -1;
	else
	  {
	    Move m(board_, selectedPiece_, selectedName());
	    if (m.isValid(board_))
	      {
		selectedPiece_ = -1;
		play(m);
	      }
	    else
	      if (board_->canBeSelected(selectedName()))
		  selectedPiece_ = selectedName();
	      else
		  selectedPiece_ = -1;
	  }
      else
	  selectedPiece_ = -1;
    }
  else
    if (selectedName() >= 0)
      selectedPiece_ = selectedName();
}



// F i l e   m e n u   f u n c t i o n s
void BlobWarViewer::selectBoardFileName()
{
#if QT_VERSION < 0x040000
  boardFileName_ = QFileDialog::getOpenFileName("Boards", "BlobWar board files (*.bwb);;All files (*)", this);
#else
  boardFileName_ = QFileDialog::getOpenFileName(this, "Select a saved game", "Boards", "BlobWar board files (*.bwb);;All files (*)");
#endif
}

void BlobWarViewer::load()
{
  selectBoardFileName();
  
  // In case of Cancel
  if (boardFileName_.isEmpty())
    return;
  
  newGame();
  fitCameraToBoard();
}

void BlobWarViewer::save()
{
#if QT_VERSION < 0x040000
  std::ofstream f(boardFileName_.latin1());
#else
  std::ofstream f(boardFileName_.toLatin1().constData());
#endif
  f << *board_;
  f.close();
}

void BlobWarViewer::saveAs()
{
#if QT_VERSION < 0x040000
  boardFileName_ = QFileDialog::getSaveFileName("Boards", "BlobWar board files (*.bwb);;All files (*)", this, boardFileName_.latin1());
#else
  boardFileName_ = QFileDialog::getSaveFileName(this, "Save game", boardFileName_, "BlobWar board files (*.bwb);;All files (*)");
#endif

  QFileInfo fi(boardFileName_);

#if QT_VERSION < 0x040000
  if (fi.extension().isEmpty())
#else
    if (fi.suffix().isEmpty())
#endif
      {
	boardFileName_ += ".bwb";
	fi.setFile(boardFileName_);
      }

#if QT_VERSION < 0x040000
  if (fi.exists())
    {
      if (QMessageBox::warning(this ,"Existing file",
			       "File "+fi.fileName()+" already exists.\nSave anyway ?",
			       QMessageBox::Yes,QMessageBox::Cancel) == QMessageBox::Cancel)
	return;

      if (!fi.isWritable())
	{
	  QMessageBox::warning(this ,"Cannot save", "File "+fi.fileName()+" is not writeable.");
	  return;
	}
    }
#endif
  
  save();
}



// G a m e   m e n u   m e t h o d s

void BlobWarViewer::bluePlayerIsHuman(bool on)
{
  setPlayerIsHuman(on, true);
}

void BlobWarViewer::redPlayerIsHuman(bool on)
{
  setPlayerIsHuman(on, false);
}

void BlobWarViewer::configureBluePlayer()
{
  configurePlayer(true);
}

void BlobWarViewer::configureRedPlayer()
{
  configurePlayer(false);
}

void BlobWarViewer::setPlayerIsHuman(bool on, bool blue)
{
  computerPlayer_[blue].setIsActive(!on);
  playNextMove();
}

void BlobWarViewer::configurePlayer(bool blue)
{
  computerPlayer_[blue].configure();
}


// U n d o   a n d   R e do
void BlobWarViewer::undo()
{
  if (board_->undo())
    {
      selectedPiece_ = -1;
      updateGL();
      playNextMove();
    }
}

void BlobWarViewer::redo()
{
  if (board_->redo())
    {
      selectedPiece_ = -1;
      updateGL();
      playNextMove();
    }
}

void BlobWarViewer::about()
{
  QMessageBox::about(this, "B l o b W a r", "B l o b W a r\nCreated by Gilles Debunne\nVersion 1.0 - January 2006");
}

void BlobWarViewer::displayRules()
{
  QMessageBox::about(this, "B l o b W a r", "Rules will soon be available");
}
