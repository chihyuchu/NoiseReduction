#include "Denoise.h"
#include <vector>
#include <math.h>
#include <numeric>

static double similarity(const std::vector<int> vec1, const std::vector<int> vec2, double sigma)
{
	assert(vec1.size() == vec2.size());
	double d_similarity;
	int sum=0;
	for (int i = 0; i < vec1.size(); i++)
	{
		sum += (vec1[i] - vec2[i]) * (vec1[i] - vec2[i]);
	}
	d_similarity = std::exp(-1.0f * (double)(sum) / (sigma * sigma));
	return d_similarity;
}


static double vecSum(const std::vector<double> vec)
{
	double sum_of_elems=0;
	for (int i = 0; i < vec.size(); i++)
		sum_of_elems += vec[i];
	return sum_of_elems;
}

static double vecMultiply(const std::vector<int> vec1, const std::vector<double> vec2)
{
	double sum = 0;
	for (int i = 0; i < vec1.size(); i++)
	{
		sum += (double)(vec1[i]) * vec2[i];
	}
	return sum;
}


void NLMeans(cv::Mat &srcImage, 
		     cv::Mat &dstImage, 
		     int templeteWindowSize, 
		     int searchWindowSize,
		     double sigma)
{	
	if (templeteWindowSize > searchWindowSize) {
		std::cout << "searchWindowSize should be equal or larger than templateWindowSize" << std::endl;
		return;
	}

	// Declare some variables
	int half_searchWindowSize = searchWindowSize >> 1;
	int half_templateWindowSize = templeteWindowSize >> 1;
	int num_pad_pixel = half_searchWindowSize + half_templateWindowSize;
	int num_pixel_searchWindow = searchWindowSize * searchWindowSize;
	int denoised_pixel;

	std::vector<int> v_searchWindow;
	std::vector<int> v_templateWindow;
	std::vector<int> v_templatePixel;
	std::vector<double> v_similarity;

 	cv::Mat srcImage_pad;
	cv::copyMakeBorder(srcImage, srcImage_pad, 
		num_pad_pixel, num_pad_pixel, num_pad_pixel, num_pad_pixel, 
		cv::BORDER_DEFAULT);

	dstImage = cv::Mat::zeros(srcImage.size(), srcImage.type());

	for (int channel = 0; channel < 3; channel++)
		for (int j = num_pad_pixel; j < srcImage.rows + num_pad_pixel; j++)
		{
			for (int i = num_pad_pixel; i < srcImage.cols + num_pad_pixel; i++)
			{
				// search window loop
				v_searchWindow.clear();
				v_templateWindow.clear();
				v_templatePixel.clear();
				v_similarity.clear();

				// get template window
				for (int s_j = -1 * half_templateWindowSize; s_j < half_templateWindowSize + 1; s_j++)
				{
					for (int s_i = -1 * half_templateWindowSize; s_i < half_templateWindowSize + 1; s_i++)
					{
						v_templateWindow.push_back(srcImage_pad.at<cv::Vec3b>(i + s_i, j + s_j)[channel]);
					}
				}

				// Main Loop: get similariy and for each search window
				for (int s_j = -1 * half_searchWindowSize + half_templateWindowSize; s_j < half_searchWindowSize + 1 - half_templateWindowSize; s_j++)
				{
					for (int s_i = -1 * half_searchWindowSize + half_templateWindowSize; s_i < half_searchWindowSize + 1 - half_templateWindowSize; s_i++)
					{
						v_searchWindow.clear();
					
						//if ((s_j == 0) && (s_i == 0)) goto DONE;

						for (int s_s_j = -1 * half_templateWindowSize; s_s_j < half_templateWindowSize + 1; s_s_j++)
						{
							for (int s_s_i = -1 * half_templateWindowSize; s_s_i < half_templateWindowSize + 1; s_s_i++)
							{
								v_searchWindow.push_back(srcImage_pad.at<cv::Vec3b>(i + s_i + s_s_i, j + s_j + s_s_j)[channel]);
							}
						}
						v_similarity.push_back(similarity(v_templateWindow, v_searchWindow, sigma));
						v_templatePixel.push_back(srcImage_pad.at<cv::Vec3b>(i + s_i, j + s_j)[channel]);
						//DONE:
					}
				}

				// Calculate denoised pixel and fill into dstImage

				denoised_pixel = (vecMultiply(v_templatePixel, v_similarity) / vecSum(v_similarity));
				dstImage.at<cv::Vec3b>(i - num_pad_pixel, j - num_pad_pixel)[channel] = denoised_pixel;

		}
	}
}
