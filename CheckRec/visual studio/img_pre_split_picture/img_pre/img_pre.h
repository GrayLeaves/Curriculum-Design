#pragma once
#ifndef __IMAGE_H__
#define __IMAGE_H__
#include <opencv2/opencv.hpp>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include <vector>
#include <string>
#include "opencv/cv.h"
#include "opencv/cxcore.h"
#include "opencv/highgui.h"
#include <vector>
#include <string>

using namespace std;

class Image {
public:
	
    Image() {};
    ~Image() {
        m_Mat.release();
    };
    //structure with a image file
    Image(const string);
    Image(const cv::Mat &pMat);
    //load a file into obj
    int LoadImg(const char *pFilename);
    //show the obj m_Mat on window
    void ShowInWindow(const string);
    //save the obj m_Mat as image file
    void SaveImg(const string, cv::Mat&);
    void SaveImg(const string);
    //RGB2GRAY
    cv::Mat toGray();
    //Binarization the gray map
    void Binarization();
    //using Hough line detect method to find the line
    /*
    * @nThsd is the threshold to judge if to
    *	     return the line
    */
    void RemoveLine(int nThsd = 40);
    //Get rid of noise by window detect
    /*
    * @pNum is the number of black pixel
    *     in a 3*3 window, default 1
    */
    void NaiveRemoveNoise(int pNum = 1);
    //Get rid of noise by contour detect
    /*
    * @pArea is the max area of a connected
    *     block, default 1
    */
    void ContoursRemoveNoise(double pArea = 1);
    static void ContoursRemoveNoise(cv::Mat &pMat, double pArea = 1);
    //Divide the map into characters by projecting the pixel in X-coordinate
    /*
    * @nMin_thsd is the min threshold of
    * 			distance in 2 characters and
    * 			default 10
    * @nMax_thsd is the max threahold and
    * 			default 20
    */
    vector<Image> xProjectDivide(int nMin_thsd = 10, int nMax_thsd = 20, bool show = true);
    //divide the map into characters by FloodFill
    /*
    * @nMin_area is the mininum area of
    *     a charactor, delete the area
    *     if its area below this value
    * @pData is the feature vector of e
    * 			-ach charactor divided
    * @pFilename is the class of each
    * 			marker charactor
    * @flag is the mark if show each di
    * 			-vided charactor
    */
    void FloodFillDivide(std::vector<std::string> &pData, int nMin_area = 30, char *pFilename = NULL, int flag = 0);
    //correct the tilted character by rotating the character from -15 degree to 15 degree
    //and choose the thinest degree
    void TiltCorrect();
    //release Mat OBJ
    void release() {
        m_Mat.release();
    }
    static void TiltCorrect(cv::Mat *pMat);
    //get channels
    int getChannel() const {
        return m_Mat.channels();
    }
private:
    int getHeight() const {
        return m_Mat.rows;
    }
    int getWidth() const {
        return m_Mat.cols;
    }
    int getPixel(int x, int y) const {
        return *(m_Mat.data + y*m_Mat.step[0] + x);
    }
    void setPixel(int x, int y, uchar pValue);
    cv::Mat m_Mat;
};

#endif		//__IMAGE_H__


/*
#ifndef IMG_PRE
#define IMG_PRE
class Img_pre {
public:
    Img_pre(std::string);
    void show_it(cv::Mat);		
    int get_c();
    int get_r();
    cv::Mat get_mat();

private:
    std::string img_p;
    cv::Mat image;
};
#endif // !IMG_PRE
*/