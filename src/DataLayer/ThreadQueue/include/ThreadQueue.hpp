/**
 * @file
 * @brief
 */

#pragma once

//

#include <mutex>
#include <queue>

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
  /// @breif Деструктор.
  ~TheadQueue() = default;

  /// @brief
  void push(T data) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.size() >= max) {
      queue_.pop();
    }
    queue_.push(std::move(data));
  }

  /// @brief
  bool pop() {
    std::unique_lock<std::mutex> lock(mutex_);

    return true;
  }

private:
  /// @brief
  std::mutex mutex_;
  /// @brief
  std::queue<T> queue_;

  /// @brief
  const std::size_t max = 30u;

};
