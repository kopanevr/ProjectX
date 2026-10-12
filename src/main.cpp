/**
 * @file
 * @brief
 */

//

#include <string>
#include <memory>

//

#include <YoloEngine.hpp>
#include <LlmEngine.hpp>

#include "Pipeline.hpp"

//

/// @brief
/// @param argc
/// @param argv
/// @return
int main(int argc, char *argv[]) {
  auto pipeline = std::unique_ptr<Pipeline>(new (std::nothrow) Pipeline(std::unique_ptr<YoloEngine>(new (std::nothrow) YoloEngine()), std::unique_ptr<LlmEngine>(new (std::nothrow) LlmEngine())));

  if (pipeline) {
    pipeline->start("/.onnx", "/.");
  }
}
