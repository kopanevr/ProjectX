/**
 * @file
 * @brief
 */

#pragma once

//

#include <memory>
#include <mutex>
#include <utility>

//

#include "TerminalPrinter.hpp"

//

/// @brief
class Logger final {
public:
  /// @brief Деструктор.
  ~Logger() = default;

  static Logger *getInstance() {
    if (!instance_) {
      instance_.reset(new (std::nothrow) Logger());
    }
    return instance_.get();
  }

  /// @brief
  /// @param args Данные для вывода.
  template <typename... Args> void log(Args &&...args) const {
    if (!terminalPrinter_) {
      terminalPrinter_->print(std::forward<Args>(args)...);
    }
  }

private:
  /// @brief Конструктор.
  Logger() {
    terminalPrinter_.reset(new (std::nothrow) TerminalPrinter());
    if (!terminalPrinter_) {
      //
    }
  }

  Logger &operator=(const Logger &) = delete;
  Logger(const Logger &) = delete;

private:
  /// @brief
  static std::unique_ptr<Logger> instance_;

  /// @brief
  std::unique_ptr<TerminalPrinter> terminalPrinter_;

};

#define LOG(...) Logger::getInstance()->log(__VA_ARGS__)

#if defined(__cplusplus) && __cplusplus >= 202002L

#ifndef NDEBUG
#define DEBUG(...) LOG("[ОТЛАДКА] " __VA_OPT__(,) __VA_ARGS__)
#else
#define DEBUG(...) ((void)0)
#endif

#define INFO(...) LOG("[ИНФО] " __VA_OPT__(,) __VA_ARGS__)
#define WARNING(...) LOG("[ВНИМАНИЕ] " __VA_OPT__(,) __VA_ARGS__)
#define ERROR(...) LOG("[ОШИБКА] " __VA_OPT__(,) __VA_ARGS__)

#define SEPARATOR LOG("------")

#else

#define DEBUG(...) ((void)0)

#define INFO(...) ((void)0)
#define WARNING(...) ((void)0)
#define ERROR(...) ((void)0)

#define SEPARATOR ((void)0)

#endif
