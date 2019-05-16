/**
 * @file Challenge.cpp
 *
 * This file implements a class that represents and handles a pass of the challenge.
 *
 * @author Arne Hasselbring
 */

#include "Challenge.h"
#include "ChallengeLog.h"
#include "DetectedWhistle.h"
#include "Metric.h"
#include <QTime>
#include <QTimer>
#include <algorithm>
#include <random>

Challenge::Challenge(const QVector<Vector2D>& whistleLocations, const QVector<Pose2D>& robotSetup, QObject* parent) :
  QAbstractTableModel(parent),
  whistleLocations(whistleLocations),
  robotSetup(robotSetup)
{
  timer = new QTimer(this);
  timer->setSingleShot(true);
  connect(timer, &QTimer::timeout, this, &Challenge::finishAttempt);

  attempts.resize(whistleLocations.size());
  for(int i = 0; i < attempts.size(); ++i)
    attempts[i].locationIndex = i;

  if(shuffleWhistleLocations)
    std::shuffle(attempts.begin(), attempts.end(), std::default_random_engine(QTime::currentTime().msecsSinceStartOfDay()));
}

bool Challenge::isFinished() const
{
  return nextAttempt == attempts.size();
}

float Challenge::getTotalScore() const
{
  return std::accumulate(attempts.begin(), attempts.end(), 0.f, [](float score, const Attempt& attempt){ return score + attempt.score; });
}

void Challenge::startAttempt()
{
  Q_ASSERT(!attemptRunning);
  Q_ASSERT(nextAttempt < attempts.size());

  ChallengeLog() << "Started attempt " << (nextAttempt + 1) << " from location " << (attempts[nextAttempt].locationIndex + 1);

  timer->start(attemptTimeLimit);
  attemptRunning = true;
}

void Challenge::handleWhistleLocation(const DetectedWhistle& whistle)
{
  if(!attemptRunning)
    return;

  attempts[nextAttempt].remainingTime = timer->remainingTime();
  timer->stop();
  attempts[nextAttempt].whistle = whistle;
  attempts[nextAttempt].score = Metric::calculateScore(robotSetup, whistleLocations[attempts[nextAttempt].locationIndex], whistle);

  finishAttempt();
}

void Challenge::finishAttempt()
{
  if(!attemptRunning)
    return;

  const bool timedOut = attempts[nextAttempt].remainingTime == -1;
  ChallengeLog() << "Finished attempt " << (nextAttempt + 1) << " from location " << (attempts[nextAttempt].locationIndex + 1) << (timedOut ? " (timed out)" : ":");
  if(!timedOut)
  {
    ChallengeLog() << "  Remaining time: " << attempts[nextAttempt].remainingTime << "ms";
    ChallengeLog() << "  Actual location: " << whistleLocations[attempts[nextAttempt].locationIndex].x << ", " << whistleLocations[attempts[nextAttempt].locationIndex].y;
    ChallengeLog() << "  Reported location: " << attempts[nextAttempt].whistle.location.x << ", " << attempts[nextAttempt].whistle.location.y;
    ChallengeLog() << "  Reported field: " << (attempts[nextAttempt].whistle.onSameField ? "same" : "other");
    ChallengeLog() << "  Score: " << attempts[nextAttempt].score;
  }

  attemptRunning = false;
  emit dataChanged(index(nextAttempt, firstDynamicColumn), index(nextAttempt, numOfColumns - 1), {Qt::DisplayRole});
  emit dataChanged(index(attempts.size(), score), index(attempts.size(), score), {Qt::DisplayRole});
  ++nextAttempt;
  emit attemptFinished();
}

int Challenge::rowCount(const QModelIndex&) const
{
  return attempts.size() + 1;
}

int Challenge::columnCount(const QModelIndex&) const
{
  return numOfColumns;
}

QVariant Challenge::data(const QModelIndex& index, int role) const
{
  if(role != Qt::DisplayRole)
    return QVariant();

  if(index.row() == attempts.size())
  {
    switch(index.column())
    {
      case score:
        return getTotalScore();
    }
  }
  else if(index.column() < firstDynamicColumn)
  {
    switch(index.column())
    {
      case locationIndex:
        return attempts[index.row()].locationIndex + 1;
    }
  }
  else if(index.row() < nextAttempt)
  {
    const bool timedOut = attempts[index.row()].remainingTime == -1;
    if(timedOut)
      return "-";

    switch(index.column())
    {
      case remainingTime:
        return attempts[index.row()].remainingTime;
      case score:
        return attempts[index.row()].score;
    }
  }

  return QVariant();
}

QVariant Challenge::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role != Qt::DisplayRole)
    return QVariant();

  if(orientation == Qt::Horizontal)
  {
    switch(section)
    {
      case locationIndex:
        return "Location";
      case remainingTime:
        return "Remaining Time";
      case score:
        return "Score";
      default:
        break;
    }
  }
  else
  {
    if(section == attempts.size())
      return "Overall";
    else
      return section + 1;
  }
  return QVariant();
}
