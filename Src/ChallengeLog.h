/**
 * @file ChallengeLog.h
 *
 * This file declares a class that logs lines of text to a log file.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include "Util/Paths.h"
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#ifdef __unix__
#include <unistd.h>
#endif

class ChallengeLog : public QTextStream
{
public:
  /** Constructor. Writes a timestamp to the stream. */
  ChallengeLog() :
    QTextStream(getLogFile())
  {
    *this << QDateTime::currentDateTime().toString(Qt::ISODate)  << ": ";
  }

  /** Destructor. Ends the line, flushes the stream and (on Unices) syncs the file to the device (in case of power loss). */
  ~ChallengeLog() override
  {
    *this << endl;
    flush();
#ifdef __unix__
    fsync(getLogFile()->handle());
#endif
  }

private:
  /**
   * Returns a pointer to the log file handle.
   * @return A pointer to the log file handle.
   */
  static QFile* getLogFile()
  {
    static QFile f(Paths::getLogPath() + "/log_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss") + ".txt");
    if(!f.isOpen())
      f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered);
    return &f;
  }
};

static inline QTextStream& operator<<(QTextStream& stream, const QVector<unsigned int>& vector)
{
  stream << "{";
  for(int i = 0; i < vector.size(); ++i)
  {
    if(i)
      stream << ", ";
    stream << vector[i];
  }
  stream << "}";
  return stream;
}
