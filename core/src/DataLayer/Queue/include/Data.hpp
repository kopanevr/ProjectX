/**
 * @file
 * @brief
 */

#pragma once

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
FrameData data;
/// @brief
std::vector<BoundingBox> boxes;
};
