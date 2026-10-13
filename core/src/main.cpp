/**
 * @file
 * @brief
 */

//

#include <memory>

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

  if (pipeline) {
    pipeline->start("models/yolo26n.onnx");
  }
}
