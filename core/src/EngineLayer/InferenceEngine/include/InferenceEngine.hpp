/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>

//

#include "Data.hpp"

//

#include "Logger.hpp"

//

#include "NvInfer.h"

//

/// @brief
class InferenceLogger : public nvinfer1::ILogger {
public:
  void log(Severity severity, nvinfer1::AsciiChar const *msg) noexcept override {
    //
  }
};

/// @brief
class IInferenceEngine {
public:
  /// @brief Деструктор.
  virtual ~IInferenceEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize(const std::string &fileName) = 0;

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
  bool initialize(const std::string &fileName) override;

  /// @brief
  /// @return
  bool run(const FrameData &data) override;

private:
  /// @brief Загрузка движка.
  /// @param fileName
  std::shared_ptr<nvinfer1::ICudaEngine> loadEngine(const std::string &fileName);


private:
  /// @brief
  InferenceLogger inferenceLogger_;

  /// @brief
  std::unique_ptr<nvinfer1::IRuntime> runtime_;

};
