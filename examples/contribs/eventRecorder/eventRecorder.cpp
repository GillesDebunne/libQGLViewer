#include "eventRecorder.h"
#include "replayInterface.h"
#include "viewer.h"
#include <qcombobox.h>
#include <qfiledialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qpoint.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qspinbox.h>

using namespace qglviewer;

/*! Creates an EventRecorder associated to the provided \c qglviewer.

  All \p qglviewer events will then be filtered by the created EventRecorder.
  See \c QObject::installEventFilter() for details. */
EventRecorder::EventRecorder(Viewer *const qglviewer)
    : qglviewer_(qglviewer), eventRecords_(25000) {
  if (!qglviewer) {
    qWarning("NULL QGLViewer in EventRecorder constructor");
    return;
  }

  qglviewer->installEventFilter(this);

  eventIndex_ = 0;
  isRecording_ = false;
  recordDuration_ = 0;

  setCameraIsRestored();
  setManipulatedFrameIsRestored();
  setSavesSnapshots();
  setSnapshotFrameRate(25);
  setSaveAtGivenFrameRate(true);
  setReplaySpeed(1.0);

  predefinedSettings_.push_back(
      PredefinedSettings(0, 0, 0)); // Custom, dummy values
  predefinedSettings_.push_back(PredefinedSettings(720, 576, 25)); // PAL
  predefinedSettings_.push_back(PredefinedSettings(720, 480, 30)); // NTSC
  predefinedSettings_.push_back(PredefinedSettings(640, 480, 20)); // VGA
  predefinedSettings_.push_back(PredefinedSettings(360, 288, 12)); // 1/2 PAL
  predefinedSettings_.push_back(PredefinedSettings(360, 240, 15)); // 1/2 NTSC
  predefinedSettings_.push_back(PredefinedSettings(320, 240, 10)); // 1/2 VGA
  predefinedSettings_.push_back(PredefinedSettings(160, 120, 8));  // 1/4 VGA
}

/*! Destructor deletes all recorded events. */
EventRecorder::~EventRecorder() {
  if (replayInterface_)
    delete replayInterface_;

  for (QValueVector<Event>::iterator it = eventRecords_.begin(),
                                     end = eventRecords_.end();
       it != end; ++it)
    delete (*it).event.keyEvent; // or any other pointer
}

EventRecorder::FrameState::FrameState(Frame *const fr) : frame(fr) {
  state.setTranslation(fr->position());
  state.setRotation(fr->orientation());
}

/*! Calls startRecording() or stopRecording() depending on whether the
  EventRecorder isRecording() or not.

  This method is called by pressing 'R'. */
void EventRecorder::toggleRecording() {
  if (isRecording())
    stopRecording();
  else
    startRecording();
}

/*! Starts recording events for the associated qglviewer(). The events are
  recorded using eventFilter(), recordCustomEvent() and recordFrameState().

  All previously recorded events are lost. Use saveEventRecord() to save current
  event recording.

  The \e current QGLViewer::camera() and QGLViewer::manipulatedFrame() (if any)
  state is stored, so that it can be restored at the next call to replay(). See
  cameraIsRestored() and manipulatedFrameIsRestored().

  Use stopRecording() or toggleRecording() to stop recording and isRecording()
  to know current state. */
void EventRecorder::startRecording() {
  eventIndex_ = 0;
  isRecording_ = true;
  initialCameraFrame_.setPosition(qglviewer()->camera()->frame()->position());
  initialCameraFrame_.setOrientation(
      qglviewer()->camera()->frame()->orientation());
  if (qglviewer()->manipulatedFrame()) {
    initialManipulatedFrame_.setPosition(
        qglviewer()->manipulatedFrame()->position());
    initialManipulatedFrame_.setOrientation(
        qglviewer()->manipulatedFrame()->orientation());
  }
  viewerWidth_ = qglviewer()->width();
  viewerHeight_ = qglviewer()->height();

  time_.start();
}

/*! Stops a recording initiated with startRecording() or toggleRecording(). */
void EventRecorder::stopRecording() {
  isRecording_ = false;
  recordDuration_ = time_.elapsed();

  // Check that last record was the TOGGLE_RECORDING key press.
  // Give it a fake time so that it is never replayed back.
  if (eventRecords_[eventIndex_ - 1].type == QEvent::KeyPress) {
    eventRecords_[eventIndex_ - 1].time = recordDuration_ + 10000;
    eventIndex_--;
  }
}

