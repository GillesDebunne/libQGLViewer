#include "agoraViewer.h"
#include "board.h"

#include <fstream>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qmessagebox.h>
#include <qstatusbar.h>
#include <qtextedit.h>
#include <qtimer.h>

#include <QGLViewer/manipulatedCameraFrame.h>

#include "ui_agoraWindow.h"
class AgoraWindow : public QMainWindow, public Ui::AgoraWindow {};

using namespace std;
using namespace qglviewer;

static QString colorString[2] = {"White", "Black"};

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
AgoraViewer::AgoraViewer(QWidget *parent, const char *name)
    : QGLViewer(parent, name),
#else
AgoraViewer::AgoraViewer(QWidget *parent)
    : QGLViewer(parent),
#endif
      boardFileName_("standard.ago"), selectedPiece_(-1),
      displayPossibleMoves_(true), animatePlays_(false /*true*/),
      animationStep_(0) {
  kfi_ = new KeyFrameInterpolator(new Frame());

  QObject::connect(kfi_, SIGNAL(interpolated()), this, SLOT(update()));
  QObject::connect(kfi_, SIGNAL(endReached()), this, SLOT(simplePlay()));

  undoTimer_ = new QTimer();
#if QT_VERSION >= 0x040000
  undoTimer_->setSingleShot(true);
#endif
  QObject::connect(undoTimer_, SIGNAL(timeout()), this, SLOT(playNextMove()));

  for (int i = 0; i < 2; ++i)
    connect(&(computerPlayer_[i]), SIGNAL(moveMade(QString, int)), this,
            SLOT(playComputerMove(QString, int)));

  computerPlayer_[0].setIsActive(false);
  computerPlayer_[1].setIsActive(false);

  QStringList programFileNames;
  programFileNames << "agoraAI"
                   << "agoraAI.exe"
                   << "../AI/agoraAI"
                   << "../../AI/release/agoraAI.exe"
                   << "../../AI/debug/agoraAI.exe"
                   << "../AI/release/agoraAI.exe"
                   << "../AI/debug/agoraAI.exe";
  for (int i = 0; i < programFileNames.size(); ++i) {
    if (QFileInfo(programFileNames.at(i)).isExecutable()) {
      for (int c = 0; c < 2; ++c)
        computerPlayer_[c].setProgramFileName(programFileNames.at(i));
      break;
    }
  }
}

// I n i t i a l i z a t i o n   f u n c t i o n s

void AgoraViewer::init() {
  initViewer();
  initSpotLight();
  initBoard();
  fitCameraToBoard();

  setMouseBinding(Qt::NoModifier, Qt::RightButton, CAMERA, ROTATE);
  setMouseBinding(Qt::NoModifier, Qt::LeftButton, SELECT);

#if QT_VERSION >= 0x040000
  // Signals and slots connections
  AgoraWindow *agoWin = (AgoraWindow *)(window());
  connect(agoWin->fileExitAction, SIGNAL(activated()), agoWin, SLOT(close()));
  connect(agoWin->fileOpenAction, SIGNAL(activated()), this, SLOT(load()));
  connect(agoWin->fileSaveAction, SIGNAL(activated()), this, SLOT(save()));
  connect(agoWin->fileSaveAsAction, SIGNAL(activated()), this, SLOT(saveAs()));
  connect(agoWin->gameNewGameAction, SIGNAL(activated()), this,
          SLOT(newGame()));
  connect(agoWin->gameUndoAction, SIGNAL(activated()), this, SLOT(undo()));
  connect(agoWin->gameRedoAction, SIGNAL(activated()), this, SLOT(redo()));

  connect(agoWin->gameBlackIsHumanAction, SIGNAL(toggled(bool)), this,
          SLOT(blackPlayerIsHuman(bool)));
  connect(agoWin->gameWhiteIsHumanAction, SIGNAL(toggled(bool)), this,
          SLOT(whitePlayerIsHuman(bool)));

  connect(agoWin->gameBlackIsHumanAction, SIGNAL(toggled(bool)),
          agoWin->gameConfigureBlackPlayerAction, SLOT(setDisabled(bool)));
  connect(agoWin->gameWhiteIsHumanAction, SIGNAL(toggled(bool)),
          agoWin->gameConfigureWhitePlayerAction, SLOT(setDisabled(bool)));

  connect(agoWin->gameConfigureBlackPlayerAction, SIGNAL(activated()), this,
          SLOT(configureBlackPlayer()));
  connect(agoWin->gameConfigureWhitePlayerAction, SIGNAL(activated()), this,
          SLOT(configureWhitePlayer()));

  connect(agoWin->togglePossibleMoveAction, SIGNAL(toggled(bool)), this,
          SLOT(toggleDisplayPossibleMoves(bool)));
  connect(agoWin->toggleAnimationAction, SIGNAL(toggled(bool)), this,
          SLOT(toggleAnimation(bool)));

  connect(agoWin->helpAboutAction, SIGNAL(activated()), this, SLOT(about()));
  connect(agoWin->helpRulesAction, SIGNAL(activated()), this,
          SLOT(displayRules()));
#endif
}

