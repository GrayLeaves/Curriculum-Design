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
    //信号映射器QSignalMapper类实现多个相同部件的相同信号进行映射。
    //创建信号映射器
    windowMapper = new QSignalMapper(this);
    //映射器重新发送信号，根据信号设置活动窗口
    connect(windowMapper, SIGNAL(mapped(QWidget*)),this, SLOT(setActiveSubWindow(QWidget*)));

    createActions();
    createMenus();
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
    saveAct->setEnabled(hasSubWindow);
    saveAsAct->setEnabled(hasSubWindow);
    pasteAct->setEnabled(hasSubWindow);
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
    cutAct->setEnabled(hasSelection);
    copyAct->setEnabled(hasSelection);
    setEnabledText(hasSelection);

    //非生成验证码子窗口打开着的情况
    bool hasSelectPic = (hasSubWindow && activeSubWindow()->getCurrentWinType() != New);
    setEnableIamgeProc(hasSelectPic);
}

void MainWindow::updateWindowMenu()
{   //新建文档会在窗口菜单中动态更新显示
    //首先清空菜单，再逐个添加各菜单动作
    //窗口栏
    windowMenu->clear();
    windowMenu->addAction(closeAct);
    windowMenu->addAction(closeAllAct);
    windowMenu->addSeparator();             //分隔符
    windowMenu->addAction(tileAct);
    windowMenu->addAction(cascadeAct);
    windowMenu->addSeparator();
    windowMenu->addAction(nextAct);
    windowMenu->addAction(previousAct);
    windowMenu->addAction(separatorAct);
    //如有活动窗口则显示分隔条
    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    separatorAct->setVisible(!windows.isEmpty());
    //遍历各个子窗口，显示当前打开着的文档子窗口项
    for (int i = 0; i < windows.size(); ++i) {
        SubWindow *subwindow = qobject_cast<SubWindow *>(windows.at(i)->widget());
        QString text;
        text = tr("&%1 %2").arg(i + 1).arg(subwindow->currentFileName());
        //添加动作到菜单，设置动作可选
        QAction *action  = windowMenu->addAction(text);
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
    connect(subwindow->subView->textEdit, SIGNAL(copyAvailable(bool)),cutAct, SLOT(setEnabled(bool)));
    connect(subwindow->subView->textEdit, SIGNAL(copyAvailable(bool)),copyAct, SLOT(setEnabled(bool)));
    return subwindow;
}

void MainWindow::createActions() //【包含几乎所有的动作信号关联到槽以实现操作调用】
{
    /*【文件】菜单动作集*/
    newAct = new QAction(QIcon(rsrcPath + "/newimage.png"), tr("创建验证码(&N)"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setToolTip("创建验证码");                           //设置工具栏按钮的提示文本
    newAct->setStatusTip(tr("创建一个生成和识别验证码的子窗体"));
    //设置状态栏提示文本，当鼠标停留在该菜单项或工具按钮时浮现
    connect(newAct, SIGNAL(triggered()), this, SLOT(NewImage()));

    openAct = new QAction(QIcon(rsrcPath + "/openimage.png"), tr("导入验证码(&O)..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setToolTip("导入验证码");
    openAct->setStatusTip(tr("创建子窗体并打开已生成的验证码"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(OpenImage()));

    screencutAct = new QAction(QIcon(rsrcPath + "/screencut.png"), tr("屏幕截图(&)..."), this);
    screencutAct->setShortcut(Qt::CTRL + Qt::Key_K);
    screencutAct->setToolTip("验证码截图");
    screencutAct->setStatusTip(tr("从屏幕中将验证码截图并导入"));
    connect(screencutAct, SIGNAL(triggered()), this, SLOT(CutImage()));

    drawdigitAct = new QAction(QIcon(rsrcPath + "/paint.png"), tr("手写字符(&)..."), this);
    //screencutAct->setShortcut(Qt::CTRL + Qt::Key_D);
    drawdigitAct->setToolTip("绘制手写体字符");
    drawdigitAct->setStatusTip(tr("生成绘制手写体字符的画板"));
    connect(drawdigitAct, SIGNAL(triggered()), this, SLOT(DrawPic()));

    clearRecentOpened = new QAction(QIcon(rsrcPath + "/recent.png"), tr("清除记录(&)..."), this);
    clearRecentOpened->setToolTip("清除记录");
    clearRecentOpened->setStatusTip(tr("清除打开文件记录"));
    connect(clearRecentOpened, SIGNAL(triggered()), this, SLOT(clearOpenedHistory()));

    saveAct = new QAction(QIcon(rsrcPath + "/saveimage.png"), tr("保存验证码(&S)"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setToolTip("保存验证码");
    saveAct->setStatusTip(tr("保存当前的验证码"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(SaveImage()));

    saveAsAct = new QAction(QIcon(rsrcPath + "/saveas.png"),("另存验证码(&A)..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("拷贝并另存当前的验证码"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(SaveImageAs()));

    exitAct = new QAction(QIcon(rsrcPath + "/quit.png"),tr("退出(&G)"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("退出应用程序"));
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows())); //不用自己实现

    /*【编辑】菜单动作集*/
    undoAct = new QAction(QIcon(rsrcPath + "/editundo.png"),tr("撤销(&U)"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setToolTip("撤销");
    undoAct->setStatusTip(tr("撤销最近一次操作"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));      //不用子窗口类去实现

    redoAct = new QAction(QIcon(rsrcPath + "/editredo.png"),tr("重做(&R)"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setToolTip("恢复");
    redoAct->setStatusTip(tr("恢复上次撤销的操作"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));      //不用子窗口类去实现

    cutAct = new QAction(QIcon(rsrcPath + "/editcut.png"),tr("剪切(&T)"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setToolTip("剪切");
    cutAct->setStatusTip(tr("从文档中裁剪所选内容，并将其放入剪贴板"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));        //不用子窗口类去实现

    copyAct = new QAction(QIcon(rsrcPath + "/editcopy.png"),tr("复制(&C)"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setToolTip("复制");
    copyAct->setStatusTip(tr("拷贝所选内容，并将其放入剪贴板"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));      //不用子窗口类去实现

    pasteAct = new QAction(QIcon(rsrcPath + "/editpaste.png"),tr("粘贴(&P)"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setToolTip("粘贴");
    pasteAct->setStatusTip(tr("将剪贴板的内容粘贴到文档"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));    //不用子窗口类去实现

    /*【格式】菜单动作集*/
    boldAct = new QAction(QIcon(rsrcPath + "/textbold.png"),tr("加粗(&B)"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(Qt::CTRL + Qt::Key_B);
    boldAct->setToolTip("加粗");
    boldAct->setStatusTip(tr("将所选文字加粗"));
    QFont bold;
    bold.setBold(true);
    boldAct->setFont(bold);
    connect(boldAct, SIGNAL(triggered()), this, SLOT(textBold()));

    italicAct = new QAction(QIcon(rsrcPath + "/textitalic.png"),tr("倾斜(&I)"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(Qt::CTRL + Qt::Key_I);
    italicAct->setToolTip("倾斜");
    italicAct->setStatusTip(tr("将所选文字用斜体显示"));
    QFont italic;
    italic.setItalic(true);
    italicAct->setFont(italic);
    connect(italicAct, SIGNAL(triggered()), this, SLOT(textItalic()));

    underlineAct = new QAction(QIcon(rsrcPath + "/textunderline.png"),tr("下划线(&U)"), this);
    underlineAct->setCheckable(true);
    underlineAct->setShortcut(Qt::CTRL + Qt::Key_U);
    underlineAct->setToolTip("下划线");
    underlineAct->setStatusTip(tr("给所选文字加下划线"));
    QFont underline;
    underline.setUnderline(true);
    underlineAct->setFont(underline);
    connect(underlineAct, SIGNAL(triggered()), this, SLOT(textUnderline()));

    //【格式】→【段落】子菜单下的各项为同一个菜单项组，只能选中其中一项
    QActionGroup *group = new QActionGroup(this);
    connect(group, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));

    if (QApplication::isLeftToRight()) {
        leftAlignAct = new QAction(QIcon(rsrcPath + "/textleft.png"),tr("左对齐(&L)"), group);
        centerAct = new QAction(QIcon(rsrcPath + "/textcenter.png"),tr("居中(&E)"), group);
        rightAlignAct = new QAction(QIcon(rsrcPath + "/textright.png"),tr("右对齐(&R)"), group);
    } else {
        rightAlignAct = new QAction(QIcon(rsrcPath + "/textright.png"),tr("右对齐(&R)"), group);
        centerAct = new QAction(QIcon(rsrcPath + "/textcenter.png"),tr("居中(&E)"), group);
        leftAlignAct = new QAction(QIcon(rsrcPath + "/textleft.png"),tr("左对齐(&L)"), group);
    }
    justifyAct = new QAction(QIcon(rsrcPath + "/textjustify.png"),tr("两端对齐(&J)"), group);

    leftAlignAct->setShortcut(Qt::CTRL + Qt::Key_L);
    leftAlignAct->setCheckable(true);
    leftAlignAct->setToolTip("左对齐");
    leftAlignAct->setStatusTip(tr("将文字左对齐"));

    centerAct->setShortcut(Qt::CTRL + Qt::Key_E);
    centerAct->setCheckable(true);
    centerAct->setToolTip("居中");
    centerAct->setStatusTip(tr("将文字居中对齐"));

    rightAlignAct->setShortcut(Qt::CTRL + Qt::Key_R);
    rightAlignAct->setCheckable(true);
    rightAlignAct->setToolTip("右对齐");
    rightAlignAct->setStatusTip(tr("将文字右对齐"));

    justifyAct->setShortcut(Qt::CTRL + Qt::Key_J);
    justifyAct->setCheckable(true);
    justifyAct->setToolTip("两端对齐");
    justifyAct->setStatusTip(tr("将文字左右两端同时对齐，并根据需要增加字间距"));

    //【编辑】→【风格】子菜单下的各项为同一个菜单项组，只能选中其中一项
    //0-"Windows" 1-"WindowsXP" 2-"WindowsVista" 3-"Fusion"
    QActionGroup *set = new QActionGroup(this);
    connect(set, SIGNAL(triggered(QAction*)), this, SLOT(windowStyle(QAction*)));
    WindowsAct = new QAction(QIcon(rsrcPath + "/textright.png"),tr("Windows(&W)"), set);
    WindowsXPAct = new QAction(QIcon(rsrcPath + "/textcenter.png"),tr("WindowsXP(&X)"), set);
    WindowsVistaAct = new QAction(QIcon(rsrcPath + "/textleft.png"),tr("WindowsVista(&V)"), set);
    FusionAct = new QAction(QIcon(rsrcPath + "/textjustify.png"),tr("Fusion(&F)"), set);

    WindowsAct->setCheckable(true);
    WindowsAct->setToolTip("Windows");
    WindowsAct->setStatusTip(tr("设置窗体为Windows风格"));

    WindowsXPAct->setCheckable(true);
    WindowsXPAct->setToolTip("WindowsXP");
    WindowsXPAct->setStatusTip(tr("设置窗体为WindowsXP风格"));

    WindowsVistaAct->setCheckable(true);
    WindowsVistaAct->setToolTip("WindowsVista");
    WindowsVistaAct->setStatusTip(tr("设置窗体为WindowsVista风格"));

    FusionAct->setCheckable(true);
    FusionAct->setToolTip("Fushion");
    FusionAct->setStatusTip(tr("设置窗体为Fushion风格"));

    QPixmap pix(28,28);
    pix.fill(Qt::black);
    colorAct = new QAction(pix, tr("颜色(&C)..."), this);
    colorAct->setToolTip("颜色");
    colorAct->setStatusTip(tr("设置文字颜色"));
    connect(colorAct, SIGNAL(triggered()), this, SLOT(textColor()));

    /*【窗口】菜单动作集*/
    /*不必自己实现的函数都是由QMdiArea类提供的，专门用于多个子窗口的管理，直接与相应菜单的动作绑定即可*/
    closeAct = new QAction(QIcon(rsrcPath + "/close.png"),tr("关闭(&O)"), this);
    closeAct->setStatusTip(tr("关闭活动文档子窗口"));
    connect(closeAct, SIGNAL(triggered()),mdiArea, SLOT(closeActiveSubWindow()));           //不用自己实现

    closeAllAct = new QAction(QIcon(rsrcPath + "/closeall.png"),tr("关闭所有(&A)"), this);
    closeAllAct->setStatusTip(tr("关闭所有子窗口"));
    connect(closeAllAct, SIGNAL(triggered()),mdiArea, SLOT(closeAllSubWindows()));          //不用自己实现

    tileAct = new QAction(QIcon(rsrcPath + "/tile.png"),tr("平铺(&T)"), this);
    tileAct->setStatusTip(tr("平铺子窗口"));
    connect(tileAct, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));                 //不用自己实现

    cascadeAct = new QAction(QIcon(rsrcPath + "/cascade.png"),tr("层叠(&C)"), this);
    cascadeAct->setStatusTip(tr("层叠子窗口"));
    connect(cascadeAct, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));           //不用自己实现

    nextAct = new QAction(QIcon(rsrcPath + "/next.png"),tr("下一个(&X)"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("移动焦点到下一个子窗口"));
    connect(nextAct, SIGNAL(triggered()),mdiArea, SLOT(activateNextSubWindow()));           //不用自己实现

    previousAct = new QAction(QIcon(rsrcPath + "/previous.png"),tr("前一个(&V)"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("移动焦点到前一个子窗口"));
    connect(previousAct, SIGNAL(triggered()),mdiArea, SLOT(activatePreviousSubWindow()));   //不用自己实现

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);

    /*【提取】菜单动作集*/
    drawHstgrmAct = new QAction(QIcon(rsrcPath + "/eye.png"),("绘制直方图(&)..."), this);
    drawHstgrmAct->setStatusTip(tr("绘制灰度、RGB分量的直方图"));
    connect(drawHstgrmAct, SIGNAL(triggered()), this, SLOT(drawHistogram()));

    greyScaleAct = new QAction(QIcon(rsrcPath + "/tooth.png"),tr("灰度变换(&G)"), this);
    greyScaleAct->setStatusTip(tr("灰度变换"));
    connect(greyScaleAct, SIGNAL(triggered()), this, SLOT(greyScale()));

    binaryzationAct = new QAction(QIcon(rsrcPath + "/binaryzation.png"),tr("二值化(&B)"), this);
    binaryzationAct ->setStatusTip(tr("二值化"));
    connect(binaryzationAct , SIGNAL(triggered()), this, SLOT(binaryzation()));

    invertAct = new QAction(QIcon(rsrcPath + "/cv.png"),tr("反色(&I)"), this);
    invertAct->setStatusTip(tr("像素点反色"));
    connect(invertAct, SIGNAL(triggered()), this, SLOT(invert()));

    sobelAct = new QAction(QIcon(rsrcPath + "/sobel.png"),tr("sobel边缘检测(&S)"), this);
    sobelAct->setStatusTip(tr("sobel边缘检测"));
    connect(sobelAct, SIGNAL(triggered()), this, SLOT(sobel()));

    laplaceAct = new QAction(QIcon(rsrcPath + "/laplace.png"),tr("拉普拉斯锐化(&L)"), this);
    laplaceAct->setStatusTip(tr("拉普拉斯锐化"));
    connect(laplaceAct, SIGNAL(triggered()), this, SLOT(laplace()));

    /*【帮助】菜单动作集*/
    aboutAct = new QAction(QIcon(rsrcPath + "/about.png"),tr("关于(&H)"), this);
    aboutAct->setStatusTip(tr("关于 MainWindow"));
    aboutAct->setShortcut(Qt::CTRL + Qt::Key_H);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(QIcon(rsrcPath + "/qt.png"),tr("关于 Qt(&Qt)"), this);
    aboutQtAct->setStatusTip(tr("关于 Qt 库"));
    aboutAct->setShortcut(Qt::SHIFT + Qt::Key_Q);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));                        //不用自己实现
}

void MainWindow::createMenus()
{
    //【文件】主菜单
    fileMenu = menuBar()->addMenu(tr("验证码(&F)"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(screencutAct);
    fileMenu->addAction(drawdigitAct);

    recentMenu = fileMenu->addMenu(tr("最近打开(&R)"));
    recentMenu->addAction(clearRecentOpened);
    recent = new RecentOpened(recentMenu);
    connect(recent,&RecentOpened::selectedItem,this,&MainWindow::recentOpened);

    fileMenu->addMenu(recentMenu);
    fileMenu->addSeparator();   //分隔线
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();   //分隔线
    fileMenu->addAction(exitAct);

    //【编辑】主菜单
    editMenu = menuBar()->addMenu(tr("编辑(&E)"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();   //分隔线
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    //【格式】主菜单
    formatMenu = menuBar()->addMenu(tr("格式(&O)"));
    fontMenu = formatMenu->addMenu(QIcon(rsrcPath + "/text.png"),tr("字体(&D)"));   //【字体】子菜单
    fontMenu->addAction(boldAct);
    fontMenu->addAction(italicAct);
    fontMenu->addAction(underlineAct);
    fontMenu->addAction(colorAct);

    alignMenu = formatMenu->addMenu(QIcon(rsrcPath + "/section.png"),tr("段落"));  //【段落】子菜单
    alignMenu->addAction(leftAlignAct);
    alignMenu->addAction(centerAct);
    alignMenu->addAction(rightAlignAct);
    alignMenu->addAction(justifyAct);

    //0-"Windows" 1-"WindowsXP" 2-"WindowsVista" 3-"Fusion"
    alignMenu = formatMenu->addMenu(QIcon(rsrcPath + "/eye.png"),tr("风格"));     //【风格】子菜单
    alignMenu->addAction(WindowsAct);
    alignMenu->addAction(WindowsXPAct);
    alignMenu->addAction(WindowsVistaAct);
    alignMenu->addAction(FusionAct);

    //【窗口】主菜单
    windowMenu = menuBar()->addMenu(tr("窗口(&W)"));
    updateWindowMenu();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));
    menuBar()->addSeparator();

    //【提取】主菜单
    extractMenu = menuBar()->addMenu(tr("提取(&T)"));
    extractMenu->addAction(drawHstgrmAct);
    extractMenu->addAction(greyScaleAct);
    extractMenu->addAction(binaryzationAct);
    extractMenu->addAction(invertAct);
    extractMenu->addSeparator();
    extractMenu->addAction(sobelAct);
    extractMenu->addAction(laplaceAct);

    //【帮助】主菜单
    helpMenu = menuBar()->addMenu(tr("帮助(&H)"));
    helpMenu->addAction(aboutAct);
    helpMenu->addSeparator();   //分隔线
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
//  QVBoxLayout *VBoxLayout = new QVBoxLayout();

    //"文件"工具栏
    QToolBar *fileToolBar = new QToolBar(tr("文件"));
    fileToolBar = addToolBar(tr("文件"));
    fileToolBar->setOrientation(Qt::Horizontal);
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(screencutAct);
    fileToolBar->addAction(drawdigitAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
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
    editToolBar->addAction(undoAct);
    editToolBar->addAction(redoAct);
    editToolBar->addSeparator();        //分隔条
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::BottomToolBarArea);
    addToolBar(editToolBar);

    //"格式"工具栏
    addToolBarBreak(Qt::TopToolBarArea);
    QToolBar *formatToolBar = new QToolBar(tr("格式"));
    formatToolBar->setOrientation(Qt::Vertical);
    formatToolBar = addToolBar(tr("格式"));
    formatToolBar->addAction(boldAct);
    formatToolBar->addAction(italicAct);
    formatToolBar->addAction(underlineAct);
    formatToolBar->addSeparator();      //分隔条
    formatToolBar->addAction(colorAct);
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
