#include "quarto.h"

#include <qapplication.h>
#include <qdialog.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include <qvariant.h>
#include <qwidget.h>
#include <signal.h>

#if QT_VERSION < 0x040000
#include <qdragobject.h>
#include <qheader.h>
#endif

#if QT_VERSION < 0x040000
Quarto::Quarto(QWidget *parent, const char *name, WFlags fl)
    : QMainWindow(parent, name, fl) {
  setName("Quarto");
  setCaption(trUtf8("Quarto"));
#else
Quarto::Quarto(QWidget *parent) : QMainWindow(parent) {
  setWindowTitle("Quarto");
#endif
  //************************************************************************************************************************//
  //************************************************************************************************************************//
  //                      Definition de l'apparence de l'interface graphique (
  //                      Tous les Widgets )                           //
  //************************************************************************************************************************//
  //************************************************************************************************************************//

  // Parametres principaux
  resize(800, 400);
  setCentralWidget(this);

  //***************************//
  // Fenetre de vue du plateau //
  //***************************//
  QuartoLayout = new QVBoxLayout(centralWidget());
  MainHLayout = new QHBoxLayout(NULL);

  GLFrameJeu = new QFrame(centralWidget());
  GLFrameJeu->setMouseTracking(true);
  GLFrameJeu->setFrameShape(QFrame::StyledPanel);
  GLFrameJeu->setFrameShadow(QFrame::Raised);
  GLFrameJeu->setFrameStyle(QFrame::Sunken | QFrame::Panel);
  GLFrameJeu->setLineWidth(2);
  // Create our OpenGL widget
  vuePlateau = new GLViewJeu(GLFrameJeu);
  HLayout1 = new QHBoxLayout(GLFrameJeu);
  //**********************************//
  //        Partie Selection         //
  //********************************//
  VLayout1 = new QVBoxLayout(NULL);

  HLayout1->addWidget(vuePlateau);
  // Ajout au tableau de fenetres
  MainHLayout->addWidget(GLFrameJeu);

  //######################//
  // Groupe de fonctions //
  //####################//
  GameGroupBox = new QGroupBox(centralWidget());
  QWidget *privateLayoutWidget = new QWidget(GameGroupBox);

  VLayout2 = new QVBoxLayout(privateLayoutWidget);
  // Indication du tour des joueurs
  HLayout2 = new QHBoxLayout(NULL);

  GameGroupBox->setMaximumSize(600, 100);
  privateLayoutWidget->setGeometry(QRect(10, 15, 280, 80));

  // titre
  TourDeJeuLabel = new QLabel(privateLayoutWidget);
  QFont TourDeJeuLabel_font(TourDeJeuLabel->font());
  TourDeJeuLabel_font.setPointSize(14);
  TourDeJeuLabel->setFont(TourDeJeuLabel_font);
  TourDeJeuLabel->setText(trUtf8("Now playing :"));
  HLayout2->addWidget(TourDeJeuLabel);
  // indicateur
  NomLabel = new QLabel(privateLayoutWidget);
  QFont NomLabel_font(NomLabel->font());
  NomLabel_font.setPointSize(14);
  NomLabel->setFont(NomLabel_font);
  HLayout2->addWidget(NomLabel);

  VLayout2->addLayout(HLayout2);

  VLayout1->addWidget(GameGroupBox);
  //############################//
  // Fenetre de vue des pieces //
  //##########################//
  GLFramePieces = new QFrame(centralWidget());
  GLFramePieces->setMouseTracking(true);
  GLFramePieces->setFrameShape(QFrame::StyledPanel);
  GLFramePieces->setFrameShadow(QFrame::Raised);
  GLFramePieces->setFrameStyle(QFrame::Sunken | QFrame::Panel);
  GLFramePieces->setLineWidth(2);
  // Create our OpenGL widget
  vuePieces = new GLViewPieces(GLFramePieces);
  HLayout4 = new QHBoxLayout(GLFramePieces);
  HLayout4->addWidget(vuePieces);
  // Ajout au tableau de fenetres
  VLayout1->addWidget(GLFramePieces);

  // Bouttons utiles
  HLayout3 = new QHBoxLayout(NULL);
  // Boutton de reset
  ResetButton = new QPushButton(privateLayoutWidget);
  QFont ResetButton_font(ResetButton->font());
  ResetButton_font.setPointSize(14);
  ResetButton->setFont(ResetButton_font);
  ResetButton->setText(trUtf8("New Game"));
  HLayout3->addWidget(ResetButton);
  // Boutton de quit
  QuitButton = new QPushButton(privateLayoutWidget);
  QFont QuitButton_font(QuitButton->font());
  QuitButton_font.setPointSize(14);
  QuitButton->setFont(QuitButton_font);
  QuitButton->setText(trUtf8("Quit"));
  HLayout3->addWidget(QuitButton);
  VLayout2->addLayout(HLayout3);

  // Ajout au tableau de fenetres
  MainHLayout->addLayout(VLayout1);
  QuartoLayout->addLayout(MainHLayout);

  //******************************************//
  //     Signals and Slots Connections        //
  //******************************************//
  connect(ResetButton, SIGNAL(clicked()), this, SLOT(New()));
  connect(QuitButton, SIGNAL(clicked()), this, SLOT(Exit()));
  connect(vuePlateau, SIGNAL(update()), vuePieces, SLOT(updateGL()));
  connect(this, SIGNAL(updategl()), vuePieces, SLOT(updateGL()));
  connect(this, SIGNAL(updategl()), vuePlateau, SLOT(updateGL()));
  connect(vuePlateau, SIGNAL(piecePlacee()), this, SLOT(piecePlacee()));
  connect(vuePieces, SIGNAL(changeJoueur()), this, SLOT(changeTour()));
  connect(vuePlateau, SIGNAL(endGame()), this, SLOT(finDeJeu()));
  // On initialise l'interface
  init(true);
}

Quarto::~Quarto() {
  delete (vuePlateau);
  delete (vuePieces);
  delete (setofpiece);
}

void Quarto::init(bool begin) {
  if (begin)
    setofpiece = new SetOfPiece();
  else
    setofpiece->init();

  vuePlateau->reset();
  vuePlateau->setPieces(setofpiece);
  vuePieces->setPieces(setofpiece);
  NomLabel->setText(trUtf8("Player 1"));
  joueur = true;
  pieceplacee = true;
}

void Quarto::New() {
  init(false);
  Q_EMIT updategl();
}

void Quarto::Exit() { qApp->exit(); }

void Quarto::piecePlacee() { pieceplacee = true; }

void Quarto::changeTour() {
  if (pieceplacee) {
    if (joueur)
      NomLabel->setText(trUtf8("Player 2"));
    else
      NomLabel->setText(trUtf8("Player 1"));
    joueur = !joueur;
    pieceplacee = false;
  }
}

void Quarto::finDeJeu() {
  if (QMessageBox::information(this, "Game over",
                               "Game is over, " + NomLabel->text() + " won.",
                               "New game", "Exit") == 0)
    New();
  else
    Exit();
}
