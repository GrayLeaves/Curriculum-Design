#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "subwindow.h"

namespace Ui {
class MainWindow;
}

class RecentOpened;
class QAction;              //事件
class QMenu;                //菜单
class QComboBox;            //组合框
class QFontComboBox;        //字体选择框
class QMdiArea;             //主窗口中心多图片区域
class QMdiSubWindow;        //子窗口
class QSignalMapper;        //信号映射器
class Histogram;            //直方图

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);    //关闭窗口
private slots:
    void NewImage();                         //新建二维码
    void reLoadImage();                      //重新导入
    void OpenImage();                        //打开图片
    void CutImage();                         //截屏操作
    void DrawPic();                          //绘制手写体
    void SaveImage();                        //保存
    void SaveImageAs();                      //另存
    QString getUserName();                   //获取用户名
    QString getUserPath();                   //获取家目录
    void screenCut(QPoint pt1, QPoint pt2);  //从桌面选定区域截图
    void showImage();                        //显示截图
    void recentOpened(int index);            //最近打开的图片
    void clearOpenedHistory();               //清除打开记录

    void drawHistogram();                    //绘制直方图
    void greyScale();                        //GrayScale
    void binaryzation();                     //二值化图像
    void invert();                           //像素点反色
    void laplace();                          //laplace锐化
    void sobel();                            //sobel边缘检测
    void prewitt();                          //Prewitt边缘检测

    //调用子窗体执行文本操作的接口
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void about();
    void textBold();
    void textItalic();
    void textUnderline();
    void textAlign(QAction *a);
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textColor();

    void updateMenus();                              //更新菜单
    void updateWindowMenu();

    SubWindow *createSubWindow(winType type = Open); //创建子窗体
    void windowStyle(QAction *s);
    void setActiveSubWindow(QWidget *window);

private:
    SubWindow *activeSubWindow();                   //活动子窗口
    QMdiSubWindow *findSubWindow(const QString &fileName); //遍历查询多图片区域的子窗口
    void openFile(const QString &fileName);         //提供统一的打开文件接口
    void createActions();                           //【动作】
    void createMenus();                             //【菜单】
    void createToolBars();                          //【工具栏】
    void createStatusBar();                         //【状态栏】
    void setEnabledText(bool ok = true);            //使得【格式】下的各项是否可用
    void setEnabledSwitch(bool ok = true);          //使得【窗口】下的各项是否可用
    void setEnableIamgeProc(bool ok = true);        //使得【提取】下的各项是否可用
    void fontChanged(const QFont &f);               //字体/字号/加粗/倾斜/下划线
    void colorChanged(const QColor &c);             //字体颜色
    void alignmentChanged(Qt::Alignment a);         //居左右中和调整
    void textChangedUpdate();                       //上面所有字体改变时更新窗体菜单
    void styleChanged(QString style);               //窗体风格改变（不必使用）
    void imageProcTriggered(QImage (*handle)(const QImage &),const QString &message); //提供统一的图形操作接口

    Ui::MainWindow *ui;
    bool finish;                          //是否截图完成
    QPixmap pixmap;                       //截屏图像
    QScreen *screen = nullptr;            //屏幕
    RecentOpened *recent = nullptr;       //最近打开的图像的记录

    QMdiArea *mdiArea = nullptr;
    QSignalMapper *windowMapper = nullptr;//信号映射器私有对象指针
    //【菜单】
    QMenu *fileMenu = nullptr;
    QMenu *recentMenu = nullptr;
    QMenu *editMenu = nullptr;
    QMenu *formatMenu = nullptr;
    QMenu *fontMenu = nullptr;
    QMenu *alignMenu = nullptr;
    QMenu *windowMenu = nullptr;
    QMenu *extractMenu = nullptr;
    QMenu *helpMenu = nullptr;
    //【工具栏】
    QToolBar *fileToolBar = nullptr;      //"文件"工具条
    QToolBar *editToolBar = nullptr;      //"编辑"工具条
    QToolBar *formatToolBar = nullptr;    //"格式"工具条
    QToolBar *comboToolBar = nullptr;     //组合选项栏
    QFontComboBox *comboFont = nullptr;   //"字体"选项框
    QComboBox *comboSize = nullptr;       //"字号"选项框
    //动作(Action)集合
    //【文件】主菜单
    QAction *newAct = nullptr;            //Ctrl+N
    QAction *openAct = nullptr;           //Ctrl+O
    QAction *screencutAct = nullptr;      //Ctrl+K
    QAction *drawdigitAct = nullptr;
    QAction *clearRecentOpened = nullptr;
    QAction *saveAct = nullptr;           //Ctrl+S
    QAction *saveAsAct = nullptr;         //Ctrl+A
    QAction *exitAct = nullptr;           //Ctrl+Q
    //【编辑】主菜单
    QAction *undoAct = nullptr;           //Ctrl+Z
    QAction *redoAct = nullptr;           //Ctrl+Y
    QAction *cutAct = nullptr;            //Ctrl+X
    QAction *copyAct = nullptr;           //Ctrl+C
    QAction *pasteAct = nullptr;          //Ctrl+V
    //【格式】主菜单
    //字体
    QAction *boldAct = nullptr;           //Ctrl+B
    QAction *italicAct = nullptr;         //Ctrl+I
    QAction *underlineAct = nullptr;      //Ctrl+U
    //段落
    QAction *leftAlignAct = nullptr;      //Ctrl+L
    QAction *centerAct = nullptr;         //Ctrl+E
    QAction *rightAlignAct = nullptr;     //Ctrl+R
    QAction *justifyAct = nullptr;        //Ctrl+J
    //风格
    QAction *WindowsAct = nullptr;
    QAction *WindowsXPAct = nullptr;
    QAction *WindowsVistaAct = nullptr;
    QAction *FusionAct = nullptr;
    //颜色
    QAction *colorAct = nullptr;          //Color
    //【窗口】主菜单
    QAction *closeAct = nullptr;          //close
    QAction *closeAllAct = nullptr;       //Ctrl+Shift+X
    QAction *tileAct = nullptr;           //tile
    QAction *cascadeAct = nullptr;        //cascade
    QAction *nextAct = nullptr;           //Ctrl+Tab
    QAction *previousAct = nullptr;       //Ctrl+Shift+Tab
    QAction *separatorAct = nullptr;      //separate
    //【提取】主菜单
    QAction *drawHstgrmAct = nullptr;     //绘制直方图
    QAction *greyScaleAct = nullptr;
    QAction *binaryzationAct = nullptr;
    QAction *invertAct = nullptr;
    QAction *sobelAct = nullptr;
    QAction *laplaceAct = nullptr;
    QAction *prewittAct = nullptr;
    QAction *contourAct = nullptr;
    //【帮助】主菜单
    QAction *aboutAct = nullptr;          //Ctrl+H
    QAction *aboutQtAct = nullptr;        //Shift+H
};

#endif // MAINWINDOW_H
