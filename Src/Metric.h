/**
 * @file Metric.h
 *
 * This file defines the functions that calculate the score for a given robot setup, whistle location and robot reply.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include "Util/Angle.h"
#include "Util/Pose2D.h"
#include "Util/Vector2D.h"
#include <QVector>
#include <algorithm>
#include <cmath>

class Metric
{
public:
  /**
   * This function calculates the overall score for a single attempt.
   * @param robotSetup The poses of the used robots on the field.
   * @param actualWhistleLocation The ground-truth position of the whistle in field coordinates.
   * @param whistle The whistle reported by the robots.
   * @return The numeric score for this attempt.
   */
  static float calculateScore(const QVector<Pose2D>& robotSetup, const Vector2D& actualWhistleLocation, const DetectedWhistle& whistle)
  {
    const Pose2D& referencePose = determineReferencePose(robotSetup, actualWhistleLocation);
    return calculateOnSameFieldDecisionScore(actualWhistleLocation, whistle) +
           calculateDirectionScore(referencePose, actualWhistleLocation, whistle) +
           calculateDistanceScore(referencePose, actualWhistleLocation, whistle);
  }

private:
  /**
   * This function determines the pose of the robot that is closest to the actual whistle location.
   * @param robotSetup The poses of the used robots on the field.
   * @param actualWhistleLocation The ground-truth position of the whistle in field coordinates.
   * @return A reference to the pose of the robot that is closest to the actual whistle location.
   */
  static const Pose2D& determineReferencePose(const QVector<Pose2D>& robotSetup, const Vector2D& actualWhistleLocation)
  {
    return *std::min_element(robotSetup.begin(), robotSetup.end(), [&actualWhistleLocation](const Pose2D& p1, const Pose2D& p2) { return (p1.translation - actualWhistleLocation).squaredNorm() < (p2.translation - actualWhistleLocation).squaredNorm(); });
  }

  /**
   * This function calculates the score resulting from the "same field"/"other field" decision.
   * @param actualWhistleLocation The ground-truth position of the whistle in field coordinates.
   * @param whistle The whistle reported by the robots.
   * @param 1 if the decision is correct, 0 if it is wrong.
   */
  static float calculateOnSameFieldDecisionScore(const Vector2D& actualWhistleLocation, const DetectedWhistle& whistle)
  {
    const bool isActuallyOnSameField = std::abs(actualWhistleLocation.x) < 5.2f && std::abs(actualWhistleLocation.y) < 3.7f;
    return (isActuallyOnSameField == whistle.onSameField) ? 1.f : 0.f;
  }

  /**
   * This function calculates the score resulting from the direction quality.
   * @param referencePose The reference pose for the score calculation in field coordinates.
   * @param actualWhistleLocation The ground-truth position of the whistle in field coordinates.
   * @param whistle The whistle reported by the robots.
   * @return A number in [0, 1] that represents how close the reported direction is to the actual direction.
   */
  static float calculateDirectionScore(const Pose2D& referencePose, const Vector2D& actualWhistleLocation, const DetectedWhistle& whistle)
  {
    static constexpr float minDeviation = 5.f, maxDeviation = 30.f;

    const float actualAngle = (actualWhistleLocation - referencePose.translation).angle();
    const float reportedAngle = (whistle.location - referencePose.translation).angle();
    const float deviation = std::abs(Angle::normalize(actualAngle - reportedAngle)) * 180.f / Angle::pi;
    return 1.f - std::max(0.f, std::min((deviation - minDeviation) / (maxDeviation - minDeviation), 1.f));
  }

  /**
   * This function calculates the score resulting from the distance quality.
   * @param referencePose The reference pose for the score calculation in field coordinates.
   * @param actualWhistleLocation The ground-truth position of the whistle in field coordinates.
   * @param whistle The whistle reported by the robots.
   * @param A number in [0, 1] that represents how close reported distance is to the actual distance.
   */
  static float calculateDistanceScore(const Pose2D& referencePose, const Vector2D& actualWhistleLocation, const DetectedWhistle& whistle)
  {
    static constexpr float minDeviation = 5.f, maxDeviation = 30.f;

    const float actualDistance = (actualWhistleLocation - referencePose.translation).norm();
    const float reportedDistance = (whistle.location - referencePose.translation).norm();
    const float deviation = std::abs(reportedDistance - actualDistance) / actualDistance * 100.f;
    return 1.f - std::max(0.f, std::min((deviation - minDeviation) / (maxDeviation - minDeviation), 1.f));
  }
};
