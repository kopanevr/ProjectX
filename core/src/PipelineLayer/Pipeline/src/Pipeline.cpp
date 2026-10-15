#include "Pipeline.hpp"

//

#include "Logger.hpp"

//

#include "opencv4/opencv2/opencv.hpp"

//

/// @brief Запуск.
/// @param filePath Путь к файлу.
bool Pipeline::start(const std::string &filePath, const std::string &inputName, const std::string &outputName) {
  INFO(layerName_, " ", "Инициализация...");
  if (!inferenceEngine_->initialize(filePath, inputName, outputName) || databaseWriter_->initialize()) {
    return false;
  }

  INFO(layerName_, " ", "Инициализация успешно завершена!");

  videoCaptureThread_ = std::jthread(&Pipeline::videoCaptureWorker, this);

  inferenceThread_ = std::jthread(&Pipeline::inferenceWorker, this);

  databaseWriterThread_ = std::jthread(&Pipeline::databaseWriterWorker, this);

  return true;
}

/// @brief Остановка.
void Pipeline::stop() {
  videoCaptureThread_.request_stop();

  inferenceThread_.request_stop();
}

/// @brief Поток захвата видео.
void Pipeline::videoCaptureWorker(std::stop_token stop_token) {
  long long index{};

  cv::VideoCapture cap(0);

  cv::Mat frame;

  if (!cap.isOpened()) {
    ERROR(layerName_, " ", "Не удалось инициализировать захват видео c камеры.");
    return;
  }

  while(!stop_token.stop_requested()) {
    cap >> frame;

    if (frame.empty()) {
      ERROR(layerName_, " ", "Пустой кадр.");
      continue;
    }

    FrameData data{index++, frame};

    queue_.push(std::move(data));
  }
}

/// @brief Поток вывода.
void Pipeline::inferenceWorker(std::stop_token stop_token) {
  FrameData data{};

  while(!stop_token.stop_requested()) {
    if (queue_.pop(data, stop_token)) {
      inferenceEngine_->run(data);

#if (1)
      INFO(layerName_, " ");
#endif
    }
  }
}

/// @brief Поток записи в базу данных.
void Pipeline::databaseWriterWorker(std::stop_token stopToken) {

}
