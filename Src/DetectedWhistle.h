/**
 * @file DetectedWhistle.h
 *
 * This file declares a struct that represents a whistle report of a robot currently doing the challenge.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include "Util/Vector2D.h"

struct DetectedWhistle
{
  bool onSameField = false; /**< Whether the whistle has been blown on the same field as the one on which the robots are. */
  Vector2D location; /**< The location where the whistle has been blown relative to the center of the field on which the robots are (in meters). */
};
