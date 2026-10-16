/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>
#include <vector>

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

//

/// @brief
struct BoundingBox {
  int x1, y1, x2, y2;
  float conf;
};

/// @brief
class IInferenceEngine {
public:
  /// @brief Деструктор.
  virtual ~IInferenceEngine() = default;

  /// @param fileName Имя файла.
  /// @param inputName Имя входа.
  /// @param outputName Имя выхода.
  /// @return Результат инициализации.
  virtual bool initialize(const std::string &fileName, const std::string &inputName, const std::string &outputName) = 0;

  /// @brief
  /// @return
  virtual bool run(const FrameData &data, const std::vector<BoundingBox> &boxes) = 0;

  /// @brief
  virtual void prepareBoundingBoxes(std::vector<BoundingBox> &boxes) = 0;

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

  /// @param fileName Имя файла.
  /// @param inputName Имя входа.
  /// @param outputName Имя выхода.
  /// @return Результат инициализации.
  bool initialize(const std::string &fileName, const std::string &inputName, const std::string &outputName) override;

  /// @brief Вывод.
  /// @return
  bool run(const FrameData &data, const std::vector<BoundingBox> &boxes) override;

  /// @brief
  /// @param boxes
  void prepareBoundingBoxes(std::vector<BoundingBox> &boxes) override;

private:
  /// @brief Загрузка движка.
  /// @param fileName
  std::shared_ptr<nvinfer1::ICudaEngine> loadEngine(const std::string &fileName);
  /// @brief
  bool createExecutionContext(std::shared_ptr<nvinfer1::ICudaEngine> engine);

  /// @brief Выделение памяти на GPU.
  bool allocateBuffers();
  /// @brief Освобождение памяти на GPU.
  void freeBuffers();

private:
  /// @brief
  InferenceLogger inferenceLogger_;

  /// @brief
  std::unique_ptr<nvinfer1::IRuntime> runtime_;
  /// @brief
  std::unique_ptr<nvinfer1::IExecutionContext> executionContext_;

  /// @brief
  std::string inputName_;
  /// @brief
  std::string outputName_;

  /// @brief Размер входа в байт.
  size_t inputSizeBytes_;
  /// @brief Размер выхода в байт.
  size_t outputSizeBytes_;

  /// @brief
  void *deviceInput_{nullptr};
  /// @brief
  void *deviceOutput_{nullptr};

  /// @brief
  static inline size_t numElements_;

  /// @brief
  FrameDisplayData displayData_;

};

/// @brief Освобождение памяти  на GPU.
inline void InferenceEngine::freeBuffers() {
  if (deviceInput_) {
    cudaFree(deviceInput_);
    deviceInput_ = nullptr;
  }
  if (deviceOutput_) {
    cudaFree(deviceOutput_);
    deviceOutput_ = nullptr;
  }
}
