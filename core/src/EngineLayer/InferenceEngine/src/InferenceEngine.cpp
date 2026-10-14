#include "InferenceEngine.hpp"

//

#include <fstream>
#include <vector>
#include <memory>

//

#include <cuda_device_runtime_api.h>

//

/// @brief Инициализация.
/// @param fileName
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

  if (!allocateBuffers()) {
    ERROR(layerName_, " ", "Не удалось выделить память на GPU.");
    return false;
  }

  return true;
}

/// @brief
/// @return
bool InferenceEngine::run(const FrameData &data) {
}

/// @brief Загрузка движка.
/// @param fileName
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

  // Десериализация движка
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

  nvinfer1::Dims4 inputDims{1, 3, 640, 640};
  if (!executionContext_->setInputShape(inputName_.c_str(), inputDims)) {
    return false;
  }

  inputSizeBytes_ = 1 * 3 * 640 * 640 * sizeof(float);
  LOG(layerName_, " ", "Размер входа: ", inputSizeBytes_, " байт.");

  if (cudaMalloc(&deviceInput_, inputSizeBytes_) != cudaSuccess) {
    return false;
  }

  // Установка адреса входа.
  if (executionContext_->setTensorAddress(inputName_.c_str(), deviceInput_)) {
    return false;
  }

  nvinfer1::Dims outputDims = executionContext_->getTensorShape(inputName_.c_str());

  outputSizeBytes_ = 1;
  for (int i = 0; i < outputDims.nbDims; ++i) {
    outputSizeBytes_ *= outputDims.d[i];
  }
  LOG(layerName_, " ", "Размер выхода: ", outputSizeBytes_, " байт.");

  if (cudaMalloc(&deviceOutput_, outputSizeBytes_) != cudaSuccess) {
    return false;
  }

  // Установка адреса выхода.
  if (executionContext_->setTensorAddress(outputName_.c_str(), deviceOutput_)) {
    return false;
  }

  LOG(layerName_, " ", "Выделение памяти на GPU успешно завершено!");

  return true;
}
