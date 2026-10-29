/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>
#include <filesystem>

//

#include "Data.hpp"

//

/// @brief
class ILogWriter {
public:
  /// @brief Деструктор.
  virtual ~ILogWriter() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize() = 0;

  /// @brief
  /// @param log
  virtual void run(const Log &log) = 0;

protected:
  /// @brief
  const std::string layerName_ = "[LogWriter]";

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
    return true;
  }

  /// @brief
  /// @param log
  void run(const Log &log) override;

protected:

private:
  static inline const std::filesystem::path LOG_FILE_DIR = "/tmp/blueberry_logs";
  static inline const std::filesystem::path LOG_FILE_PATH = LOG_FILE_DIR / "customer_counter_raw.blueberry_retail.log";
  static inline const std::filesystem::path TEMP_LOG_FILE_PATH = std::filesystem::path(LOG_FILE_PATH) += ".tmp";

};
