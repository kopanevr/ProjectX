/**
 * @file
 * @brief Описание менеджера таймера.
 */

#pragma once

//

#include <cassert>

//

#include <chrono>

//

#include "Timer.hpp"

//

using Duration = std::chrono::steady_clock::duration;

//

/// @brief Менеджер таймера.
class TimerManager final {
public:
  TimerManager() = default;
  ~TimerManager() = default;

  /// @brief Запуск таймера.
  /// @details Запуск таймера без установки периода времени отсчета.
  /// @param timer Таймер.
  void start(Timer &timer) {
    timer.start = std::chrono::steady_clock::now();
    timer.isStarted = true;
    timer.isDurationSet = false;
  }

  /// @brief Запуск таймера и возврат времени запуска.
  /// @details Запуск таймера без установки периода времени отсчета.
  /// @return Время запуска.
  /// @param timer Таймер.
  [[nodiscard]] std::chrono::steady_clock::time_point
  startAndGetStartTime(Timer &timer) {
    timer.start = std::chrono::steady_clock::now();
    timer.isStarted = true;
    timer.isDurationSet = false;
    return timer.start;
  }

  /// @brief Запуск таймера.
  /// @details Запуск таймера c установкой периода времени отсчета.
  /// @param timer Таймер.
  /// @param duration Период времени.
  void start(Timer &timer, const Duration duration) {
    timer.start = std::chrono::steady_clock::now();
    timer.end = timer.start + duration;
    timer.isStarted = true;
    timer.isDurationSet = true;
  }

  /// @brief Остановка таймера.
  /// @param timer Таймер.
  void stop(Timer &timer) {
    if (timer.isStarted) {
      timer.end = std::chrono::steady_clock::now();
      timer.isStarted = false;
    }
  }

  /// @brief Остановка таймера и возврат истекшего периода времени.
  /// @return Истекший период времени.
  /// @param timer Таймер.
  [[nodiscard]] Duration stopAndGetElapsedTime(Timer &timer) {
    if (timer.isStarted) {
      timer.end = std::chrono::steady_clock::now();
      timer.isStarted = false;
      return timer.end - timer.start;
    }
    assert(false);
    return {};
  }

  /// @brief Сброс таймера.
  /// @param timer Таймер.
  void reset(Timer &timer) {
    if (timer.isStarted)
      timer.start = std::chrono::steady_clock::now();
  }

  /// @brief Проверка запуска таймера.
  /// @return Состояние запуска таймера.
  /// @param timer Таймер.
  [[nodiscard]] bool isStarted(const Timer &timer) const {
    return timer.isStarted;
  }

  /// @brief Проверка работы таймера.
  /// @return Состояние работы таймера.
  /// @param timer Таймер.
  [[nodiscard]] bool isRunning(const Timer &timer) const {
    return timer.isStarted && !isFinished(timer);
  }

  /// @brief Проверка завершения таймера.
  /// @return Состояние завершения таймера.
  /// @param timer Таймер.
  [[nodiscard]] bool isFinished(const Timer &timer) const {
    if (!timer.isStarted) {
      return true;
    }
    // При отсутствии периода времени отсчета.
    if (!timer.isDurationSet) {
      return false;
    }
    return std::chrono::steady_clock::now() >= timer.end;
  }

  /// @brief Проверка остановки таймера.
  /// @return Состояние остановки таймера.
  /// @param timer Таймер.
  [[nodiscard]] bool isStopped(const Timer &timer) const {
    return !timer.isStarted;
  }

  /// @brief Возвращает истекший период времени.
  /// @return Истекший период времени.
  /// @param timer Таймер.
  Duration getElapsedTime(const Timer &timer) const {
    if (timer.isStarted) {
      return timer.end - timer.start;
    }
    return std::chrono::steady_clock::now() - timer.start;
  }

};
