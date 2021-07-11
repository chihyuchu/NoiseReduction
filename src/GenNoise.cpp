#include "GenNoise.h"

void AddNoise_Gaussian(cv::Mat& srcImage, cv::Mat& dstImage, double sigma)
{
	cv::Mat src_copy;
	srcImage.convertTo(src_copy, CV_16S);
	
	cv::Mat noise(src_copy.size(), CV_16S);
	cv::randn(noise, 0, sigma);
	
	cv::Mat dstImage_copy = src_copy + noise;
	dstImage_copy.convertTo(dstImage, CV_8U);
;}

void AddNoise_Pepper(cv::Mat& srcImage, cv::Mat& dstImage, double pepper_ratio)
{

    // TODO: check efficiency btw the pixel value method
    // a. get row from uchar*, then get the column for pixel value
    // b. get pixel value through Image.at<Vec3b>(i,j)

    cv::RNG rng;
    double a1, a2;
    for (int j = 0; j < srcImage.rows; j++) {
        for (int i = 0; i < srcImage.cols; i++) {
            a1 = rng.uniform((double)0, (double)1);
            if (a1 > pepper_ratio)
                dstImage.at<cv::Vec3b>(i, j) = srcImage.at<cv::Vec3b>(i, j);
            else {
                a2 = rng.uniform((double)0, (double)1);
                if (a2 > 0.5) dstImage.at<cv::Vec3b>(i, j) = 0;
                else dstImage.at<cv::Vec3b>(i, j) = 255;
            }
        }
    }
}


void AddNoise(cv::Mat& srcImage, cv::Mat& dstImage, double sigma, double pepper_ratio)
{
    if (srcImage.channels() == 1) // if single channel
    {
        AddNoise_Gaussian(srcImage, dstImage, sigma);
        if (pepper_ratio != 0)
        {
            AddNoise_Pepper(dstImage, dstImage, pepper_ratio);
            return;
        }
    }
    else
    {
        cv::Mat src_array[3];
        cv::Mat dst_array[3];
        cv::split(srcImage, src_array);
        for (int i = 0; i < srcImage.channels(); i++)
        {
            AddNoise_Gaussian(src_array[i], dst_array[i], sigma);
            if (pepper_ratio != 0)
            {
                AddNoise_Pepper(dst_array[i], dst_array[i], pepper_ratio);
            }
        }

        cv::merge(dst_array, 3, dstImage); 
    }
}




static double getPSNR(cv::Mat& srcImage, cv::Mat& dstImage)
{
    double sse = 0;
    double mse, psnr;
    for (int j = 0; j < srcImage.rows; j++)
    {
        uchar* d = dstImage.ptr(j);
        uchar* s = srcImage.ptr(j);
        for (int i = 0; i < srcImage.cols; i++)
            sse += ((double)(d[i] - s[i]) * (double)(d[i] - s[i]));
    }

    if (sse == 0)
        return 0;
    else
    {
        mse = sse / ((double)srcImage.cols * (double)srcImage.rows);
        psnr = 10 * log10(((double)255 * (double)255) / mse);
        return psnr;
    }
}

double calcPSNR(cv::Mat& srcImage, cv::Mat& dstImage) {
    cv::Mat src_copy;
    cv::Mat dst_copy;
    if (srcImage.channels() == 1) {
        srcImage.copyTo(src_copy);
        dstImage.copyTo(dst_copy);
    }
    else {
        cv::cvtColor(srcImage, src_copy, CV_BGR2YUV);
        cv::cvtColor(dstImage, dst_copy, CV_BGR2YUV);
    }
    double sn = getPSNR(src_copy, dst_copy);
    return sn;
}