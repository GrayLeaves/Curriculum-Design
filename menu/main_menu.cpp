#include "main_menu.h"
#include "common.h"

FileMenu::FileMenu()
{
    /*【文件】菜单动作集*/
    newAct = new QAction(QIcon(rsrcPath + "/newimage.png"), tr("创建验证码(&N)"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setToolTip("创建验证码");                           //设置工具栏按钮的提示文本
    newAct->setStatusTip(tr("创建一个生成和识别验证码的子窗体"));

    openAct = new QAction(QIcon(rsrcPath + "/openimage.png"), tr("导入验证码(&O)..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setToolTip("导入验证码");
    openAct->setStatusTip(tr("创建子窗体并打开已生成的验证码"));

    screencutAct = new QAction(QIcon(rsrcPath + "/screencut.png"), tr("屏幕截图(&)..."), this);
    screencutAct->setShortcut(Qt::CTRL + Qt::Key_K);
    screencutAct->setToolTip("验证码截图");
    screencutAct->setStatusTip(tr("从屏幕中将验证码截图并导入"));

    drawdigitAct = new QAction(QIcon(rsrcPath + "/paint.png"), tr("手写字符(&)..."), this);
    //screencutAct->setShortcut(Qt::CTRL + Qt::Key_D);
    drawdigitAct->setToolTip("绘制手写体字符");
    drawdigitAct->setStatusTip(tr("生成绘制手写体字符的画板"));

    qrcodeAct = new QAction(QIcon(rsrcPath + "/qrcode.png"), tr("创建二维码(&)"), this);
    qrcodeAct->setShortcuts(QKeySequence::New);
    qrcodeAct->setToolTip("创建二维码");                           //设置工具栏按钮的提示文本
    qrcodeAct->setStatusTip(tr("创建一个生成二维码的子窗体"));

    clearRecentOpened = new QAction(QIcon(rsrcPath + "/recent.png"), tr("清除记录(&)..."), this);
    clearRecentOpened->setToolTip("清除记录");
    clearRecentOpened->setStatusTip(tr("清除打开文件记录"));

    saveAct = new QAction(QIcon(rsrcPath + "/saveimage.png"), tr("保存验证码(&S)"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setToolTip("保存验证码");
    saveAct->setStatusTip(tr("保存当前的验证码"));

    saveAsAct = new QAction(QIcon(rsrcPath + "/saveas.png"),("另存验证码(&A)..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("拷贝并另存当前的验证码"));

    exitAct = new QAction(QIcon(rsrcPath + "/quit.png"),tr("退出(&G)"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("退出应用程序"));
}

FileMenu::~FileMenu()
{
    delete_s(menu);
    delete_s(newAct);
    delete_s(openAct);
    delete_s(screencutAct);
    delete_s(drawdigitAct);
    delete_s(qrcodeAct);
    delete_s(recentMenu);
    delete_s(clearRecentOpened);
    delete_s(saveAct);
    delete_s(saveAsAct);
    delete_s(exitAct);
}

EditMenu::EditMenu()
{
    /*【编辑】菜单动作集*/
    undoAct = new QAction(QIcon(rsrcPath + "/editundo.png"),tr("撤销(&U)"), this);
    undoAct->setShortcut(QKeySequence::Undo);
    undoAct->setToolTip("撤销");
    undoAct->setStatusTip(tr("撤销最近一次操作"));

    redoAct = new QAction(QIcon(rsrcPath + "/editredo.png"),tr("重做(&R)"), this);
    redoAct->setShortcut(QKeySequence::Redo);
    redoAct->setToolTip("恢复");
    redoAct->setStatusTip(tr("恢复上次撤销的操作"));

    cutAct = new QAction(QIcon(rsrcPath + "/editcut.png"),tr("剪切(&T)"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setToolTip("剪切");
    cutAct->setStatusTip(tr("从文档中裁剪所选内容，并将其放入剪贴板"));

    copyAct = new QAction(QIcon(rsrcPath + "/editcopy.png"),tr("复制(&C)"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setToolTip("复制");
    copyAct->setStatusTip(tr("拷贝所选内容，并将其放入剪贴板"));

    pasteAct = new QAction(QIcon(rsrcPath + "/editpaste.png"),tr("粘贴(&P)"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setToolTip("粘贴");
    pasteAct->setStatusTip(tr("将剪贴板的内容粘贴到文档"));
}

EditMenu::~EditMenu()
{
    delete_s(menu);
    delete_s(undoAct);
    delete_s(redoAct);
    delete_s(cutAct);
    delete_s(copyAct);
    delete_s(pasteAct);
}

FormatMenu::FormatMenu()
{
    /*【格式】菜单动作集*/
    boldAct = new QAction(QIcon(rsrcPath + "/textbold.png"),tr("加粗(&B)"), this);
    boldAct->setCheckable(true);
    boldAct->setShortcut(Qt::CTRL + Qt::Key_B);
    boldAct->setToolTip("加粗");
    boldAct->setStatusTip(tr("将所选文字加粗"));
    QFont bold;
    bold.setBold(true);
    boldAct->setFont(bold);

    italicAct = new QAction(QIcon(rsrcPath + "/textitalic.png"),tr("倾斜(&I)"), this);
    italicAct->setCheckable(true);
    italicAct->setShortcut(Qt::CTRL + Qt::Key_I);
    italicAct->setToolTip("倾斜");
    italicAct->setStatusTip(tr("将所选文字用斜体显示"));
    QFont italic;
    italic.setItalic(true);
    italicAct->setFont(italic);

    underlineAct = new QAction(QIcon(rsrcPath + "/textunderline.png"),tr("下划线(&U)"), this);
    underlineAct->setCheckable(true);
    underlineAct->setShortcut(Qt::CTRL + Qt::Key_U);
    underlineAct->setToolTip("下划线");
    underlineAct->setStatusTip(tr("给所选文字加下划线"));
    QFont underline;
    underline.setUnderline(true);
    underlineAct->setFont(underline);

    //【格式】→【段落】子菜单下的各项为同一个菜单项组，只能选中其中一项
    group = new QActionGroup(this);
    leftAlignAct = new QAction(QIcon(rsrcPath + "/textleft.png"),tr("左对齐(&L)"), group);
    centerAct = new QAction(QIcon(rsrcPath + "/textcenter.png"),tr("居中(&E)"), group);
    rightAlignAct = new QAction(QIcon(rsrcPath + "/textright.png"),tr("右对齐(&R)"), group);
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
    set = new QActionGroup(this);
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
}

FormatMenu::~FormatMenu()
{
    delete_s(menu);
    delete_s(fontMenu);
    delete_s(alignMenu);
    delete_s(styleMenu);
    delete_s(boldAct);
    delete_s(italicAct);
    delete_s(underlineAct);
    delete_s(group);
    delete_s(leftAlignAct);
    delete_s(centerAct);
    delete_s(rightAlignAct);
    delete_s(justifyAct);
    delete_s(set);
    delete_s(colorAct);
    delete_s(WindowsAct);
    delete_s(WindowsXPAct);
    delete_s(WindowsVistaAct);
    delete_s(FusionAct);
}

WindowMenu::WindowMenu()
{
    /*【窗口】菜单动作集*/
    /*不必自己实现的函数都是由QMdiArea类提供的，专门用于多个子窗口的管理，直接与相应菜单的动作绑定即可*/
    closeAct = new QAction(QIcon(rsrcPath + "/close.png"),tr("关闭(&O)"), this);
    closeAct->setStatusTip(tr("关闭活动文档子窗口"));

    closeAllAct = new QAction(QIcon(rsrcPath + "/closeall.png"),tr("关闭所有(&A)"), this);
    closeAllAct->setStatusTip(tr("关闭所有子窗口"));

    tileAct = new QAction(QIcon(rsrcPath + "/tile.png"),tr("平铺(&T)"), this);
    tileAct->setStatusTip(tr("平铺子窗口"));

    cascadeAct = new QAction(QIcon(rsrcPath + "/cascade.png"),tr("层叠(&C)"), this);
    cascadeAct->setStatusTip(tr("层叠子窗口"));

    nextAct = new QAction(QIcon(rsrcPath + "/next.png"),tr("下一个(&X)"), this);
    nextAct->setShortcuts(QKeySequence::NextChild);
    nextAct->setStatusTip(tr("移动焦点到下一个子窗口"));

    previousAct = new QAction(QIcon(rsrcPath + "/previous.png"),tr("前一个(&V)"), this);
    previousAct->setShortcuts(QKeySequence::PreviousChild);
    previousAct->setStatusTip(tr("移动焦点到前一个子窗口"));

    separatorAct = new QAction(this);
    separatorAct->setSeparator(true);
}

WindowMenu::~WindowMenu()
{
    delete_s(menu);
    delete_s(closeAct);
    delete_s(closeAllAct);
    delete_s(tileAct);
    delete_s(cascadeAct);
    delete_s(nextAct);
    delete_s(previousAct);
    delete_s(separatorAct);
}

ExtractMenu::ExtractMenu()
{
    /*【提取】菜单动作集*/
    drawHstgrmAct = new QAction(QIcon(rsrcPath + "/eye.png"),("绘制直方图(&)..."), this);
    drawHstgrmAct->setStatusTip(tr("绘制灰度、RGB分量的直方图"));

    greyScaleAct = new QAction(QIcon(rsrcPath + "/tooth.png"),tr("灰度变换(&G)"), this);
    greyScaleAct->setStatusTip(tr("灰度变换"));

    binaryzationAct = new QAction(QIcon(rsrcPath + "/binaryzation.png"),tr("二值化(&B)"), this);
    binaryzationAct ->setStatusTip(tr("二值化"));

    invertAct = new QAction(QIcon(rsrcPath + "/cv.png"),tr("反色(&I)"), this);
    invertAct->setStatusTip(tr("像素点反色"));

    sobelAct = new QAction(QIcon(rsrcPath + "/sobel.png"),tr("sobel边缘检测(&S)"), this);
    sobelAct->setStatusTip(tr("sobel边缘检测"));

    laplaceAct = new QAction(QIcon(rsrcPath + "/laplace.png"),tr("拉普拉斯锐化(&L)"), this);
    laplaceAct->setStatusTip(tr("拉普拉斯锐化"));
}

ExtractMenu::~ExtractMenu()
{
    delete_s(menu);
    delete_s(drawHstgrmAct);
    delete_s(greyScaleAct);
    delete_s(binaryzationAct);
    delete_s(invertAct);
    delete_s(sobelAct);
    delete_s(laplaceAct);
}

HelpMenu::HelpMenu()
{
    /*【帮助】菜单动作集*/
    aboutAct = new QAction(QIcon(rsrcPath + "/about.png"),tr("关于(&H)"), this);
    aboutAct->setStatusTip(tr("关于 MainWindow"));
    aboutAct->setShortcut(Qt::CTRL + Qt::Key_H);

    aboutQtAct = new QAction(QIcon(rsrcPath + "/qt.png"),tr("关于 Qt(&Qt)"), this);
    aboutQtAct->setStatusTip(tr("关于 Qt 库"));
    aboutAct->setShortcut(Qt::SHIFT + Qt::Key_Q);
}

HelpMenu::~HelpMenu()
{
    delete_s(menu);
    delete_s(aboutAct);
    delete_s(aboutQtAct);
}
