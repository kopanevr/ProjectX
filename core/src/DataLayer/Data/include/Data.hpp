/**
 * @file
 * @brief
 */

#pragma once

//

#include <array>

//

#include "Config.hpp"

//

#include "BoundingBox.hpp"

//

#include "opencv4/opencv2/opencv.hpp"

//

/// @brief
struct FrameData {
/// @brief Индекс.
long long index;
/// @brief Данные.
cv::Mat data;
};

/// @brief
struct FrameDisplayData {
/// @brief
FrameDisplayData() {
  boxes.resize(Config::numElements);
}

/// @brief
FrameData data;
/// @brief
std::vector<BoundingBox> boxes;
};
