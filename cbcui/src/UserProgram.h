#ifndef __USER_PROGRAM_H__
#define __USER_PROGRAM_H__

#include <QObject>
#include <QProcess>
#include <QString>

class UserProgram : public QObject
{
Q_OBJECT

public:

  static UserProgram *instance();
  QString getProgramName() { return programName; }
  bool isRunning();

public slots:
  void toggleState();
  void start();
  void stop();
  void updateProgramName();
  void compileFinished(int eCode, QProcess::ExitStatus eStatus);
  
signals:
  void started();
  void stopped();
  void consoleOutput(QString text);
  void stateChange(int state);

protected:
  UserProgram();
  ~UserProgram();
  
private slots:
  void readStdout();
  void readStderr();
  void programFinished(int exitCode, QProcess::ExitStatus exitStatus);
  void programStarted();

private:
  QProcess m_userProgram;
  QString programName;
  bool state;
};

#endif