/*! Reimplementation of the \c QObject function. Always returns \p false
  (meaning event is \e not handled and should be forwarded to the \p qglviewer).

  When EventRecorder isRecording(), the following \c QEvent::type are recorded
  in an internal data structure: \c KeyPress, \c KeyRelease, \c
  MouseButtonPress, \c MouseButtonRelease, \c MouseButtonDblClick, \c MouseMove,
  \c Wheel and \c Timer (used by QGLViewer::animate()). Does nothing when
  isRecording() is \c false.

  You may overload this method to filter other \c QEvent::type (your
  implementation should probably call this function).

  Other specific events can be recorded using recordFrameState() and
  recordCustomEvent(). */
bool EventRecorder::eventFilter(QObject *, QEvent *e) {
  if (isRecording()) {
    bool record = true;
    switch (e->type()) {
    case QEvent::KeyPress:
    case QEvent::KeyRelease: {
      QKeyEvent *ke = (QKeyEvent *)(e);
      eventRecords_[eventIndex_].event.keyEvent =
          new QKeyEvent(e->type(), ke->key(), ke->ascii(), int(ke->state()));
      break;
    }
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove: {
      QMouseEvent *me = (QMouseEvent *)(e);
      eventRecords_[eventIndex_].event.mouseEvent = new QMouseEvent(
          e->type(), me->pos(), int(me->button()), int(me->state()));
      break;
    }
    case QEvent::Wheel: {
      QWheelEvent *we = (QWheelEvent *)(e);
      eventRecords_[eventIndex_].event.wheelEvent =
          new QWheelEvent(we->pos(), we->delta(), int(we->state()));
      break;
    }
    case QEvent::Timer: {
      eventRecords_[eventIndex_].event.keyEvent = NULL; // or any other pointer
      break;
    }
    default:
      record = false;
      break;
    }

    if (record) {
      eventRecords_[eventIndex_].type = e->type();
      eventRecords_[eventIndex_].time = time_.elapsed();
      eventIndex_++;
    }
  }
  return false;
}

/*! Saves current recorded events in a file in XML format. If no \p filename is
  provided (default), opens a file dialog to ask for a name. See also
  loadEventRecord(). */
