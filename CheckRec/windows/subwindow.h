#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QtWidgets>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QMouseEvent>
#include <QTextStream>
#include <QStandardPaths>
#include <QTranslator>
#include "subwindow_view.h"
#include "checkcode/checkcode.h"  //生成验证码
#include "drawdigit/drawdigit.h"  //绘制手写体

enum winType{New,Open,Cut,Draw};
//enum Aligns{Left = 1,Center,Right,Justify};

class SubWindow : public QWidget
{
    Q_OBJECT
public:
    SubWindow(winType type = Open); //+
    ~SubWindow();//+
    SubView* subView = nullptr;
    inline winType getCurrentWinType()const {return currentWinType;} //获取当前窗口类型+

    void updateImage(const QPixmap &pix);              //更新图片
    void cleanImage();                                 //清除图片
    void restoreImage();                               //还原图像
    void changeWindows();                              //更换图像
    void processCodeArea();                            //处理验证码图片

    void newFile();                                    //新建验证码
    void cutFile();                                    //验证码屏幕截图
    void drawFile();                                   //手绘字符
    bool loadFile(const QString &fileName);            //从文件浏览器导入验证码
    bool save();                                       //保存新建的验证码
    bool saveAs();                                     //另存为其他格式
    bool saveAs(QString& fileName);                    //可返回保存的路径
    bool saveFile(QString fileName);                   //保存图片
    bool showFile(QPixmap pix);                        //显示图片
    void setImageStatus(bool status){toSaveIt = status;} //设置保存状态

    QString currentFileName();                         //提取文件名
    QString currentPath() const { return curPath; }    //返回当前路径

    void mergeFormat(const QTextCharFormat &format);   //格式字体设置
    void setAlign(int align);                          //段落对齐设置
    void resetZoom();
    QGraphicsPixmapItem * Item() const
        {return pixmapItem;}                           //获取Item()->pixmap()

protected:
    void closeEvent(QCloseEvent *event);                //关闭事件
    //void mouseMoveEvent(QMouseEvent *event);          //鼠标事件

signals:
    reOpenImage();                                      //关闭此窗口并重新打开文件
    textUpdate();                                       //启用调整字体时发送更新主窗体字体菜单状态栏的信号

private slots:
    void isModified();                                  //是否修改过文本标志
    bool readPath(QString& fullPath);
    void genLabels();
    void showResult();
    void recognizeCode();                               //将识别结果送到编辑框
    void recognizePic();
    void setCode();                                     //设置代码
    void checkCode();                                   //用户输入验证码结果
    void replaceCode();                                 //更换验证码
    void zoomInOut(int);                                //放大缩小
    void zoomIn();
    void zoomOut();
    void tool();                                        //下拉功能：识别、认证和生成/更换

private:
    void all_connect(winType type);
    SubView* createView(winType type);           //TODO: simple factory+
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);  //获取较短的绝对路径
    QString curPath;                                    //保存当前文件路径
    winType currentWinType;                             //当前所处的窗口类型
    bool toSaveIt;                                      //该文件是否需要提示保存
    QMessageBox *msgBox = nullptr;                      //用户认证对错触发消息
    QPixmap pixmap;                                     //显示导入的验证码图片
    QGraphicsPixmapItem *pixmapItem = nullptr;
};

#endif // SUBWINDOW_H
