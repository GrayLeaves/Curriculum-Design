#include "drawdigit.h"
#include <QtGui>
#include <QPen>
#include "common.h"


DrawDigit::DrawDigit(QWidget *parent):QWidget(parent)
{
    setAutoFillBackground(true);
    setPalette(QPalette(Qt::white));
    width = 5;
    setFixedSize(140,180);
    last_pix = new QPixmap(size());
    last_pix->fill(Qt::white);
    pix = new QPixmap(size()); //绘板
    pix->fill(Qt::white);      //白底黑字
}

DrawDigit::~DrawDigit(){
    delete_s(pix);
    delete_s(last_pix);
}

void DrawDigit::mousePressEvent(QMouseEvent *e)
{
    startPos = e->pos();
}

void DrawDigit::mouseMoveEvent(QMouseEvent *e)
{
    int d = square_dist(e->pos(), startPos);
    if(d > width*5){
        QPainter paint;
        QPen pen;
        pen.setStyle(Qt::SolidLine); //线形
        pen.setWidth(width); //线宽
        pen.setColor(Qt::black); //黑色粉末
        paint.begin(pix);
        paint.setPen(pen);
        paint.drawLine(startPos,e->pos());
        paint.end();
        startPos = e->pos(); //跟踪绘制
        update();
    }
}

void DrawDigit::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.drawPixmap(QPoint(0,0),*pix);
}

void DrawDigit::setWidth(int w){
    if(w<3 || w>8){
        qDebug() << w << " is invalid value.";
        return;
    }
    width = w;
}

void DrawDigit::clearPic()
{
    delete_s(last_pix);
    last_pix = pix;
    pix = new QPixmap(size());
    pix->fill(Qt::white);
    update();
}

void DrawDigit::restorePic(){
    if(last_pix != nullptr){
        delete_s(pix);
        pix = last_pix;
        last_pix = nullptr;
        update();
    }
}

bool DrawDigit::savePic(QString& fileName){
    if(fileName.isEmpty())
        return false;
    return pix->save(fileName);
}

void DrawDigit::smoothPic(){}

int DrawDigit::square_dist(const QPoint& l,const QPoint& r){
    return (l.x()-r.x())*(l.x()-r.x()) + (l.y()-r.y())*(l.y()-r.y());
}
