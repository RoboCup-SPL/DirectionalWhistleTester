/**
 * @file ChallengeStartDialog.h
 *
 * This file declares a class that asks the user to specify the team and the robots used in the challenge.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include <QDialog>
#include <QVector>

class QCheckBox;
class QComboBox;
class QWidget;

class ChallengeStartDialog : public QDialog
{
public:
  /**
   * Constructor.
   * @param numOfRobots The maximum number of robots that could be used.
   * @param parent The Qt parent widget.
   */
  explicit ChallengeStartDialog(int numOfRobots, QWidget* parent = nullptr);

  /**
   * Returns the name of the selected team.
   * @return The name of the selected team.
   */
  QString getTeamName() const;

  /**
   * Returns the list of jersey numbers of the robots that will participate in the challenge.
   * @return The list of jersey numbers of the robots that will participate in the challenge.
   */
  QVector<unsigned int> getRobotNumbers() const;

private:
  QComboBox* teamComboBox = nullptr; /**< A combo box to select the team will do the challenge. */
  QVector<QCheckBox*> robotCheckBoxes; /**< A list of checkboxes to select which robots will participate in the challenge. */
};