void EventRecorder::saveEventRecord(const QString &filename) const {
  QString name;
  bool ok = false;
  if (filename.isEmpty())
    while (!ok) {
      name = QFileDialog::getSaveFileName();
      if (name.isEmpty()) // means cancel
        return;

      ok = true;

      QFileInfo info(name);
      if (info.exists())
        ok = (QMessageBox::warning(
                  qglviewer(), "Overwrite file ?",
                  "File " + info.fileName() + " already exists.\nSave anyway ?",
                  QMessageBox::Yes, QMessageBox::Cancel) == QMessageBox::Yes);
    }
  else
    name = filename;

  // Create the DOM document
  QDomDocument doc("EVENTRECORDER");

  QDomElement de = doc.createElement("EventRecorder");
  de.setAttribute("nbEvents", eventIndex_);
  de.setAttribute("duration", recordDuration_);

  QDomElement replay = doc.createElement("Replay");
  replay.setAttribute("restoreCamera", (cameraIsRestored() ? "true" : "false"));
  replay.setAttribute("restoreManipulatedFrame",
                      (manipulatedFrameIsRestored() ? "true" : "false"));
  replay.setAttribute("savesSnapshot", (savesSnapshots() ? "true" : "false"));
  replay.setAttribute("saveAtGivenFrameRate",
                      (saveAtGivenFrameRate() ? "true" : "false"));
  replay.setAttribute("replaySpeed", replaySpeed());
  replay.appendChild(initialCameraFrame_.domElement("InitialCamera", doc));
  replay.appendChild(
      initialManipulatedFrame_.domElement("InitialManipulatedFrame", doc));

  QDomElement viewer = doc.createElement("Viewer");
  viewer.setAttribute("width", viewerWidth_);
  viewer.setAttribute("height", viewerHeight_);
  replay.appendChild(viewer);

  de.appendChild(replay);

  QDomElement events = doc.createElement("Events");
  for (int i = 0; i < eventIndex_; ++i) {
    QDomElement event = doc.createElement("event");

    switch (eventRecords_[i].type) {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
      event.setAttribute("key", eventRecords_[i].event.keyEvent->key());
      event.setAttribute("ascii", eventRecords_[i].event.keyEvent->ascii());
      event.setAttribute("state",
                         int(eventRecords_[i].event.keyEvent->state()));
      break;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseMove:
      event.setAttribute("posx", eventRecords_[i].event.mouseEvent->x());
      event.setAttribute("posy", eventRecords_[i].event.mouseEvent->y());
      event.setAttribute("button",
                         int(eventRecords_[i].event.mouseEvent->button()));
      event.setAttribute("state",
                         int(eventRecords_[i].event.mouseEvent->state()));
      break;
    case QEvent::Wheel:
      event.setAttribute("posx", eventRecords_[i].event.wheelEvent->x());
      event.setAttribute("posy", eventRecords_[i].event.wheelEvent->y());
      event.setAttribute("delta",
                         int(eventRecords_[i].event.wheelEvent->delta()));
      event.setAttribute("state",
                         int(eventRecords_[i].event.wheelEvent->state()));
      break;
    case QEvent::Timer:
      break;
    case QEvent::MaxUser: // Actually means Frame state
    {
      QString frameName = "unknown";
      if (eventRecords_[i].event.frameState->frame ==
          qglviewer()->camera()->frame())
        frameName = "camera";
      else if ((eventRecords_[i].event.frameState->frame ==
                qglviewer()->manipulatedFrame()) &&
               (qglviewer()->manipulatedFrame()))
        frameName = "manipulatedFrame";
      event = eventRecords_[i].event.frameState->state.domElement("event", doc);
      event.setAttribute("name", frameName);
      break;
    }
    case QEvent::User:
      event.setAttribute("id", (int)(eventRecords_[i].event.frameState));
      break;
    default:
      qWarning("Unknown event type " + QString::number(eventRecords_[i].type));
      break;
    }
    event.setAttribute("time", eventRecords_[i].time);
    event.setAttribute("type", eventRecords_[i].type);
    events.appendChild(event);
  }

  de.appendChild(events);
  doc.appendChild(de);

  // Write the DOM tree to file
  QFile f(name);
  if (f.open(IO_WriteOnly)) {
    QTextStream out(&f);
    doc.save(out, 2);
    f.flush();
    f.close();
  } else
#if QT_VERSION < 0x030200
    QMessageBox::warning(qglviewer(), "Save to file error",
                         "Unable to save to file " + name);
#else
    QMessageBox::warning(qglviewer(), "Save to file error",
                         "Unable to save to file " + name + ":\n" +
                             f.errorString());
#endif
}

/*! Loads a previously recorded event list from a file. If no \p filename is
  provided (default), opens a file dialog to select a file. See also
  saveEventRecord().

  \attention The loaded event record is independent of the viewer and of the
  scene. Trying to replay() an event list recorded in an other context is most
  likely to produce unexpected results. Only the initial camera and
  manipulatedFrame (if any) states are loaded back and can be restored (see
  cameraIsRestored() and manipulatedFrameIsRestored()). */
