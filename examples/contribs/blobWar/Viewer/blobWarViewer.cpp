#include "blobWarViewer.h"
#include "board.h"

#include <fstream>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <qtextedit.h>
#include <qtimer.h>

#include <QGLViewer/manipulatedCameraFrame.h>

#include "ui_blobWarWindow.h"
class BlobWarWindow : public QMainWindow, public Ui::BlobWarWindow {};

using namespace std;
using namespace qglviewer;

static QString colorString[2] = {"Red", "Blue"};

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

#if QT_VERSION < 0x040000
BlobWarViewer::BlobWarViewer(QWidget *parent, const char *name)
    : QGLViewer(parent, name),
#else
BlobWarViewer::BlobWarViewer(QWidget *parent)
    : QGLViewer(parent),
#endif
      boardFileName_("4x4.bwb"), selectedPiece_(-1),
      displayPossibleMoves_(true), animatePlays_(true), animationStep_(0) {
  kfi_ = new KeyFrameInterpolator(new Frame());

  QObject::connect(kfi_, SIGNAL(interpolated()), this, SLOT(update()));
  QObject::connect(kfi_, SIGNAL(endReached()), this, SLOT(flipColor()));

  undoTimer_ = new QTimer();
#if QT_VERSION >= 0x040000
  undoTimer_->setSingleShot(true);
#endif
  QObject::connect(undoTimer_, SIGNAL(timeout()), this, SLOT(playNextMove()));

  for (int i = 0; i < 2; ++i)
    connect(&(computerPlayer_[i]), SIGNAL(moveMade(QString, int)), this,
            SLOT(playComputerMove(QString, int)));

  QStringList programFileNames;
  programFileNames << "blobWarAI"
                   << "blobWarAI.exe"
                   << "../AI/blobWarAI"
                   << "../../AI/release/blobWarAI.exe"
                   << "../../AI/debug/blobWarAI.exe";
  for (int i = 0; i < programFileNames.size(); ++i) {
    if (QFileInfo(programFileNames.at(i)).isExecutable()) {
      computerPlayer_[0].setProgramFileName(programFileNames.at(i));
      computerPlayer_[0].setIsActive(true);
      break;
    }
  }
}

// I n i t i a l i z a t i o n   f u n c t i o n s

void BlobWarViewer::init() {
  initViewer();
  fitCameraToBoard();

  setMouseBinding(Qt::NoModifier, Qt::RightButton, CAMERA, ROTATE);
  setMouseBinding(Qt::NoModifier, Qt::LeftButton, SELECT);

#if QT_VERSION >= 0x040000
  // Signals and slots connections
  BlobWarWindow *bww = (BlobWarWindow *)(window());
  connect(bww->fileExitAction, SIGNAL(activated()), bww, SLOT(close()));
  connect(bww->fileOpenAction, SIGNAL(activated()), this, SLOT(load()));
  connect(bww->fileSaveAction, SIGNAL(activated()), this, SLOT(save()));
  connect(bww->fileSaveAsAction, SIGNAL(activated()), this, SLOT(saveAs()));
  connect(bww->gameNewGameAction, SIGNAL(activated()), this, SLOT(newGame()));
  connect(bww->gameUndoAction, SIGNAL(activated()), this, SLOT(undo()));
  connect(bww->gameRedoAction, SIGNAL(activated()), this, SLOT(redo()));

  connect(bww->gameBlueIsHumanAction, SIGNAL(toggled(bool)), this,
          SLOT(bluePlayerIsHuman(bool)));
  connect(bww->gameRedIsHumanAction, SIGNAL(toggled(bool)), this,
          SLOT(redPlayerIsHuman(bool)));

  connect(bww->gameBlueIsHumanAction, SIGNAL(toggled(bool)),
          bww->gameConfigureBluePlayerAction, SLOT(setDisabled(bool)));
  connect(bww->gameRedIsHumanAction, SIGNAL(toggled(bool)),
          bww->gameConfigureRedPlayerAction, SLOT(setDisabled(bool)));

  connect(bww->gameConfigureBluePlayerAction, SIGNAL(activated()), this,
          SLOT(configureBluePlayer()));
  connect(bww->gameConfigureRedPlayerAction, SIGNAL(activated()), this,
          SLOT(configureRedPlayer()));

  connect(bww->togglePossibleMoveAction, SIGNAL(toggled(bool)), this,
          SLOT(toggleDisplayPossibleMoves(bool)));
  connect(bww->toggleAnimationAction, SIGNAL(toggled(bool)), this,
          SLOT(toggleAnimation(bool)));

  connect(bww->helpAboutAction, SIGNAL(activated()), this, SLOT(about()));
  connect(bww->helpRulesAction, SIGNAL(activated()), this,
          SLOT(displayRules()));
#endif
}