void AgoraViewer::initViewer() {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Enable GL textures
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Nice texture coordinate interpolation
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  const QString texFileName("wood.png");
  QImage img(texFileName);

  if (img.isNull()) {
    QMessageBox::warning(
        NULL, QString("Texture file not found"),
        QString("Unable to load wood texture (%1).").arg(texFileName));
    return;
  }

  // 1E-3 needed. Just try with width=128 and see !
  if ((img.width() != 1 << (int)(1 + log(img.width() - 1 + 1E-3) / log(2.0))) ||
      (img.height() !=
       1 << (int)(1 + log(img.height() - 1 + 1E-3) / log(2.0)))) {
    QMessageBox::warning(
        NULL, QString("Incorrect texture"),
        QString("Wood texture dimensions are not powers of 2."));
    return;
  }

  QImage glImg = QGLWidget::convertToGLFormat(img); // flipped 32bit RGBA

  // Bind the img texture...
  glTexImage2D(GL_TEXTURE_2D, 0, 4, glImg.width(), glImg.height(), 0, GL_RGBA,
               GL_UNSIGNED_BYTE, glImg.bits());
  glDisable(GL_TEXTURE_2D);
}

void AgoraViewer::initBoard() {
  board_ = new Board();

  QStringList boardDir;
  boardFileName_ = "";
  boardDir << "AgoraBoards"
           << "../AgoraBoards"
           << "../../AgoraBoards";
  for (int i = 0; i < boardDir.size(); ++i) {
    if (QFileInfo(boardDir.at(i)).isDir()) {
      boardFileName_ = boardDir.at(i) + "/standard.ago";
      break;
    }
  }

  if (boardFileName_.isEmpty()) {
    QMessageBox::warning(NULL, "Unable to find agora board directory",
                         "Unable to find agora board directory\n(tried " +
                             boardDir.join(", ") + ")");
    return;
  }

  newGame();
}

void AgoraViewer::initSpotLight() {
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_LIGHT1);
  glLoadIdentity();

  // Light default parameters
  const GLfloat light_ambient[4] = {2.0, 2.0, 2.0, 1.0};
  const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
  const GLfloat light_diffuse[4] = {2.0, 2.0, 2.0, 1.0};

  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0);
  glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.1f);
  glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.3f);
  glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.3f);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
}

