#ifndef AGORA_VIEWER_H
#define AGORA_VIEWER_H
#include "QGLViewer/qglviewer.h"
#include "agora.h"

typedef Agora<unsigned short> Agora_t;
typedef Agora_t::Pile_t Pile;
typedef Agora_t::Coup Play;
typedef Case_t Position;
typedef Agora_t::Possible_t Possibles;


class AgoraViewer : public QGLViewer
{
  Q_OBJECT

public:
  AgoraViewer(QWidget* parent=NULL, const char* name=0);

 public slots:
 // F i l e   m e n u
 void load();
 void save();
 void saveAs();
 void print() { std::cout << "Print not yet implemented" << std::endl; }

  // G a m e   m e n u
  void undo();
  void redo();
  void startNewGame() { initGame(); updateGL(); }
  void suggestPlay() { play_ = agora.Suggest(play_, blackPlay_, gameLevel_.computerMaxReflexionTime_, gameLevel_.computerMaxDepth_); play(); }
  void togglePlayAgainstComputer(bool on);
  void levelIsEasy()      { gameLevel_.computerMaxDepth_ = 3; }
  void levelIsAverage()   { gameLevel_.computerMaxReflexionTime_ = 1.5; gameLevel_.computerMaxDepth_ = 0; }
  void levelIsDifficult() { gameLevel_.computerMaxReflexionTime_ = 8.0; gameLevel_.computerMaxDepth_ = 0; }
  void toggleComputerIsBlack(bool black) { computerIsBlack_ = black; }

  // D i s p l a y   m e n u
  void toggleAnimation(bool on) { animatePlays_ = on; }
  void toggleLight(bool on) { if (on) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1); updateGL(); }
  void toggleTexture(bool on) { textures_ = on; updateGL(); }
  void toggleShowPossible(bool on) { displayPossibleDestination_ = on; updateGL(); }

  // H e l p   m e n u
  void help();
  void about();

protected :
  void draw();
  void mouseDoubleClickEvent(QMouseEvent *) {};
  void keyPressEvent(QKeyEvent *) {};
  void select(const QMouseEvent*);
  void init();
  void play();

private slots:
 void simplePlay();

  void swapRevoStartColor();
  void swapRevoEndColor();
  void swapArrivalColor();

private :
  // D r a w i n g   f u n c t i o n s
  void drawAgora() const;
  void drawAllPieces(bool select = false) const;
  void drawPiece() const;
  void highlightSelectedPiece() const;
  void drawPossibleDestinations(bool select = false) const;

  // D r a w i n g   u t i l s
  void drawBorder(float width, float height, bool out=true, float heightMin=0.0) const;
  void drawBorderLines(float width, float height, bool out=true, float heightMin=0.0) const;
  void drawFace(float width, float height, bool up) const;
  void drawRing(float width, float height, float thickness=1.0) const;
  void drawSeparatingBars(float width, float height, float thickness=1.0) const;
  void drawLeveLines(float width, float height) const;

  // I n i t i a l i z a t i o n
  void initSpotLight();
  void initCamera();
  void initViewer();
  void initGame();

  // G a m e   i n t e r f a c e
  void reactToSelection(int selected, bool onTop);
  void updatePiecePositions();
  int higherPieceOnPosition(Position pos) const;
  void animatePlay();
  void deselect();

  // Size of one Agora position is 1.0, entire board is hence 6.0 x 6.0.
  static const float pieceSize   = 29.0 / 38.0;
  static const float pieceHeight = 5.0 / 38.0;
  qglviewer::Vec normal[5];

  Agora_t agora;
  Play play_;

  bool blackPlay_;
  bool computerIsBlack_;

  struct AgoraLevel {
    float computerMaxReflexionTime_;
    int computerMaxDepth_;
  } gameLevel_;

  int selectedPiece_;
  bool playerIsComputer_;
  bool gameIsOver_;

  // D i s p l a y   F l a g s
  bool displayPossibleDestination_;
  bool animatePlays_;
  bool textures_;

  struct Piece
  {
    Position square;
    bool isBlack;
    qglviewer::Frame frame;
    float scale;
    int level;
  };
  Piece piece_[16];
  qglviewer::Vec casePosition[36];
  Possibles possibleDest_;

  qglviewer::KeyFrameInterpolator kfi_[16];
  std::vector<int> revoStart_, revoEnd_, swapArrival_;
  QString fileName;

  struct Undo
  {
    Undo() {};
    Undo(const Play& play, const Agora_t& agora);
    Position pos1,    pos2;
    Pile     before1, before2;
    Pile     after1,  after2;
  };
  std::vector<Undo> history_;
  void updateUndoHistory(bool before);
  unsigned int undoIndex_, maxUndoIndex_;
};

#endif // AGORA_VIEWER_H