void BlobWarViewer::initViewer() {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  board_ = new Board();

  QStringList boardDir;
  boardDir << "BlobWarBoards"
           << "../BlobWarBoards"
           << "../../BlobWarBoards";
  for (int i = 0; i < boardDir.size(); ++i) {
    if (QFileInfo(boardDir.at(i)).isDir()) {
      boardFileName_ = boardDir.at(i) + "/4x4.bwb";
      break;
    }
  }

  newGame();
}

void BlobWarViewer::newGame() {
  if (!QFileInfo(boardFileName_).isReadable()) {
    QMessageBox::warning(NULL, "Unreadable board file",
                         "Unable to read board file (" + boardFileName_ +
                             ").\nLoad another board.");
    return;
  }

#if QT_VERSION < 0x040000
  std::ifstream f(boardFileName_.latin1());
#else
  std::ifstream f(boardFileName_.toLatin1().constData());
#endif
  f >> *board_;
  f.close();

  selectedPiece_ = -1;

  playNextMove();
}

void BlobWarViewer::fitCameraToBoard() {
  static BoardConstraint *boardConstraint = new BoardConstraint();

  // Free camera rotation motion
  camera()->frame()->setConstraint(NULL);

  setSceneCenter(
      Vec(board_->size().width() / 2.0, board_->size().height() / 2.0, 0.0));
  setSceneRadius(sceneCenter().norm());

  camera()->setUpVector(Vec(0.0, 0.0, 1.0));
  camera()->setPosition(Vec(2.0 * sceneCenter().x, -1.5 * sceneCenter().y,
                            0.8 * (sceneCenter().x + sceneCenter().y)));
  camera()->lookAt(sceneCenter());
  showEntireScene();

  // Limit camera rotation motion
  camera()->frame()->setConstraint(boardConstraint);
}

// D r a w i n g  f u n c t i o n s
void BlobWarViewer::draw() {
  glEnable(GL_LIGHTING);

  if (animationStep_ > 0) {
    glPushMatrix();
    glMultMatrixd(kfi_->frame()->matrix());
    Board::drawPiece(board_->bluePlays());
    glPopMatrix();

    if (animationStep_ > 1)
      board_->drawFlippingPieces(currentMove_.end(), animationStep_ % 2);
  }

  board_->draw();

  if (selectedPiece_ >= 0) {
    board_->drawSelectedPiece(selectedPiece_);
    if (displayPossibleMoves_)
      board_->drawPossibleDestinations(selectedPiece_);
  }

  board_->drawShadows();
}

// G a m e   I n t e r f a c e
void BlobWarViewer::play(const Move &m) {
  currentMove_ = m;
  if (animatePlays_)
    animatePlay();
  else
    simplePlay();
}

void BlobWarViewer::simplePlay() {
  if ((!currentMove_.isClose()) && (animatePlays_))
    board_->doDrawPiece(currentMove_.start());

  board_->play(currentMove_);
  update();
  playNextMove();
}

