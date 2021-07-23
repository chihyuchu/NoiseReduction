# Noise Reduction

This is a simple side-project for noise reduction, just for practices for a classic ISP issue and openCV/C++ coding
For this project, I will keep updating different algorithms about NR, and do some analysis in README.md

Current Algorithm:
1. Gaussian Filter (default opencv function)
2. Bilateral Filter (default opencv function, maybe implement by myself in the future)
3. NL-Means (implemented)
4. BM3D
5. ...


## Evaluate algo on RGB Lena (512 x 512):
PSNR of noisy image:             31.4529  
PSNR of gaussian filtered image: 31.0715  
PSNR of bilateral filtered image: 39.2667  
PSNR of NLMeans filtered image: 42.4844  

Reference:
1. https://github.com/GitHberChen/NL-means
