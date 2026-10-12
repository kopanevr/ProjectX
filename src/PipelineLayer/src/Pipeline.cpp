#include "Pipeline.hpp"

//

#include "Logger.hpp"

//

/// @brief
/// @param yoloModelPath Путь к модели YOLO.
/// @param llmModelPath Путь к модели LLM.
void Pipeline::start(const std::string &yoloModelPath, const std::string &llmModelPath) {
  if (!yoloEngine_->initialize(yoloModelPath) || llmEngine_->initialize(llmModelPath)) {
    ERROR(layerName_, " ", "Не удалось инициализировать движки.");
    return;
  }

  videCaptureThread_ = std::jthread(&Pipeline::videoCaptureWorker, this);

  yoloInferenceThread_ = std::jthread(&Pipeline::yoloInferenceWorker, this);
  llmInferenceThread_ = std::jthread(&Pipeline::llmInferenceWorker, this);
}

/// @brief Остановка.
void Pipeline::stop() {

}

/// @brief  Поток захвата видео.
void Pipeline::videoCaptureWorker() {
  while(1) {

  }
}

/// @brief Поток вывода YOLO.
void Pipeline::yoloInferenceWorker() {
  while(1) {

  }
}

/// @brief Поток вывода LLM.
void Pipeline::llmInferenceWorker() {
  while(1) {

  }
}
