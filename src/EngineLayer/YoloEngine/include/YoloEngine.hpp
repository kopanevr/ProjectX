/**
 * @file
 * @brief
 */

#pragma once

//

/// @brief
class IYoloEngine {
public:
  /// @brief Деструктор.
  virtual ~IYoloEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize() = 0;

private:

};

/// @brief
class YoloEngine final : public IYoloEngine {
public:
  /// @brief Деструктор.
  ~YoloEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize() {}

private:

};
