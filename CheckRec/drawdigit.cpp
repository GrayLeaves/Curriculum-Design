#include "drawdigit.h"
#include <QtGui>
#include <QPen>
#include "common.h"
DrawDigit::DrawDigit(QWidget *parent):QWidget(parent)
{
    setAutoFillBackground(true);
    setPalette(QPalette(Qt::white));
    pix =new QPixmap(size()); //绘板
    pix->fill(Qt::black); //黑底白字
    setFixedSize(100,168); //固定画板大小，黄金
}

DrawDigit::~DrawDigit(){
    delete_s(pix);
}

void DrawDigit::mousePressEvent(QMouseEvent *e)
{
    startPos = e->pos();
}

void DrawDigit::mouseMoveEvent(QMouseEvent *e)
{
    QPainter paint;
    QPen pen;
    pen.setStyle(Qt::SolidLine); //线形
    pen.setWidth(width); //线宽
    pen.setColor(Qt::white); //白色粉末
    paint.begin(pix);
    paint.setPen(pen);
    paint.drawLine(startPos,e->pos());
    paint.end();
    startPos =e->pos(); //跟踪绘制
    update();
}

void DrawDigit::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.drawPixmap(QPoint(0,0),*pix);
}

void DrawDigit::clearPic()
{
    delete_s(pix);
    pix = new QPixmap(size());
    pix->fill(Qt::black);
    update();
}

void DrawDigit::savePic(){

}
