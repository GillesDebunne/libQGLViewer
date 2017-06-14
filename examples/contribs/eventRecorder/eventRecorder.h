#ifndef EVENT_RECORDER_H
#define EVENT_RECORDER_H

#include "QGLViewer/frame.h"

#include <qdatetime.h>
#include <qevent.h>

class Viewer;
class ReplayInterface;

/*! \brief A QGLViewer event recorder, that allows for scenario recording and
 replay.

 An EventRecorder is associated to a QGLViewer. When the EventRecorder
 isRecording(), the different events that are sent to the qglviewer() are
 recorded in an internal data structure. The EventRecorder then can replay()
 these events with an identical timing in order to reproduce the recorded
 scenario. Snapshots can optionally be captured during playback, and the
 resulting image series can be converted into a movie.

 <h3>Event recording</h3>

 Press the 'R' to toggle event recording. A red dot appears in the upper right
 corner of the qglviewer() window during recording.

 The EventRecorder records all mouse and keyboard events using the eventFilter()
 function. These are usually sufficient to reproduce a given scenario.

 However, Frame specific events can be recorded using recordFrameState() and
 other arbitrary events are recorded using recordCustomEvent().

 Note that the mouse to camera motion conversion depends on the window's size
 (so that translations match for instance). The QGLViewer window is hence
 resized to the size it had during record at the beginning of replay.

 <h3>Replay</h3>

 Press 'Shift+R' to openReplayInterfaceWindow(). You can tune replay parameters,
 save and load recorded events, and replay() the recorded events, with an
 optional snapshot saving.

 Note that interaction with the viewer is still possible during and between
 replay(). This can be used to change the display mode during or between
 replay(), in order to generate several renderings of a given scenario.

 See cameraIsRestored(), manipulatedFrameIsRestored(), savesSnapshot(),
 snapshotFrameRate(), saveAtGivenFrameRate(), replaySpeed() for details on the
 replay() parameters.

 <h3>Movie creation</h3>

 The generated numbered image sequence can be converted into a movie by most non
 linear editing software. Here is a way to easily do this using the \c transcode
 publically available software (see <a
 href="http://zebra.fh-weingarten.de/~transcode/index.html">transcode home
 page</a>).

 \code
 > \ls -1 snapshot-* | sort -n > list.txt
 > transcode -i list.txt -x imlist,null -g 720x576 -y xvid,null -f 25 -o
 movie.avi -H 0 \endcode Replace \c 720x576 by your actual snapshot size, and 25
 by the frame rate you want. See the transcode man page for details.

 \nosubgrouping */
class EventRecorder : public QObject {
#ifndef DOXYGEN
  friend class ::ReplayInterface;
#endif
  Q_OBJECT

public:
  EventRecorder(Viewer *const qglviewer);
  virtual ~EventRecorder();

  /*! @name Recording interface */
  //@{
public:
  /*! Returns \c true between calls to startRecording() and stopRecording(). */
  bool isRecording() const { return isRecording_; };

public Q_SLOTS:
  void startRecording();
  void stopRecording();
  void toggleRecording();

  void replay();
  void openReplayInterfaceWindow();
  void saveEventRecord(const QString &filename = "") const;
  void loadEventRecord(const QString &filename = "");
  //@}

