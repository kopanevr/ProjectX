/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>

//

/// @brief
class ILogWriter {
public:
  /// @brief Деструктор.
  virtual ~ILogWriter() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize() = 0;

protected:

private:

};

/// @brief
class LogWriter final : public ILogWriter {
public:
  /// @brief Деструктор.
  ~LogWriter() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  bool initialize() override {
    return {};
  }

protected:

private:

};
