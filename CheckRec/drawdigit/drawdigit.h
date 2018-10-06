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
#include <QString>
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
    void setWidth(int w);
    void clearPic();
    bool savePic(QString& fileName);
    void restorePic();
    void smoothPic();
    QString getResult();

private:
    int square_dist(const QPoint& l,const QPoint& r);
    QPixmap *pix = nullptr;
    QPixmap *last_pix = nullptr;
    QPoint startPos;
    QPoint endPos;
    int width;
};

#endif // DRAWDIGIT_H
