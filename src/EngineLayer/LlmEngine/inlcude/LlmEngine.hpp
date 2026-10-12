/**
 * @file
 * @brief
 */

#pragma once

//

/// @brief
class ILlmEngine {
public:
  /// @brief Деструктор.
  virtual ~ILlmEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize() = 0;

private:

};
