#include "windows/mainwindow.h"
#include "windows/subwindow.h"
#include "common.h"

void MainWindow::undo()
{
    if(activeSubWindow())
        activeSubWindow()->subView->textEdit->undo();
}

void MainWindow::redo()
{
    if(activeSubWindow())
        activeSubWindow()->subView->textEdit->redo();
}

void MainWindow::cut()
{
    if (activeSubWindow())
        activeSubWindow()->subView->textEdit->cut();
}

void MainWindow::copy()
{
    if (activeSubWindow())
        activeSubWindow()->subView->textEdit->copy();
}

void MainWindow::paste()
{
    if (activeSubWindow())
        activeSubWindow()->subView->textEdit->paste();
}

void MainWindow::setEnabledText(bool ok)
{
    formatMenu->boldAct->setEnabled(ok);            //加粗
    formatMenu->italicAct->setEnabled(ok);          //倾斜
    formatMenu->underlineAct->setEnabled(ok);       //下划线
    formatMenu->leftAlignAct->setEnabled(ok);       //居左
    formatMenu->centerAct->setEnabled(ok);          //居中
    formatMenu->rightAlignAct->setEnabled(ok);      //居右
    formatMenu->justifyAct->setEnabled(ok);         //调整
    formatMenu->colorAct->setEnabled(ok);           //颜色
}
void MainWindow::setEnabledSwitch(bool ok)
{
    windowMenu->closeAct->setEnabled(ok);
    windowMenu->closeAllAct->setEnabled(ok);
    windowMenu->tileAct->setEnabled(ok);
    windowMenu->cascadeAct->setEnabled(ok);
    windowMenu->nextAct->setEnabled(ok);
    windowMenu->previousAct->setEnabled(ok);
    windowMenu->separatorAct->setVisible(ok);
}
void MainWindow::setEnableIamgeProc(bool ok)
{
    extractMenu->drawHstgrmAct->setEnabled(ok);      //直方图
    extractMenu->greyScaleAct->setEnabled(ok);       //灰度化
    extractMenu->binaryzationAct->setEnabled(ok);    //二值化
    extractMenu->invertAct->setEnabled(ok);          //反色相
    extractMenu->sobelAct->setEnabled(ok);           //sobel
    extractMenu->laplaceAct->setEnabled(ok);         //laplace
}

void MainWindow::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(formatMenu->boldAct->isChecked() ? QFont::Bold : QFont::Normal);
    if(activeSubWindow())
        activeSubWindow()->mergeFormat(fmt);
}

void MainWindow::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(formatMenu->italicAct->isChecked());
    if(activeSubWindow())
        activeSubWindow()->mergeFormat(fmt);
}

void MainWindow::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(formatMenu->underlineAct->isChecked());
    if(activeSubWindow())
        activeSubWindow()->mergeFormat(fmt);
}

void MainWindow::textAlign(QAction *a)
{
    if(activeSubWindow())
    {
        if (a == formatMenu->leftAlignAct)
            activeSubWindow()->setAlign(1);
        else if (a == formatMenu->centerAct)
            activeSubWindow()->setAlign(2);
        else if (a == formatMenu->rightAlignAct)
            activeSubWindow()->setAlign(3);
        else if (a == formatMenu->justifyAct)
            activeSubWindow()->setAlign(4);
    }
}

void MainWindow::windowStyle(QAction *s)
{
    //0-"Windows" 1-"WindowsXP" 2-"WindowsVista" 3-"Fusion"
   if (s == formatMenu->WindowsAct){
       QApplication::setStyle(QStyleFactory::create("Windows"));      //设置内置风格
   }
   else if (s == formatMenu->WindowsXPAct){
       QApplication::setStyle(QStyleFactory::create("WindowsXP"));    //设置内置风格
   }
   else if (s == formatMenu->WindowsVistaAct){
       QApplication::setStyle(QStyleFactory::create("WindowsVista")); //设置内置风格
   }
   else if (s == formatMenu->FusionAct){
       QApplication::setStyle(QStyleFactory::create("Fusion"));       //设置内置风格
   }

   QApplication::setPalette(QApplication::style()->standardPalette());//选择风格默认颜色
}

void MainWindow::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    if(activeSubWindow())
        activeSubWindow()->mergeFormat(fmt);
}

void MainWindow::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        if(activeSubWindow()){
            activeSubWindow()->mergeFormat(fmt);
        }
    }
}

void MainWindow::textColor()
{
    if(activeSubWindow())
    {
        QColor col = QColorDialog::getColor(activeSubWindow()->subView->textEdit->textColor(), this);
        if (!col.isValid())
            return;
        QTextCharFormat fmt;
        fmt.setForeground(col);
        activeSubWindow()->mergeFormat(fmt);
        colorChanged(col);
    }
}

void MainWindow::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    formatMenu->boldAct->setChecked(f.bold());
    formatMenu->italicAct->setChecked(f.italic());
    formatMenu->underlineAct->setChecked(f.underline());
}

void MainWindow::colorChanged(const QColor &c)
{
    QPixmap pix(28, 28);
    pix.fill(c);
    formatMenu->colorAct->setIcon(pix);
}

void MainWindow::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft)
        formatMenu->leftAlignAct->setChecked(true);
    else if (a & Qt::AlignHCenter)
        formatMenu->centerAct->setChecked(true);
    else if (a & Qt::AlignRight)
        formatMenu->rightAlignAct->setChecked(true);
    else if (a & Qt::AlignJustify)
        formatMenu->justifyAct->setChecked(true);
}

void MainWindow::styleChanged(QString style)
{
    //0-"Windows" 1-"WindowsXP" 2-"WindowsVista" 3-"Fusion"
    if (style == "Windows")
        formatMenu->WindowsAct->setChecked(true);
    else if (style == "WindowsXP")
        formatMenu->WindowsXPAct->setChecked(true);
    else if (style == "WindowsVista")
        formatMenu->WindowsVistaAct->setChecked(true);
    else if (style == "Fusion")
        formatMenu->FusionAct->setChecked(true);
}

void MainWindow::textChangedUpdate()
{
    if(activeSubWindow())
    {
        QFont font = activeSubWindow()->subView->textEdit->currentFont();
        fontChanged(font);
    }
}

void MainWindow::about()
{
    QMessageBox message(QMessageBox::NoIcon, tr("About This Software"), tr("<h1>Qt Verification Code Recognition 1.0</h1>"
                        "<h2>基于Qt5.8.0设计的多窗口验证码生成和识别软件</h2>"
                        "<p>Copyright &copy; 2018.06.30 WuZhenGuang Inc."
                        "<p>This is a small application that demostrates "
                        "QAction, QMainWindow, QMenuBar, QStatusBar, QToolBar, and "
                        "many other classes."));
    message.setIconPixmap(QPixmap(rsrcPath + "/about.png"));
    message.exec();
}

//格式设置
void SubWindow::mergeFormat(const QTextCharFormat &format)
{
    QTextCursor cursor = subView->textEdit->textCursor();
    if (!cursor.hasSelection()) //选择光标下的单词，如不在则不选择对应文本
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    subView->textEdit->mergeCurrentCharFormat(format);
}

//段落对齐设置
void SubWindow::setAlign(int align)
{
    if (align == 1)
        subView->textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (align == 2)
        subView->textEdit->setAlignment(Qt::AlignHCenter);
    else if (align == 3)
        subView->textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (align == 4)
        subView->textEdit->setAlignment(Qt::AlignJustify);
}
