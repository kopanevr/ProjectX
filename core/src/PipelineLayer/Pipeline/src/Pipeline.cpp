#include "Pipeline.hpp"

//

#include <stop_token>

//

#include "Logger.hpp"

//

#include "opencv4/opencv2/opencv.hpp"

//

/// @brief Запуск.
/// @param filePath Путь к файлу.
/// @param inputName
/// @param outputName
bool Pipeline::start(const std::string &filePath, const std::string &inputName, const std::string &outputName) {
  INFO(layerName_, " ", "Инициализация C++ ядра...");
  if (!inferenceEngine_->initialize(filePath, inputName, outputName) || databaseWriter_->initialize()) {
    return false;
  }

  INFO(layerName_, " ", "Инициализация C++ ядра успешно завершена!");

  videoCaptureThread_ = std::jthread(&Pipeline::videoCaptureWorker, this);

  inferenceThread_ = std::jthread(&Pipeline::inferenceWorker, this);

  databaseWriterThread_ = std::jthread(&Pipeline::databaseWriterWorker, this);

  return true;
}

/// @brief Процесс.
/// @brief
void Pipeline::process(std::stop_source source, std::stop_token stoken) {
  const cv::Scalar color(255., 45., 85.); // pink

  /*
  const cv::Scalar color(255., 55., 95.); // pink
  const cv::Scalar color(231., 18., 77.); // pink
  */

  const int thickness = 1;
  const int lineType = cv::LINE_4;
  const int fontFace = cv::FONT_HERSHEY_SIMPLEX;

  FrameDisplayData displayData{};

  if (getFrameForDisplay(displayData, stoken)) {
    for (const auto &box: displayData.boxes) {
      // Отрисовка рамки.
      cv::rectangle(
        displayData.data.data,
        {box.x1, box.y1},
        {box.x2, box.y2},
        color,
        thickness,
        lineType
      );
    }

    // Отрисовка текста.
    cv::putText(
      displayData.data.data,
      ": ",
      {10, 10},
      fontFace,
      1.,
      color
    );

    cv::imshow("temp", displayData.data.data);
  }

  if (cv::waitKey(1) == 113) { // q
    //
    source.stop_requested();
  }
}

/// @brief Остановка.
void Pipeline::stop() {
  videoCaptureThread_.request_stop();

  inferenceThread_.request_stop();
}

/// @brief Поток захвата видео.
/// @param stop_token
void Pipeline::videoCaptureWorker(std::stop_token stoken) {
  long long index{};

  cv::VideoCapture cap(0);

  cv::Mat frame;

  if (!cap.isOpened()) {
    ERROR(layerName_, " ", "Не удалось инициализировать захват видео c камеры.");
    return;
  }

  while(!stoken.stop_requested()) {
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
/// @param stoken
void Pipeline::inferenceWorker(std::stop_token stoken) {
  FrameDisplayData displayData{};

  //
  displayData.boxes.reserve(Config::outputSizeBytes);

  while(!stoken.stop_requested()) {
    if (queue_.pop(displayData.data, stoken)) {

      displayData.boxes.clear();

      if (inferenceEngine_->run(displayData.data, displayData.boxes)) {
        // Отправка в очередь.
        displayQueue_.push(std::move(displayData));

        displayData = FrameDisplayData();
      }
    }
  }
}

/// @brief Поток записи в базу данных.
/// @param
void Pipeline::databaseWriterWorker(std::stop_token token) {
  //
}
