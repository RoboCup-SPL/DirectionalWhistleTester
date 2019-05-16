/**
 * @file TeamList.cpp
 *
 * This file implements a class that represents the list of all participating teams.
 *
 * @author Arne Hasselbring
 */

#include "TeamList.h"
#include "Util/Paths.h"
#include <QFile>
#include <QTextStream>

TeamList::TeamList()
{
  QFile file(Paths::getConfigPath() + "/teams.cfg");
  Q_ASSERT(file.exists());
  file.open(QIODevice::ReadOnly | QIODevice::Text);

  QTextStream in(&file);
  bool ok;
  while(!in.atEnd())
  {
    const QString line = in.readLine();
    const int equalsIndex = line.indexOf('=');
    Q_ASSERT(equalsIndex > 0);
    const QString teamNumberPart = line.left(equalsIndex);
    const int teamNumber = teamNumberPart.toInt(&ok);
    Q_ASSERT(ok);
    Q_ASSERT(teamNumber >= 0);

    teams.insert(line.mid(equalsIndex + 1, line.indexOf(',') - equalsIndex - 1).trimmed(), static_cast<unsigned int>(teamNumber));
  }
}

QStringList TeamList::getTeamNames() const
{
  return teams.keys();
}

unsigned int TeamList::getTeamNumberByName(const QString& name) const
{
  return teams[name];
}

TeamList& TeamList::getInstance()
{
  static TeamList instance;
  return instance;
}
