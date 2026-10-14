/**
 * @file
 * @brief
 */

#pragma once

//

#include <mutex>
#include <queue>
#include <condition_variable>

//

/// @brief
template<typename T>
class Queue final {
public:
  /// @brief Конструктор.
  Queue() = default;
  /// @brief Деструктор.
  ~Queue() = default;

  /// @brief Добавляет элемент.
  /// @param data Данные.
  void push(T data) {
    std::unique_lock<std::mutex> lock(mutex_);
    // Удаление первого элемента.
    if (queue_.size() >= max) {
      queue_.pop();
    }
    queue_.push(std::move(data));
    cv_.notify_one();
  }

  /// @brief Удаляет первый элемент.
  /// @param stop_token Токен проверки.
  bool pop(T &data, std::stop_token stop_token) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, stop_token, [this]() -> bool {
      return !queue_.empty();
    });
    if (queue_.empty()) {
      return false;
    }
    data = std::move(queue_.front());
    queue_.pop();
    return true;
  }

private:
  /// @brief
  std::mutex mutex_;
  /// @brief
  std::queue<T> queue_;
  /// @brief
  std::condition_variable_any cv_;

  /// @brief
  const std::size_t max = 16u;

};