void AgoraViewer::newGame() {
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

void AgoraViewer::fitCameraToBoard() {
  static BoardConstraint *boardConstraint = new BoardConstraint();

  // Free camera rotation motion
  camera()->frame()->setConstraint(NULL);

  setSceneCenter(
      Vec(board_->size().width() / 2.0, board_->size().height() / 2.0, 0.0));
  setSceneRadius(sceneCenter().norm());

  camera()->setUpVector(Vec(0.0, 0.0, 1.0));
  camera()->setPosition(Vec(2.0f * sceneCenter().x, -1.5f * sceneCenter().y,
                            0.9f * (sceneCenter().x + sceneCenter().y)));
  camera()->lookAt(sceneCenter());
  // showEntireScene();

  // Limit camera rotation motion
  camera()->frame()->setConstraint(boardConstraint);
}

// D r a w i n g   f u n c t i o n s
void AgoraViewer::draw() {
  glEnable(GL_LIGHTING);

  if (animationStep_ > 0) {
    glPushMatrix();
    glMultMatrixd(kfi_->frame()->matrix());
    // Board::drawPiece(board_->blackPlays());
    glPopMatrix();

    // if (animationStep_ > 1)
    //	board_->drawFlippingPieces(currentMove_.end(), animationStep_%2);
  }

  const GLfloat pos[4] = {board_->size().width() / 2.0,
                          board_->size().height() / 2.0, 3.0, 1.0};
  const GLfloat spot_dir[3] = {0.0, 0.0, -1.0};
  glLightfv(GL_LIGHT1, GL_POSITION, pos);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);

  board_->draw();

  if (selectedPiece_ >= 0) {
    board_->drawSelectedPiece(selectedPiece_);
    if (displayPossibleMoves_)
      board_->drawPossibleDestinations(selectedPiece_);
  }
}

// G a m e   I n t e r f a c e
void AgoraViewer::play(const Move &m) {
  currentMove_ = m;
  if (animatePlays_)
    animatePlay();
  else
    simplePlay();
}

void AgoraViewer::simplePlay() {
  board_->play(currentMove_);
  update();
  playNextMove();
}

void AgoraViewer::playNextMove() {
  static const QString stateFileName = "board.ago";

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

    computerPlayer_[board_->blackPlays()].play(
        board_->blackPlays(), stateFileName, board_->nbMovesLeft());
  }
}

void AgoraViewer::playComputerMove(QString move, int duration) {
  QString message = colorString[board_->blackPlays()] +
                    " program played %1 in %2 out of %3 seconds.";
  message =
      message.arg(move)
          .arg(duration / 1000.0, 0, 'f', 1)
          .arg(computerPlayer_[board_->blackPlays()].allowedTime() / 1000.0, 0,
               'f', 1);
#if QT_VERSION < 0x040000
  qWarning(message.latin1());
#else
  qWarning("%s", message.toLatin1().constData());
#endif
  Move m(move);
  if (m.isValid(board_))
    play(m);
  else
    QMessageBox::warning(this, "Unvalid move",
                         "The move " + move + " is not valid for " +
                             colorString[board_->blackPlays()]);
}

void AgoraViewer::animatePlay() {
  kfi_->deletePath();
  // Cut/pasted from board posOf(). Not shared to prevent QGLViewer dependency
  // in board.h

  //	kfi_->addKeyFrame(Frame(Vec(currentMove_.start().x()+0.5,currentMove_.start().y()+0.5,0.0),
  //Quaternion()));
  //	kfi_->addKeyFrame(Frame(Vec((currentMove_.start().x()+currentMove_.end().x()+1.0)/2.0,
  //		(currentMove_.start().y()+currentMove_.end().y()+1.0)/2.0, 1.0),
  //Quaternion()), 0.6f);
  //  kfi_->addKeyFrame(Frame(Vec(currentMove_.end().x()+0.5,currentMove_.end().y()+0.5,0.0),
  //  Quaternion()), 1.2f);
  kfi_->addKeyFrame(Frame(
      Vec(currentMove_.end().x() + 0.5, currentMove_.end().y() + 0.5, 0.0),
      Quaternion()));

  animationStep_ = 1;
  kfi_->startInterpolation();
}

// M o v e   S e l e c t i o n
void AgoraViewer::drawWithNames() {
  // Selection enabled only when the computer program is not active
  if ((!computerPlayer_[board_->blackPlays()].isActive()) &&
      (animationStep_ == 0)) {
    if (selectedPiece_ >= 0)
      board_->drawPossibleDestinations(selectedPiece_, true);

    // Always drawned, so that a new selection can occur
    board_->drawSelectablePieces();
  }
}

