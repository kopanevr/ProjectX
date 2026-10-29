/**
 * @file
 * @brief
 */

//

#include <cstdlib>

//

#include <memory>
#include <stop_token>

//

#include "LogWriter.hpp"

//

#include "InferenceEngine.hpp"

//

#include "Pipeline.hpp"

//

/// @brief
/// @param argc
/// @param argv
/// @return
int main(int argc, char *argv[]) {
  auto inferenceEngine = std::unique_ptr<InferenceEngine>(new (std::nothrow) InferenceEngine());
  auto logWriter = std::unique_ptr<LogWriter>(new (std::nothrow) LogWriter());

  const auto pipeline = std::unique_ptr<Pipeline>(new (std::nothrow) Pipeline(
      std::move(inferenceEngine),
      std::move(logWriter)
    ));
  if (!pipeline) {
    return EXIT_FAILURE;
  }

  // Запуск конвейера.
  if (!pipeline->start("../../../models/.hef", "images", "output0")) {
    return EXIT_FAILURE;
  }

  std::stop_source source{};
  std::stop_token stoken = source.get_token();

  while (!stoken.stop_requested()) {
#if(0)
    pipeline->process(source, stoken);
#endif
  }

  // Остановка конвейера.
  pipeline->stop();

  return EXIT_SUCCESS;
}
