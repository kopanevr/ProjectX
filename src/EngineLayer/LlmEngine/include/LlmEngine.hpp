/**
 * @file
 * @brief
 */

#pragma once

//

#include <string>

//

#include "Logger.hpp"

//

/// @brief
class ILlmEngine {
public:
  /// @brief Деструктор.
  virtual ~ILlmEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize(const std::string &llmModelPath) = 0;

protected:
  /// @brief
  const std::string layerName_ = "[LLM Engine] ";

private:

};

/// @brief
class LlmEngine final : public ILlmEngine {
public:
  /// @brief Деструктор.
  ~LlmEngine() = default;

  /// @brief Инициализация.
  /// @return Результат инициализации.
  virtual bool initialize(const std::string &llmModelPath) override {
    LOG(layerName_, " ", "Загрузка модели: ", llmModelPath);

    //
  }

private:

};
