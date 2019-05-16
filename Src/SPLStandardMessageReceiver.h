/**
 * @file SPLStandardMessageReceiver.h
 *
 * This file declares a class that receives SPL standard messages from a specific team.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include "DetectedWhistle.h"
#include <QObject>

class QUdpSocket;

class SPLStandardMessageReceiver : public QObject
{
  Q_OBJECT
public:
  /**
   * Constructor. Creates and binds a socket and registers a message handler.
   * @param teamNumber The number of the team for which to receive messages.
   * @param parent The Qt parent object.
   */
  explicit SPLStandardMessageReceiver(unsigned int teamNumber, QObject* parent = nullptr);

signals:
  /**
   * This signal is emitted when a (complete and formally correct) whistle location has been received.
   * @param whistle The whistle reported by the robots.
   */
  void whistleLocationReceived(const DetectedWhistle& whistle);

private slots:
  /** Handles all received messages, checks them and emits signals for them. */
  void handleReceivedMessages();

private:
  static constexpr std::uint8_t specialSPLStandardMessageVersion = 255; /**< Messages meant for the tester must have this special version number. */

  QUdpSocket* socket; /**< The socket which receives messages. */
  unsigned int teamNumber; /**< The number of the team for which to receive messages. */
};
