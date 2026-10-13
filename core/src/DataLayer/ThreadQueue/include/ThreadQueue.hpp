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

#include "opencv4/opencv2/opencv.hpp"

//

struct FrameData {
/// @brief Индекс.
long long index;
/// @brief Данные.
cv::Mat data;
};

/// @brief
template<typename T>
class TheadQueue final {
public:
  /// @brief Конструктор.
  TheadQueue() = default;
  /// @brief Деструктор.
  ~TheadQueue() = default;

  /// @brief
  /// @param data
  void push(T data) {
    std::unique_lock<std::mutex> lock(mutex_);

    if (queue_.size() >= max) {
      queue_.pop();
    }

    queue_.push(std::move(data));
    cv_.notify_one();
  }

  /// @brief
  /// @param stop_token
  bool pop(T &data, std::stop_token stopToken) {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, stopToken, [this]() -> bool {
      return !queue_.empty();
    });

    if (queue_.empty()) {
      return false;
    }

    data = std::move(queue_.front());

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
