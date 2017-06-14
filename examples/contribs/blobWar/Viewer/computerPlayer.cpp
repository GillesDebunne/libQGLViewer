#include "computerPlayer.h"
#include "qlineedit.h"
#include "qprocess.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include <qfiledialog.h>
#include <qmessagebox.h>

#if QT_VERSION >= 0x040000
#include "ui_computerPlayerInterface.h"
#include <QTime>
class ComputerPlayerInterface : public QDialog,
                                public Ui::ComputerPlayerInterface {
public:
  ComputerPlayerInterface() { setupUi(this); }
};
#else
#include "qdatetime.h"
#if QT_VERSION >= 0x030000
#include "computerPlayerInterface.Qt3.h"
#else
#error "No designer .ui file available for Qt 2"
#endif
#endif

static QTime Clock;

ComputerPlayer::ComputerPlayer() : isActive_(false) {
  interface_ = new ComputerPlayerInterface();

  connect(interface_->browseButton, SIGNAL(released()), this,
          SLOT(selectProgram()));
  interface_->hide();

  setAllowedTime(3);
}

ComputerPlayer::~ComputerPlayer() { delete interface_; }

void ComputerPlayer::selectProgram() {
#if QT_VERSION < 0x040000
  QString fileName = QFileDialog::getOpenFileName(
      programFileName(), "Computer programs (*)", NULL);
#else
  QString fileName =
      QFileDialog::getOpenFileName(NULL, "Select a computer program",
                                   programFileName(), "Computer programs (*)");
#endif

  if (!fileName.isEmpty())
    setProgramFileName(fileName);
}

void ComputerPlayer::setIsActive(bool on) {
  if (on && (programFileName().isEmpty()))
    configure();
  isActive_ = on;
}

void ComputerPlayer::configure() {
  int previousAllowedTime = allowedTime();
  QString previousProgramFileName = programFileName();

  if (interface_->exec() == QDialog::Rejected) {
    setAllowedTime(previousAllowedTime);
    setProgramFileName(previousProgramFileName);
  }
}

int ComputerPlayer::allowedTime() const {
  return interface_->allowedTimeSpinBox->value();
}

void ComputerPlayer::setAllowedTime(int time) {
  interface_->allowedTimeSpinBox->setValue(time);
}

QString ComputerPlayer::programFileName() const {
  return interface_->programNameLineEdit->text();
}

void ComputerPlayer::setProgramFileName(const QString &name) {
  interface_->programNameLineEdit->setText(name);
}

void ComputerPlayer::play(bool blue, const QString &stateFileName) {
  if (!isActive_)
    return; // So that human user can play

  while (true) {
    const QFileInfo fi(programFileName());

    if (fi.exists())
      if (fi.isExecutable()) {
#if QT_VERSION < 0x040000
        process_ = new QProcess(programFileName());
        process_->addArgument(stateFileName);
        process_->addArgument(
            QString::number(blue ? allowedTime() : -allowedTime()));
        connect(process_, SIGNAL(processExited()), this,
                SLOT(readFromStdout()));
        if (process_->start())
#else
        process_ = new QProcess();
        connect(process_, SIGNAL(finished(int)), this, SLOT(readFromStdout()));
        process_->start(programFileName(),
                        QStringList()
                            << stateFileName
                            << QString::number(blue ? allowedTime()
                                                    : -allowedTime()));
        if (process_->waitForStarted())
#endif
        {
          Clock.start();
          break;
        } else
          QMessageBox::warning(NULL, "Unable to start process",
                               "Unable to start process.\nSelect another "
                               "program or update permissions");
      } else
        QMessageBox::warning(NULL, "Non executable program file",
                             "Program file cannot be executed.\nSelect another "
                             "program or update permissions.");
    else
      QMessageBox::warning(
          NULL, "Program file not found",
          "Program file does not exist.\nSelect another program.");

    configure();
  }
}

void ComputerPlayer::readFromStdout() {
  int duration = Clock.elapsed();
#if QT_VERSION < 0x040000
  QString result = process_->readLineStdout();
#else
  QString result = QString((process_->readAllStandardOutput()).trimmed());
#endif
  Q_EMIT moveMade(result, duration);
#if QT_VERSION < 0x040000
  delete process_;
#else
  process_->deleteLater();
#endif
}
