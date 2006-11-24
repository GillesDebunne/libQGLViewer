#include "quarto.h"
#include <signal.h>

#include <qvariant.h>
#include <qapplication.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qheader.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qmime.h>
#include <qdragobject.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qdialog.h>

Quarto::Quarto( QWidget* parent,  const char* name, WFlags fl )
  : QMainWindow( parent, name, fl )
{

  //************************************************************************************************************************//
  //************************************************************************************************************************//
  //                      Definition de l'apparence de l'interface graphique ( Tous les Widgets )                           //
  //************************************************************************************************************************//
  //************************************************************************************************************************//

  // Parametres principaux
  if ( !name )
    setName( "Quarto" );
  resize( 800, 400 );
  setCaption( trUtf8( "Quarto" ) );
  setCentralWidget( new QWidget( this, "qt_central_widget" ) );
  QuartoLayout = new QVBoxLayout( centralWidget(), 2, -1, "QuartoLayout");

  //***************************//
  // Fenetre de vue du plateau //
  //***************************//
  MainHLayout = new QHBoxLayout( 0, 0, 2, "MainHLayout"  );

  GLFrameJeu = new QFrame( centralWidget(), "GLFrameJeu" );
  GLFrameJeu->setMouseTracking( TRUE );
  GLFrameJeu->setFrameShape( QFrame::StyledPanel );
  GLFrameJeu->setFrameShadow( QFrame::Raised );
  GLFrameJeu->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  GLFrameJeu->setLineWidth( 2 );
  // Create our OpenGL widget
  vuePlateau = new GLViewJeu( GLFrameJeu );
  HLayout1 = new QHBoxLayout( GLFrameJeu, 2, 2, "HLayout1"  );
  HLayout1->addWidget( vuePlateau );
  // Ajout au tableau de fenetres
  MainHLayout->addWidget( GLFrameJeu );

  //**********************************//
  //        Partie Selection         //
  //********************************//
  VLayout1 = new QVBoxLayout( 0, 0, 2, "VLayout1");

  //######################//
  // Groupe de fonctions //
  //####################//
  GameGroupBox = new QGroupBox( centralWidget(), "GameGroupBox" );
  GameGroupBox->setMaximumSize( 600, 100 );

  QWidget* privateLayoutWidget = new QWidget( GameGroupBox, "VLayout2" );
  privateLayoutWidget->setGeometry( QRect( 10, 15, 280, 80 ) );
  VLayout2 = new QVBoxLayout( privateLayoutWidget, 0, 2, "VLayout2");

  // Indication du tour des joueurs
  HLayout2 = new QHBoxLayout( 0, 0, 2, "HLayout2");
  // titre
  TourDeJeuLabel = new QLabel( privateLayoutWidget, "TourDeJeuLabel" );
  QFont TourDeJeuLabel_font(  TourDeJeuLabel->font() );
  TourDeJeuLabel_font.setPointSize( 14 );
  TourDeJeuLabel->setFont( TourDeJeuLabel_font );
  TourDeJeuLabel->setText( trUtf8( "Now playing :" ) );
  HLayout2->addWidget( TourDeJeuLabel );
  // indicateur
  NomLabel = new QLabel( privateLayoutWidget, "NomLabel" );
  QFont NomLabel_font(  NomLabel->font() );
  NomLabel_font.setPointSize( 14 );
  NomLabel->setFont( NomLabel_font );
  HLayout2->addWidget( NomLabel );

  VLayout2->addLayout( HLayout2 );

  VLayout1->addWidget( GameGroupBox );
  //############################//
  // Fenetre de vue des pieces //
  //##########################//
  GLFramePieces = new QFrame( centralWidget(), "GLFramePieces" );
  GLFramePieces->setMouseTracking( TRUE );
  GLFramePieces->setFrameShape( QFrame::StyledPanel );
  GLFramePieces->setFrameShadow( QFrame::Raised );
  GLFramePieces->setFrameStyle( QFrame::Sunken | QFrame::Panel );
  GLFramePieces->setLineWidth( 2 );
  // Create our OpenGL widget
  vuePieces = new GLViewPieces( GLFramePieces );
  HLayout4 = new QHBoxLayout( GLFramePieces, 2, 2, "HLayout4"  );
  HLayout4->addWidget( vuePieces );
  // Ajout au tableau de fenetres
  VLayout1->addWidget( GLFramePieces );

    // Bouttons utiles
  HLayout3 = new QHBoxLayout( 0, 0, 2, "HLayout3");
  // Boutton de reset
  ResetButton = new QPushButton( privateLayoutWidget, "ResetButton" );
  QFont ResetButton_font(  ResetButton->font() );
  ResetButton_font.setPointSize( 14 );
  ResetButton->setFont( ResetButton_font );
  ResetButton->setText( trUtf8( "New Game" ) );
  HLayout3->addWidget( ResetButton );
  // Boutton de quit
  QuitButton = new QPushButton( privateLayoutWidget, "QuitButton" );
  QFont QuitButton_font(  QuitButton->font() );
  QuitButton_font.setPointSize( 14 );
  QuitButton->setFont( QuitButton_font );
  QuitButton->setText( trUtf8( "Quit" ) );
  HLayout3->addWidget( QuitButton );
  VLayout2->addLayout( HLayout3 );

  // Ajout au tableau de fenetres
  MainHLayout->addLayout( VLayout1 );
  QuartoLayout->addLayout( MainHLayout );

  //******************************************//
  //     Signals and Slots Connections        //
  //******************************************//
  connect( ResetButton, SIGNAL( clicked() ), this, SLOT( New() ) );
  connect( QuitButton, SIGNAL( clicked() ), this, SLOT( Exit() ) );
  connect( vuePlateau, SIGNAL( update() ), vuePieces, SLOT( updateGL() ) );
  connect( this, SIGNAL( updategl() ), vuePieces, SLOT( updateGL() ) );
  connect( this, SIGNAL( updategl() ), vuePlateau, SLOT( updateGL() ) );
  connect( vuePlateau, SIGNAL( piecePlacee() ), this, SLOT( piecePlacee() ) );
  connect( vuePieces, SIGNAL( changeJoueur() ), this, SLOT( changeTour() ) );
  connect( vuePlateau, SIGNAL( endGame() ), this, SLOT( finDeJeu() ) );
  // On initialise l'interface
  init(true);
}

Quarto::~Quarto()
{
  delete(vuePlateau);
  delete(vuePieces);
  delete(setofpiece);
}


void Quarto::init(bool begin)
{
  if (begin)
    setofpiece=new SetOfPiece();
  else
    setofpiece->init();

  vuePlateau->reset();
  vuePlateau->setPieces(setofpiece);
  vuePieces->setPieces(setofpiece);
  NomLabel->setText( trUtf8( "Player 1" ) );
  joueur=true;
  pieceplacee=true;
}

void Quarto::New()
{
  init(false);
  emit updategl();
}

void Quarto::Exit()
{
  qApp->exit();
}

void Quarto::piecePlacee() { pieceplacee=true; }

void Quarto::changeTour()
{
  if (pieceplacee)
    {
      if (joueur)
	NomLabel->setText( trUtf8( "Player 2" ) );
      else
	NomLabel->setText( trUtf8( "Player 1" ) );
      joueur = !joueur;
      pieceplacee=false;
    }
}

void Quarto::finDeJeu()
{
  if (QMessageBox::information(this, "Game over", "Game is over, "+NomLabel->text()+" won.", "New game", "Exit") == 0)
    New();
  else
    Exit();
}
