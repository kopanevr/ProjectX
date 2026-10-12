/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>

//

#include "Logger.hpp"

//

#include "ThreadQueue.hpp"

//

/// @brief
class IYoloEngine {
public:
  /// @brief Деструктор.
  virtual ~IYoloEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize(const std::string &yoloModelPath) = 0;

  /// @brief
  /// @return
  virtual bool run(const FrameData &data) = 0;

protected:
  /// @brief
  const std::string layerName_ = "[YOLO Engine]";

private:

};

/// @brief
class YoloEngine final : public IYoloEngine {
public:
  /// @brief Деструктор.
  ~YoloEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  bool initialize(const std::string &yoloModelPath) override {
    LOG(layerName_, " ", "Загрузка модели: ", yoloModelPath);

    //

    return true;
  }

  /// @brief
  /// @return
  bool run(const FrameData &data) override {
    return {};
  }

private:

};
