#pragma once

//

#include <iostream>
#include <mutex>
#include <memory>

//

/// @brief
class TerminalPrinter final {
public:
  /// @brief
  TerminalPrinter() = default;
  /// @brief
  ~TerminalPrinter() = default;

  /// @brief
  /// @brief Выводит данные в терминал.
  /// @param args Данные для вывода.
  template <typename... Args> void print(Args &&...args) const {
    std::lock_guard<std::mutex> lock(mutex_);
    ((std::cout << std::forward<Args>(args)), ...);
    std::cout << std::endl;
  }

private:
  /// @brief
  mutable std::mutex mutex_;

};
