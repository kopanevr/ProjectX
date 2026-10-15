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
class IPipeline {
protected:
  /// @brief
  const std::string layerName_ = "[Pipeline]";

};

/// @brief
class Pipeline final : public IPipeline {
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
  /// @param filePath Путь к файлу.
  bool start(const std::string &filePath, const std::string &inputName, const std::string &outputName);
  /// @brief Процесс.
  /// @return
  bool process();
  /// @brief Останова.
  void stop();

  /// @brief
  /// @return
  bool getFrameForDisplay(FrameData &data) {
    return displayQueue_.pop(, data);
  }

private:
  /// @brief Поток захвата видео.
  void videoCaptureWorker(std::stop_token stop_token);
  /// @brief Поток вывода.
  void inferenceWorker(std::stop_token stop_token);
  /// @brief Поток записи в базу данных.
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
  /// @brief
  Queue<FrameData> displayQueue_;

};
