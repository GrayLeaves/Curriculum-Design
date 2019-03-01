#include "dialog.h"
#include "windows/mainwindow.h"
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
Dialog::Dialog(QWidget *parent) : QDialog(parent),
    pt1(0,0),pt2(0,0),mousedown(false)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    screen = QGuiApplication::primaryScreen();
    resize(screen->size());
    this->setMouseTracking(true);
}

void Dialog::mousePressEvent(QMouseEvent * event)
{   //点击右键选定完毕则关闭截屏
    if(event->button()==Qt::RightButton)
    {
        closeF();
        this->close();
    }   //点击左键则开始记录矩形框
    if(event->button()==Qt::LeftButton)
    {
        pt1.setX(event->x());
        pt1.setY(event->y());
        mousedown=true;
    }
}

void Dialog::mouseMoveEvent(QMouseEvent * event)
{   //记录并更新最新的位置
    if(mousedown)
    {
        pt2.setX(event->x());
        pt2.setY(event->y());
        update();
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent * event)
{   //若再次点击左键则取消截屏但不关闭截屏
    if(event->button()==Qt::LeftButton)
    {
        pt2.setX(event->x());
        pt2.setY(event->y());
        update();
        mousedown=false;
        emit finish(pt1,pt2);
    }
}

void Dialog::keyPressEvent(QKeyEvent * event)
{   //当按下空格键或esc键也可选中关闭截屏
    int key=event->key();
    if(key==Qt::Key_Space||key==Qt::Key_Return)
    {
        closeF();
        this->close();
    }
}

void Dialog::paintEvent(QPaintEvent *)
{   //截屏时绘制矩形框
    int alpha=1;
    QPainter paint(this);
    paint.fillRect(0, 0, width(), height(), QColor(0, 0, 0, alpha));
    QPen pen(Qt::darkGreen,1);
    paint.setPen(pen);
    paint.drawRect(pt1.x(), pt1.y(), pt2.x()-pt1.x(), pt2.y()-pt1.y());
}