  /*! @name Replay parameters */
  //@{
public:
  /*! If \c true, at the beginning of replay(), the state of the qglviewer()
    QGLViewer::manipulatedFrame() is restored to the position and orientation it
    had when the EventRecorder startRecording(), so that the replay() can start
    from a similar viewer configuration.

    Default value is \c true. Set using setManipulatedFrameIsRestored() or using
    the EventRecorder interface window (see openReplayInterfaceWindow()). See
    also cameraIsRestored().

    Use resetBeforeReplay() to reset other viewer and scene related parameters
    before replay() is started. */
  bool manipulatedFrameIsRestored() const {
    return manipulatedFrameIsRestored_;
  };
  /*! If \c true, at the beginning of replay(), the state of the qglviewer()
    QGLViewer::camera() is restored to the position and orientation it had when
    the EventRecorder startRecording(), so that the replay() can start from a
    similar viewer configuration.

    Default value is \c true. Set using setCameraIsRestored() or using the
    EventRecorder interface window (see openReplayInterfaceWindow()). See also
    manipulatedFrameIsRestored().

    Use resetBeforeReplay() to reset other viewer and scene related parameters
    before replay() is started. */
  bool cameraIsRestored() const { return cameraIsRestored_; };
  /*! If \c true, calls QGLViewer::saveSnapshot() during replay(), at a
    snapshotFrameRate() frequency. This is useful to create snapshot series that
    can then be converted into a movie. See the Detailed description section for
    details on movie creation.

    Default value is \c true. Set using setSavesSnapshots() or using the
    EventRecorder interface window (see openReplayInterfaceWindow()). */
  bool savesSnapshots() const { return savesSnapshots_; };
  /*! Replay speed, with respect to the timing when the events were recorded.
    Default value is 1.0, meaning replay matches recording speed.

    0.5 means that replay will be twice as long as recording, while 2.0 means it
    will two times faster. Set using setReplaySpeed().

    This value is only meaningful for real time replays. It is not taken into
    account when savesSnapshots() is \c true. Use setSnapshotFrameRate()
    instead. */
  float replaySpeed() const { return replaySpeed_; };
  /*! When \c true (default), a snapshot is saved at the snapshotFrameRate()
    frequency during replay. When \c false, a snapshot is saved after each
    qglviewer() redraw.

    Set this value to \c true to create a movie out of an interactive demo
    (where you move the camera, pause for a while, toggle some flags, then move
    again...) so that the "rythm" of your scenario is preserved in the resulting
    image sequence. Think of it as if a camera was filming the screen during
    your demo. Tune the frame rate using setSnapshotFrameRate().

    Set this value to \c false when an external timer paces your redraw rate.
    This is typically the case when you use the QGLViewer::animate() function,
    which in a loop computes an animation step and then draws the scene. In that
    case, no matter how long lasts an animation step, you want to save a
    snapshot after each redraw, so that each image corresponds to a given
    animation step, and hence a to a given animation time.

    This value is of course only meaningful when saveSnapshot() is \c true. Can
    be modified when you openReplayInterfaceWindow() or using
    setSaveAtGivenFrameRate(). */
  bool saveAtGivenFrameRate() const { return saveAtGivenFrameRate_; };
  /*! Defines the number of snapshots that will be generated per second during
    replay(), when savesSnapshots() and saveAtGivenFrameRate() are both \c true
    (otherwise meaningless).

    Default value is 25. Set using setReplayFrameRate() or using the
    EventRecorder interface window (see openReplayInterfaceWindow()). */
  int snapshotFrameRate() const { return snapshotFrameRate_; };

public Q_SLOTS:
  /*! Sets the manipulatedFrameIsRestored() value. Default value is \c true. */
  void setManipulatedFrameIsRestored(bool restored = true) {
    manipulatedFrameIsRestored_ = restored;
  };
  /*! Sets the cameraIsRestored() value. Default value is \c true. */
  void setCameraIsRestored(bool restored = true) {
    cameraIsRestored_ = restored;
  };
  /*! Sets whether replay savesSnapshots() or not. Default value is \c true. */
  void setSavesSnapshots(bool savesSnapshots = true) {
    savesSnapshots_ = savesSnapshots;
  };
  /*! Sets the snapshotFrameRate() frequency. Default value is 25 (Hertz). */
  void setSnapshotFrameRate(int snapshotFrameRate) {
    snapshotFrameRate_ = snapshotFrameRate;
  };
  /*! Defines the replaySpeed(). Default value is 1.0 (same speed as during
    recording). Values lower or equal to 0.0 are silently ignored. */
  void setReplaySpeed(float replaySpeed) {
    if (replaySpeed > 0.0)
      replaySpeed_ = replaySpeed;
  };
  /*! Sets the saveAtGivenFrameRate() value. Default is \c true. */
  void setSaveAtGivenFrameRate(bool saveAtGivenFrameRate) {
    saveAtGivenFrameRate_ = saveAtGivenFrameRate;
  };
  //@}

  /*! @name Record extra events */
  //@{
public:
Q_SIGNALS:
  /*! This signal is emitted during replay() when a custom event should be
    replayed back. Custom events are recorded using recordCustomEvent(). Connect
    this signal to slots that are able to reproduce this event.

    The \p id parameter is the one given in recordCustomEvent(). It can be used
    by the slot to select which custom event to reproduce. */
  void replayCustomEvent(int id);

public Q_SLOTS:
  void recordFrameState();
  void recordCustomEvent(int id = 0);
  //@}

protected:
  bool eventFilter(QObject *o, QEvent *e);

private Q_SLOTS:
  void changeImageSize(int i);
  void selectSnapshotName();
  void triggerNextEvent();
  void saveNumberedSnapshot() const;

private:
  Viewer *const qglviewer() const { return qglviewer_; };
  int predefinedFormat();
  void replayNextEvent();

  class FrameState {
  public:
    FrameState(qglviewer::Frame *const fr);
    qglviewer::Frame *const frame;
    qglviewer::Frame state;
  };

  union EventPointer {
    QKeyEvent *keyEvent;
    QMouseEvent *mouseEvent;
    QWheelEvent *wheelEvent;
    FrameState *frameState;
    ;
  };

  class Event {
  public:
    union EventPointer event;
    QEvent::Type type;
    int time;
  };

  struct PredefinedSettings {
    PredefinedSettings(){};
    PredefinedSettings(int w, int h, int fr)
        : width(w), height(h), frameRate(fr){};
    int width, height, frameRate;
  };

  // A s s o c i a t e d   Q G L V i e w e r
  Viewer *const qglviewer_;
  int viewerWidth_, viewerHeight_;

// R e c o r d s
#if QT_VERSION < 0x040000
  QValueVector<Event> eventRecords_;
#else
  QList<Event> eventRecords_;
#endif
  int eventIndex_;
  QTime time_;

  // I n t e r n a l   d a t a
  bool isRecording_;
#if QT_VERSION < 0x040000
  QValueVector<PredefinedSettings> predefinedSettings_;
#else
  QList<PredefinedSettings> predefinedSettings_;
#endif
  int nextSnapshotTime_;
  int nextReplayEvent_;
  bool nextEventIsSaveSnapshot_;

  // R e p l a y   p a r a m e t e r s
  qglviewer::Frame initialCameraFrame_, initialManipulatedFrame_;
  bool cameraIsRestored_, manipulatedFrameIsRestored_, savesSnapshots_;
  int snapshotFrameRate_;
  int recordDuration_;
  float replaySpeed_;
  bool saveAtGivenFrameRate_;

  // R e p l a y   i n t e r f a c e
  ReplayInterface *replayInterface_;
  int originalWidth_, originalHeight_;
};
#endif // EVENT_RECORDER_H