void BlobWarViewer::playNextMove() {
  static const QString stateFileName = "board.bwb";

// Updates statusBar
#if QT_VERSION < 0x040000
  ((QMainWindow *)(((QWidget *)parent())->parent()))
      ->statusBar()
      ->message(board_->statusMessage());
#else
  ((QMainWindow *)(((QWidget *)parent())->parent()))
      ->statusBar()
      ->showMessage(board_->statusMessage());
#endif

  if (board_->gameIsOver())
    QMessageBox::information(this, "Game over", board_->statusMessage());
  else {
// Save board state
#if QT_VERSION < 0x040000
    std::ofstream f(stateFileName.latin1());
#else
    std::ofstream f(stateFileName.toLatin1().constData());
#endif
    f << *board_;
    f.close();

    computerPlayer_[board_->bluePlays()].play(board_->bluePlays(),
                                              stateFileName);
  }
}

void BlobWarViewer::playComputerMove(QString move, int duration) {
  QString message = colorString[board_->bluePlays()] +
                    " program played %1 in %2 out of %3 seconds.";
  message = message.arg(move)
                .arg(duration / 1000.0, 0, 'f', 1)
                .arg(computerPlayer_[board_->bluePlays()].allowedTime());
#if QT_VERSION < 0x040000
  qWarning(message.latin1());
#else
  qWarning("%s", message.toLatin1().constData());
#endif
  Move m(move);
  if (m.isValid(board_))
    play(m);
  else
    QMessageBox::warning(this, "Wrong move",
                         "The move " + move + " is not legal for " +
                             colorString[board_->bluePlays()]);
}

void BlobWarViewer::animatePlay() {
  kfi_->deletePath();
  // Cut/pasted from board posOf(). Not shared to prevent QGLViewer dependency
  // in board.h

  if (currentMove_.isClose()) {
    kfi_->addKeyFrame(Frame(Vec(currentMove_.start().x() + 0.5,
                                currentMove_.start().y() + 0.5, 0.0),
                            Quaternion()));
    kfi_->addKeyFrame(Frame(
        Vec(currentMove_.end().x() + 0.5, currentMove_.end().y() + 0.5, 0.0),
        Quaternion()));
  } else {
    kfi_->addKeyFrame(Frame(Vec(currentMove_.start().x() + 0.5,
                                currentMove_.start().y() + 0.5, 0.0),
                            Quaternion()));
    kfi_->addKeyFrame(
        Frame(
            Vec((currentMove_.start().x() + currentMove_.end().x() + 1.0) / 2.0,
                (currentMove_.start().y() + currentMove_.end().y() + 1.0) / 2.0,
                1.0),
            Quaternion()),
        0.6f);
    kfi_->addKeyFrame(Frame(Vec(currentMove_.end().x() + 0.5,
                                currentMove_.end().y() + 0.5, 0.0),
                            Quaternion()),
                      1.2f);
  }

  if (!currentMove_.isClose())
    board_->doNotDrawPiece(currentMove_.start());

  animationStep_ = 1;
  kfi_->startInterpolation();
}

void BlobWarViewer::flipColor() {
  update();

  if (animationStep_++ < 10)
    QTimer::singleShot(100, this, SLOT(flipColor()));
  else {
    animationStep_ = 0;
    simplePlay();
  }
}

// M o v e   S e l e c t i o n
void BlobWarViewer::drawWithNames() {
  // Selection enabled only when the computer program is not active
  if ((!computerPlayer_[board_->bluePlays()].isActive()) &&
      (animationStep_ == 0)) {
    if (selectedPiece_ >= 0)
      board_->drawPossibleDestinations(selectedPiece_, true);

    // Always drawned, so that a new selection can occur
    board_->drawSelectablePieces();
  }
}

void BlobWarViewer::postSelection(const QPoint &) {
  if (selectedPiece_ >= 0) {
    if (selectedName() >= 0)
      if (selectedName() == selectedPiece_)
        selectedPiece_ = -1;
      else {
        Move m(board_, selectedPiece_, selectedName());
        if (m.isValid(board_)) {
          selectedPiece_ = -1;
          play(m);
        } else if (board_->canBeSelected(selectedName()))
          selectedPiece_ = selectedName();
        else
          selectedPiece_ = -1;
      }
    else
      selectedPiece_ = -1;
  } else if (selectedName() >= 0)
    selectedPiece_ = selectedName();
}

