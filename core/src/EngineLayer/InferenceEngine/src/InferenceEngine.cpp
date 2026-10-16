#include "InferenceEngine.hpp"

//

#include <fstream>
#include <vector>
#include <memory>

//

#include "Config.hpp"

//

#include <cuda_runtime.h>

//

/// @brief Инициализация.
/// @param fileName Имя файла.
/// @param inputName Имя входа.
/// @param outputName Имя выхода.
/// @return Результат инициализации.
bool InferenceEngine::initialize(const std::string &fileName, const std::string &inputName, const std::string &outputName) {
  LOG(layerName_, " ", "Загрузка TensorRT движка: ", fileName);

  LOG(layerName_, " ", "Имя входа: ", inputName_ = inputName);
  LOG(layerName_, " ", "Имя выхода: ", outputName_ = outputName);

  // Загрузка движка.
  auto engine = loadEngine(fileName);
  if (engine) {
    ERROR(layerName_, " ", "Не удалось загрузить TensorRT движок.");
    return false;
  }

  // Создание контекста.
  if (!createExecutionContext(engine)) {
    ERROR(layerName_, " ", "Не удалось создать контекст.");
    return false;
  }

  // Выделение памяти на GPU.
  if (!allocateBuffers()) {
    ERROR(layerName_, " ", "Не удалось выделить память на GPU.");
    return false;
  }

  return true;
}

/// @brief Вывод.
/// @param data
/// @param boxes
/// @return
bool InferenceEngine::run(const FrameData &data, std::vector<BoundingBox> &boxes) {
  cudaStream_t cudaStream{};
  if (cudaStreamCreate(&cudaStream) != cudaSuccess) {
    return false;
  }

  // Копирование данных c CPU на GPU.
  if (cudaMemcpyAsync(
    deviceInput_,
    data.data.data,
    Config::inputSizeBytes,
    cudaMemcpyHostToDevice,
    cudaStream
  ) != cudaSuccess) {
    cudaStreamDestroy(cudaStream);
    return false;
  }

  // Запуск.
  if (!executionContext_->enqueueV3(cudaStream)) {
    cudaStreamDestroy(cudaStream);
    return false;
  }

  // Копирование данных c GPU на CPU.
  if (cudaMemcpyAsync(
    deviceOutput_,
    boxes.data(),
    Config::outputSizeBytes,
    cudaMemcpyDeviceToHost,
    cudaStream
  ) != cudaSuccess) {
    cudaStreamDestroy(cudaStream);
    return false;
  }

  // Ожидание завершения .
  cudaStreamSynchronize(cudaStream);

  cudaStreamDestroy(cudaStream);

  return true;
}

/// @brief Загрузка движка.
/// @param fileName Имя файла.
std::shared_ptr<nvinfer1::ICudaEngine> InferenceEngine::loadEngine(const std::string &fileName) {
  LOG(layerName_, " ", "Запуск десериализации движка...");

  std::ifstream file(fileName, std::ios::binary);
  if (!file.good()) {
    return nullptr;
  }

  file.seekg(0, std::ios::end);
  const auto size = file.tellg();
  LOG(layerName_, " ", "Размер файла движка: ", size, " байт.");
  file.seekg(0, std::ios::beg);

  std::vector<char> buffer(size);
  if (!file.read(buffer.data(), size)) {
    return nullptr;
  }

  if (!runtime_) {
    const auto rawRuntime = nvinfer1::createInferRuntime(inferenceLogger_);
    if (!rawRuntime ) {
      return nullptr;
    }

    runtime_ = std::unique_ptr<nvinfer1::IRuntime>(rawRuntime);
  }

  // Десериализация движка.
  const auto rawEngine = runtime_->deserializeCudaEngine(buffer.data(), size);
  if (!rawEngine ) {
    return nullptr;
  }

  LOG(layerName_, " ", "Десериализиация движка успешно завершена!", fileName);

  return std::shared_ptr<nvinfer1::ICudaEngine>(rawEngine);
}

/// @brief
/// @param engine
bool InferenceEngine::createExecutionContext(std::shared_ptr<nvinfer1::ICudaEngine> engine) {
  const auto rawExecutionContext = engine->createExecutionContext();
  if (!rawExecutionContext ) {
    return false;
  }

  executionContext_= std::unique_ptr<nvinfer1::IExecutionContext>(engine->createExecutionContext());

  return true;
}

/// @brief Выделение памяти на GPU.
/// @return
bool InferenceEngine::allocateBuffers() {
  LOG(layerName_, " ", "Выделение памяти на GPU...");

  LOG(layerName_, " ", "Размер входа: ", Config::inputSizeBytes, " байт.");

  if (cudaMalloc(&deviceInput_, Config::inputSizeBytes) != cudaSuccess) {
    return false;
  }

  // Установка адреса входа.
  if (!executionContext_->setTensorAddress(inputName_.c_str(), deviceInput_)) {
    return false;
  }

  LOG(layerName_, " ", "Размер выхода: ", Config::outputSizeBytes, " байт.");

  if (cudaMalloc(&deviceOutput_, Config::outputSizeBytes) != cudaSuccess) {
    return false;
  }

  // Установка адреса выхода.
  if (!executionContext_->setTensorAddress(outputName_.c_str(), deviceOutput_)) {
    return false;
  }

  LOG(layerName_, " ", "Выделение памяти на GPU успешно завершено!");

  return true;
}
