#ifndef SUBWINDOW_H
#define SUBWINDOW_H
/* 1)打开指定文件，并读取文件内容到编辑器
 * 2)设置当前文件setCurrenFile(),获取文件路径，完成文件和窗口的状态设置
 * 3)关联文档内容改变信号到显示文档更改信号标志槽documentWasModified()
 *   加载文件操作采用loadFile()函数实现
 */
#include <QtWidgets>

#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QSpinBox>
#include <QSlider>
#include <QTextStream>

#include <QLabel>
#include <QTextEdit>
#include <QStandardPaths>
#include <QTranslator>
#include <QComboBox>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDebug>
#include <QPushButton>

#include "checkcode/checkcode.h"  //生成验证码
#include "drawdigit/drawdigit.h"            //绘制手写体

enum winType{New,Open,Cut,Draw};
//enum Aligns{Left = 1,Center,Right,Justify};

class SubWindow : public QWidget
{
    Q_OBJECT
public:
    SubWindow(winType type = Open);
    ~SubWindow();

    inline winType getCurrentWinType()const {return currentWinType;} //获取当前窗口类型
    inline QTextEdit* getTextEdit() const {return textEdit;}  //获取当前窗口的编辑框信息

    void updateImage(const QPixmap &pix);              //更新图片
    void cleanImage();                                 //清除图片
    void restoreImage();                               //还原图像
    void changeWindows();                              //更换图像
    void processCodeArea();                            //处理验证码图片

    void newFile();                                    //新建验证码
    void cutFile();                                    //验证码屏幕截图
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
    //QPointF mapToMap(QPointF);                       //实现场景与图片间的映射

protected:
    void closeEvent(QCloseEvent *event);                //关闭事件
    //void mouseMoveEvent(QMouseEvent *event);          //鼠标事件

signals:
    reOpenImage();                                      //关闭此窗口并重新打开文件
    textUpdate();                                       //启用调整字体时发送更新主窗体字体菜单状态栏的信号

private slots:
    void isModified();                                  //是否修改过文本标志
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
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    void generateView();
    void generateTextCnt();
    void generateNew();
    void generateOpen();
    void generateCut();
    void generateDraw();
    QString strippedName(const QString &fullFileName);  //获取较短的绝对路径
    QString curPath;                                    //保存当前文件路径
    winType currentWinType;                             //当前所处的窗口类型

    bool toSaveIt;                                      //该文件是否需要提示保存
    QLabel *userText = nullptr;                         //用户输入提示
    QTextEdit *textEdit = nullptr;                      //编辑框

    QPixmap pixmap;                                     //显示导入的验证码图片
    QLabel *size = nullptr;                             //显示图片像素
    //QFileInfo *info;                                  //图片信息集合,已废除
    QGraphicsPixmapItem *pixmapItem = nullptr;
    QGraphicsView *view = nullptr;
    QGraphicsScene  *scene = nullptr;
    QVBoxLayout *mainLayout = nullptr;                  //核心布局
    QHBoxLayout *zoomLayout = nullptr;                  //缩放滚条
    QHBoxLayout *textLayout = nullptr;                  //文本布局
    QHBoxLayout *btnLayout = nullptr;                   //按钮布局
    /*QLabel *viewCoord;                                //QGraphicsView坐标
    QLabel *sceneCoord;                                 //QGraphicsScene坐标
    QLabel *mapCoord;                                   //图片坐标轴实际坐标映射
    */

    qreal zoom;                                         //缩放系数
    QSpinBox *spinbox = nullptr;
    QSlider *slider = nullptr;                          //缩放控制滚动条
    QPushButton *zoomin = nullptr;
    QPushButton *zoomout = nullptr;

    //生成验证码
    CodeArea *codeArea = nullptr;                       //显示生成的验证码
    QMessageBox *msgBox = nullptr;                      //用户认证对错触发消息
    QComboBox *combox = nullptr;                        //下拉菜单
    QPushButton *functionBtn = nullptr;                 //用户验证码校对

    //【绘图】
    DrawDigit *drawdigit = nullptr;
    QLabel *widthLabel = nullptr;
    QSpinBox *widthSpinBox = nullptr;
    QHBoxLayout *lineWidth = nullptr;
};
#endif // SUBWINDOW_H
