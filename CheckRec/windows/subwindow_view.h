#ifndef SUBWINDOW_VIEW_H
#define SUBWINDOW_VIEW_H
#include <QWidget>
#include "checkcode/checkcode.h"  //生成验证码
#include "drawdigit/drawdigit.h"  //绘制手写体

class QLabel;
class QTextEdit;
class QComboBox;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGraphicsScene;
class QGraphicsView;
class QSpinBox;
class QSlider;

class SubView : QWidget
{
    Q_OBJECT
public:
    SubView(){}
    ~SubView(){}
    virtual void generateView() = 0;
    QTextEdit *textEdit = nullptr;                      //编辑框
    QComboBox *combox = nullptr;                        //下拉菜单
    QPushButton *functionBtn = nullptr;                 //用户验证码校对
    QVBoxLayout *mainLayout = nullptr;                  //核心布局
protected:
    virtual void generateTextCnt();
    QLabel *userText = nullptr;                         //用户输入提示
    QHBoxLayout *textLayout = nullptr;                  //文本布局
    QHBoxLayout *btnLayout = nullptr;                   //按钮布局
};


//enum winType{New,Open,Cut,Draw};
class newView : public SubView
{
public:
    void generateView() override;
    CodeArea *codeArea = nullptr;                       //显示生成的验证码
};

class openView : public SubView
{
public:
    void generateView() override;
    QGraphicsScene  *scene = nullptr;
    QGraphicsView *gview = nullptr;
    QLabel *size = nullptr;                             //显示图片像素
    qreal zoom;                                         //缩放系数
    QSpinBox *spinbox = nullptr;
    QSlider *slider = nullptr;                          //缩放控制滚动条
    QPushButton *zoomin = nullptr;
    QPushButton *zoomout = nullptr;
protected:
    virtual void generateTitle();
    QHBoxLayout *zoomLayout = nullptr;                  //缩放滚条
};

class cutView : public openView
{
public:
    void generateView() override;
};

class drawView : public SubView
{
public:
    void generateView() override;
    DrawDigit *drawdigit = nullptr;
    QLabel *widthLabel = nullptr;
    QSpinBox *widthSpinBox = nullptr;
protected:
    QHBoxLayout *lineWidth = nullptr;
};
#endif // SUBWINDOW_VIEW_H
