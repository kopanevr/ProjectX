/**
 * @file
 * @brief
 */

//

#include <memory>

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
  auto pipeline = std::unique_ptr<Pipeline>(new (std::nothrow) Pipeline(std::unique_ptr<InferenceEngine>(new (std::nothrow) InferenceEngine())));

  if (pipeline) {
    pipeline->start("models/yolo26n.onnx");
  }
}
