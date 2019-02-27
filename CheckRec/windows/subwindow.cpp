//#include "recognition/recognition.h"
#include "subwindow.h"
#include "drawdigit/drawdigit.h"
#include "common.h"
#include "proc/proc.h"
#include <string>
#include <QtMath>
#include <QTextStream>
#include <QString>
/*
 * 设置窗口编号
 * 保存文件路径，为curFile赋初值
 * 设置子窗口标题
 * 关联文档内容改变信号contentsChange()到显示文档更改状态标志槽documentWasModified()
*/

SubWindow::SubWindow(winType type) : currentWinType(type)
{
    subView = createView(type);
    subView->generateView();
    setLayout(subView->mainLayout);
    all_connect(type);
    setAttribute(Qt::WA_DeleteOnClose);    //设置在主窗口关闭时销毁这个类的对象实例
}

void SubWindow::all_connect(winType type)
{
    connect(subView->functionBtn,SIGNAL(clicked()),this,SLOT(tool()));
    switch(type){
        case New:  break;
        case Open: {
            openView* view = nullptr;
            try{
                view = dynamic_cast<openView *>(subView);
            }catch (std::bad_cast& bc){
                qDebug() << "bad_cast caught: " << bc.what();
            }
            //关联信号到槽，将spinbox - slider - zoomInOut 关联起来
            connect(view->spinbox,SIGNAL(valueChanged(int)),view->slider,SLOT(setValue(int)));
            connect(view->slider,SIGNAL(valueChanged(int)),view->spinbox,SLOT(setValue(int)));
            connect(view->slider,SIGNAL(valueChanged(int)),this,SLOT(zoomInOut(int)));
            connect(view->zoomin,SIGNAL(clicked()), this, SLOT(zoomIn()));
            connect(view->zoomout,SIGNAL(clicked()), this, SLOT(zoomOut()));
        } break;
        case Cut: break;
        case Draw:{
            drawView* view = nullptr;
            try{
                view = dynamic_cast<drawView *>(subView);
            }catch (std::bad_cast& bc){
                qDebug() << "bad_cast caught: " << bc.what();
            }
            connect(view->widthSpinBox, SIGNAL(valueChanged(int)), view->drawdigit, SLOT(setWidth(int)));
        } break;
        default:;
    }
}
SubView* SubWindow::createView(winType type){ //static
    SubView* view = nullptr;
    switch(type){
        case New: view = new newView(); break;
        case Open: view = new openView(); break;
        case Cut: view = new cutView(); break;
        case Draw: view = new drawView(); break;
        default:;
    }
    return view;
}

SubWindow::~SubWindow()
{
    /*delete_s(size);
    delete_s(codeArea);
    delete_s(scene);
    delete_s(slider);
    delete_s(textEdit);
    delete_s(msgBox);*/
}

void SubWindow::newFile()
{
    newView* view = nullptr;
    try{
        view = dynamic_cast<newView *>(subView);
    }catch (std::bad_cast& bc){
        qDebug() << "bad_cast caught: " << bc.what();
    }

    toSaveIt = false;
    //新建验证码的默认命名验证码的字符值一致
    curPath = view->codeArea->getCode();
    setWindowTitle(curPath);
    //编辑框的内容被更改时发送contentsChange()信号来执行isModified()函数
    connect(subView->textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(isModified()));
}

void SubWindow::cutFile()
{
    //设置文档编号，因编号始终存在，故为静态变量
    static int sequenceNum = 1;
    //屏幕截图获得的图片默认命名为“屏幕截图_序号”的形式，编号
    toSaveIt = true;        //提醒保存
    curPath = tr("Screen_cut_%1").arg(sequenceNum++);
    //设置窗体标题，若使用[*]表示还未被保存过
    setWindowTitle(curPath + "[*]");
    setWindowModified(true);
    //若编辑框的内容被更改时发送contentsChange()信号来执行isModified()函数
    connect(subView->textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(isModified()));
}

void SubWindow::drawFile(){
    //设置文档编号，因编号始终存在，故为静态变量
    static int sequenceNum = 0;
    //屏幕截图获得的图片默认命名为“屏幕截图_序号”的形式，编号
    toSaveIt = true;        //提醒保存
    curPath = tr("Qc_%1").arg(sequenceNum++);
    //设置窗体标题，若使用[*]表示还未被保存过
    setWindowTitle(curPath + "[*]");
    setWindowModified(true);
    //若编辑框的内容被更改时发送contentsChange()信号来执行isModified()函数
    connect(subView->textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(isModified()));
}

