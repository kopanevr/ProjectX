/**
 * @file
 * @brief Описание таймера.
 */

#pragma once

//

#include <chrono>

//

using TimerID = uint8_t;

//

namespace timer {
using TimePoint = std::chrono::steady_clock::time_point;
} // namespace timer

//

/// @brief Таймер.
struct Timer final {
  Timer() = default;

  /// @brief Идентификатор таймера.
  TimerID id;

  timer::TimePoint start{};
  timer::TimePoint end{};

  /// @brief
  bool isStarted : 1 {false};
  /// @brief
  bool isDurationSet : 1 {false};

  /// @brief Конструктор.
  constexpr explicit Timer(const TimerID timerId) : id(timerId) {}
};
