/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>
#include <memory>
#include <thread>

//

#include "YoloEngine.hpp"

#include "ThreadQueue.hpp"

//

/// @brief
class IPepeline {
protected:
  /// @brief
  const std::string layerName_ = "[Pipeline]";

};

/// @brief
class Pipeline final : public IPepeline {
public:
  /// @brief Конструктор.
  Pipeline(std::unique_ptr<IYoloEngine> yoloEngine)
      : yoloEngine_(std::move(yoloEngine)) {}

  /// Деструктор.
  ~Pipeline() {
    stop();
  }

  /// @brief Запуск.
  void start(const std::string &yoloModelPath);
  void stop();

private:
  /// @brief Поток захвата видео.
  void videoCaptureWorker(std::stop_token stop_token);
  /// @brief Поток вывода YOLO.
  void yoloInferenceWorker(std::stop_token stop_token);

private:
  /// @brief
  std::unique_ptr<IYoloEngine> yoloEngine_;

  /// @brief
  std::jthread videCaptureThread_;
  /// @brief
  std::jthread yoloInferenceThread_;

  /// @brief
  TheadQueue<FrameData> threadQueue_;

};
