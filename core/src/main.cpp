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

#include "DatabaseWriter.hpp"

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
  auto pipeline = std::unique_ptr<Pipeline>(new (std::nothrow) Pipeline(std::unique_ptr<InferenceEngine>(new (std::nothrow) InferenceEngine()), std::unique_ptr<DatabaseWriter>(new (std::nothrow) DatabaseWriter())));
  if (!pipeline) {
    return EXIT_FAILURE;
  }

  // Запуск конвейера.
  if (pipeline->start("models/model.engine", "input", "output")) {
    return EXIT_FAILURE;
  }

  std::stop_source source{};
  std::stop_token stoken = source.get_token();

  while (!stoken.stop_requested()) {
#if (1)
  pipeline->process(source, stoken);
#endif
  }

  // Остановка конвейера.
  pipeline->stop();

  return EXIT_SUCCESS;
}