void EventRecorder::loadEventRecord(const QString &filename) {
  QString name;
  if (filename.isEmpty()) {
    name = *((QFileDialog::getOpenFileNames()).begin());
    if (name.isEmpty()) // means cancel
      return;
  } else
    name = filename;

  // Create the DOM document
  QDomDocument doc("EVENTRECORDER");

  // Write the DOM tree to file
  QFile f(name);
  if (f.open(IO_ReadOnly) == true) {
    doc.setContent(&f);
    f.close();
    // Parse the DOM tree
    QDomElement root = doc.documentElement();

    eventIndex_ = (root.attribute("nbEvents", "0")).toInt();
    recordDuration_ = (root.attribute("duration", "0")).toInt();

    QDomElement child = root.firstChild().toElement();
    while (!child.isNull()) {
      if (child.tagName() == "Replay") {
        setCameraIsRestored((child.attribute("restoreCamera", "true") == "true")
                                ? true
                                : false);
        setManipulatedFrameIsRestored(
            (child.attribute("restoreManipulatedFrame", "true") == "true")
                ? true
                : false);
        setSavesSnapshots((child.attribute("savesSnapshots", "true") == "true")
                              ? true
                              : false);
        setSaveAtGivenFrameRate(
            (child.attribute("saveAtGivenFrameRate", "true") == "true")
                ? true
                : false);
        setReplaySpeed(child.attribute("replaySpeed", "1.0").toFloat());

        QDomElement subChild = child.firstChild().toElement();
        while (!subChild.isNull()) {
          if (subChild.tagName() == "InitialCamera")
            initialCameraFrame_.initFromDOMElement(subChild);
          if (subChild.tagName() == "InitialManipulatedFrame")
            initialManipulatedFrame_.initFromDOMElement(subChild);
          if (subChild.tagName() == "Viewer") {
            viewerWidth_ =
                subChild
                    .attribute("width", QString::number(qglviewer()->width()))
                    .toInt();
            viewerHeight_ =
                subChild
                    .attribute("height", QString::number(qglviewer()->height()))
                    .toInt();
          }

          subChild = subChild.nextSibling().toElement();
        }
      }

      if (child.tagName() == "Events") {
        int count = 0;
        QDomElement event = child.firstChild().toElement();
        while (!event.isNull()) {
          eventRecords_[count].type =
              QEvent::Type((event.attribute("type", "0")).toInt());
          eventRecords_[count].time = (event.attribute("time", "0")).toInt();

          switch (eventRecords_[count].type) {
          case QEvent::KeyPress:
          case QEvent::KeyRelease: {
            int key = (event.attribute("key", "0")).toInt();
            int ascii = (event.attribute("ascii", "0")).toInt();
            int state = (event.attribute("state", "0")).toInt();
            eventRecords_[count].event.keyEvent =
                new QKeyEvent(eventRecords_[count].type, key, ascii, state);
            break;
          }
          case QEvent::MouseButtonRelease:
          case QEvent::MouseButtonDblClick:
          case QEvent::MouseMove: {
            int posx = (event.attribute("posx", "0")).toInt();
            int posy = (event.attribute("posy", "0")).toInt();
            int button = (event.attribute("button", "0")).toInt();
            int state = (event.attribute("state", "0")).toInt();
            eventRecords_[count].event.mouseEvent = new QMouseEvent(
                eventRecords_[count].type, QPoint(posx, posy), button, state);
            break;
          }
          case QEvent::Wheel: {
            int posx = (event.attribute("posx", "0")).toInt();
            int posy = (event.attribute("posy", "0")).toInt();
            int delta = (event.attribute("delta", "0")).toInt();
            int state = (event.attribute("state", "0")).toInt();
            eventRecords_[count].event.wheelEvent =
                new QWheelEvent(QPoint(posx, posy), delta, state);
            break;
          }
          case QEvent::Timer:
            eventRecords_[count].event.wheelEvent = NULL;
            break;
          case QEvent::MaxUser: // Actually means Frame state
          {
            QString frameName = event.attribute("name", "unknown");
            if (frameName == "camera")
              eventRecords_[count].event.frameState =
                  new EventRecorder::FrameState(qglviewer()->camera()->frame());
            else if ((frameName == "manipulatedFrame") &&
                     (qglviewer()->manipulatedFrame()))
              eventRecords_[count].event.frameState =
                  new EventRecorder::FrameState(
                      qglviewer()->manipulatedFrame());
            else
              qWarning("Unable to affect Frame state event record");
            eventRecords_[count].event.frameState->state.initFromDOMElement(
                event);
            break;
          }
          case QEvent::User: {
            int id = (event.attribute("id", "0")).toInt();
            eventRecords_[count].event.frameState = (FrameState *)(id);
            break;
          }
          default:
            qWarning("Unknown event type " +
                     QString::number(eventRecords_[count].type));
            break;
          }
          count++;
          event = event.nextSibling().toElement();
        }
      }
      child = child.nextSibling().toElement();
    }
  } else
#if QT_VERSION < 0x030200
    QMessageBox::warning(qglviewer(), "Open file error",
                         "Unable to open file " + name);
#else
    QMessageBox::warning(qglviewer(), "Open file error",
                         "Unable to open file " + name + ":\n" +
                             f.errorString());
#endif
}

