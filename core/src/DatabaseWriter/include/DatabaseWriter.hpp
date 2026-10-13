/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>

//

/// @brief
class IDatabaseWriter {
public:
  /// @brief Деструктор.
  virtual ~IDatabaseWriter() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize() = 0;

protected:

private:

};

/// @brief
class DatabaseWriter final : public IDatabaseWriter {
public:
  /// @brief Деструктор.
  ~DatabaseWriter() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  bool initialize() override {
    return {};
  }

protected:

private:

};
