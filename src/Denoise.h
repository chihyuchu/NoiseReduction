#pragma once
#include <opencv2/opencv.hpp>

void NLMeans(cv::Mat &srcImage, cv::Mat &dstImage,
			int templeteWindowSize,	int searchWindowSize, double sigma);

