#include "mainwindow.h"
#include "subwindow.h"
#include "recentopened/recentopened.h"
#include "common.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::MainWindow),finish(false)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(rsrcPath + "/logo.png"));
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);

    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(mdiArea);
    //当由活动窗口更新菜单，关联多文档区域的活动子窗口信号到更新菜单槽上去，以便实现即时更新
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),this, SLOT(updateMenus()));
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)),this, SLOT(updateWindowMenu()));
    //信号映射器QSignalMapper类实现多个相同部件的相同信号进行映射。
    //创建信号映射器
    windowMapper = new QSignalMapper(this);
    //映射器重新发送信号，根据信号设置活动窗口
    connect(windowMapper, SIGNAL(mapped(QWidget*)),this, SLOT(setActiveSubWindow(QWidget*)));

    //createActions();
    createMenus();
    all_connect();
    createToolBars();
    createStatusBar();
    updateMenus();

    move(284,50);
    setMaximumSize(1200,675);
    resize(800,500);

    QPixmap pixmap(rsrcPath + "/background.bmp");
    mdiArea->setBackground(QBrush(pixmap));
    mdiArea->autoFillBackground();

    setWindowTitle(tr("MainWindow"));
    setUnifiedTitleAndToolBarOnMac(true);
    /*QStringList styles = QStyleFactory::keys();
    foreach(QString style , styles)
        qDebug() << style;*/
    //0-"Windows" 1-"WindowsXP" 2-"WindowsVista" 3-"Fusion"
    QApplication::setStyle(QStyleFactory::create("Fusion"));//设置内置风格
    QApplication::setPalette(QApplication::style()->standardPalette());//选择风格默认颜色
}

MainWindow::~MainWindow()
{
    delete ui;
    delete_s(recent);
    //delete_s(hstgrmDialog);
    //delete_s(scrollArea);
    //delete_s(hstgrm);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->currentSubWindow()) {
        event->ignore();
    } else {
        event->accept();
    }
}

void MainWindow::updateMenus()
{
    //至少有一个子窗口打开着的情况
    bool hasSubWindow = (activeSubWindow()!=0);
    fileMenu->saveAct->setEnabled(hasSubWindow);
    fileMenu->saveAsAct->setEnabled(hasSubWindow);
    editMenu->pasteAct->setEnabled(hasSubWindow);
    setEnabledSwitch(hasSubWindow);

    //更新对应的字体大小工具栏的值，实现请参考fontChanged()
    if(hasSubWindow){
        QFont font = activeSubWindow()->subView->textEdit->currentFont();
        comboFont->setCurrentFont(font);
        QString sizenum = QString::number(font.pointSize());
        comboSize->setCurrentIndex(comboSize->findText(sizenum));//下标和字号不完全对应下的转换
    }

    //子窗口打开着并且其中有内容被选中的情况
    bool hasSelection = (hasSubWindow && activeSubWindow()->subView->textEdit->textCursor().hasSelection());
    editMenu->cutAct->setEnabled(hasSelection);
    editMenu->copyAct->setEnabled(hasSelection);
    setEnabledText(hasSelection);

    //非生成验证码子窗口打开着的情况
    bool hasSelectPic = (hasSubWindow && activeSubWindow()->getCurrentWinType() != New);
    setEnableIamgeProc(hasSelectPic);
}

void MainWindow::updateWindowMenu()
{   //新建文档会在窗口菜单中动态更新显示
    //首先清空菜单，再逐个添加各菜单动作
    //窗口栏
    // qDebug() << "Enter updateWindowMenu()";
    selfmenu = windowMenu->menu;
    selfmenu->clear();
    selfmenu->addAction(windowMenu->closeAct);
    selfmenu->addAction(windowMenu->closeAllAct);
    selfmenu->addSeparator();             //分隔符
    selfmenu->addAction(windowMenu->tileAct);
    selfmenu->addAction(windowMenu->cascadeAct);
    selfmenu->addSeparator();
    selfmenu->addAction(windowMenu->nextAct);
    selfmenu->addAction(windowMenu->previousAct);
    selfmenu->addAction(windowMenu->separatorAct);
    //如有活动窗口则显示分隔条
    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    windowMenu->separatorAct->setVisible(!windows.isEmpty());
    //遍历各个子窗口，显示当前打开着的文档子窗口项
    for (int i = 0; i < windows.size(); ++i) {
        SubWindow *subwindow = qobject_cast<SubWindow *>(windows.at(i)->widget());
        QString text;
        text = tr("&%1 %2").arg(i + 1).arg(subwindow->currentFileName());
        //添加动作到菜单，设置动作可选
        QAction *action  = selfmenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(subwindow == activeSubWindow()); //设置当前活动窗口动作为选中状态
        //关联动作的触发信号映射器的map()槽，该槽会发送mapped()信号
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        //将动作与相应的窗口部件进行映射，在发送mapped()信号时就以该窗口为参数
        windowMapper->setMapping(action, windows.at(i));
    }
    setEnabledText(true);          //使得字体设置菜单可用
}

