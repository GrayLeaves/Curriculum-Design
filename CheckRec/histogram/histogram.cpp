#include "histogram.h"

#include <sstream>
#include <iostream>


Histogram::Histogram(QWidget * parent) : QLabel(parent)
{
    // index 0 to 255 => count of image's pixels for this value
    // index 256 => maximum value
    // index 257/258 => total value of the dark/light component
    for(int i = 0;i<256;i++) {
        bw[i] = 0; red[i] = 0;
        green[i] = 0; blue[i] = 0;
    }
    bw[256] = -1; red[256] = -1;
    green[256] = -1; blue[256] = -1;
    red[257] = 0; green[257] = 0; blue[257] = 0;
    bw[257] = 0; bw[258] = 0;
}

Histogram::Histogram(QWidget * parent, Histogram * hstgrm) : QLabel(parent)
{
    for(int i = 0;i<258;i++) {
        bw[i] = hstgrm->bw[i];         red[i] = hstgrm->red[i];
        green[i] = hstgrm->green[i];  blue[i] = hstgrm->blue[i];
    }
    bw[258] = hstgrm->bw[258];
}

void Histogram::computeHstgrm(QImage img)
{
    if (!img.isNull()) {
        for(int i = 0;i<img.height();i++) {
            for(int j = 0;j<img.width();j++) {
                int bwValue = qGray(img.pixel(j, i));
                int redValue = qRed(img.pixel(j, i));
                int greenValue = qGreen(img.pixel(j, i));
                int blueValue = qBlue(img.pixel(j, i));

                bw[bwValue]++;  red[redValue]++;
                green[greenValue]++;  blue[blueValue]++;
            }
        }

        for(int i = 0;i<256;i++){
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

    int step = 100;              // distance between histograms
    int height = 255+10;        // histograms height
    int xBase = 99;             // x coordinate of the first histogram origin
    int yBase = 30+height+1;    // y coordinate of the first histogram origin

    QPainter painter(this);
    painter.setPen(Qt::black);

    // 显示在第一行
    // bw hstgrm
    if (bw[256] != -1)
        drawbw(xBase, yBase, height);
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

void Histogram::drawbw(int xBase, int yBase, int height)
{
    QPainter painter(this);
    painter.setPen(Qt::darkGray);
    float max = bw[256];
    if (max < red[256])
        max = red[256];
    if (max < green[256])
        max = green[256];
    if (max < blue[256])
        max = blue[256];
    // drawing the histogram
    for(int i = 0;i<256;i++)
    {
        painter.drawLine(xBase+1+i, yBase, xBase+1+i,
            yBase-(float)(256./max)*(float)bw[i]);
    }
    painter.drawText(xBase, yBase+25, tr("深色"));
    painter.drawText(xBase+220, yBase+25, tr("浅色"));
    painter.setPen(Qt::black);
    painter.drawText(xBase+45, yBase-height-10, tr("灰度分量直方图"));
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

void Histogram::drawred(int xBase, int yBase, int height)
{
    QPainter painter(this);
    painter.setPen(Qt::darkRed);
    float max = bw[256];
    if (max < red[256])
        max = red[256];
    if (max < green[256])
        max = green[256];
    if (max < blue[256])
        max = blue[256];
    // drawing the histogram
    for(int i = 0;i<256;i++)
    {
        painter.drawLine(xBase+1+i, yBase, xBase+1+i,
            yBase-(float)(256./max)*(float)red[i]);
    }

    painter.drawText(xBase, yBase+25, tr("深色"));
    painter.drawText(xBase+225, yBase+25, tr("浅色"));
    painter.setPen(Qt::black);
    painter.drawText(xBase+30, yBase-height-10, tr("红色分量直方图"));
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

void Histogram::drawgreen(int xBase, int yBase, int height)
{
    QPainter painter(this);
    painter.setPen(Qt::darkGreen);
    float max = bw[256];
    if (max < red[256])
        max = red[256];
    if (max < green[256])
        max = green[256];
    if (max < blue[256])
        max = blue[256];

    // drawing the histogram
    for(int i = 0;i<256;i++)
    {
        painter.drawLine(xBase+1+i, yBase, xBase+1+i,
            yBase-(float)(256./max)*(float)green[i]);
    }
    painter.drawText(xBase, yBase+25, tr("深色"));
    painter.drawText(xBase+225, yBase+25, tr("浅色"));
    painter.setPen(Qt::black);
    painter.drawText(xBase+20, yBase-height-10, tr("绿色分量直方图"));
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

void Histogram::drawblue(int xBase, int yBase, int height)
{
    QPainter painter(this);
    painter.setPen(Qt::darkBlue);
    float max = bw[256];
    if (max < red[256])
        max = red[256];
    if (max < green[256])
        max = green[256];
    if (max < blue[256])
        max = blue[256];
    // drawing the histogram
    for(int i = 0;i<256;i++)
    {
        painter.drawLine(xBase+1+i, yBase, xBase+1+i,
            yBase-(float)(256./max)*(float)blue[i]);
    }

    painter.drawText(xBase, yBase+25, tr("深色"));
    painter.drawText(xBase+225, yBase+25, tr("浅色"));
    painter.setPen(Qt::black);
    painter.drawText(xBase+20, yBase-height-10, tr("蓝色分量直方图"));
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

int Histogram::getbw(int index)
{
    if (index >= 0 && index <= 258)
        return bw[index];
    else
        return -2;
}

int Histogram::getred(int index)
{
    if (index >= 0 && index <= 257)
        return red[index];
    else
        return -2;
}

int Histogram::getgreen(int index)
{
    if (index >= 0 && index <= 257)
        return green[index];
    else
        return -2;
}

int Histogram::getblue(int index)
{
    if (index >= 0 && index <= 257)
        return blue[index];
    else
        return -2;
}
