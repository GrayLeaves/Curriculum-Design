#include "dialog.h"
#include "mainwindow.h"
#include "subwindow.h"
#include "recentopened.h"
void MainWindow::CutImage()
{
    Dialog dlg;
    /*
    connect(&dlg,SIGNAL(Dialog::finish(QPoint,QPoint)),this,SLOT(screenCut(QPoint,QPoint)));
    connect(&dlg,SIGNAL(Dialog::closeF()),this,SLOT(showImage()));*/
    connect(&dlg,&Dialog::finish,this,&MainWindow::screenCut);
    connect(&dlg,&Dialog::closeF,this,&MainWindow::showImage);
    this->hide();           //隐藏主窗口
    dlg.exec();
}

void MainWindow::showImage()
{
    if (finish) {
        winType type = Cut;
        SubWindow *subwindow = createSubWindow(type);
        if(subwindow->showFile(pixmap))
        {
            //关联子窗体的信号：关闭原有窗口，重新导入同类型文件
            connect(subwindow,&SubWindow::reOpenImage,this,&MainWindow::reLoadImage);
            //关联子窗体信号，根据字体来实时更新主窗体字体菜单状态栏
            connect(subwindow,&SubWindow::textUpdate,this,&MainWindow::textChangedUpdate);

            statusBar()->showMessage(tr("图片已从截图载入"), 2000);
            subwindow->show();
            setEnabledText(true);          //使得字体设置菜单可用
            subwindow->cutFile();
        }
    } else {
        statusBar()->showMessage(tr("取消从屏幕载入图片"), 2000);
    }
    this->show();
}

void MainWindow::recentOpened(int index)
{
    QString fileName;
    QSettings settings("settings.ini",QSettings::IniFormat);
    settings.beginGroup("RecentlyOpened");
    fileName = settings.value(QString::number(index)).toString();
    settings.endGroup();

    openFile(fileName);
}

void MainWindow::clearOpenedHistory()
{
    recent->clearActions();
}

void MainWindow::screenCut(QPoint pt1,QPoint pt2)
{
    int x = pt1.x()+1, y = pt1.y()+1;
    int w = pt2.x()-pt1.x()-2, h = pt2.y()-pt1.y()-2;
    screen = QGuiApplication::primaryScreen();
    pixmap = screen->grabWindow(QApplication::desktop()->winId(),x,y,w,h);
    //pixmap = QScreen::grabWindow(QApplication::desktop()->winId(),x,y,w,h);
    /*QClipboard * clipborder=QApplication::clipboard();
    clipborder->setPixmap(this->pixmap);*/
    finish = pixmap.isNull()? false:true;
}
/*
void MainWindow::resizeEvent(QResizeEvent * event)//重写
{
    label->setPixmap(this->pic.scaled(ui->label->size()));
}*/
