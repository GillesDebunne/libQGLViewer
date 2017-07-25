#ifndef OBJECT_H_
#define OBJECT_H_

#include "QGLViewer/frame.h"

class Object {
public:
  void draw() const;
  qglviewer::Frame frame;
};

#endif // OBJECT_H_
