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

#include "Data.hpp"

//

/// @brief
class IInferenceEngine {
public:
  /// @brief Деструктор.
  virtual ~IInferenceEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize(const std::string &modelPath) = 0;

  /// @brief
  /// @return
  virtual bool run(const FrameData &data) = 0;

protected:
  /// @brief
  const std::string layerName_ = "[Inference Engine]";

private:

};

/// @brief
class InferenceEngine final : public IInferenceEngine {
public:
  /// @brief Деструктор.
  ~InferenceEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  bool initialize(const std::string &modelPath) override {
    LOG(layerName_, " ", "Загрузка TensorRT модели: ", modelPath);

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