SubWindow *MainWindow::createSubWindow(winType type)
{
    SubWindow *subwindow = new SubWindow(type);     //创建子窗口部件
    mdiArea->addSubWindow(subwindow);               //多文档区域添加子窗口，作为中心部件
    //根据QTextEdit类鉴别复制、剪切和复制是否可用
    connect(subwindow->subView->textEdit, SIGNAL(copyAvailable(bool)),editMenu->cutAct, SLOT(setEnabled(bool)));
    connect(subwindow->subView->textEdit, SIGNAL(copyAvailable(bool)),editMenu->copyAct, SLOT(setEnabled(bool)));
    return subwindow;
}

void MainWindow::all_connect()
{
    //【file】
    //设置状态栏提示文本，当鼠标停留在该菜单项或工具按钮时浮现
    connect(fileMenu->newAct, SIGNAL(triggered()), this, SLOT(NewImage()));
    connect(fileMenu->openAct, SIGNAL(triggered()), this, SLOT(OpenImage()));
    connect(fileMenu->screencutAct, SIGNAL(triggered()), this, SLOT(CutImage()));
    connect(fileMenu->drawdigitAct, SIGNAL(triggered()), this, SLOT(DrawPic()));
    connect(fileMenu->qrcodeAct, SIGNAL(triggered()), this, SLOT(NewQrcode()));
    connect(fileMenu->clearRecentOpened, SIGNAL(triggered()), this, SLOT(clearOpenedHistory()));
    connect(fileMenu->saveAct, SIGNAL(triggered()), this, SLOT(SaveImage()));
    connect(fileMenu->saveAsAct, SIGNAL(triggered()), this, SLOT(SaveImageAs()));
    connect(fileMenu->exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows())); //不用自己实现
    //【编辑】
    connect(editMenu->undoAct, SIGNAL(triggered()), this, SLOT(undo()));      //不用子窗口类去实现
    connect(editMenu->redoAct, SIGNAL(triggered()), this, SLOT(redo()));      //不用子窗口类去实现
    connect(editMenu->cutAct, SIGNAL(triggered()), this, SLOT(cut()));        //不用子窗口类去实现
    connect(editMenu->copyAct, SIGNAL(triggered()), this, SLOT(copy()));      //不用子窗口类去实现
    connect(editMenu->pasteAct, SIGNAL(triggered()), this, SLOT(paste()));    //不用子窗口类去实现
    //【格式】
    connect(formatMenu->boldAct, SIGNAL(triggered()), this, SLOT(textBold()));
    connect(formatMenu->italicAct, SIGNAL(triggered()), this, SLOT(textItalic()));
    connect(formatMenu->underlineAct, SIGNAL(triggered()), this, SLOT(textUnderline()));
    connect(formatMenu->colorAct, SIGNAL(triggered()), this, SLOT(textColor()));
    connect(formatMenu->group, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));
    connect(formatMenu->set, SIGNAL(triggered(QAction*)), this, SLOT(windowStyle(QAction*)));
    //【窗口】
    connect(windowMenu->closeAct, SIGNAL(triggered()),mdiArea, SLOT(closeActiveSubWindow())); //不用自己实现
    connect(windowMenu->closeAllAct, SIGNAL(triggered()),mdiArea, SLOT(closeAllSubWindows()));//不用自己实现
    connect(windowMenu->tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));                 //不用自己实现
    connect(windowMenu->cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));           //不用自己实现
    connect(windowMenu->nextAct, SIGNAL(triggered()),mdiArea, SLOT(activateNextSubWindow()));           //不用自己实现
    connect(windowMenu->previousAct, SIGNAL(triggered()),mdiArea, SLOT(activatePreviousSubWindow()));   //不用自己实现
    //【提取】
    connect(extractMenu->drawHstgrmAct, SIGNAL(triggered()), this, SLOT(drawHistogram()));
    connect(extractMenu->greyScaleAct, SIGNAL(triggered()), this, SLOT(greyScale()));
    connect(extractMenu->binaryzationAct , SIGNAL(triggered()), this, SLOT(binaryzation()));
    connect(extractMenu->invertAct, SIGNAL(triggered()), this, SLOT(invert()));
    connect(extractMenu->sobelAct, SIGNAL(triggered()), this, SLOT(sobel()));
    connect(extractMenu->laplaceAct, SIGNAL(triggered()), this, SLOT(laplace()));
    //【帮助】
    connect(helpMenu->aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    connect(helpMenu->aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));         //不用自己实现
}

