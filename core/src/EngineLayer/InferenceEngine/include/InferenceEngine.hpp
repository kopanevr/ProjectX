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

#include "hailo/hailort.hpp"

//

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
  virtual bool run(const FrameData &data, std::vector<BoundingBox> &boxes) = 0;

protected:
  /// @brief
  const std::string layerName_ = "[Inference Engine]";

private:

};

/// @brief
class InferenceEngine final : public IInferenceEngine {
public:
  /// @brief Конструктор.
  InferenceEngine() = default;
  /// @brief Деструктор.
  ~InferenceEngine() = default;

  /// @param fileName Имя файла.
  /// @param inputName Имя входа.
  /// @param outputName Имя выхода.
  /// @return Результат инициализации.
  bool initialize(const std::string &fileName, const std::string &inputName, const std::string &outputName) override;

  /// @brief Вывод.
  /// @return
  bool run(const FrameData &data, std::vector<BoundingBox> &boxes) override;

private:
  /// @brief
  /// @return
  bool createDevice();

  /// @brief
  /// @param fileName Имя файла.
  /// @return
  bool loadModel(const std::string &fileName);

  /// @brief
  /// @return
  bool configureDevice();

  /// @brief
  /// @return
  bool createInputVstreams();

  /// @brief
  /// @return
  bool createOutputVstreams();

  /// @brief
  /// @return
  bool activateNetwork();

  /// @brief
  /// @return
  void prepareBuffers();

  /// @brief
  /// @return
  void prepareOutputData(std::vector<BoundingBox> &boxes);

private:
  /// @brief
  std::string inputName_;
  /// @brief
  std::string outputName_;

  /// @brief
  std::unique_ptr<hailort::Device> device_;

  /// @brief
  std::unique_ptr<hailort::Hef> hef_;

  /// @brief
  std::shared_ptr<hailort::ConfiguredNetworkGroup> configuredNetworkGroup_;

  /// @brief
  std::unique_ptr<hailort::InputVStream> inputVStream_;
  /// @brief
  std::unique_ptr<hailort::OutputVStream> outputVStream_;

  /// @brief
  std::unique_ptr<hailort::ActivatedNetworkGroup> activatedNetworkGroup_;

  /// @brief
  void *deviceInput_{nullptr};
  /// @brief
  void *deviceOutput_{nullptr};

  /// @brief
  std::vector<float> outputData_;

};
