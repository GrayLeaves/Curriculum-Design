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
    const int NOfPictures = 7; //ͼƬ��
	string image_p = "C:\\Users\\wuzhenguang\\Pictures\\checkcode\\";
	string save_p = "C:\\Users\\wuzhenguang\\Pictures\\data\\";
	/*
	Image img(image_p + "1.jpg");
	Mat Gray = img.toGray();
	Mat srcImage = Gray;
	if (!srcImage.data)
	{
		cout << "��ȡͼƬ����������������ȷ·����\n";
		system("pause");
		return -1;
	}
    imshow("��Դͼ��", srcImage); 

	Mat dstImage;
	boxFilter(srcImage, dstImage, -1, Size(3, 3), Point(-1, -1), false); //falseΪ�ǹ�һ�����  
	imshow("����ʽ�˲�/�����˲���", dstImage);
	waitKey(0);

	blur(srcImage, dstImage, Size(8, 8));
	imshow("����ֵ�˲���", dstImage);
	waitKey(0);

	GaussianBlur(srcImage, dstImage, Size(9, 9), 0, 0);
	imshow("����˹�˲���", dstImage);
    waitKey(0);

	medianBlur(srcImage, dstImage, 9);//��ֵ�˲�  
	imshow("����ֵ�˲���", dstImage);
	waitKey(0);

	bilateralFilter(srcImage, dstImage, 11, 11 * 11, 11 / 2);//˫���˲�  
	imshow("��˫���˲���", dstImage);
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