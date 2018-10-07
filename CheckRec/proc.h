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

using namespace cv;

namespace proc {
    QImage Binaryzation(const QImage &origin);			//二值化
    QImage Invert(const QImage &origin);                //反色相
    QImage GreyScale(const QImage &origin);             //灰度化
	QImage LaplaceSharpen(const QImage &origin);		//拉普拉斯锐化
	QImage SobelEdge(const QImage &origin);				//sobel边缘检测
    QImage PrewittEdge(const QImage &origin);			//Prewitt边缘检测

    QStringList readDir(const QString& path);
    void sortFiles(QStringList& imgFiles);
    bool generateXMLDataFile(const QStringList& imgFiles);
    bool trainModel(Ptr<ml::KNearest>& kNearest);
    bool useModel(const QString& fileName,int& charRec);
}

#endif // PROC_H
