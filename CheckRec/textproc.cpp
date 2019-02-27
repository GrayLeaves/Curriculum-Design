#include "mainwindow.h"
#include "subwindow.h"
#include "common.h"

void MainWindow::undo()
{
    if(activeSubWindow())
        activeSubWindow()->getTextEdit()->undo();
}

void MainWindow::redo()
{
    if(activeSubWindow())
        activeSubWindow()->getTextEdit()->redo();
}

void MainWindow::cut()
{
    if (activeSubWindow())
        activeSubWindow()->getTextEdit()->cut();
}

void MainWindow::copy()
{
    if (activeSubWindow())
        activeSubWindow()->getTextEdit()->copy();
}

void MainWindow::paste()
{
    if (activeSubWindow())
        activeSubWindow()->getTextEdit()->paste();
}

void MainWindow::setEnabledText(bool ok)
{
    boldAct->setEnabled(ok);            //加粗
    italicAct->setEnabled(ok);          //倾斜
    underlineAct->setEnabled(ok);       //下划线
    leftAlignAct->setEnabled(ok);       //居左
    centerAct->setEnabled(ok);          //居中
    rightAlignAct->setEnabled(ok);      //居右
    justifyAct->setEnabled(ok);         //调整
    colorAct->setEnabled(ok);           //颜色
}
void MainWindow::setEnabledSwitch(bool ok)
{
    closeAct->setEnabled(ok);
    closeAllAct->setEnabled(ok);
    tileAct->setEnabled(ok);
    cascadeAct->setEnabled(ok);
    nextAct->setEnabled(ok);
    previousAct->setEnabled(ok);
    separatorAct->setVisible(ok);
}
void MainWindow::setEnableIamgeProc(bool ok)
{
    drawHstgrmAct->setEnabled(ok);      //直方图
    greyScaleAct->setEnabled(ok);       //灰度化
    binaryzationAct->setEnabled(ok);    //二值化
    invertAct->setEnabled(ok);          //反色相
    sobelAct->setEnabled(ok);           //sobel
    laplaceAct->setEnabled(ok);         //laplace
}

void MainWindow::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldAct->isChecked() ? QFont::Bold : QFont::Normal);
    if(activeSubWindow())
        activeSubWindow()->mergeFormat(fmt);
}

void MainWindow::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italicAct->isChecked());
    if(activeSubWindow())
        activeSubWindow()->mergeFormat(fmt);
}

void MainWindow::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineAct->isChecked());
    if(activeSubWindow())
        activeSubWindow()->mergeFormat(fmt);
}

void MainWindow::textAlign(QAction *a)
{
    if(activeSubWindow())
    {
        if (a == leftAlignAct)
            activeSubWindow()->setAlign(1);
        else if (a == centerAct)
            activeSubWindow()->setAlign(2);
        else if (a == rightAlignAct)
            activeSubWindow()->setAlign(3);
        else if (a == justifyAct)
            activeSubWindow()->setAlign(4);
    }
}

void MainWindow::windowStyle(QAction *s)
{
    //0-"Windows" 1-"WindowsXP" 2-"WindowsVista" 3-"Fusion"
   if (s == WindowsAct){
       QApplication::setStyle(QStyleFactory::create("Windows"));      //设置内置风格
   }
   else if (s == WindowsXPAct){
       QApplication::setStyle(QStyleFactory::create("WindowsXP"));    //设置内置风格
   }
   else if (s == WindowsVistaAct){
       QApplication::setStyle(QStyleFactory::create("WindowsVista")); //设置内置风格
   }
   else if (s == FusionAct){
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
        QColor col = QColorDialog::getColor(activeSubWindow()->getTextEdit()->textColor(), this);
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
    boldAct->setChecked(f.bold());
    italicAct->setChecked(f.italic());
    underlineAct->setChecked(f.underline());
}

void MainWindow::colorChanged(const QColor &c)
{
    QPixmap pix(28, 28);
    pix.fill(c);
    colorAct->setIcon(pix);
}

void MainWindow::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft)
        leftAlignAct->setChecked(true);
    else if (a & Qt::AlignHCenter)
        centerAct->setChecked(true);
    else if (a & Qt::AlignRight)
        rightAlignAct->setChecked(true);
    else if (a & Qt::AlignJustify)
        justifyAct->setChecked(true);
}

void MainWindow::styleChanged(QString style)
{
    //0-"Windows" 1-"WindowsXP" 2-"WindowsVista" 3-"Fusion"
    if (style == "Windows")
        WindowsAct->setChecked(true);
    else if (style == "WindowsXP")
        WindowsXPAct->setChecked(true);
    else if (style == "WindowsVista")
        WindowsVistaAct->setChecked(true);
    else if (style == "Fusion")
        FusionAct->setChecked(true);
}

void MainWindow::textChangedUpdate()
{
    if(activeSubWindow())
    {
        QFont font = activeSubWindow()->getTextEdit()->currentFont();
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
    QTextCursor cursor = this->textEdit->textCursor();
    if (!cursor.hasSelection()) //选择光标下的单词，如不在则不选择对应文本
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    this->textEdit->mergeCurrentCharFormat(format);
}

//段落对齐设置
void SubWindow::setAlign(int align)
{
    if (align == 1)
        this->textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (align == 2)
        this->textEdit->setAlignment(Qt::AlignHCenter);
    else if (align == 3)
        this->textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (align == 4)
        this->textEdit->setAlignment(Qt::AlignJustify);
}