bool SubWindow::loadFile(const QString &fileName)
{
    if (!fileName.isEmpty())
    {
        if (!QFile::exists(fileName))
            return false;
        QFile file(fileName);
        //以只读方式打开，出错则提示并返回false
        if (!file.open(QFile::ReadOnly)){
            QMessageBox::critical(this, tr("打开错误"),tr("无法打开该图片"));
            return false;
        }

        //info = new QFileInfo(fileName);
        pixmap.load(fileName);

        openView* view = nullptr;
        try{
            view = dynamic_cast<openView *>(subView);
        }catch (std::bad_cast& bc){
            qDebug() << "bad_cast caught: " << bc.what();
        }
        pixmapItem = view->scene->addPixmap(pixmap);                      //显示导入的验证码
        view->scene->setSceneRect(QRectF(pixmap.rect()));                 //设置显示大小

        //qDebug() << "depth:" << pixmap.depth();
        //qDebug() << "hasAlpha:" << pixmap.hasAlpha();
        setCurrentFile(fileName);
        view->size->setText("Dimensions:" + QString::number(pixmapItem->pixmap().width())
                      + " x " + QString::number(pixmapItem->pixmap().height()));
        //若编辑框的内容被更改时发送contentsChange()信号来执行isModified()函数
        connect(subView->textEdit->document(), SIGNAL(contentsChanged()),this, SLOT(isModified()));
        return true;
    }
    return false;
}

void SubWindow::isModified()
{
    //编辑器是否被更改修改窗口状态setWindowModified()改为更新主窗口更新字体菜单
    if(subView->textEdit->document()->isModified())
    {
        textUpdate();       //发送更新主窗体字体菜单状态栏的信号
    }
}

bool SubWindow::readPath(QString& fullPath){
    QString strText = subView->textEdit->toPlainText();
    QTextStream ts(&strText);
    fullPath = ts.readLine();
    QDir dir(fullPath);
    if(dir.exists())
        return true;
    else
        return false;
}

void SubWindow::genLabels(){

    QFont srcfont(QString::fromLocal8Bit("Arial"),10); //路径很长，调整字体
    subView->textEdit->setFont(srcfont);

    QString path;
    bool success = readPath(path);
    if(path.isNull()){
        msgBox = new QMessageBox(QMessageBox::Information, QStringLiteral("Tips"),
                 QStringLiteral("请在编辑框内输入文件路径"),QMessageBox::Ok, this);
        msgBox->setIconPixmap(QPixmap(rsrcPath + "/sobel.png"));
    }
    else{
        if(!success){ //读取路径
                msgBox = new QMessageBox(QMessageBox::Information, QStringLiteral("Tips"),
                            QStringLiteral("FilePath doesn't exist."),QMessageBox::Ok, this);
                msgBox->setIconPixmap(QPixmap(rsrcPath + "/again.png"));
        }
        else{
            if(proc::genLabels(path)){
                msgBox = new QMessageBox(QMessageBox::Information, QStringLiteral("Tips"),
                         QStringLiteral("Train completed."),QMessageBox::Ok, this);
                msgBox->setIconPixmap(QPixmap(rsrcPath + "/success.png"));
            }
            else{
                msgBox = new QMessageBox(QMessageBox::Information, QStringLiteral("Tips"),
                         QStringLiteral("Train failed."),QMessageBox::Ok, this);
                msgBox->setIconPixmap(QPixmap(rsrcPath + "/failure.png"));
            }
        }
        QFont dstfont(QString::fromLocal8Bit("Arial"),14); //还原字体
        subView->textEdit->setFont(dstfont);
        subView->textEdit->clear();
    }

    msgBox->exec();
    delete_s(msgBox);
}

void SubWindow::showResult(){
    QString show;
    if(!toSaveIt){
        qDebug() << curPath;
        int charRec = proc::useModel(curPath);
        if(charRec>=0 && charRec<=9){
            show = QString('0'+charRec);
        }
        else{
            show = QString("Wrong");
            qDebug() << "识别失败";
        }
    }
    qDebug() << show;

    subView->textEdit->clear();
    subView->textEdit->setText(show);
    subView->textEdit->setAlignment(Qt::AlignCenter);
}

