/**
 * @file Paths.h
 *
 * This file defines functions that get paths to relevant directories.
 *
 * @author Arne Hasselbring
 */

#pragma once

#include <QCoreApplication>

class Paths
{
public:
  /**
   * Returns the path to the directory in which configuration files are stored.
   * @return The path to the directory in which configuration files are stored.
   */
  static QString getConfigPath()
  {
    return QCoreApplication::applicationDirPath() + "/../Config";
  }

  /**
   * Returns the path to the directory in which logs should be saved.
   * @return The path to the directory in which logs should be saved.
   */
  static QString getLogPath()
  {
    return QCoreApplication::applicationDirPath() + "/../Logs";
  }
};
