#include "histogram.h"

#include <sstream>
#include <iostream>
#include <common.h>

Histogram::Histogram(QWidget * parent) : QLabel(parent)
{
    // index 0 to 255 => count of image's pixels for this value
    // index 256 => maximum value
    // index 257/258 => total value of the dark/light component
    bw.resize(259); red.resize(258); green.resize(258); blue.resize(258);
    bw.fill(0);  red.fill(0); green.fill(0); blue.fill(0);
    bw[256] = -1; red[256] = -1;
    green[256] = -1; blue[256] = -1;
}

Histogram::Histogram(QWidget * parent, Histogram * hstgrm) : QLabel(parent)
{
    for(int i = 0; i<258; i++) {
        bw[i] = hstgrm->bw[i];        red[i] = hstgrm->red[i];
        green[i] = hstgrm->green[i];  blue[i] = hstgrm->blue[i];
    }
    bw[258] = hstgrm->bw[258];
}

void Histogram::computeHstgrm(QImage img)
{
    if (!img.isNull()) {
        int bwVal, redVal, greenVal, blueVal;
        for(int i = 0; i<img.height(); i++) {
            for(int j = 0; j<img.width(); j++) {
                bwVal = qGray(img.pixel(j, i));
                redVal = qRed(img.pixel(j, i));
                greenVal = qGreen(img.pixel(j, i));
                blueVal = qBlue(img.pixel(j, i));

                bw[bwVal]++;  red[redVal]++;
                green[greenVal]++;  blue[blueVal]++;
            }
        }

        for(int i = 0; i<RGBSize; i++){
            // maximum values
            if (bw[256] < bw[i])  bw[256] = bw[i];
            if (red[256] < red[i]) red[256] = red[i];
            if (green[256] < green[i]) green[256] = green[i];
            if (blue[256] < blue[i]) blue[256] = blue[i];
            // values of colour components
            red[257] += i*red[i]; green[257] += i*green[i]; blue[257] += i*blue[i];

            // values of dark and light component
            if (i <= 127)  bw[257] += (127-i)*bw[i];
            else   bw[258] += (i-127)*bw[i];
        }
    }
}

void Histogram::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    int step = 100;             // distance between histograms
    int height = 255+10;        // histograms height
    int xBase = 99;             // x coordinate of the first histogram origin
    int yBase = 30+height+1;    // y coordinate of the first histogram origin

    QPainter painter(this);
    painter.setPen(Qt::black);

    // 显示在第一行
    // bw hstgrm
    if (bw[256] != -1)
        drawgray(xBase, yBase, height);
    else
        painter.drawText(xBase, yBase-height/2+5, tr("无法加载灰度分量直方图"));

    // red hstgrm
    if (red[256] != -1)
        drawred(xBase+step+height, yBase, height);
    else
        painter.drawText(xBase+step+height+1-height/2+5, yBase, tr("无法加载红色分量直方图."));

    // 显示在第二行
    // green hstgrm
    if (green[256] != -1)
        drawgreen(xBase, yBase+step+height+1, height);
    else
        painter.drawText(xBase, yBase+(step+height+1), tr("无法加载绿色分量直方图."));
    // blue hstgrm
    if (blue[256] != -1)
        drawblue(xBase+step+height, yBase+step+height+1, height);
    else
        painter.drawText(xBase+step+height, yBase+step+height+1, tr("无法加载蓝色分量直方图."));
}

void Histogram::drawgray(int xBase, int yBase, int height)
{
    drawHstgrm(xBase, yBase, height, RGB::BW);
}

void Histogram::drawred(int xBase, int yBase, int height)
{
    drawHstgrm(xBase, yBase, height, RGB::R);
}

void Histogram::drawgreen(int xBase, int yBase, int height)
{
    drawHstgrm(xBase, yBase, height, RGB::G);
}

void Histogram::drawblue(int xBase, int yBase, int height)
{
    drawHstgrm(xBase, yBase, height, RGB::B);
}

void Histogram::drawHstgrm(int xBase, int yBase, int height, RGB type)
{
    const char* str[] = {"灰度分量直方图", "红色分量直方图", "绿色分量直方图", "蓝色量直方图"};
    QPainter painter(this);
    QVector<int> vec;
    int index;
    switch(type){
        case RGB::BW: painter.setPen(Qt::darkGray);  vec = bw;    index = 0; break;
        case RGB::R : painter.setPen(Qt::darkRed);   vec = red;   index = 1; break;
        case RGB::G : painter.setPen(Qt::darkGreen); vec = green; index = 2; break;
        case RGB::B : painter.setPen(Qt::darkBlue);  vec = blue;  index = 3;
    }
    //painter.setPen(Qt::darkGray);  //1
    int max = Max({bw[256], red[256], green[256], blue[256]});
    // drawing the histogram
    for(int i = 0; i<256; i++)
    {
        painter.drawLine(xBase+1+i, yBase, xBase+1+i,
            yBase-(float)(256./max)*(float)vec[i]); //2
    }
    painter.drawText(xBase, yBase+25, tr("深色"));
    painter.drawText(xBase+220, yBase+25, tr("浅色"));
    painter.setPen(Qt::black);
    painter.drawText(xBase+45, yBase-height-10, tr(str[index])); //3
    //painter.drawText(xBase+100, yBase+15, tr("强度大小"));
    painter.drawText(xBase-80, yBase-height/2+5, tr("像素点数"));

    // abscissa
    painter.drawLine(xBase, yBase, xBase+256+1, yBase);
    painter.drawLine(xBase, yBase+1, xBase+256+1, yBase+1);
    // left ordinate
    painter.drawLine(xBase, yBase, xBase, yBase-height);
    painter.drawLine(xBase-1, yBase, xBase-1, yBase-height);
    // right ordinate
    painter.drawLine(xBase+256+1, yBase, xBase+256+1, yBase-height);
    painter.drawLine(xBase+256+2, yBase, xBase+256+2, yBase-height);
    // left ordinate arrow
    painter.drawLine(xBase, yBase-height, xBase+4, yBase-height+7);
    painter.drawLine(xBase-1, yBase-height, xBase-1-4, yBase-height+7);
    // right ordinate arrow
    painter.drawLine(xBase+256+1, yBase-height, xBase+256+1-4, yBase-height+7);
    painter.drawLine(xBase+256+2, yBase-height, xBase+256+2+4, yBase-height+7);
}
