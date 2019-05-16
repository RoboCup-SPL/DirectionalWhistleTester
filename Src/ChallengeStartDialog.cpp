/**
 * @file ChallengeStartDialog.cpp
 *
 * This file implements a class that asks the user to specify the team and the robots used in the challenge.
 *
 * @author Arne Hasselbring
 */

#include "ChallengeStartDialog.h"
#include "TeamList.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

ChallengeStartDialog::ChallengeStartDialog(int numOfRobots, QWidget* parent) :
  QDialog(parent)
{
  auto* layout = new QVBoxLayout(this);

  auto* teamLabel = new QLabel("Team:", this);
  layout->addWidget(teamLabel);

  teamComboBox = new QComboBox(this);
  teamComboBox->addItems(TeamList::getInstance().getTeamNames());
  teamLabel->setBuddy(teamComboBox);
  layout->addWidget(teamComboBox);

  auto* robotsLabel = new QLabel("Participating Robots:", this);
  layout->addWidget(robotsLabel);

  auto* startButton = new QPushButton("Start", this);
  connect(startButton, &QPushButton::clicked, this, &QDialog::accept);
  startButton->setEnabled(false);

  auto checkSelectedRobots = [this, startButton]
  {
    startButton->setEnabled(std::any_of(robotCheckBoxes.begin(), robotCheckBoxes.end(), [](QCheckBox* checkBox){ return checkBox->isChecked(); }));
  };

  for(int i = 0; i < numOfRobots; ++i)
  {
    robotCheckBoxes.append(new QCheckBox("&" + QString::number(i + 1), this));
    connect(robotCheckBoxes.last(), &QCheckBox::stateChanged, this, checkSelectedRobots);
    layout->addWidget(robotCheckBoxes.last());
  }

  layout->addWidget(startButton);

  setLayout(layout);
  setWindowTitle("Start Challenge");
}

QString ChallengeStartDialog::getTeamName() const
{
  return teamComboBox->currentText();
}

QVector<unsigned int> ChallengeStartDialog::getRobotNumbers() const
{
  QVector<unsigned int> result;
  for(int i = 0; i < robotCheckBoxes.size(); ++i)
    if(robotCheckBoxes[i]->isChecked())
      result.append(i + 1);
  return result;
}