void AgoraViewer::postSelection(const QPoint &) {
  if (selectedPiece_ >= 0) {
    if (selectedName() >= 0)
      if (selectedName() == selectedPiece_)
        selectedPiece_ = -1;
      else {
        Move m(board_, selectedPiece_, selectedName(),
               qApp->keyboardModifiers() != Qt::NoModifier);
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
void AgoraViewer::selectBoardFileName() {
#if QT_VERSION < 0x040000
  boardFileName_ = QFileDialog::getOpenFileName(
      "Boards", "Agora files (*.ago);;All files (*)", this);
#else
  boardFileName_ =
      QFileDialog::getOpenFileName(this, "Select a saved game", boardFileName_,
                                   "Agora files (*.ago);;All files (*)");
#endif
}

void AgoraViewer::load() {
  selectBoardFileName();

  // In case of Cancel
  if (boardFileName_.isEmpty())
    return;

  newGame();
  fitCameraToBoard();
}

void AgoraViewer::save() {
#if QT_VERSION < 0x040000
  std::ofstream f(boardFileName_.latin1());
#else
  std::ofstream f(boardFileName_.toLatin1().constData());
#endif
  f << *board_;
  f.close();
}

void AgoraViewer::saveAs() {
#if QT_VERSION < 0x040000
  boardFileName_ = QFileDialog::getSaveFileName(
      "Boards", "Agora files (*.ago);;All files (*)", this,
      boardFileName_.latin1());
#else
  boardFileName_ = QFileDialog::getSaveFileName(
      this, "Save game", boardFileName_, "Agora files (*.ago);;All files (*)");
#endif

  QFileInfo fi(boardFileName_);

#if QT_VERSION < 0x040000
  if (fi.extension().isEmpty())
#else
  if (fi.suffix().isEmpty())
#endif
  {
    boardFileName_ += ".ago";
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

void AgoraViewer::blackPlayerIsHuman(bool on) { setPlayerIsHuman(on, true); }

void AgoraViewer::whitePlayerIsHuman(bool on) { setPlayerIsHuman(on, false); }

void AgoraViewer::configureBlackPlayer() { configurePlayer(true); }

void AgoraViewer::configureWhitePlayer() { configurePlayer(false); }

void AgoraViewer::setPlayerIsHuman(bool on, bool black) {
  computerPlayer_[black].setIsActive(!on);
  playNextMove();
}

void AgoraViewer::configurePlayer(bool black) {
  computerPlayer_[black].configure();
}

// U n d o   a n d   R e do
void AgoraViewer::undo() {
  if ((animationStep_ == 0) && (board_->undo()))
    finalizeUndoRedo();
}

void AgoraViewer::redo() {
  if ((animationStep_ == 0) && (board_->redo()))
    finalizeUndoRedo();
}

void AgoraViewer::finalizeUndoRedo() {
  selectedPiece_ = -1;
  update();
#if QT_VERSION < 0x040000
  undoTimer_->start(1000, true);
#else
  undoTimer_->start(1000);
#endif
}

void AgoraViewer::about() {
  QMessageBox::about(
      this, "A g o r a",
      "A g o r a\nCreated by Gilles Debunne\nVersion 2.0 - January 2008");
}

void AgoraViewer::displayRules() {
  static QTextEdit *te;

  static const QString rules =
      "<b>A g o r a</b><br><br>"
      "Agora is a strategy game for two players.<br><br>"
      "The goal is to have the maximum number of pieces of your color when the "
      "number of allowed moves is reached "
      "(or to convert all the pieces into your color before that).<br><br>"
      "A piece can be moved on a nearby square... "
      "details to come.<br><br>"
      "Enjoy !";

  if (!te) {
    te = new QTextEdit(rules);
    te->resize(500, 300);
  }

  te->show();
}
