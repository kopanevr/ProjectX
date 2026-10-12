/**
 * @file
 * @brief
 */

#pragma once

//

#include <memory>
#include <thread>

//

#include "YoloEngine.hpp"
#include "LlmEngine.hpp"

//

/// @brief
class Pipeline final {
public:
  /// @brief Конструктор.
  Pipeline(std::unique_ptr<IYoloEngine> yoloEngine, std::unique_ptr<ILlmEngine> llmEngine)
      : yoloEngine_(std::move(yoloEngine)),
        llmEngine_(std::move(llmEngine)) {}

  /// Деструктор.
  ~Pipeline() {
    stop();
  }

  void start();
  void stop();
private:
  /// @brief
  std::unique_ptr<IYoloEngine> yoloEngine_;
  /// brief
  std::unique_ptr<ILlmEngine> llmEngine_;

};
