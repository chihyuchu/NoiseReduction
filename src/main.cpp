#include <iostream>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\types_c.h>

#include "GenNoise.h"
#include "Denoise.h"

int main()
{
	// --------------- Initialization -------------------
	cv::Mat srcImage, nsyImage;
	std::string ImagePath;

	// Assign image here:
	ImagePath = "./Img/test.png";

	// Loading the image
	srcImage = cv::imread(ImagePath, 1);
	if (srcImage.empty()) {
		std::cout << "[ERROR] Could not open or find the image" << std::endl;
		std::cin.get();
		return -1;
	}

	cv::Mat test = srcImage(cv::Range(10, 20), cv::Range(31, 41));
	
	AddNoise(srcImage, nsyImage, 10.0, 0.0);

	// --------------- Noise Reduction Algorithm -------------------
	// Reference: https://github.com/GitHberChen/NL-means

	// 1. Gaussian Filter Denoise
	cv::Mat deNsy_Gaussian;
	cv::GaussianBlur(srcImage, deNsy_Gaussian, cv::Size(7, 7), 3.0, 3.0);

	// 2. Bilateral Filter
	cv::Mat deNsy_Bilateral;
	cv::bilateralFilter(srcImage, deNsy_Bilateral, 7, 35, 5);

	// 3. NLMeans
	cv::Mat deNsy_NLMeans;
	NLMeans(srcImage, deNsy_NLMeans, 3, 7, 20);


	// --------------- Evaluation -------------------
	// Calculate PSNR for noisy image
	double nsyPSNR;
	nsyPSNR = calcPSNR(srcImage, nsyImage);
	std::cout << "PSNR of noisy image:             " << nsyPSNR << "\n";

	// Calculate PSNR for gaussian filtered image
	double gaussian_PSNR;
	gaussian_PSNR = calcPSNR(srcImage, deNsy_Gaussian);
	std::cout << "PSNR of gaussian filtered image: " << gaussian_PSNR << "\n";

	// Calculate PSNR for bilateral filetered image
	double bilateral_PSNR;
	bilateral_PSNR = calcPSNR(srcImage, deNsy_Bilateral);
	std::cout << "PSNR of bilateral filtered image: " << bilateral_PSNR << "\n";

	// Calculate PSNR for NLMeans filetered image
	double NLMeans_PSNR;
	NLMeans_PSNR = calcPSNR(srcImage, deNsy_NLMeans);
	std::cout << "PSNR of NLMeans filtered image: " << NLMeans_PSNR << "\n";

		
	// --------------- Plot and Write Image-------------------
	cv::namedWindow("source Image");
	cv::namedWindow("noisy Image");
	cv::namedWindow("gaussian filtered Image");
	cv::namedWindow("bilateral filtered Image");
	cv::namedWindow("NLMeans filtered Image");
	//cv::namedWindow("NLMeans Image");

	cv::imshow("source Image", srcImage);
	cv::imshow("noisy Image", nsyImage);
	cv::imshow("gaussian filtered Image", deNsy_Gaussian);
	cv::imshow("bilateral filtered Image", deNsy_Bilateral);
	cv::imshow("NLMeans filtered Image", deNsy_NLMeans);
	//cv::imshow("NLMeans Image", nsyImage);
	
	cv::imwrite("./Img/test_nsy.jpg", nsyImage);
	cv::imwrite("./Img/test_gauss.jpg", deNsy_Gaussian);
	cv::imwrite("./Img/test_bilateral.jpg", deNsy_Bilateral);
	cv::imwrite("./Img/test_NLMeans.jpg", deNsy_NLMeans);

	cv::waitKey(0);

	std::cin.get();
	return 0;
}

