#include "ImageException.h"
#include "img_pre.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <opencv\ml.h>

using namespace std;
using namespace cv;
using namespace cv::ml;

int main() {
    const int NOfPictures = 7; //图片数
	string image_p = "C:\\Users\\wuzhenguang\\Pictures\\checkcode\\";
	string save_p = "C:\\Users\\wuzhenguang\\Pictures\\data\\";
	/*
	Image img(image_p + "1.jpg");
	Mat Gray = img.toGray();
	Mat srcImage = Gray;
	if (!srcImage.data)
	{
		cout << "读取图片错误，请重新输入正确路径！\n";
		system("pause");
		return -1;
	}
    imshow("【源图像】", srcImage); 

	Mat dstImage;
	boxFilter(srcImage, dstImage, -1, Size(3, 3), Point(-1, -1), false); //false为非归一化情况  
	imshow("【盒式滤波/方框滤波】", dstImage);
	waitKey(0);

	blur(srcImage, dstImage, Size(8, 8));
	imshow("【均值滤波】", dstImage);
	waitKey(0);

	GaussianBlur(srcImage, dstImage, Size(9, 9), 0, 0);
	imshow("【高斯滤波】", dstImage);
    waitKey(0);

	medianBlur(srcImage, dstImage, 9);//中值滤波  
	imshow("【中值滤波】", dstImage);
	waitKey(0);

	bilateralFilter(srcImage, dstImage, 11, 11 * 11, 11 / 2);//双边滤波  
	imshow("【双边滤波】", dstImage);
	waitKey(0);*/
    
	for (int i = 1; i <= NOfPictures; i++) {
		Image img(image_p + to_string(i) + ".jpg");
		Mat Gray = img.toGray();
		Image black(Gray);
		black.RemoveLine(40);
		black.NaiveRemoveNoise(1);
		black.Binarization();
		vector<Image> single_codes = black.xProjectDivide(10, 20, false);
		//black.SaveImg(save_p + "demo.jpg");
		for (int j = 0; j < single_codes.size(); j++) {
			single_codes[j].SaveImg(save_p + to_string(i) + "_" + to_string(j) + ".jpg");
		}
	}
	return 0;
}