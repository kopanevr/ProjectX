/**
 * @file
 * @brief
 */

//

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
  auto pipeline = std::make_unique<Pipeline>(std::make_unique<YoloEngine>(), std::make_unique<LlmEngine>());

  yoloModelPath = "";

  pipeline->start();
}