void SubWindow::replaceCode()
{
    if(currentWinType == New){
        newView* view = nullptr;
        try{
            view = dynamic_cast<newView *>(subView);
        }catch (std::bad_cast& bc){
            qDebug() << "bad_cast caught: " << bc.what();
        }
        view->codeArea->replaceCodePic(); //更新验证码
    }
}
//将结果送到编辑框，自定义生成的
void SubWindow::recognizeCode()
{
    if(currentWinType == New){
        newView* view = nullptr;
        try{
            view = dynamic_cast<newView *>(subView);
        }catch (std::bad_cast& bc){
            qDebug() << "bad_cast caught: " << bc.what();
        }
        subView->textEdit->clear();
        subView->textEdit->setText(view->codeArea->getCode());
        subView->textEdit->setAlignment(Qt::AlignCenter);
    }
}

//将识别结果送到编辑框，用dnn获得的
void SubWindow::recognizePic()
{
    subView->textEdit->clear();
    winType type = getCurrentWinType();
    if(type == Cut && toSaveIt == true)
    {
        msgBox = new QMessageBox(QMessageBox::Information, QStringLiteral("Tips"),
                                 QStringLiteral("Warning."),QMessageBox::Ok, this);
        msgBox->setIconPixmap(QPixmap(rsrcPath + "/again.png"));
        this->save();
    }
    std::string picture(curPath.toLatin1().data());
    qDebug() << picture.c_str();
    std::string result /*= recognition(picture)*/;
    if(result.size()<4 || result.size()>5)
        subView->textEdit->setText(QString("Unexpected."));
    else
        subView->textEdit->setText(QString(result.c_str()));
    subView->textEdit->setAlignment(Qt::AlignCenter);
}

//设置验证码
void SubWindow::setCode()
{
    QString strText = subView->textEdit->toPlainText();
    QTextStream ts(&strText);
    /*while( ! ts.atEnd())
    {
        qDebug()<<ts.readLine();
    }*/
    QString code = ts.readLine();
    newView* view = nullptr;
    try{
        view = dynamic_cast<newView *>(subView);
    }catch (std::bad_cast& bc){
        qDebug() << "bad_cast caught: " << bc.what();
    }
    view->codeArea->setCode(code);
    newFile();
}
//认证检验
void SubWindow::checkCode()
{
    newView* view = nullptr;
    try{
        view = dynamic_cast<newView *>(subView);
    }catch (std::bad_cast& bc){
        qDebug() << "bad_cast caught: " << bc.what();
    }
    QString strText = subView->textEdit->toPlainText();
    QTextStream ts(&strText);
    QString user = ts.readLine();
    QString code = "";
    for(int i=0; i<user.size();i++)
    {
        QChar c = user[i];
        if(c.isDigit()||c.isLetter())
            code += QString(c);
    }
    if(code.size() == Constant::DEF_CODECOUNT)
    {
        if (view->codeArea->checkCode(user))
        {
           msgBox = new QMessageBox(QMessageBox::Information, QStringLiteral("Tips"),
                                    QStringLiteral("Success."),QMessageBox::Ok, this);
           msgBox->setIconPixmap(QPixmap(rsrcPath + "/success.png"));
        }
        else
        {
           msgBox = new QMessageBox(QMessageBox::Information, QStringLiteral("Tips"),
                                    QStringLiteral("Failure."),QMessageBox::Ok, this);
           msgBox->setIconPixmap(QPixmap(rsrcPath + "/failure.png"));
        }
    }
    else{
          msgBox = new QMessageBox(QMessageBox::Information, QStringLiteral("Tips"),
                                 QStringLiteral("Mismatch."),QMessageBox::Ok, this);
          msgBox->setIconPixmap(QPixmap(rsrcPath + "/again.png"));
    }
    msgBox->exec();
    delete_s(msgBox);
}
//图片缩放
void SubWindow::zoomInOut(int value)   	//缩放
{
    openView* view = nullptr;
    try{
        view = dynamic_cast<openView *>(subView);
    }catch (std::bad_cast& bc){
        qDebug() << "bad_cast caught: " << bc.what();
    }
    qreal s = 1.0;
    if(value > view->zoom)                 		//放大
    {
        s=pow(1.02,(value - view->zoom));
    }
    else                             	//缩小
    {
        s=pow(1/1.02,(view->zoom - value));
    }
    view->gview->scale(s,s);
    view->zoom = value;
}
void SubWindow::zoomIn()   	//缩放
{
    openView* view = nullptr;
    try{
        view = dynamic_cast<openView *>(subView);
    }catch (std::bad_cast& bc){
        qDebug() << "bad_cast caught: " << bc.what();
    }
    qreal s = view->slider->value();
    if(s < 99.0) s += 1.0;
    else s = 99.0;
    view->slider->setValue(s);
}

