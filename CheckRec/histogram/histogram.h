#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <array>

class Histogram : public QLabel
{
public:
    enum RGB{BW,R,G,B};  //直方图样式
    Histogram(QWidget* parent = 0);
    Histogram(QWidget*, Histogram*);

    void computeHstgrm(QImage img);   //生成直方图
    void paintEvent(QPaintEvent *e);  //绘图
    void drawbw(int xBase, int yBase, int height);    //灰度直方图
    void drawred(int xBase, int yBase, int height);   //红色分量
    void drawgreen(int xBase, int yBase, int height); //绿色分量
    void drawblue(int xBase, int yBase, int height);  //蓝色分量
    int getbw(int index);
    int getred(int index);
    int getgreen(int index);
    int getblue(int index);

private:
    bool drawHstgrm(int xBase, int yBase, int height, RGB type);
    // index 0 to 255 => count of image's pixels for this value
    // index 256 => maximum value
    // index 257/258 => total value of the dark/light component
    int bw[259];
    // index 257 => total value of the component
    int red[258]; int green[258]; int blue[258];
};

#endif // HISTOGRAM_H
