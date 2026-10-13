/**
 * @file
 * @brief
 */

#pragma once

//

#include "opencv4/opencv2/opencv.hpp"

//

struct FrameData {
/// @brief Индекс.
long long index;
/// @brief Данные.
cv::Mat data;
};
