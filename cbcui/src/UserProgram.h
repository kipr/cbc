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

public slots:
  void toggleState();
  void start();
  void stop();
  
signals:
  void started();
  void stopped();
  void consoleOutput(QString text);

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
};

#endif
