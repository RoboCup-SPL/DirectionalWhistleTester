/**
 * @file Vector2D.h
 *
 * This file declares a struct that represents a vector in the 2D space.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include <cmath>

struct Vector2D
{
  /** Constructor of a zero vector. */
  Vector2D() : x(0.f), y(0.f) {}
  /** Constructor with the coordinates as parameters. */
  Vector2D(float x, float y) : x(x), y(y) {}

  /**
   * Subtracts another vector from this vector.
   * @param other The vector to subtract.
   * @return The resulting vector.
   */
  Vector2D operator-(const Vector2D& other) const
  {
    return {x - other.x, y - other.y};
  }

  /**
   * Calculates the length of this vector.
   * @return The length of this vector.
   */
  float norm() const
  {
    return std::sqrt(x * x + y * y);
  }

  /**
   * Calculates the squared length of this vector.
   * @return The squared length of this vector.
   */
  float squaredNorm() const
  {
    return x * x + y * y;
  }

  /**
   * Calculates the angle of this vector in polar coordinates.
   * @return An angle in the range [-pi,pi].
   */
  float angle() const
  {
    return std::atan2(y, x);
  }

  float x; /**< The x coordinate of the vector. */
  float y; /**< The y coordinate of the vector. */
};
