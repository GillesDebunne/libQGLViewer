#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "qobject.h"
#include "qstring.h"

class ComputerPlayerInterface;
class QProcess;

class ComputerPlayer : public QObject {
  Q_OBJECT

public:
  ComputerPlayer();
  ~ComputerPlayer();

  bool isActive() const { return isActive_; }
  void setIsActive(bool on);

  int allowedTime() const;
  void setAllowedTime(int time);

  QString programFileName() const;
  void setProgramFileName(const QString &name);

  void configure();

  void play(bool blue, const QString &stateFileName);

public:
Q_SIGNALS:
  void moveMade(QString move, int duration);

private Q_SLOTS:
  void selectProgram();
  void readFromStdout();

private:
  bool isActive_;
  ComputerPlayerInterface *interface_;
  QProcess *process_;
};

#endif // COMPUTER_PLAYER_H
