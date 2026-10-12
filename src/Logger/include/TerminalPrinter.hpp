#pragma once

//

#include <mutex>

//

/// @brief
class TerminalPrinter final {
public:
  TerminalPrinter() = default;
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
  std::mutex mutex_;

};
