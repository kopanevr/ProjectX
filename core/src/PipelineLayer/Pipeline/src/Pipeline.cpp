#include "Pipeline.hpp"

//

#include "Logger.hpp"

//

#include "opencv4/opencv2/opencv.hpp"

//

/// @brief Запуск.
/// @param modelPath Путь к модели.
void Pipeline::start(const std::string &modelPath) {
  INFO("Запуск конвейера.");
  if (!inferenceEngine_>initialize(modelPath)) {
    ERROR(layerName_, " ", "Не удалось инициализировать движки.");
    return;
  }

  videoCaptureThread_ = std::jthread(&Pipeline::videoCaptureWorker, this);

  inferenceThread_ = std::jthread(&Pipeline::inferenceWorker, this);
  dataWriterThread_ = std::jthread(&Pipeline::dataWriterWorker, this);
}

/// @brief Остановка.
void Pipeline::stop() {
  videoCaptureThread_.request_stop();

  inferenceThread_.request_stop();
}

/// @brief Поток захвата видео.
void Pipeline::videoCaptureWorker(std::stop_token stopToken) {
  long long index{};

  cv::VideoCapture cap(0);

  cv::Mat frame;

  if (!cap.isOpened()) {
    ERROR(layerName_, " ", "Не удалось инициализировать захват видео c камеры.");
    return;
  }

  while(!stopToken.stop_requested()) {
    cap >> frame;

    if (frame.empty()) {
      ERROR(layerName_, " ", "Пустой кадр.");
      continue;
    }

    FrameData data{index++, frame};

    threadQueue_.push(std::move(data));
  }
}

/// @brief Поток вывода YOLO.
void Pipeline::yoloInferenceWorker(std::stop_token stopToken) {
  FrameData data{};

  while(!stopToken.stop_requested()) {
    if (threadQueue_.pop(data, stopToken)) {
      yoloEngine_->run(data);
    }
  }
}

/// @brief Поток.
void Pipeline::dataWriterWorker(std::stop_token stopToken) {

}