/*! Use this method to make the EventRecorder record Frame related events that
are not generated by a keyboard or mouse event. It is needed when a Frame (or a
ManipulatedFrame(), or any derived class) is modified by external events, such
as a QTimer. These events will not be properly reproduced using the standard
keyboard and mouse events replay.

A typical example is when a Frame is interpolated using a KeyFrameInterpolator
(which regularily updates the Frame position). To record these update events you
would use: /code connect(myFrame, SIGNAL( interpolated() ),
myViewer->eventRecorder(), SLOT(recordFrameState())); /encode

Each time the Frame is interpolated, the recordFrameState() method will record
the Frame position and orientation, so that it can be restored back during
replay().

Note that the QGLViewer::camera() and QGLViewer::manipulatedFrame()
Frame::interpolated() and SpinningFrame::spinned() signals are automatically
connected to this method.

Use recordCustomEvent() to record arbitrary events. Keyboard and mouse events
are recorded by eventFilter(). */
void EventRecorder::recordFrameState() {
  if (isRecording()) {
    const Frame *frame = dynamic_cast<const Frame *>(sender());

    if (!frame) {
      qWarning("Unable to cast to Frame *");
      return;
    }

    eventRecords_[eventIndex_].type = QEvent::MaxUser; // Dummy
    eventRecords_[eventIndex_].time = time_.elapsed();
    eventRecords_[eventIndex_].event.frameState =
        new EventRecorder::FrameState(const_cast<Frame *>(frame));
    eventIndex_++;
  }
}

/*! Use this method to record a very specific event that should be reproduced
during replay().

Standard mouse and keyboard events are recorded using eventFilter(). Frame (and
derived classes) modification can be recorded using recordFrameState(). This
function is dedicated to other cases.

The \p id parameter will be used to differentiate the different custom events
during replay(). During replay(), the replayCustomEvent() signal will be emitted
when a custom event needs to be reproduced. Connect this signal to slots that
are able to reproduce this event. */
void EventRecorder::recordCustomEvent(int id) {
  if (isRecording()) {
    eventRecords_[eventIndex_].type = QEvent::User;
    eventRecords_[eventIndex_].time = time_.elapsed();
    eventRecords_[eventIndex_].event.frameState = (FrameState *)(id);
    eventIndex_++;
  }
}

void EventRecorder::changeImageSize(int i) {
  if (i == 0) {
    replayInterface_->imageWidth->setValue(originalWidth_);
    replayInterface_->imageHeight->setValue(originalHeight_);
  } else {
    replayInterface_->imageWidth->setValue(predefinedSettings_[i].width);
    replayInterface_->imageHeight->setValue(predefinedSettings_[i].height);
    replayInterface_->snapshotFrameRate->setValue(
        predefinedSettings_[i].frameRate);
  }

  qglviewer()->resize(replayInterface_->imageWidth->value(),
                      replayInterface_->imageHeight->value());
  qglviewer()->updateGL();
}

int EventRecorder::predefinedFormat() {
  for (unsigned int i = 1; i < predefinedSettings_.size(); ++i) {
    if ((qglviewer()->width() == predefinedSettings_[i].width) &&
        (qglviewer()->height() == predefinedSettings_[i].height)) {
      replayInterface_->snapshotFrameRate->setValue(
          predefinedSettings_[i].frameRate);
      return i;
    }
  }
  return 0;
}

void EventRecorder::selectSnapshotName() {
  QString name = QFileDialog::getSaveFileName();
  if (name.isEmpty()) // cancel
    return;

  name.replace(QDir::currentDirPath() + "/", "");
  replayInterface_->snapshotFileName->setText(name);
}

/*! Opens an EventRecorder dedicated interface window. All the EventRecorder
  parameters can be tuned. The qglviewer() size can be modified to fit TV
  standards (useful to create movies from snapshots), you can saveEventRecord()
  and loadEventRecord() and replay() the events.

  This method is called by the associated qglviewer() by pressing 'Shift+R'.

  See also cameraIsRestored(), manipulatedFrameIsRestored() and
  savesSnapshots(). */
