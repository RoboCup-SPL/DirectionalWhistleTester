/**
 * @file MainWindow.cpp
 *
 * This file implements the main window class of the program.
 *
 * @author Arne Hasselbring
 */

#include "MainWindow.h"
#include "Challenge.h"
#include "ChallengeLog.h"
#include "ChallengeStartDialog.h"
#include "SPLStandardMessageReceiver.h"
#include "TeamList.h"
#include "Util/Paths.h"
#include "Util/Reader.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget* parent) :
  QMainWindow(parent)
{
  ChallengeLog() << "Started DirectionWhistleTester";
  Reader::readPose2DList(Paths::getConfigPath() + "/robotPoses.json", robotPoses);
  Reader::readVector2DList(Paths::getConfigPath() + "/whistleLocations.json", whistleLocations);

  auto* centralWidget = new QWidget(this);

  challengeStartButton = new QPushButton("&Start Challenge...", this);
  attemptStartButton = new QPushButton("Start &Attempt", this);
  attemptStartButton->setEnabled(false);

  challengeView = new QTableView(this);
  challengeView->setCornerButtonEnabled(false);
  challengeView->setSelectionMode(QAbstractItemView::NoSelection);
  challengeView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  challengeView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  challengeView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  challengeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  challengeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  connect(challengeStartButton, &QPushButton::clicked, this, [this]
  {
    if(challenge && !challenge->isFinished())
    {
      switch(QMessageBox::warning(this, "Warning", "A challenge pass is already running.\nAre you sure to start a new pass?", QMessageBox::Yes | QMessageBox::No))
      {
        case QMessageBox::Yes:
          break;
        case QMessageBox::No:
        default:
          return;
      }
    }

    ChallengeStartDialog dialog(robotPoses.size(), this);
    if(dialog.exec() != QDialog::Accepted)
      return;

    delete receiver;
    delete challenge;

    ChallengeLog() << "Started challenge pass of team " << dialog.getTeamName() << " with robots " << dialog.getRobotNumbers();

    QVector<Pose2D> robotSetup;
    for(unsigned int jerseyNumber : dialog.getRobotNumbers())
      robotSetup.append(robotPoses[jerseyNumber - 1]);

    receiver = new SPLStandardMessageReceiver(TeamList::getInstance().getTeamNumberByName(dialog.getTeamName()), this);
    challenge = new Challenge(whistleLocations, robotSetup, this);
    challengeView->setModel(challenge);
    challengeView->setFixedSize(challengeView->horizontalHeader()->length() + challengeView->verticalHeader()->width(),
                                challengeView->verticalHeader()->length() + challengeView->horizontalHeader()->height());

    const QString teamName = dialog.getTeamName();
    connect(receiver, &SPLStandardMessageReceiver::whistleLocationReceived, challenge, &Challenge::handleWhistleLocation);
    connect(attemptStartButton, &QPushButton::clicked, challenge, &Challenge::startAttempt);
    connect(challenge, &Challenge::attemptFinished, this, [this, teamName]
    {
      if(!challenge->isFinished())
        attemptStartButton->setEnabled(true);
      else
        ChallengeLog() << "Finished challenge pass of team " << teamName << " with final score " << challenge->getTotalScore();
    });
    attemptStartButton->setEnabled(true);
  });

  connect(attemptStartButton, &QPushButton::clicked, this, [this]
  {
    attemptStartButton->setEnabled(false);
  });

  auto* buttonLayout = new QVBoxLayout;
  buttonLayout->setSpacing(20);
  buttonLayout->addWidget(challengeStartButton);
  buttonLayout->addWidget(attemptStartButton);
  buttonLayout->addStretch();

  auto* layout = new QHBoxLayout(centralWidget);
  layout->addLayout(buttonLayout);
  layout->addWidget(challengeView);

  setCentralWidget(centralWidget);
}
