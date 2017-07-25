#include <QGLViewer/qglviewer.h>

class EventRecorder;

class Viewer : public QGLViewer {
  Q_OBJECT

#ifndef DOXYGEN
  friend class EventRecorder;
#endif

public:
  /*! Returns the viewer associated qglviewer::EventRecorder.

  Use 'R' to toggle the recording of the scenario. The recorded scenario can
  then be replayed using 'Shift+R'. */
  EventRecorder *eventRecorder() const { return eventRecorder_; };

public:
Q_SIGNALS:
  /*! Signal emitted by the default QGLViewer::resetBeforeReplay() function.
  Connect this signal to any object that should be notified before
  qglviewer::EventRecorder::replay() starts replaying a scenario.

  Use this signal-slot mechanism ONLY IF you don't inherit your viewer from
  QGLViewer (in that case, simply overload the resetBeforeReplay() function). */
  void replayStarted();

protected Q_SLOTS:
  /*! This function is called before the eventRecorder() starts to replay a
  scenario. It should reset the scene state to the state is was when
  qglviewer::EventRecorder::startRecording() was called, so that the replay
  starts from the same state.

  Note that the camera() and manipulatedFrame() state (position and orientation)
  can be automatically restored by qglviewer::EventRecorder::replay(). See
  qglviewer::EventRecorder::cameraIsRestored() and
  qglviewer::EventRecorder::manipulatedFrameIsRestored() for details.

  The default behavior of this function is simply to emit the replayStarted()
  signal. Overload this method or connect this signal to any object that shouyld
  be notified. You probably want to call updateGL() at the end of this function,
  so that the first snapshot actually corresponds to the state you have reset.
*/
  virtual void resetBeforeReplay() { Q_EMIT replayStarted(); };

  void setManipulatedFrame(qglviewer::ManipulatedFrame *fr);

protected:
  virtual void draw();
  virtual void postDraw();
  virtual void init();
  virtual QString helpString() const;
  virtual void keyPressEvent(QKeyEvent *e);

private:
  EventRecorder *eventRecorder_;
};
