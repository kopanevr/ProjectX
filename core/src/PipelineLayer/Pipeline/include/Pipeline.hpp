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

#include "DatabaseWriter.hpp"

//

#include "InferenceEngine.hpp"

//

#include "Data.hpp"
#include "Queue.hpp"

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
  Pipeline(std::unique_ptr<IInferenceEngine> inferenceEngine, std::unique_ptr<IDatabaseWriter> databaseWriter)
      : inferenceEngine_(std::move(inferenceEngine)),
        databaseWriter_(std::move(databaseWriter)) {}

  /// @brief Деструктор.
  ~Pipeline() {
    stop();
  }

  /// @brief Запуск.
  void start(const std::string &yoloModelPath);
  void stop();

private:
  /// @brief Поток захвата видео.
  void videoCaptureWorker(std::stop_token stop_token);
  /// @brief Поток вывода.
  void inferenceWorker(std::stop_token stop_token);
  /// @brief
  void databaseWriterWorker(std::stop_token stop_token);

private:
  /// @brief
  std::unique_ptr<IInferenceEngine> inferenceEngine_;

  /// @brief
  std::unique_ptr<IDatabaseWriter> databaseWriter_;

  /// @brief
  std::jthread videoCaptureThread_;
  /// @brief
  std::jthread inferenceThread_;
    /// @brief
  std::jthread databaseWriterThread_;

  /// @brief
  Queue<FrameData> queue_;

};