void EventRecorder::openReplayInterfaceWindow() {
  if (!replayInterface_) {
    replayInterface_ = new ReplayInterface(qglviewer());
    connect(replayInterface_->imageSize, SIGNAL(activated(int)),
            SLOT(changeImageSize(int)));
    connect(replayInterface_->browseButton, SIGNAL(released()),
            SLOT(selectSnapshotName()));
    connect(replayInterface_->saveButton, SIGNAL(released()),
            SLOT(saveEventRecord()));
    connect(replayInterface_->loadButton, SIGNAL(released()),
            SLOT(loadEventRecord()));
  }

  // Image size settings
  originalWidth_ = qglviewer()->width();
  originalHeight_ = qglviewer()->height();
  replayInterface_->imageWidth->setValue(originalWidth_);
  replayInterface_->imageHeight->setValue(originalHeight_);
  replayInterface_->imageSize->setCurrentItem(predefinedFormat());
  replayInterface_->imageQuality->setValue(qglviewer()->snapshotQuality());

  // Other settings
  replayInterface_->restoreCamera->setChecked(cameraIsRestored());
  replayInterface_->restoreManip->setChecked(manipulatedFrameIsRestored());
  replayInterface_->savesSnapshots->setChecked(savesSnapshots());
  replayInterface_->snapshotFrameRate->setValue(snapshotFrameRate());
  replayInterface_->saveAtGivenFrameRate->setChecked(saveAtGivenFrameRate());
  replayInterface_->replaySpeed->setValue(int(100.0 * replaySpeed()));

  if (eventIndex_ > 0) {
    replayInterface_->replayText->setText(
        "Current record lasts for " +
        QString::number(recordDuration_ / 1000.0, 'f', 1) + " seconds (" +
        QString::number(eventIndex_) + " events).");
    replayInterface_->saveButton->setEnabled(true);
    replayInterface_->replayButton->setEnabled(true);
  } else {
    replayInterface_->replayText->setText("No scenario has been recorded.");
    replayInterface_->saveButton->setEnabled(false);
    replayInterface_->replayButton->setEnabled(false);
  }

  if (replayInterface_->exec() == QDialog::Accepted) {
    qglviewer()->setSnapshotFileName(
        replayInterface_->snapshotFileName->text());
    qglviewer()->setSnapshotFormat(replayInterface_->fileFormat->currentText());
    qglviewer()->setSnapshotCounter(replayInterface_->firstNumber->value());
    qglviewer()->setSnapshotQuality(replayInterface_->imageQuality->value());

    setCameraIsRestored(replayInterface_->restoreCamera->isChecked());
    setManipulatedFrameIsRestored(replayInterface_->restoreManip->isChecked());
    setSavesSnapshots(replayInterface_->savesSnapshots->isChecked());
    setSnapshotFrameRate(replayInterface_->snapshotFrameRate->value());
    setSaveAtGivenFrameRate(
        replayInterface_->saveAtGivenFrameRate->isChecked());
    setReplaySpeed(replayInterface_->replaySpeed->value() / 100.0);

    replay();
  }
}

/*! Replays the events that were recorded from the qglviewer() between the
  previous calls to startRecording() and stopRecording() (or from a scenario
  re-loaded from a file using loadEventRecord()).

  If cameraIsRestored() (resp. manipulatedFrameIsRestored()) is \c true, the
  QGLViewer::camera() (resp. QGLViewer::manipulatedFrame()) state is first
  restored to the position and orientation it had when startRecording() was
  called.

  The qglviewer() resetBeforeReplay() function is then called, so that the other
  scene and viewer states could be reset to their state when startRecording()
  was called.

  The recorded events are then played back:

  \arg If savesSnapshots(), a snapshot is generated at the snapshotFrameRate()
  frequency while events are played back. Note that the resulting replay speed
  may differ from the original one because of the calls to
  QGLViewer::savesSnapshots(). No events are lost anyway, since the replay is
  "paused" during snapshot savings. Modify the QGLViewer::snapshotQuality() to
  tune image quality and size (this can be done through the replay interface,
  see openReplayInterfaceWindow()). When saveAtGivenFrameRate() is \c false, a
  snapshot is only generated after each redraw of the qglviewer().

  \arg If savesSnapshots() is \c false, the recorded events are simply played
  back, at the same speed than during the recording (at a precision of a few
  milliseconds). This is useful to check the scenario before it is saved or
  rendered to image files using savesSnapshots(). */
void EventRecorder::replay() {
  if (isRecording())
    return;

  bool updateGLNeeded = false;
  if (cameraIsRestored()) {
    qglviewer()->camera()->frame()->setPosition(initialCameraFrame_.position());
    qglviewer()->camera()->frame()->setOrientation(
        initialCameraFrame_.orientation());
    updateGLNeeded = true;
  }

  if ((manipulatedFrameIsRestored()) && (qglviewer()->manipulatedFrame())) {
    qglviewer()->manipulatedFrame()->setPosition(
        initialManipulatedFrame_.position());
    qglviewer()->manipulatedFrame()->setOrientation(
        initialManipulatedFrame_.orientation());
    updateGLNeeded = true;
  }

  if (updateGLNeeded)
    qglviewer()->updateGL();

  qglviewer()->resize(viewerWidth_, viewerHeight_);
  qglviewer()->resetBeforeReplay();

  nextReplayEvent_ = 0;
  nextEventIsSaveSnapshot_ = false;

  if (savesSnapshots()) {
    if (saveAtGivenFrameRate()) {
      nextEventIsSaveSnapshot_ = true;
      QTimer::singleShot(0, this, SLOT(triggerNextEvent()));
    } else {
      connect(qglviewer(), SIGNAL(drawFinished()), this,
              SLOT(saveNumberedSnapshot()));
      QTimer::singleShot(eventRecords_[nextReplayEvent_].time, this,
                         SLOT(triggerNextEvent()));
    }
  } else
    QTimer::singleShot(eventRecords_[nextReplayEvent_].time, this,
                       SLOT(triggerNextEvent()));

  time_.start();
}

