#include "Pipeline.hpp"

//

#include <string>
#include <chrono>
#include <format>
#include <fstream>
#include <filesystem>
#include <stop_token>
#include <condition_variable>

//

#include "Logger.hpp"

//

#include "opencv4/opencv2/opencv.hpp"

//

/// @brief Запуск.
/// @param filePath Путь к файлу.
/// @param inputName Имя входа.
/// @param outputName Имя выхода.
bool Pipeline::start(const std::string &filePath, const std::string &inputName, const std::string &outputName) {
  INFO(layerName_, " ", "Инициализация счетчика клиентов...");
  if (!inferenceEngine_->initialize(filePath, inputName, outputName) || !logWriter_->initialize()) {
    return false;
  }

  INFO(layerName_, " ", "Инициализация счетчика клиентов успешно завершена!");

  videoCaptureThread_ = std::jthread(&Pipeline::videoCaptureWorker, this);

  inferenceThread_ = std::jthread(&Pipeline::inferenceWorker, this);

  logWriterThread_ = std::jthread(&Pipeline::logWriterWorker, this);

  return true;
}

/// @brief Процесс.
/// @brief
void Pipeline::process(std::stop_source source, std::stop_token stoken) {
  size_t visitors_in{};
  size_t visitors_out{};

  //

  const cv::Scalar color(242., 242., 247.); // gray

  /*
  const cv::Scalar color(255., 45., 85.); // pink
  const cv::Scalar color(255., 55., 95.); // pink
  const cv::Scalar color(231., 18., 77.); // pink
  */

  const int recLineThickness = 1;
  const int textThickness = 1;
  const int lineType = cv::LINE_4;
  const int fontFace = cv::FONT_HERSHEY_SIMPLEX;

  auto lastTime = std::chrono::steady_clock::now();
  double fps{};

  auto displayData = std::unique_ptr<FrameDisplayData>();

  if (getFrameForDisplay(std::move(displayData), stoken)) {
    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = currentTime - lastTime;

    if (elapsed.count() > 0) {
      fps = 1. / elapsed.count();
    }

    for (const auto &box: displayData->boxes) {
      // Отрисовка рамки.
      cv::rectangle(
        displayData->data.data,
        {box.x1, box.y1},
        {box.x2, box.y2},
        color,
        recLineThickness,
        lineType
      );
    }

    // Отрисовка текста.
    cv::putText(
      displayData->data.data,
      "Версия ПО: ",
      {10, 10},
      fontFace,
      textThickness,
      color
    );

    // Отрисовка текста.
    cv::putText(
      displayData->data.data,
      "Подверсия ПО: ",
      {10, 20},
      fontFace,
      textThickness,
      color
    );

    // Отрисовка текста.
    cv::putText(
      displayData->data.data,
      "Кадр/секунда: " + std::to_string(fps),
      {10, 40},
      fontFace,
      textThickness,
      color
    );

    // Отрисовка текста.
    cv::putText(
      displayData->data.data,
      "Вошло: ",
      {10, 60},
      fontFace,
      textThickness,
      color
    );

    // Отрисовка текста.
    cv::putText(
      displayData->data.data,
      "Вышло: " + std::to_string(visitors_out),
      {10, 70},
      fontFace,
      textThickness,
      color
    );

    // Отрисовка текста.
    cv::putText(
      displayData->data.data,
      ": " + std::to_string(visitors_out),
      {10, 10},
      fontFace,
      textThickness,
      color
    );

    cv::imshow("temp", displayData->data.data);

    // Отправка в очередь.
    freeDisplayQueue_.push(std::move(displayData));

    if (cv::waitKey(1) == 113) { // q
      if (!source.stop_requested()) {
        //
      }
    }
  }
}

/// @brief Остановка.
void Pipeline::stop() {
  videoCaptureThread_.request_stop();

  inferenceThread_.request_stop();

  logWriterThread_.request_stop();
}

/// @brief Поток захвата видео.
/// @param stop_token
void Pipeline::videoCaptureWorker(std::stop_token stoken) {
  long long index{};

#if (1)
  cv::VideoCapture cap("../doc/videos/test.mp4");
#else
  cv::VideoCapture cap(0);
#endif

  if (!cap.isOpened()) {
    ERROR(layerName_, " ", "Не удалось инициализировать захват видео c камеры!");
    videoCaptureThread_.request_stop();
    return;
  }

  while(!stoken.stop_requested()) {
    cv::Mat frame;

    cap >> frame;

    if (frame.empty()) {
      ERROR(layerName_, " ", "Пустой кадр.");
      continue;
    }

    FrameData data{index++, std::move(frame)};

    // Отправка в очередь.
    queue_.push(std::move(data));
  }
}

/// @brief Поток вывода.
/// @param stoken
void Pipeline::inferenceWorker(std::stop_token stoken) {
  auto displayData = std::unique_ptr<FrameDisplayData>();

  while(!stoken.stop_requested()) {
    if (!freeDisplayQueue_.pop(displayData, stoken)) {
      //
    }

    if (queue_.pop(displayData->data, stoken)) {
      displayData->boxes.clear();

      if (inferenceEngine_->run(displayData->data, displayData->boxes)) {
        // Отправка в очередь.
        displayQueue_.push(std::move(displayData));
      } else {
        // Отправка в очередь.
        freeDisplayQueue_.push(std::move(displayData));
      }
    } else {
      // Отправка в очередь.
      freeDisplayQueue_.push(std::move(displayData));
    }
  }
}

/// @brief Поток записи логов.
/// @param
void Pipeline::logWriterWorker(std::stop_token stoken) {
  const std::filesystem::path LOG_FILE_DIR = "/tmp/blueberry_logs";
  const std::filesystem::path LOG_FILE_PATH = LOG_FILE_DIR / "customer_counter_raw.blueberry_retail.log";
  const std::filesystem::path TEMP_LOG_FILE_PATH = std::filesystem::path(LOG_FILE_PATH) += ".tmp";

  const auto PERIOD = std::chrono::minutes(1); // 1 МИН

  size_t visitors_in{};
  size_t visitors_out{};

  std::condition_variable_any cv{};

  if (!std::filesystem::create_directories(LOG_FILE_DIR)) {
    //
  }

  while(!stoken.stop_requested()) {
    // Получение данных.
    {
      std::unique_lock<std::mutex> lock(mutex_);
      cv.wait_for(lock, stoken, PERIOD, [&]() -> bool { return stoken.stop_requested(); });

      visitors_in = log_.visitors_in;
      visitors_out = log_.visitors_out;
    }

    const auto now = std::chrono::system_clock::now();
    const auto iso_string = std::format("{:%FT%T%z}", now);

    const auto payload = std::format(
      R"({{"timestamp": "{}", "visitors_in": {}, "visitors_out": {}}})",
      iso_string,
      visitors_in,
      visitors_out
    ) + "\n";

    std::error_code err{};

    if (std::filesystem::exists(LOG_FILE_PATH)) {
      std::filesystem::copy_file(
        LOG_FILE_PATH,
        TEMP_LOG_FILE_PATH,
        std::filesystem::copy_options::overwrite_existing,
        err
      );
    }

    // Запись в файл.
    {
      std::ofstream file(TEMP_LOG_FILE_PATH, std::ios::app);
      if (file.is_open()) {
        file.write(payload.data(), payload.size());
      } else {
        ERROR(layerName_, " ", "Не удалось записать лог в файл!");
      }
    }

    std::filesystem::rename(TEMP_LOG_FILE_PATH, LOG_FILE_PATH, err);
  }
}
