/**
 * @file Challenge.h
 *
 * This file declares a class that represents and handles a pass of the challenge.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include "DetectedWhistle.h"
#include "Util/Pose2D.h"
#include "Util/Vector2D.h"
#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVariant>
#include <QVector>

struct DetectedWhistle;
class QObject;
class QTimer;

class Challenge : public QAbstractTableModel
{
  Q_OBJECT
public:
  /**
   * Constructor.
   * @param whistleLocations The set of locations from which the whistle is blown.
   * @param robotSetup The set of poses of the robots that participate in this challenge.
   * @param parent The Qt parent object.
   */
  Challenge(const QVector<Vector2D>& whistleLocations, const QVector<Pose2D>& robotSetup, QObject* parent = nullptr);

  /**
   * Returns whether the challenge pass is finished (i.e. all whistle locations have been done).
   * @return Whether the challenge pass is finished (i.e. all whistle locations have been done).
   */
  bool isFinished() const;

  /**
   * Returns the total score of this challenge pass.
   * @return The total score of this challenge pass.
   */
  float getTotalScore() const;

signals:
  /** This signal is emitted when an attempt is finished (whether it is by a received message or timeout). */
  void attemptFinished();

public slots:
  /** This method starts the next attempt (assuming that the challenge is not finished yet). */
  void startAttempt();

  /**
   * This method assigns a whistle to the currently running attempt (if there is one) and finishes it.
   * @param whistle The whistle reported by the robots.
   */
  void handleWhistleLocation(const DetectedWhistle& whistle);

private slots:
  /** This method finishes a currently running attempt (if there is one). */
  void finishAttempt();

private:
  static constexpr bool shuffleWhistleLocations = true; /**< Whether the order of whistle locations should be shuffled for each challenge pass. */
  static constexpr int attemptTimeLimit = 5000; /**< The amount of time (ms) that the team has to react to the whistle. */

  struct Attempt
  {
    int locationIndex = -1; /**< The index in the whistle location array that this attempt corresponds to. */
    int remainingTime = -1; /**< The time that was remaining when the whistle message arrived (-1=timeout). */
    DetectedWhistle whistle; /**< The whistle response that the team gave. */
    float score = 0.f; /**< The overall score for this attempt. */
  };

  enum Column
  {
    locationIndex,
    firstDynamicColumn,
    remainingTime = firstDynamicColumn,
    score,
    numOfColumns
  };

  /**
   * Returns the number of rows of the table.
   * @param parent The parent index (ignored since this is a table).
   * @return The number of rows of the table.
   */
  int rowCount(const QModelIndex& parent) const override;

  /**
   * Returns the number of columns of the table.
   * @param parent The parent index (ignored since this is a table).
   * @return The number of columns of the table.
   */
  int columnCount(const QModelIndex& parent) const override;

  /**
   * Gets the data for a certain index and role.
   * @param index The index for which to get the data.
   * @param role The role for which to get the data (only the display role is used).
   * @return The data for this index.
   */
  QVariant data(const QModelIndex& index, int role) const override;

  /**
   * Gets the header data for a certain section, orientation and role.
   * @param section The section (i.e. column index) for which to get the header data.
   * @param orientation The orientation for which to get the data (horizontal or vertical).
   * @param role The role for which to get the header data (only the display role is used).
   * @return The header data for this section and orientation.
   */
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  QTimer* timer = nullptr; /**< The timer that handles the time limit per attempt. */
  int nextAttempt = 0; /**< The index of the next/current attempt. */
  bool attemptRunning = false; /**< Whether an attempt is currently running (if it is, it has the index \c nextAttempt). */
  const QVector<Vector2D>& whistleLocations; /**< The set of locations from which the whistle is blown. */
  QVector<Pose2D> robotSetup; /**< The set of poses of the robots that participate in this challenge. */
  QVector<Attempt> attempts; /**< The list of all attempts in this challenge pass (one per whistle location). */
};
