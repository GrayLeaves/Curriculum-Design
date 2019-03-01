#ifndef QRCODE_H
#define QRCODE_H

#include "qrencode.h"
#include <QWidget>
#include <QPainter>


class Qrcode : public QWidget
{
    Q_OBJECT

public:
    explicit Qrcode();
    ~Qrcode();
    QString qstr() const {return string;}
    void generateString(QString str);
    bool saveImage(QString fileName, int size=250); //默认保存为250*250像素的图片
private:
    void draw(QPainter &painter, int width, int height);
    QString string;
    QRcode *qr = nullptr;           //二维码类指针
protected:
    void paintEvent(QPaintEvent *); //绘制二维码的事件
};

#endif // QRCODE_H
