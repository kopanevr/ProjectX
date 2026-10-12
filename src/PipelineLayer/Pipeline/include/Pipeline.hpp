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

#include "DataWriterEngine.hpp"

//

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
  void videoCaptureWorker(std::stop_token stopToken);
  /// @brief Поток вывода YOLO.
  void yoloInferenceWorker(std::stop_token stopToken);
  /// @brief
  void dataWriterWorker(std::stop_token stopToken);

private:
  /// @brief
  std::unique_ptr<IYoloEngine> yoloEngine_;
  /// @brief
  std::unique_ptr<DataWriterEngine> dataWriterEngine_;

  /// @brief
  std::jthread videoCaptureThread_;
  /// @brief
  std::jthread yoloInferenceThread_;
    /// @brief
  std::jthread dataWriterThread_;

  /// @brief
  TheadQueue<FrameData> threadQueue_;

};
