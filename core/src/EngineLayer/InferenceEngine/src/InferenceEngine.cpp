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
  INFO(layerName_, " ", "Имя входа: ", inputName_ = inputName);
  INFO(layerName_, " ", "Имя выхода: ", outputName_ = outputName);

#ifdef USE_HAILO
#endif

  return true;
}

/// @brief Вывод.
/// @param data
/// @param boxes
/// @return
bool InferenceEngine::run(const FrameData &data, std::vector<BoundingBox> &boxes) {
  return true;
}
