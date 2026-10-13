/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>

//

/// @brief
class DataWriterEngine {
public:
  /// @brief Деструктор.
  virtual ~DataWriterEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize() = 0;

protected:
  /// @brief
  const std::string layerName_ = "[Data Writer Engine]";

private:

};