void EventRecorder::saveNumberedSnapshot() const {
  qglviewer()->saveSnapshot(true, true);
}

void EventRecorder::triggerNextEvent() {
  replayNextEvent();

  // Speed

  int nextTime;
  if ((eventRecords_[nextReplayEvent_].time > nextSnapshotTime_) &&
      savesSnapshots() && saveAtGivenFrameRate()) {
    nextEventIsSaveSnapshot_ = true;
    nextTime = nextSnapshotTime_;
  } else {
    nextEventIsSaveSnapshot_ = false;
    nextTime = eventRecords_[nextReplayEvent_].time;
  }

  if (nextTime <= recordDuration_)
    // QTimer::singleShot(nextTime-time-.elapsed(), this,
    // SLOT(triggerNextEvent()));
    QTimer::singleShot(int((nextTime - time_.elapsed()) / replaySpeed()), this,
                       SLOT(triggerNextEvent()));
  else if (savesSnapshots()) {
    if (!saveAtGivenFrameRate())
      disconnect(qglviewer(), SIGNAL(drawFinished()), this,
                 SLOT(saveNumberedSnapshot()));

    QMessageBox::information(qglviewer(), "Replay complete",
                             "Replay complete\n");
  } else
    qglviewer()->updateGL();
}

void EventRecorder::replayNextEvent() {
  if (nextEventIsSaveSnapshot_) {
    qglviewer()->saveSnapshot(true, true);
    nextSnapshotTime_ += 1000 / snapshotFrameRate();
  } else {
    switch (eventRecords_[nextReplayEvent_].type) {
    case QEvent::KeyPress:
      qglviewer()->keyPressEvent(
          eventRecords_[nextReplayEvent_].event.keyEvent);
      break;
    case QEvent::KeyRelease:
      qglviewer()->keyReleaseEvent(
          eventRecords_[nextReplayEvent_].event.keyEvent);
      break;
    case QEvent::MouseButtonPress:
      qglviewer()->mousePressEvent(
          eventRecords_[nextReplayEvent_].event.mouseEvent);
      break;
    case QEvent::MouseButtonRelease:
      qglviewer()->mouseReleaseEvent(
          eventRecords_[nextReplayEvent_].event.mouseEvent);
      break;
    case QEvent::MouseButtonDblClick:
      qglviewer()->mouseDoubleClickEvent(
          eventRecords_[nextReplayEvent_].event.mouseEvent);
      break;
    case QEvent::MouseMove:
      qglviewer()->mouseMoveEvent(
          eventRecords_[nextReplayEvent_].event.mouseEvent);
      break;
    case QEvent::Wheel:
      qglviewer()->wheelEvent(eventRecords_[nextReplayEvent_].event.wheelEvent);
      break;
    case QEvent::Timer:
      qglviewer()->animate();
      qglviewer()->updateGL();
      break;
    case QEvent::MaxUser: // Actually means Frame state
      eventRecords_[nextReplayEvent_].event.frameState->frame->setPosition(
          eventRecords_[nextReplayEvent_]
              .event.frameState->state.translation());
      eventRecords_[nextReplayEvent_].event.frameState->frame->setOrientation(
          eventRecords_[nextReplayEvent_].event.frameState->state.rotation());
      qglviewer()->updateGL();
      break;
    case QEvent::User:
      Q_EMIT replayCustomEvent(
          (int)(eventRecords_[nextReplayEvent_].event.frameState));
      break;
    default:
      qWarning("Unknown event type " +
               QString::number(eventRecords_[nextReplayEvent_].type));
      break;
    }
    nextReplayEvent_++;
  }
}
