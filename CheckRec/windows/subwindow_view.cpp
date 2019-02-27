#include "subwindow_view.h"
#include "drawdigit/drawdigit.h"
#include "common.h"
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSpinBox>
#include <QSlider>

void SubView::generateTextCnt(){
    userText = new QLabel(tr("编辑栏"));
    userText->setFrameStyle(QFrame::Panel|QFrame::Raised);
    userText->setFixedHeight(25);
    userText->resize(160,25);
    textEdit = new QTextEdit;
    textEdit->setFixedHeight(30);
    textEdit->document()->setMaximumBlockCount(1); //设置最大行数为1行
    textEdit->setAlignment(Qt::AlignCenter);
    QFont font(QString::fromLocal8Bit("Arial"),14);
    textEdit->setFont(font);
    //connect(comboBox,SIGNAL(activated(int)),this,SLOT(textEdit->clear()));  //关联相应的槽函数
    functionBtn = new QPushButton(tr("OK"));
    functionBtn->setFixedHeight(30);
}

void newView::generateView(){
    codeArea = new CodeArea;      // 生成验证码
    codeArea->setFixedSize(150,77); // 指定区域，防止窗口缩放中变形

    generateTextCnt();              // 生成编辑框，提示信息组合框和按钮
    combox = new QComboBox;
    combox->addItem(QIcon(rsrcPath + "/recognize.png"),tr("揭晓")); //0 识别
    combox->addItem(QIcon(rsrcPath + "/check.png"),tr("校对")); //1
    combox->addItem(QIcon(rsrcPath + "/generate.png"),tr("更新"));//2
    combox->addItem(QIcon(rsrcPath + "/tooth.png"),tr("处理"));//3
    combox->setFixedHeight(30);

    mainLayout = new QVBoxLayout; //实现布局
    mainLayout->addWidget(codeArea,0,Qt::AlignCenter);
    mainLayout->setSpacing(5);

    textLayout = new QHBoxLayout;
    textLayout->addWidget(userText);                //提示信息
    textLayout->addWidget(textEdit);                //编辑框
    mainLayout->addLayout(textLayout);
    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(combox);                  //菜单栏
    btnLayout->addWidget(functionBtn);             //功能按钮
    mainLayout->addLayout(btnLayout);
    //resize(180,200);
}

void openView::generateView(){
    generateTitle(); // 图形视图，生成图片像素的标签和初始化graphics/view
    generateTextCnt(); // 生成编辑框，提示信息组合框和按钮
    combox = new QComboBox;
    combox->addItem(QIcon(rsrcPath + "/recognize.png"),tr("揭晓")); //0 识别
    combox->addItem(QIcon(rsrcPath + "/cycle.png"),tr("还原"));//1
    combox->addItem(QIcon(rsrcPath + "/openimage.png"),tr("替换"));//2
    combox->setFixedHeight(30);

    mainLayout = new QVBoxLayout; //实现布局
    mainLayout->addWidget(size); //显示验证码像素值
    mainLayout->setSpacing(5);  //分割区域
    //缩放控制子布局
    zoomLayout = new QHBoxLayout;
    zoomLayout->addWidget(spinbox);
    zoomLayout->addWidget(zoomout);
    zoomLayout->addWidget(slider);
    zoomLayout->addWidget(zoomin);
    mainLayout->addWidget(gview);
    mainLayout->addLayout(zoomLayout);
    mainLayout->setSpacing(5);

    textLayout = new QHBoxLayout;
    textLayout->addWidget(userText);                //提示信息
    textLayout->addWidget(textEdit);                //编辑框
    mainLayout->addLayout(textLayout);
    btnLayout = new QHBoxLayout;
    btnLayout->addWidget(combox);                  //菜单栏
    btnLayout->addWidget(functionBtn);             //功能按钮
    mainLayout->addLayout(btnLayout);
    //resize(180,200);
}

void openView::generateTitle()
{
    size = new QLabel(tr("Dimensions：*** x ***"));
    zoom = 50.0;
    //info = NULL;
    spinbox = new QSpinBox;
    spinbox->setRange(1,99);
    spinbox->setValue(zoom);
    spinbox->setFixedWidth(40);
    slider = new QSlider;                   //用于缩放的滚动条
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(1,99);
    slider->setTickInterval(10);
    slider->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    slider->setValue(zoom);

    zoomin = new QPushButton;
    zoomin->setIcon(QPixmap(rsrcPath + "/zoomin.png"));
    zoomin->setFixedSize(QPixmap(rsrcPath + "/zoomin.png").size());
    zoomin->setStatusTip(tr("放大"));

    zoomout = new QPushButton;
    zoomout->setIcon(QPixmap(rsrcPath + "/zoomout.png"));
    //zoomout->setIconSize(QPixmap(rsrcPath + "/zoomout.png").size());
    zoomout->setFixedSize(QPixmap(rsrcPath + "/zoomout.png").size());
    zoomout->setStatusTip(tr("缩小"));
    scene = new QGraphicsScene;
    scene->setBackgroundBrush(QColor::fromRgb(224,224,224)); //填充背景
    gview = new QGraphicsView;
    gview->setScene(scene);
    gview->setMinimumSize(160,65);
    gview->resize(200,120);
    gview->setCacheMode(QGraphicsView::CacheBackground);
    //view->show();
}

void cutView::generateView(){openView::generateView();}

void drawView::generateView(){
    widthLabel =new QLabel(tr("线宽"));
    widthLabel->setFrameStyle(QFrame::Panel|QFrame::Raised);
    widthLabel->setFixedSize(40,25);
    widthSpinBox =new QSpinBox;
    widthSpinBox->setFixedHeight(30);
    widthSpinBox->setRange(3,8);
    widthSpinBox->setValue(5);

    lineWidth = new QHBoxLayout;
    lineWidth->addWidget(widthLabel);
    lineWidth->addWidget(widthSpinBox);
    drawdigit = new DrawDigit;
    drawdigit->setFixedSize(140,180);
    generateTextCnt();
    combox = new QComboBox;
    combox->addItem(QIcon(rsrcPath + "/recognize.png"),tr("揭晓")); //0 识别
    combox->addItem(QIcon(rsrcPath + "/cycle.png"),tr("还原"));//1
    combox->addItem(QIcon(rsrcPath + "/delete.png"),tr("清除")); //2
    combox->addItem(QIcon(rsrcPath + "/sobel.png"),tr("训练"));//3
    combox->setFixedHeight(30);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(lineWidth);
    mainLayout->addWidget(drawdigit,0,Qt::AlignCenter);
    mainLayout->setSpacing(5);

    textLayout = new QHBoxLayout;
    textLayout->addWidget(userText);                //提示信息
    textLayout->addWidget(textEdit);                //编辑框
    mainLayout->addLayout(textLayout);
    btnLayout = new QHBoxLayout;
    btnLayout->addWidget(combox);                  //菜单栏
    btnLayout->addWidget(functionBtn);             //功能按钮
    mainLayout->addLayout(btnLayout);
}
