#include "InferenceEngine.hpp"

//

#include <fstream>
#include <vector>
#include <memory>

//

/// @brief Инициализация.
/// @param fileName
/// @return Результат инициализации.
bool InferenceEngine::initialize(const std::string &fileName) {
  LOG(layerName_, " ", "Загрузка TensorRT движка: ", fileName);
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
