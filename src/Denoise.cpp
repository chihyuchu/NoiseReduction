#include "Denoise.h"
#include <vector>


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
	int searchWindowAvg;
	int half_searchWindowSize = searchWindowSize >> 1;
	int half_templateWindowSize = templeteWindowSize >> 1;
	int num_pad_pixel = half_searchWindowSize + half_templateWindowSize;
	int num_pixel_searchWindow = searchWindowSize * searchWindowSize;

 	cv::Mat srcImage_pad;
	cv::copyMakeBorder(srcImage, srcImage_pad, 
		num_pad_pixel, num_pad_pixel, num_pad_pixel, num_pad_pixel, 
		cv::BORDER_DEFAULT);

	dstImage = cv::Mat::zeros(srcImage.size(), srcImage.type());
	

	for (int j = num_pad_pixel; j < srcImage_pad.rows; j++)
	{
		for (int i = num_pad_pixel; i < srcImage_pad.cols; i++)
		{
			// search window loop
			searchWindowAvg = 0;
			for (int s_j = -1 * half_searchWindowSize; s_j < half_searchWindowSize + 1; s_j++)
			{
				for (int s_i = -1 * half_searchWindowSize; s_i < half_searchWindowSize + 1; s_i++)
				{
					searchWindowAvg += srcImage_pad.at<cv::Vec3b>(i, j)[0];
				}
			}
			searchWindowAvg = searchWindowAvg / (searchWindowSize * searchWindowSize);

		}
	}



}
