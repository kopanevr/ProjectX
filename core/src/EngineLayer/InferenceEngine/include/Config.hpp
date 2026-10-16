/**
 * @file
 * @brief
 */

#pragma once

//

#include <cstddef>

//

#include "BoundingBox.hpp"

//

/// @brief
namespace Config {
inline constexpr size_t inputBatch = 1;
inline constexpr size_t inputChannels = 3;
inline constexpr size_t inputHeight = 640;
inline constexpr size_t inputWidth = 640;

inline constexpr size_t inputSizeBytes = inputBatch * inputChannels * inputHeight * inputWidth * sizeof(float);

inline constexpr size_t outputBatch = 1;
inline constexpr size_t outputChannels = 300;
inline constexpr size_t outputElements = 6;

inline constexpr size_t outputSizeBytes = outputBatch * outputChannels * outputElements * sizeof(float);

inline constexpr size_t numElements = outputSizeBytes / sizeof(BoundingBox);
}
