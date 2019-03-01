#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPoint>
#include <QScreen>

class QMouseEvent;
class QPaintEvent;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    //~Dialog();
protected:
    virtual void mousePressEvent(QMouseEvent *);
    virtual void keyPressEvent(QKeyEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);

signals:
    void finish(QPoint pt1,QPoint pt2);
    void closeF();

private:
    QPoint pt1;
    QPoint pt2;
    QScreen *screen;            //屏幕
    bool mousedown;
};

#endif // DIALOG_H
