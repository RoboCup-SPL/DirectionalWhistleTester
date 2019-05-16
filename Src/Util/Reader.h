/**
 * @file Reader.h
 *
 * This file defines functions to read certain datastructures from JSON files.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include "Angle.h"
#include "Pose2D.h"
#include "Vector2D.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVector>

class Reader
{
public:
  /**
   * This function reads a list of vectors from a JSON file.
   * @param path The path to the JSON file from which to load the vectors.
   * @param vectors The list of vectors that will be replaced by the list from the file.
   */
  static void readVector2DList(const QString& path, QVector<Vector2D>& vectors)
  {
    vectors.clear();

    QFile file(path);
    Q_ASSERT(file.exists());
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    const QByteArray data = file.readAll();
    const QJsonDocument document = QJsonDocument::fromJson(data);
    Q_ASSERT(document.isArray());

    const QJsonArray vectorArray = document.array();
    for(const QJsonValue& vector : vectorArray)
    {
      Q_ASSERT(vector.isObject());
      const QJsonObject vectorObject = vector.toObject();
      vectors.append(Vector2D(vectorObject["x"].toDouble(), vectorObject["y"].toDouble()));
    }
  }

  /**
   * This function reads a list of poses from a JSON file.
   * @param path The path to the JSON file from which to load the poses.
   * @param poses The list of poses that will be replaced by the list from the file.
   */
  static void readPose2DList(const QString& path, QVector<Pose2D>& poses)
  {
    poses.clear();

    QFile file(path);
    Q_ASSERT(file.exists());
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    const QByteArray data = file.readAll();
    const QJsonDocument document = QJsonDocument::fromJson(data);
    Q_ASSERT(document.isArray());

    const QJsonArray poseArray = document.array();
    for(const QJsonValue& pose : poseArray)
    {
      Q_ASSERT(pose.isObject());
      const QJsonObject poseObject = pose.toObject();
      poses.append(Pose2D(poseObject["rotation"].toDouble() * Angle::pi / 180.f, poseObject["x"].toDouble(), poseObject["y"].toDouble()));
    }
  }
};
