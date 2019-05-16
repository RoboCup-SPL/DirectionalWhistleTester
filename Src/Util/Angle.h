/**
 * @file Angle.h
 *
 * This file defines functions to work with angles.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include <cmath>

namespace Angle
{
  constexpr float pi = 3.14159265358979f; /**< The circle constant. */
  constexpr float pi2 = 2.f * pi; /**< Twice the circle constant. */

  /**
   * Normalizes an angle to [-pi,pi[.
   * @param angle Any number that represents an angle.
   * @return A number that represents the same angle, but in the range [-pi,pi[.
   */
  inline float normalize(float angle)
  {
    if(angle >= -pi && angle < pi)
      return angle;
    else
    {
      angle -= static_cast<int>(angle / pi2) * pi2;
      return angle >= pi ? (angle - pi2) : angle < -pi ? (angle + pi2) : angle;
    }
  }
}
