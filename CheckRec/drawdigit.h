#ifndef DRAWDIGIT_H
#define DRAWDIGIT_H
#include <QWidget>
#include <QtGui>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPixmap>
#include <QPoint>
#include <QPainter>
#include <QPalette>

class DrawDigit : public QWidget
{
    Q_OBJECT
public:
    explicit DrawDigit(QWidget *parent = 0);
    ~DrawDigit();
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

signals:

public slots:
    void setPenWidth(int w){width = w;}
    void clearPic();
    void savePic();
private:
    QPixmap *pix = nullptr;
    QPoint startPos;
    QPoint endPos;
    int width;
};

#endif // DRAWDIGIT_H
