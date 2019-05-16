/**
 * @file MainWindow.h
 *
 * This file declares the main window class of the program.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include "Util/Pose2D.h"
#include "Util/Vector2D.h"
#include <QMainWindow>
#include <QVector>

class Challenge;
class SPLStandardMessageReceiver;
class QPushButton;
class QTableView;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  /**
   * Constructor. Creates sub-widgets and connects their signals and slots.
   * @param parent The Qt parent widget.
   */
  explicit MainWindow(QWidget* parent = nullptr);

private:
  QPushButton* challengeStartButton = nullptr; /**< A button that starts a challenge pass. */
  QPushButton* attemptStartButton = nullptr; /**< A button that starts an attempt within a challenge pass. */
  QTableView* challengeView = nullptr; /**< A table view that displays the results of the challenge. */
  Challenge* challenge = nullptr; /**< The currently running challenge pass. */
  SPLStandardMessageReceiver* receiver = nullptr; /**< The receiver for SPL messages for the currently running challenge pass. */
  QVector<Vector2D> whistleLocations; /**< The set of locations from which the whistle is blown. */
  QVector<Pose2D> robotPoses; /**< The set of poses at which robots can be placed. */
};
