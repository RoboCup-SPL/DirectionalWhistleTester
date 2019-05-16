/**
 * @file TeamList.h
 *
 * This file declares a class that represents the list of all participating teams.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include <QMap>
#include <QString>

class TeamList
{
public:
  /**
   * This function returns the instance of the team list.
   * @return A reference to the instance of the team list.
   */
  static TeamList& getInstance();

  /**
   * This method returns an alphabetically sorted list of all team names.
   * @return A list of all team names.
   */
  QStringList getTeamNames() const;

  /**
   * This method returns the number of a team with a given name.
   * @param name The name of the team to get the number for.
   * @return The number of that team.
   */
  unsigned int getTeamNumberByName(const QString& name) const;

  TeamList(const TeamList&) = delete;
  void operator=(const TeamList&) = delete;

private:
  /** Constructor. Reads in the list of teams. */
  TeamList();

  QMap<QString, unsigned int> teams; /**< A map from team names to team numbers. */
};
