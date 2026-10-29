#include "LogWriter.hpp"

//

#include <chrono>
#include <format>
#include <fstream>
#include <condition_variable>
#include <stop_token>

//

#include "Logger.hpp"

//

/// @brief
/// @param log
void LogWriter::run(const Log &log) {
  std::error_code err{};

  if (!std::filesystem::create_directories(LOG_FILE_DIR, err)) {
    //
  }

  const auto now = std::chrono::system_clock::now();
  const auto iso_string = std::format("{:%FT%T%z}", now);

  const auto payload = std::format(
    R"({{"timestamp": "{}", "visitors_in": {}, "visitors_out": {}}})",
    iso_string,
    log.visitors_in,
    log.visitors_out
  ) + "\n";


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