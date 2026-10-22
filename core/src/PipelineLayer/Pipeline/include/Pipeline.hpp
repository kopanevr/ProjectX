/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>
#include <mutex>
#include <memory>
#include <thread>

//

#include "LogWriter.hpp"

//

#include "InferenceEngine.hpp"

//

#include "Data.hpp"
#include "Queue.hpp"

//

/// @brief
class IPipeline {
protected:
  /// @brief
  const std::string layerName_ = "[Pipeline]";

};

/// @brief
class Pipeline final : public IPipeline {
public:
  /// @brief Конструктор.
  /// @param inputName Имя входа.
  /// @param outputName Имя выхода.
  Pipeline(std::unique_ptr<IInferenceEngine> inferenceEngine, std::unique_ptr<ILogWriter> logWriter)
      : inferenceEngine_(std::move(inferenceEngine)),
        logWriter_(std::move(logWriter)) {}

  /// @brief Деструктор.
  ~Pipeline() {
    stop();
  }

  /// @brief Запуск.
  /// @param filePath Путь к файлу.
  bool start(const std::string &filePath, const std::string &inputName, const std::string &outputName);
  /// @brief Процесс.
  void process(std::stop_source source, std::stop_token stoken);
  /// @brief Останова.
  void stop();

  /// @brief
  /// @return
  bool getFrameForDisplay(std::unique_ptr<FrameDisplayData> data, std::stop_token stoken) {
    return displayQueue_.pop(data, stoken);
  }

private:
  /// @brief Поток захвата видео.
  void videoCaptureWorker(std::stop_token stoken);
  /// @brief Поток вывода.
  void inferenceWorker(std::stop_token stoken);
  /// @brief Поток записи в базу данных.
  void logWriterWorker(std::stop_token stoken);

private:
  /// @brief
  std::unique_ptr<IInferenceEngine> inferenceEngine_;

  /// @brief
  std::unique_ptr<ILogWriter> logWriter_;

  /// @brief
  std::jthread videoCaptureThread_;
  /// @brief
  std::jthread inferenceThread_;
    /// @brief
  std::jthread logWriterThread_;

  /// @brief
  Queue<FrameData> queue_;

  /// @brief
  Queue<std::unique_ptr<FrameDisplayData>> displayQueue_;
  /// @brief
  Queue<std::unique_ptr<FrameDisplayData>> freeDisplayQueue_;

  /// @brief
  std::mutex mutex_;

  /// @brief
  struct {
    size_t visitors_in{};
    size_t visitors_out{};
  } log_;

};
