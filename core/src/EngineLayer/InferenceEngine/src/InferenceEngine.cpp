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
  INFO(layerName_, " ", "Файл модели: ", fileName);

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

  if (!createInputVstreams() || !createOutputVstreams()) {
    return false;
  }
  INFO(layerName_, " ", "Входные и выходные успешно созданы!");

  if (!activateNetwork()) {
    return false;
  }
  INFO(layerName_, " ", "Сеть успешно активирована!");

  prepareBuffers();
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
  auto hef = hailort::Hef::create(fileName);
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

  configuredNetworkGroup_ = std::move(configuredNetworkGroup.value().at(0));

  return true;
}

/// @brief
/// @return
bool InferenceEngine::createInputVstreams() {
  auto inputVirtualStreamParams = configuredNetworkGroup_->make_input_vstream_params(
      false,
      HAILO_FORMAT_TYPE_UINT8,
      HAILO_DEFAULT_VSTREAM_TIMEOUT_MS, // Время записи в поток.
      HAILO_DEFAULT_VSTREAM_QUEUE_SIZE
    );
  if (!inputVirtualStreamParams) {
    return false;
  }

  auto inputVstreams = hailort::VStreamsBuilder::create_input_vstreams(*configuredNetworkGroup_, inputVirtualStreamParams.value());
  if (!inputVstreams) {
    return false;
  }

  auto inputVstream = std::move(inputVstreams.value().at(0));

  inputVStream_ = std::unique_ptr<hailort::InputVStream>(new (std::nothrow) hailort::InputVStream(std::move(inputVstream)));

  return true;
}

/// @brief
/// @return
bool InferenceEngine::createOutputVstreams() {
  auto outputVirtualStreamParams = configuredNetworkGroup_->make_output_vstream_params(
    false,
    HAILO_FORMAT_TYPE_FLOAT32,
    HAILO_DEFAULT_VSTREAM_TIMEOUT_MS,
    HAILO_DEFAULT_VSTREAM_QUEUE_SIZE
  );
  if (!outputVirtualStreamParams) {
    return false;
  }

  auto outputVstreams = hailort::VStreamsBuilder::create_output_vstreams(*configuredNetworkGroup_, outputVirtualStreamParams.value());
  if (!outputVstreams) {
    return false;
  }

  auto outputVstream = std::move(outputVstreams.value().at(0));

  outputVStream_ = std::unique_ptr<hailort::OutputVStream>(new (std::nothrow) hailort::OutputVStream(std::move(outputVstream)));

  return true;
}

/// @brief
/// @return
bool InferenceEngine::activateNetwork() {
  auto activatedNetworkGroup = configuredNetworkGroup_->activate();
  if (!activatedNetworkGroup) {
    ERROR(layerName_, " ", "Ошибка активации сети: ", activatedNetworkGroup.status());
    return false;
  }

  activatedNetworkGroup_ = std::move(activatedNetworkGroup.value());

  return true;
}

/// @brief
/// @return
void InferenceEngine::prepareBuffers() {
  outputData_.resize(outputVStream_->get_frame_size() / sizeof(float));
}

/// @brief
/// @return
void InferenceEngine::prepareOutputData(std::vector<BoundingBox> &boxes) {
  const BoundingBox *boundingBoxes = reinterpret_cast<const BoundingBox *>(outputData_.data());

  for (size_t i = 0; i < Config::numElements; i++) {
    boxes.push_back(boundingBoxes[i]);
  }
}

/// @brief Вывод.
/// @param data
/// @param boxes
/// @return
bool InferenceEngine::run(const FrameData &data, std::vector<BoundingBox> &boxes) {
  auto status = inputVStream_->write(hailort::MemoryView(static_cast<void *>(data.data.data), data.data.total() * data.data.elemSize()));
  if (status != HAILO_SUCCESS) {
    return false;
  }

  status = outputVStream_->read(hailort::MemoryView(outputData_.data(), outputData_.size()));
  if (status != HAILO_SUCCESS) {
    return false;
  }

  //
  prepareOutputData(boxes);

  return true;
}
