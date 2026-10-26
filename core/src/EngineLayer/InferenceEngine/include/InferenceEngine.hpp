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

private:
  /// @brief
  std::string inputName_;
  /// @brief
  std::string outputName_;

  /// @brief
  void *deviceInput_{nullptr};
  /// @brief
  void *deviceOutput_{nullptr};

  /// @brief
  FrameDisplayData displayData_;

};
