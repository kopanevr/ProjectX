#include "InferenceEngine.hpp"

//

#include <fstream>
#include <vector>
#include <memory>

//

#include "Config.hpp"

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
  if (!createDevice()) {
    return false;
  }
  INFO(layerName_, " ", "Устройство Hailo успешно подключено!");

  if (!loadModel(fileName)) {
    return false;
  }
  INFO(layerName_, " ", "Файл HEF успешно загружен!");

  if (!configureDevice()) {
    return false;
  }
  INFO(layerName_, " ", "Устройство Hailo успешно сконфигурировано!");

#endif

  return true;
}

/// @brief
/// @return
bool InferenceEngine::createDevice() {
  auto device =  hailort::Device::create();
  if (!device) {
    ERROR(layerName_, " ", "Ошибка создания устройства: ", device.status());
    return false;
  }

  device_ = std::move(device.value());

  INFO(layerName_, " ", "ID устройства: ", device_->get_dev_id());

  return true;
}

/// @brief
/// @param fileName Имя файла.
/// @return
bool InferenceEngine::loadModel(const std::string &fileName) {
  auto hef =  hailort::Hef::create(fileName);
  if (!hef) {
    ERROR(layerName_, " ", "Ошибка загрузки HEF файла: ", hef.status());
    return false;
  }

  hef_ = std::unique_ptr<hailort::Hef>(new (std::nothrow) hailort::Hef(std::move(hef.value())));

  return true;
}

/// @brief
/// @return
bool InferenceEngine::configureDevice() {
  auto networkGroupsParams = device_->create_configure_params(*hef_);
  auto configuredNetworkGroup = device_->configure(*hef_, networkGroupsParams.value());
  if (!configuredNetworkGroup) {
    ERROR(layerName_, " ", "Ошибка конфигурации устройства: ", configuredNetworkGroup.status());
    return false;
  }

  configuredNetworkGroup_ = std::move(configuredNetworkGroup.value());

  return true;
}

/// @brief Вывод.
/// @param data
/// @param boxes
/// @return
bool InferenceEngine::run(const FrameData &data, std::vector<BoundingBox> &boxes) {
  return true;
}
