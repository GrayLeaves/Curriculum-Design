#include "../proc/proc.h"
#include "../common.h"
#include <opencv.hpp>
#include <opencv2\opencv.hpp>
#include <QDebug>
#include <QDir>
#include <vector>

using namespace cv;
using namespace  std;

template <typename T>
using Matrix = vector<vector<T> >;
Ptr<ml::KNearest> kNearest;

QStringList proc::readDir(const QString& path){
    QDir dir(path);
    QStringList typeFilters;
    typeFilters << "*.png";
    QStringList imgFiles = dir.entryList(typeFilters, QDir::Files|QDir::Readable, QDir::Name);
    for(QString& file : imgFiles)
        qDebug() << file;
    return imgFiles;
}

bool proc::genLabels(const QString& path){
    QStringList imgFiles = readDir(path);
    if(!imgFiles.empty())
        sortFiles(imgFiles);
    else
        return false;
    if(generateXMLDataFile(path,imgFiles))
        return true;
    else
        return false;
}

void proc::sortFiles(QStringList& imgFiles){
    vector<string> imgs;
    for(int i=0; i<imgFiles.size(); ++i) imgs.push_back(imgFiles[i].toStdString());
    stable_sort(imgs.begin(),imgs.end(),greater<string>());
    imgFiles.clear();
    for(auto &s : imgs){
        imgFiles << QString::fromStdString(s);
    }
}

bool proc::generateXMLDataFile(const QString& path,const QStringList& imgFiles){
    int len = imgFiles.size();
    if(len % 10 != 0) return false;

    //生成训练标签
    Mat charToRec;
    Mat charROI;
    for(int i=0; i<10; ++i){ //0-9
        for(int j=0; j<len/10; ++j)//how many pictures for each number
            charToRec.push_back(i);
    }
    string fullPath = path.toStdString();
    for(int i=0; i<len; ++i){
        QString fileName = imgFiles[i];
        string strFileName = fileName.toStdString();
        string fullPathFile = fullPath+strFileName;
        Mat matROI = imread(fullPathFile,0);
        if(matROI.empty()){
            qDebug() << "Can't read file :" << path << fileName;
            return false;
        }
        Mat matROIClone = matROI.clone();
        Matrix<Point> contours;
        vector<Vec4i> hierarchy;
        findContours(matROI, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        Rect boundRect = boundingRect(contours[0]);
        Mat img = matROIClone(boundRect).clone();
        //QSize(140,180)
        resize(img, img, Size(70, 90));
        //winSize(64,128), blockSize(16,16), blockStride(8,8),cellSize(8,8), nbins(9)
        HOGDescriptor *hog = new HOGDescriptor(Size(70, 90), Size(14, 18), Size(7, 9), Size(7, 9), 9);
        vector<float> descriptors;
        hog->compute(img, descriptors);
        Mat dst(1, static_cast<int>(descriptors.size()), CV_32FC1, descriptors.data());
        charROI.push_back(dst);
        delete_s(hog);
    }

    FileStorage labels("labels.xml", FileStorage::WRITE);
    if (!labels.isOpened()) {
            qDebug() << "Can't Open file : labels.xml";
            return false;
    }
    labels << "labels" << charToRec;
    labels.release();
    FileStorage trainImg("images.xml", FileStorage::WRITE);
    if (!trainImg.isOpened()) {
          qDebug() << "Can't Open file : images.xml";
          return false;
    }
    trainImg << "images" << charROI;
    trainImg.release();

    qDebug() << "Generate Successful.";
    return true;
}

bool proc::trainModel(Ptr<ml::KNearest>& kNearest)
{
    FileStorage labels("labels.xml", FileStorage::READ);
    if (!labels.isOpened()) {
        qDebug() << "Can't open labels.xml";
        return false;
    }
    Mat charToRec;
    labels["labels"] >> charToRec;
    labels.release();
    FileStorage TrainImg("images.xml", FileStorage::READ);
    if (!TrainImg.isOpened()) {
        qDebug() << "Can't open images.xml";
        return false;
    }
    Mat charROI;
    TrainImg["images"] >> charROI;
    TrainImg.release();
    //Train
    kNearest->train(charROI, ml::ROW_SAMPLE, charToRec);

    qDebug() << "Train Completed.";
    return true;
}

int proc::useModel(const QString& fileName){

    Ptr<ml::KNearest> kNearest = ml::KNearest::create();  //KNN
    bool isTrained = proc::trainModel(kNearest);
    if(!isTrained) return -1;

    string strFileName = fileName.toStdString();
    Mat matROI = imread(strFileName,0);
    Mat matROIClone = matROI.clone();
    Matrix<Point> contours;
    vector<Vec4i> hierarchy;
    findContours(matROI, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    Rect boundRect = boundingRect(contours[0]);
    Mat img = matROIClone(boundRect).clone();
    //QSize(140,180)
    resize(img, img, Size(70, 90));
    //winSize(64,128), blockSize(16,16), blockStride(8,8),cellSize(8,8), nbins(9)
    HOGDescriptor *hog = new HOGDescriptor(Size(70, 90), Size(14, 18), Size(7, 9), Size(7, 9), 9);
    vector<float> descriptors;
    hog->compute(img, descriptors);
    Mat dst(1, static_cast<int>(descriptors.size()), CV_32FC1, descriptors.data());

    Mat matRecChar(0, 0, CV_32F);
    kNearest->findNearest(dst, 1, matRecChar);
    int charRec = static_cast<int>(matRecChar.at<float>(0, 0));
    delete_s(hog);

    qDebug() << "The number is " << charRec;
    return charRec;
}