void SubWindow::zoomOut()   	//缩放
{
    openView* view = nullptr;
    try{
        view = dynamic_cast<openView *>(subView);
    }catch (std::bad_cast& bc){
        qDebug() << "bad_cast caught: " << bc.what();
    }
    qreal s = view->slider->value();
    if(s > 1.0) s -= 1.0;
    else s = 1.0;
    view->slider->setValue(s);
}
//还原
void SubWindow::resetZoom()   	//缩放
{
    openView* view = nullptr;
    try{
        view = dynamic_cast<openView *>(subView);
    }catch (std::bad_cast& bc){
        qDebug() << "bad_cast caught: " << bc.what();
    }
    qreal s = 1.0;
    qreal v = view->slider->value();
    if(v > 50.0)                 		//已放大
    {
        s=pow(1/1.02,(v-50.0));
    }
    else                             	//已缩小
    {
        s=pow(1.02,(50.0-v));
    }
    view->gview->scale(s,s);
    view->zoom = 50.0;
    view->slider->setValue(view->zoom);
}

void SubWindow::tool()
{
    winType type = getCurrentWinType();
    int index = subView->combox->currentIndex();
    switch(index)
    {
        case 0: {
                    switch(type){
                        case New: recognizeCode(); break;//揭晓验证码的答案
                        case Open: case Cut: recognizePic(); break;//调用验证码识别功能
                        case Draw: showResult();  break; //返回识别结果
                        default:;
                    }
                    break;
                }
        case 1: {
                    switch(type){
                        case New: checkCode(); break;//用户校对验证码的结果
                        case Open: case Cut: restoreImage(); break; //恢复图像
                        case Draw: {
                            drawView* view = nullptr;
                            try{
                                view = dynamic_cast<drawView *>(subView);
                            }catch (std::bad_cast& bc){
                                qDebug() << "bad_cast caught: " << bc.what();
                            }
                            view->drawdigit->restorePic(); break;  }//还原到上次清除的图像
                            default:;
                        }
                    break;
                }
        case 2: {
                    switch(type){
                        case New: setCode(); break; //读取编辑框的信息并生成对应验证码
                        case Open: case Cut: changeWindows(); break; //导入图片更换窗口
                        case Draw: {
                                drawView* view = nullptr;
                                try{
                                    view = dynamic_cast<drawView *>(subView);
                                }catch (std::bad_cast& bc){
                                    qDebug() << "bad_cast caught: " << bc.what();
                                }
                                view->drawdigit->clearPic(); break;  }//清除
                                default:;
                            };
                    break;
                }
        case 3: {
                    if(type == New) processCodeArea();
                    else{
                        if(type == Draw)
                            genLabels();
                        else
                            qDebug() << "Not support for Open here.";
                    }
                    break;
                }
        default:   qDebug() << "Unknow Windows Type.";
    }
}
//全路径中取文件名
QString SubWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString SubWindow::currentFileName()
{
    return strippedName(curPath);
}

void SubWindow::setCurrentFile(const QString &fileName)
{
    //canonicalFilePath()可去掉路径中的符号链接，如"./"和"../"
    curPath = QFileInfo(fileName).canonicalFilePath(); //此时已被保存
    toSaveIt = false;                               //设置成没被更改过
    //this->textEdit->document()->setModified(false); //窗口不再显示被更改的标志'*'
    setWindowModified(false);                       //设置窗口标题并返回文件名
    setWindowTitle(currentFileName());
}

