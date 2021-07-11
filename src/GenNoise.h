#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

void AddNoise_Gaussian(cv::Mat& srcImage, cv::Mat& dstImage, double sigma);

void AddNoise_Pepper(cv::Mat& srcImage, cv::Mat& dstImage, double pepper_ratio);

void AddNoise(cv::Mat& srcImage, cv::Mat& dstImage, double sigma, double pepper_ratio);

double calcPSNR(cv::Mat& srcImage, cv::Mat& dstImage);