void MainWindow::createMenus()
{
    //【文件】主菜单
    fileMenu = new FileMenu;
    fileMenu->menu = menuBar()->addMenu(tr("菜单&F)"));
    selfmenu = fileMenu->menu;
    selfmenu->addAction(fileMenu->newAct);
    selfmenu->addAction(fileMenu->openAct);
    selfmenu->addAction(fileMenu->screencutAct);
    selfmenu->addAction(fileMenu->drawdigitAct);
    selfmenu->addAction(fileMenu->qrcodeAct);

    fileMenu->recentMenu = selfmenu->addMenu(tr("最近打开(&R)"));
    fileMenu->recentMenu->addAction(fileMenu->clearRecentOpened);
    recent = new RecentOpened(fileMenu->recentMenu);
    connect(recent,&RecentOpened::selectedItem,this,&MainWindow::recentOpened);
    selfmenu->addMenu(fileMenu->recentMenu);

    selfmenu->addSeparator();   //分隔线

    selfmenu->addAction(fileMenu->saveAct);
    selfmenu->addAction(fileMenu->saveAsAct);
    selfmenu->addSeparator();   //分隔线
    selfmenu->addAction(fileMenu->exitAct);

    //【编辑】主菜单
    editMenu = new EditMenu;
    editMenu->menu = menuBar()->addMenu(tr("编辑(&E)"));
    selfmenu = editMenu->menu;
    selfmenu->addAction(editMenu->undoAct);
    selfmenu->addAction(editMenu->redoAct);
    selfmenu->addSeparator();   //分隔线
    selfmenu->addAction(editMenu->cutAct);
    selfmenu->addAction(editMenu->copyAct);
    selfmenu->addAction(editMenu->pasteAct);

    //【格式】主菜单
    formatMenu = new FormatMenu;
    formatMenu->menu = menuBar()->addMenu(tr("格式(&O)"));
    selfmenu = formatMenu->menu;
    formatMenu->fontMenu = selfmenu->addMenu(QIcon(rsrcPath + "/text.png"),tr("字体(&D)"));   //【字体】子菜单
    formatMenu->fontMenu->addAction(formatMenu->boldAct);
    formatMenu->fontMenu->addAction(formatMenu->italicAct);
    formatMenu->fontMenu->addAction(formatMenu->underlineAct);
    formatMenu->fontMenu->addAction(formatMenu->colorAct);

    formatMenu->alignMenu = selfmenu->addMenu(QIcon(rsrcPath + "/section.png"),tr("段落"));  //【段落】子菜单
    formatMenu->alignMenu->addAction(formatMenu->leftAlignAct);
    formatMenu->alignMenu->addAction(formatMenu->centerAct);
    formatMenu->alignMenu->addAction(formatMenu->rightAlignAct);
    formatMenu->alignMenu->addAction(formatMenu->justifyAct);

    //0-"Windows" 1-"WindowsXP" 2-"WindowsVista" 3-"Fusion"
    formatMenu->styleMenu = selfmenu->addMenu(QIcon(rsrcPath + "/eye.png"),tr("风格"));     //【风格】子菜单
    formatMenu->styleMenu->addAction(formatMenu->WindowsAct);
    formatMenu->styleMenu->addAction(formatMenu->WindowsXPAct);
    formatMenu->styleMenu->addAction(formatMenu->WindowsVistaAct);
    formatMenu->styleMenu->addAction(formatMenu->FusionAct);

    //【窗口】主菜单
    windowMenu = new WindowMenu;
    windowMenu->menu = menuBar()->addMenu(tr("窗口(&W)"));
    updateWindowMenu();
    //connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));//不够稳定,有时没有更新
    menuBar()->addSeparator();

    //【提取】主菜单
    extractMenu = new ExtractMenu;
    extractMenu->menu = menuBar()->addMenu(tr("提取(&T)"));
    selfmenu = extractMenu->menu;
    selfmenu->addAction(extractMenu->drawHstgrmAct);
    selfmenu->addAction(extractMenu->greyScaleAct);
    selfmenu->addAction(extractMenu->binaryzationAct);
    selfmenu->addAction(extractMenu->invertAct);
    selfmenu->addSeparator();
    selfmenu->addAction(extractMenu->sobelAct);
    selfmenu->addAction(extractMenu->laplaceAct);

    //【帮助】主菜单
    helpMenu = new HelpMenu;
    helpMenu->menu = menuBar()->addMenu(tr("帮助(&H)"));
    selfmenu = helpMenu->menu;
    selfmenu->addAction(helpMenu->aboutAct);
    selfmenu->addSeparator();   //分隔线
    selfmenu->addAction(helpMenu->aboutQtAct);
}

