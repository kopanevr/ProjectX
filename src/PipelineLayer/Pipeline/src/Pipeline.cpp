#include "Pipeline.hpp"

//

#include "Logger.hpp"

//

#include "opencv4/opencv2/opencv.hpp"

//

/// @brief Запуск.
/// @param yoloModelPath Путь к модели YOLO.
/// @param llmModelPath Путь к модели LLM.
void Pipeline::start(const std::string &yoloModelPath) {
  INFO("Запуск конвейера.");
  if (!yoloEngine_->initialize(yoloModelPath)) {
    ERROR(layerName_, " ", "Не удалось инициализировать движки.");
    return;
  }

  videCaptureThread_ = std::jthread(&Pipeline::videoCaptureWorker, this);

  yoloInferenceThread_ = std::jthread(&Pipeline::yoloInferenceWorker, this);
}

/// @brief Остановка.
void Pipeline::stop() {
  videCaptureThread_.request_stop();

  yoloInferenceThread_.request_stop();
}

/// @brief  Поток захвата видео.
void Pipeline::videoCaptureWorker(std::stop_token stop_token) {
  long long index{};

  cv::VideoCapture videCapture(0);

  cv::Mat frame;

  if (!videCapture.isOpened()) {
    ERROR(layerName_, " ", "Не удалось инициализировать захват видео c камеры.");
    return;
  }

  while(!stop_token.stop_requested()) {
    if (frame.empty()) {
      ERROR(layerName_, " ", "Не удалось ");
      continue;
    }

    FrameData data{index++, frame};

    threadQueue_.push(std::move(data));
  }
}

/// @brief Поток вывода YOLO.
void Pipeline::yoloInferenceWorker(std::stop_token stop_token) {
  while(!stop_token.stop_requested()) {

  }
}
