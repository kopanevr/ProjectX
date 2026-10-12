#include "Pipeline.hpp"

//

/// @brief
/// @param yoloModelPath Путь к модели .
/// @param llmModelPath Путь к модели LLM.
void Pipeline::start(const std::string &yoloModelPath, const std::string &llmModelPath) {
  yoloEngineThread_ = st::jthread();
}

/// @brief Остановка.
void Pipeline::stop() {

}