// F i l e   m e n u   f u n c t i o n s
void BlobWarViewer::selectBoardFileName() {
#if QT_VERSION < 0x040000
  boardFileName_ = QFileDialog::getOpenFileName(
      "Boards", "BlobWar board files (*.bwb);;All files (*)", this);
#else
  boardFileName_ = QFileDialog::getOpenFileName(
      this, "Select a saved game", boardFileName_,
      "BlobWar board files (*.bwb);;All files (*)");
#endif
}

void BlobWarViewer::load() {
  selectBoardFileName();

  // In case of Cancel
  if (boardFileName_.isEmpty())
    return;

  newGame();
  fitCameraToBoard();
}

void BlobWarViewer::save() {
#if QT_VERSION < 0x040000
  std::ofstream f(boardFileName_.latin1());
#else
  std::ofstream f(boardFileName_.toLatin1().constData());
#endif
  f << *board_;
  f.close();
}

void BlobWarViewer::saveAs() {
#if QT_VERSION < 0x040000
  boardFileName_ = QFileDialog::getSaveFileName(
      "Boards", "BlobWar board files (*.bwb);;All files (*)", this,
      boardFileName_.latin1());
#else
  boardFileName_ = QFileDialog::getSaveFileName(
      this, "Save game", boardFileName_,
      "BlobWar board files (*.bwb);;All files (*)");
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
  if (fi.exists()) {
    if (QMessageBox::warning(
            this, "Existing file",
            "File " + fi.fileName() + " already exists.\nSave anyway ?",
            QMessageBox::Yes, QMessageBox::Cancel) == QMessageBox::Cancel)
      return;

    if (!fi.isWritable()) {
      QMessageBox::warning(this, "Cannot save",
                           "File " + fi.fileName() + " is not writeable.");
      return;
    }
  }
#endif

  save();
}

// G a m e   m e n u   m e t h o d s

void BlobWarViewer::bluePlayerIsHuman(bool on) { setPlayerIsHuman(on, true); }

void BlobWarViewer::redPlayerIsHuman(bool on) { setPlayerIsHuman(on, false); }

void BlobWarViewer::configureBluePlayer() { configurePlayer(true); }

void BlobWarViewer::configureRedPlayer() { configurePlayer(false); }

void BlobWarViewer::setPlayerIsHuman(bool on, bool blue) {
  computerPlayer_[blue].setIsActive(!on);
  playNextMove();
}

void BlobWarViewer::configurePlayer(bool blue) {
  computerPlayer_[blue].configure();
}

// U n d o   a n d   R e do
void BlobWarViewer::undo() {
  if ((animationStep_ == 0) && (board_->undo()))
    finalizeUndoRedo();
}

void BlobWarViewer::redo() {
  if ((animationStep_ == 0) && (board_->redo()))
    finalizeUndoRedo();
}

void BlobWarViewer::finalizeUndoRedo() {
  selectedPiece_ = -1;
  update();
#if QT_VERSION < 0x040000
  undoTimer_->start(1000, true);
#else
  undoTimer_->start(1000);
#endif
}

void BlobWarViewer::about() {
  QMessageBox::about(
      this, "B l o b W a r",
      "B l o b W a r\nCreated by Gilles Debunne\nVersion 1.0 - January 2006");
}

void BlobWarViewer::displayRules() {
  static QTextEdit *te;

  static const QString rules =
      "<b>B l o b   W a r</b><br><br>"
      "Blob war is a strategy game for two players.<br><br>"
      "The goal is to have the maximum number of pieces when the board is "
      "full.<br><br>"
      "A piece can be moved on a empty square at a distance of one (i.e. "
      "adjacent) or two squares. "
      "If the distance is one, the piece is cloned, otherwise it jumps to its "
      "destination.<br><br>"
      "In both cases, all the pieces located in the neighborhood of the "
      "destination square change their color "
      "to that of the current player.<br><br>"
      "Enjoy !";

  if (!te) {
    te = new QTextEdit(rules);
    te->resize(500, 300);
  }

  te->show();
}
