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
#include "LlmEngine.hpp"

//

class IPepeline {
protected:
  /// @brief
  const std::string layerName_ = "[Pipeline]";

}

/// @brief
class Pipeline final : public IPepeline {
public:
  /// @brief Конструктор.
  Pipeline(std::unique_ptr<IYoloEngine> yoloEngine, std::unique_ptr<ILlmEngine> llmEngine)
      : yoloEngine_(std::move(yoloEngine)),
        llmEngine_(std::move(llmEngine)) {}

  /// Деструктор.
  ~Pipeline() {
    stop();
  }

  /// @brief Запуск.
  void start(const std::string &yoloModelPath, const std::string &llmModelPath);
  void stop();

private:
  /// @brief  Поток захвата видео.
  void videoCaptureWorker();
  /// @brief Поток вывода YOLO.
  void yoloInferenceWorker();
  /// @brief Поток вывода LLM.
  void llmInferenceWorker();

private:
  /// @brief
  std::unique_ptr<IYoloEngine> yoloEngine_;
  /// @brief
  std::unique_ptr<ILlmEngine> llmEngine_;

  /// @brief
  std::jthread videCaptureThread_;
  /// @brief
  std::jthread yoloInferenceThread_;
  /// @brief
  std::jthread llmInferenceThread_;

};
