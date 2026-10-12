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

/// @brief
class IYoloEngine {
public:
  /// @brief Деструктор.
  virtual ~IYoloEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize(const std::string &yoloModelPath) = 0;

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
  virtual bool initialize(const std::string &yoloModelPath) override {
    LOG(layerName_, " ", "Загрузка модели: ", yoloModelPath);

    //
  }

private:

};
