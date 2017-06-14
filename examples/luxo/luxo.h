#include <QGLViewer/qglviewer.h>

class Luxo {
public:
  Luxo();

  void draw(bool names = false);

  qglviewer::ManipulatedFrame *frame(unsigned short i) { return frame_[i]; }
  void setSelectedFrameNumber(unsigned short nb) { selected = nb; }

private:
  // The four articulations of the lamp
  qglviewer::ManipulatedFrame *frame_[4];
  // Used to draw the selected element in a different color
  unsigned short selected;

  void drawCone(float zMin, float zMax, float r1, float r2, int nbSub);
  void drawBase();
  void drawArm();
  void drawCylinder();
  void setColor(unsigned short nb);
  void drawHead();
};

class Viewer : public QGLViewer {
protected:
  virtual void draw();
  virtual void init();
  virtual void drawWithNames();
  virtual void postSelection(const QPoint &point);
  virtual QString helpString() const;

  void initSpotLight();

private:
  Luxo luxo;
};
