#ifndef PROC_H
#define PROC_H

#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QPainter>
#include <QtMath>

namespace proc {
    QImage Binaryzation(const QImage &origin);			//二值化
    QImage Invert(const QImage &origin);                //反色相
    QImage GreyScale(const QImage &origin);             //灰度化
	QImage LaplaceSharpen(const QImage &origin);		//拉普拉斯锐化
	QImage SobelEdge(const QImage &origin);				//sobel边缘检测
    QImage PrewittEdge(const QImage &origin);			//Prewitt边缘检测
}

#endif // PROC_H
