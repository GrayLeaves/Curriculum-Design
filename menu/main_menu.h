#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include <QMenu>
#include <QAction>

class FileMenu : public QMenu
{
public:
    explicit FileMenu();
    ~FileMenu();
    QMenu *menu = nullptr;
    //【文件】主菜单
    QAction *newAct = nullptr;            //Ctrl+N
    QAction *openAct = nullptr;           //Ctrl+O
    QAction *screencutAct = nullptr;      //Ctrl+K
    QAction *drawdigitAct = nullptr;
    QAction *qrcodeAct = nullptr;
    QMenu *recentMenu = nullptr;
    QAction *clearRecentOpened = nullptr;
    QAction *saveAct = nullptr;           //Ctrl+S
    QAction *saveAsAct = nullptr;         //Ctrl+A
    QAction *exitAct = nullptr;           //Ctrl+Q
};

class EditMenu : public QMenu
{
public:
    explicit EditMenu();
    ~EditMenu();
    QMenu *menu = nullptr;
    //【编辑】主菜单
    QAction *undoAct = nullptr;           //Ctrl+Z
    QAction *redoAct = nullptr;           //Ctrl+Y
    QAction *cutAct = nullptr;            //Ctrl+X
    QAction *copyAct = nullptr;           //Ctrl+C
    QAction *pasteAct = nullptr;          //Ctrl+V
};

class FormatMenu : public QMenu
{
public:
    explicit FormatMenu();
    ~FormatMenu();
    QMenu *menu = nullptr;
    //【格式】主菜单
    QMenu *fontMenu = nullptr;
    QMenu *alignMenu = nullptr;
    QMenu *styleMenu = nullptr;
    QActionGroup *group = nullptr;
    QActionGroup *set = nullptr;
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
};

class WindowMenu : public QMenu
{
public:
    explicit WindowMenu();
    ~WindowMenu();
    QMenu *menu = nullptr;
    //【窗口】主菜单
    QAction *closeAct = nullptr;          //close
    QAction *closeAllAct = nullptr;       //Ctrl+Shift+X
    QAction *tileAct = nullptr;           //tile
    QAction *cascadeAct = nullptr;        //cascade
    QAction *nextAct = nullptr;           //Ctrl+Tab
    QAction *previousAct = nullptr;       //Ctrl+Shift+Tab
    QAction *separatorAct = nullptr;      //separate
};

class ExtractMenu : public QMenu
{
public:
    explicit ExtractMenu();
    ~ExtractMenu();
    QMenu *menu = nullptr;
    //【提取】主菜单
    QAction *drawHstgrmAct = nullptr;     //绘制直方图
    QAction *greyScaleAct = nullptr;
    QAction *binaryzationAct = nullptr;
    QAction *invertAct = nullptr;
    QAction *sobelAct = nullptr;
    QAction *laplaceAct = nullptr;
};

class HelpMenu : public QMenu
{
public:
    explicit HelpMenu();
    ~HelpMenu();
    QMenu *menu = nullptr;
    //【帮助】主菜单
    QAction *aboutAct = nullptr;          //Ctrl+H
    QAction *aboutQtAct = nullptr;        //Shift+H
};
#endif
