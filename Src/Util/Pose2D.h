/**
 * @file Pose2D.h
 *
 * This file declares a struct that represents a pose (translation+rotation) in the 2D space.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include "Vector2D.h"

struct Pose2D
{
  /** Constructor of a zero pose. */
  Pose2D() : rotation(0.f) {}
  /** Constructor with a rotation, an x and a y coordinate as parameters. */
  Pose2D(float rotation, float x, float y) : rotation(rotation), translation(x, y) {}
  /** Constructor with a rotation and a translation vector as parameters. */
  Pose2D(float rotation, const Vector2D& translation) : rotation(rotation), translation(translation) {}

  float rotation; /**< The rotation of the pose (must be normalized to [-pi,pi[). */
  Vector2D translation; /**< The translation of the pose. */
};

