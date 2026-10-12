/**
 * @file
 * @brief
 */

//

#include <string>
#include <memory>

//

#include "YoloEngine.hpp"

#include "Pipeline.hpp"

//

/// @brief
/// @param argc
/// @param argv
/// @return
int main(int argc, char *argv[]) {
  auto pipeline = std::unique_ptr<Pipeline>(new (std::nothrow) Pipeline(std::unique_ptr<YoloEngine>(new (std::nothrow) YoloEngine())));

  if (pipeline) {
    pipeline->start("models/yolo26n.onnx");
  }
}
