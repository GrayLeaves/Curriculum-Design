#ifndef PROC_H
#define PROC_H

#include <opencv.hpp>
#include <opencv2\opencv.hpp>
#include <QString>
#include <QStringList>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QtMath>

namespace proc {
    cv::Mat QImageToMat(QImage image);
    QImage MatToQImage(const cv::Mat& mat);
    QImage Binaryzation(const QImage &origin);	 //二值化
    QImage Invert(const QImage &origin);         //反色相
    QImage GreyScale(const QImage &origin);      //灰度化
    QImage LaplaceSharpen(const QImage &origin); //拉普拉斯锐化
    QImage SobelEdge(const QImage &origin);		 //sobel边缘检测

    bool genLabels(const QString& path);
    QStringList readDir(const QString& path);
    void sortFiles(QStringList& imgFiles);
    bool generateXMLDataFile(const QString& path,const QStringList& imgFiles);
    bool trainModel(cv::Ptr<cv::ml::KNearest>& kNearest);
    int useModel(const QString& fileName);
}

#endif // PROC_H