void MainWindow::createToolBars()
{
//  QVBoxLayout *VBoxLayout = new QVBoxLayout();

    //"文件"工具栏
    QToolBar *fileToolBar = new QToolBar(tr("文件"));
    fileToolBar = addToolBar(tr("文件"));
    fileToolBar->setOrientation(Qt::Horizontal);
    fileToolBar->addAction(fileMenu->newAct);
    fileToolBar->addAction(fileMenu->openAct);
    fileToolBar->addAction(fileMenu->screencutAct);
    fileToolBar->addAction(fileMenu->drawdigitAct);
    fileToolBar->addAction(fileMenu->qrcodeAct);
    fileToolBar->addAction(fileMenu->saveAct);
    fileToolBar->addAction(fileMenu->saveAsAct);
    fileToolBar->addSeparator();        //分隔条

    //组合工具栏
    // addToolBarBreak(Qt::TopToolBarArea);    //使这个工具条在界面上另起一行显示
    comboToolBar = addToolBar(tr("组合选择"));
    comboFont = new QFontComboBox();
    QFont font(QString::fromLocal8Bit("Arial"));
    comboFont->setCurrentFont(font);
    comboFont->setFontFilters(QFontComboBox::ProportionalFonts);
    comboToolBar->addWidget(comboFont);
    comboFont->setStatusTip("更改字体");
    connect(comboFont, SIGNAL(activated(QString)), this, SLOT(textFamily(QString)));

    comboSize = new QComboBox();
    comboToolBar->addWidget(comboSize);
    comboSize->setEditable(true);
    comboSize->setStatusTip("更改字号");

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)), this, SLOT(textSize(QString)));
    //根据各窗口的设置调整字体大小，默认为14号字体,仅能根据选择调节字体大小，不能记录窗口原本的字体大小值
    //comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));//磅值
    comboSize->setCurrentIndex(7);

    //"编辑"工具栏
    addToolBarBreak(Qt::TopToolBarArea);
    QToolBar *editToolBar = new QToolBar(tr("编辑"));
    editToolBar->setOrientation(Qt::Vertical);
    editToolBar = addToolBar(tr("编辑"));
    editToolBar->addAction(editMenu->undoAct);
    editToolBar->addAction(editMenu->redoAct);
    editToolBar->addSeparator();        //分隔条
    editToolBar->addAction(editMenu->cutAct);
    editToolBar->addAction(editMenu->copyAct);
    editToolBar->addAction(editMenu->pasteAct);
    editToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::BottomToolBarArea);
    addToolBar(editToolBar);

    //"格式"工具栏
    addToolBarBreak(Qt::TopToolBarArea);
    QToolBar *formatToolBar = new QToolBar(tr("格式"));
    formatToolBar->setOrientation(Qt::Vertical);
    formatToolBar = addToolBar(tr("格式"));
    formatToolBar->addAction(formatMenu->boldAct);
    formatToolBar->addAction(formatMenu->italicAct);
    formatToolBar->addAction(formatMenu->underlineAct);
    formatToolBar->addSeparator();      //分隔条
    formatToolBar->addAction(formatMenu->colorAct);
    formatToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::BottomToolBarArea);
    addToolBar(formatToolBar);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("就绪"));
}

SubWindow *MainWindow::activeSubWindow()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<SubWindow *>(activeSubWindow->widget());
    return 0;
}

//获得当前用户的用户名
QString MainWindow::getUserName()
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString userName = userPath.section("/", -1, -1);
    return userName;
}

//获得当前用户的家目录
QString MainWindow::getUserPath()
{
    QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    return userPath;
}

//打开文件用,遍历多文档区域的子窗口查找是否已打开，以免重复打开
QMdiSubWindow *MainWindow::findSubWindow(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        SubWindow *subwindow = qobject_cast<SubWindow *>(window->widget());
        if (subwindow->currentPath() == canonicalFilePath)
            return window;
    }
    return nullptr;
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}
