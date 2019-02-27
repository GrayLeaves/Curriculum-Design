#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QDebug>
#include <QVector>

class Histogram : public QLabel
{
public:
    enum class RGB{BW,R,G,B};  //直方图样式
    Histogram(QWidget* parent = 0);
    Histogram(QWidget*, Histogram*);
    void computeHstgrm(QImage img);   //生成直方图
    void paintEvent(QPaintEvent *e);  //绘图
private:
    static const int RGBSize = 256;
    void drawHstgrm(int xBase, int yBase, int height, RGB type);
    void drawgray(int xBase, int yBase, int height);  //灰度直方图
    void drawred(int xBase, int yBase, int height);   //红色分量
    void drawgreen(int xBase, int yBase, int height); //绿色分量
    void drawblue(int xBase, int yBase, int height);  //蓝色分量
    // index 0 to 255 => count of image's pixels for this value
    // index 256 => maximum value
    // index 257/258 => total value of the dark/light component
    QVector<int> bw;
    // index 257 => total value of the component
    QVector<int> red, green, blue;
};

#endif // HISTOGRAM_H
