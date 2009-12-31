#include "UserProgram.h"
#include <QFileInfo>


UserProgram::UserProgram()
{
  QObject::connect(&m_userProgram, SIGNAL(started()), this, SLOT(programStarted()));
  QObject::connect(&m_userProgram, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(programFinished(int, QProcess::ExitStatus)));
  QObject::connect(&m_userProgram, SIGNAL(readyReadStandardError()), this, SLOT(readStderr()));
  QObject::connect(&m_userProgram, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdout()));
}

UserProgram::~UserProgram()
{
}

UserProgram *UserProgram::instance()
{
  static UserProgram user_program;
  
  return &user_program;
}

void UserProgram::toggleState()
{
  QFileInfo robot("/mnt/user/bin/robot");
  
  if(m_userProgram.state() == QProcess::NotRunning) {
    if(robot.exists()) {
      m_userProgram.start("/mnt/user/bin/robot");
    }
    else {
      emit consoleOutput(QString("No program to run!\n"));
    }
  }
  else {
    emit consoleOutput(QString("Stopping program..\n"));
    m_userProgram.kill();
    emit stopped();
  }
}

void UserProgram::readStdout()
{
  emit consoleOutput(QString(m_userProgram.readAllStandardOutput()));
}

void UserProgram::readStderr()
{
  emit consoleOutput(QString(m_userProgram.readAllStandardError()));
}

void UserProgram::programStarted()
{
  emit stateChange(1);
  emit started();
}

void UserProgram::programFinished(int, QProcess::ExitStatus)
{
  emit stateChange(0);
  emit stopped();
}

void UserProgram::start()
{
  if(m_userProgram.state() == QProcess::NotRunning) {
    toggleState();
  }
}

void UserProgram::stop()
{
  if(m_userProgram.state() != QProcess::NotRunning) {
    toggleState();
  }
}