//关闭事件
void SubWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
/*
void SubWindow::mouseMoveEvent(QMouseEvent *event)
{
    //QGraphicsView 坐标
    QPoint viewPoint = event->pos();
    viewCoord->setText(QString::number(viewPoint.x())+","+QString::number(viewPoint.y()));

    //QGraphicsScene 坐标
    QPointF scenePoint = mapToScene(viewPoint);
    sceneCoord->setText(QString::number(scenePoint.x())+","+QString::number(scenePoint.y()));

    //图片坐标轴映射
    QPointF location = mapToMap(scenePoint);
    mapCoord->setText(QString::number(location.x())+","+QString::number(location.y()));
}*/

//提醒保存文件
bool SubWindow::maybeSave()
{
    if (!toSaveIt)
        return true;
    QMessageBox::StandardButton ret;
    ret = QMessageBox::question(this, tr("Qt Image"),tr("%1还未被保存，是否保存此图片?").arg(currentFileName()),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return save();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

bool SubWindow::save()
{
    if (toSaveIt) {
        return saveAs();
    } else {
        return saveFile(curPath);
    }
}

bool SubWindow::saveAs()
{   //要求全名
    QString fileName;
    return saveAs(fileName);
}

bool SubWindow::saveAs(QString& fileName)
{
    //qDebug() << "SaveAs Called.";

    fileName = QFileDialog::getSaveFileName(this, tr("另存为"), curPath,
                tr("All files (*);;"  "Image PNG (*.png);;""Image JPG (*.jpg);;"
                   "Image BPM (*.bpm);;" "Image GIF (*.gif);;""Image JPEG (*.jpeg);;"
                   "Image PPM (*.ppm);;" "Image XBM (*.xbm);;" "Image XPM (*.xpm);;"));
    //获取文件路径名不为空则写入文件
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool SubWindow::saveFile(QString fileName)
{
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("另存错误"), tr("此图片无法保存."));
            return false;
        }
        bool success = false;
        //保存图片到新路径上
        winType type = getCurrentWinType();
        switch(type){
            case New: {
                    newView* view = nullptr;
                    try{
                        view = dynamic_cast<newView *>(subView);
                    }catch (std::bad_cast& bc){
                        qDebug() << "bad_cast caught: " << bc.what();
                    }
                success = view->codeArea->saveCode(fileName); break;
            }
            case Open: case Cut: success = pixmapItem->pixmap().save(fileName); break;
            case Draw: {
                    drawView* view = nullptr;
                    try{
                        view = dynamic_cast<drawView *>(subView);
                    }catch (std::bad_cast& bc){
                        qDebug() << "bad_cast caught: " << bc.what();
                    }
                    success = view->drawdigit->savePic(fileName); break;
            }
            default : qDebug() << "Unknown Windows Type.";
        }
        if (success)
            setCurrentFile(fileName);
        return success;
    }
    else return false;
    /*
    if (!(fileName.endsWith(".png", Qt::CaseInsensitive) || fileName.endsWith(".jpg", Qt::CaseInsensitive)))
    {
        fileName += ".png"; // 默认保存为 png 图片
    }
    QTextDocumentWriter writer(fileName);
    bool success = writer.write(this->document());
    if (success)
        setCurrentFile(fileName);
    return success;*/
}

bool SubWindow::showFile(QPixmap pix)
{
    openView* view = nullptr;
    try{
        view = dynamic_cast<openView *>(subView);
    }catch (std::bad_cast& bc){
        qDebug() << "bad_cast caught: " << bc.what();
    }
    if(!pix.isNull()){
        pixmap = pix;
        pixmapItem = view->scene->addPixmap(pixmap);                      //显示导入的验证码
        view->scene->setSceneRect(QRectF(pixmap.rect()));                  //设置显示大小
        qDebug() << "depth:" << pixmap.depth();
        qDebug() << "hasAlpha:" << pixmap.hasAlpha();
        view->size->setText("Dimensions:" + QString::number(pixmapItem->pixmap().width())
                      + " x " + QString::number(pixmapItem->pixmap().height()));

        //connect(this->textEdit->document(), SIGNAL(contentsChanged()),this, SLOT(isModified()));
        connect(view->slider,SIGNAL(valueChanged(int)),this,SLOT(zoomInOut(int)));
        return true;
    }
    else return false;
}
