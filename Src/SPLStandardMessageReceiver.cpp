/**
 * @file SPLStandardMessageReceiver.cpp
 *
 * This file implements a class that receives SPL standard messages from a specific team.
 *
 * @author Arne Hasselbring
 */

#include "SPLStandardMessageReceiver.h"
#include "SPLStandardMessage.h"
#include <QUdpSocket>
#include <cstddef>
#include <cstring>

SPLStandardMessageReceiver::SPLStandardMessageReceiver(unsigned int teamNumber, QObject* parent) :
  QObject(parent),
  teamNumber(teamNumber)
{
  Q_ASSERT(teamNumber < 100);

  socket = new QUdpSocket(this);
  socket->bind(QHostAddress::Any, static_cast<quint16>(10000 + teamNumber), QAbstractSocket::ReuseAddressHint);
  connect(socket, &QUdpSocket::readyRead, this, &SPLStandardMessageReceiver::handleReceivedMessages);
}

void SPLStandardMessageReceiver::handleReceivedMessages()
{
  while(socket->hasPendingDatagrams())
  {
    SPLStandardMessage message;
    const quint64 actualSize = std::max<qint64>(0, socket->readDatagram(reinterpret_cast<char*>(&message), sizeof(SPLStandardMessage)));

    // The usual sanity checks for an SPL standard message.
    if(actualSize < offsetof(SPLStandardMessage, data) || actualSize > sizeof(SPLStandardMessage))
    {
      qDebug().nospace() << "Receiving SPLStandardMessage failed!";
      break;
    }
    if(std::strncmp(message.header, SPL_STANDARD_MESSAGE_STRUCT_HEADER, sizeof(message.header)) != 0)
    {
      qDebug().nospace() << "SPLStandardMessage: Header mismatch!";
      break;
    }
    // A different version number does not indicate an error because it may be a message that is meant for other robots.
    // Still, it should be ignored.
    if(message.version != specialSPLStandardMessageVersion)
      break;
    if(message.playerNum < 1 || message.playerNum > 5)
    {
      qDebug().nospace() << "SPLStandardMessage: Player number must be in [1, 5] (is " << message.playerNum << ")!";
      break;
    }
    if(message.teamNum != teamNumber)
    {
      qDebug().nospace() << "SPLStandardMessage: Team number must be the correct one for this port (should be " << teamNumber << ", is " << message.teamNum << ")!";
      break;
    }
    if(message.numOfDataBytes > SPL_STANDARD_MESSAGE_DATA_SIZE || offsetof(SPLStandardMessage, data) + message.numOfDataBytes > actualSize)
    {
      qDebug().nospace() << "SPLStandardMessage: Illegal number of data bytes (is " << message.numOfDataBytes << ")!";
      break;
    }

    DetectedWhistle whistle;
    whistle.onSameField = message.fallen != 0;
    whistle.location = Vector2D(message.pose[0] / 1000.f, message.pose[1] / 1000.f);

    emit whistleLocationReceived(whistle);
  }
}
