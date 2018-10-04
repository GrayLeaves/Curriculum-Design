#include "subwindow.h"
#include "common.h"
#include "recognition/recognition.h"
#include <string>
#include <QtMath>
#include <QTextStream>
#include <QString>
/*
 * 设置窗口编号
 * 设置文件状态为为保存isUntitled = True
 * 保存文件路径，为curFile赋初值
 * 设置子窗口标题
 * 关联文档内容改变信号contentsChange()到显示文档更改状态标志槽documentWasModified()
*/

SubWindow::SubWindow(winType type) : currentWinType(type)
{
    switch(type){
        case New: generateNew();   break;
        case Open: generateOpen(); break;
        case Cut: generateCut();   break;
        case Draw: generateDraw(); break;
        default: ;
    }
    setAttribute(Qt::WA_DeleteOnClose);    //设置在主窗口关闭时销毁这个类的对象实例
}

SubWindow::~SubWindow()
{
    delete_s(size);
    delete_s(codeArea);
    delete_s(scene);
    delete_s(slider);
    delete_s(textEdit);
    delete_s(msgBox);
}
void SubWindow::generateNew(){
    codeArea = new CodeArea();      // 生成验证码
    codeArea->setFixedSize(150,60); // 指定区域，防止窗口缩放中变形
    generateTextCnt();              // 生成编辑框，提示信息组合框和按钮
    mainLayout = new QVBoxLayout(); //实现布局
    mainLayout->addWidget(codeArea,0,Qt::AlignCenter);
    mainLayout->setSpacing(5);
    textLayout = new QHBoxLayout;
    textLayout->addWidget(userText);                //提示信息
    textLayout->addWidget(textEdit);                //编辑框
    mainLayout->addLayout(textLayout);

    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(combox);                  //菜单栏
    btnLayout->addWidget(functionBtn);             //功能按钮
    mainLayout->addLayout(btnLayout);
    //resize(180,200);
    setLayout(mainLayout);
}
void SubWindow::generateOpen(){
    generateView(); // 图形视图，生成图片像素的标签和初始化graphics/view
    generateTextCnt(); // 生成编辑框，提示信息组合框和按钮
    mainLayout = new QVBoxLayout(); //实现布局
    mainLayout->addWidget(size); //显示验证码像素值
    mainLayout->setSpacing(5);  //分割区域
    //缩放控制子布局
    zoomLayout = new QHBoxLayout();
    zoomLayout->addWidget(spinbox);
    zoomLayout->addWidget(zoomout);
    zoomLayout->addWidget(slider);
    zoomLayout->addWidget(zoomin);
    mainLayout->addWidget(view);
    mainLayout->addLayout(zoomLayout);
    mainLayout->setSpacing(5);
    textLayout = new QHBoxLayout;
    textLayout->addWidget(userText);                //提示信息
    textLayout->addWidget(textEdit);                //编辑框
    mainLayout->addLayout(textLayout);

    btnLayout = new QHBoxLayout();
    btnLayout->addWidget(combox);                  //菜单栏
    btnLayout->addWidget(functionBtn);             //功能按钮
    mainLayout->addLayout(btnLayout);
    //resize(180,200);
    setLayout(mainLayout);
}
void SubWindow::generateCut(){generateOpen();}
void SubWindow::generateDraw(){}
void SubWindow::generateView()
{
    size = new QLabel(tr("Dimensions：*** x ***"));
    zoom = 50.0;
    //info = NULL;
    spinbox = new QSpinBox;
    spinbox->setRange(1,99);
    spinbox->setValue(zoom);
    spinbox->setFixedWidth(40);
    slider = new QSlider;                   //用于缩放的滚动条
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(1,99);
    slider->setTickInterval(10);
    slider->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    slider->setValue(zoom);
    //关联信号到槽，将spinbox - slider - zoomInOut 关联起来
    connect(spinbox,SIGNAL(valueChanged(int)),slider,SLOT(setValue(int)));
    connect(slider,SIGNAL(valueChanged(int)),spinbox,SLOT(setValue(int)));
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(zoomInOut(int)));

    zoomin = new QPushButton;
    zoomin->setIcon(QPixmap(rsrcPath + "/zoomin.png"));
    zoomin->setFixedSize(QPixmap(rsrcPath + "/zoomin.png").size());
    zoomin->setStatusTip(tr("放大"));
    connect(zoomin,SIGNAL(clicked()),this,SLOT(zoomIn()));
    zoomout = new QPushButton;
    zoomout->setIcon(QPixmap(rsrcPath + "/zoomout.png"));
    //zoomout->setIconSize(QPixmap(rsrcPath + "/zoomout.png").size());
    zoomout->setFixedSize(QPixmap(rsrcPath + "/zoomout.png").size());
    zoomout->setStatusTip(tr("缩小"));
    connect(zoomout,SIGNAL(clicked()),this,SLOT(zoomOut()));
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QColor::fromRgb(224,224,224)); //填充背景
    view = new QGraphicsView;
    view->setScene(scene);
    view->setMinimumSize(160,65);
    view->resize(200,120);
    view->setCacheMode(QGraphicsView::CacheBackground);
    //view->show();
}

void SubWindow::generateTextCnt()
{
    userText = new QLabel(tr("编辑框"));
    userText->setFrameStyle(QFrame::Panel|QFrame::Raised);
    userText->setFixedHeight(25);
    userText->resize(160,25);
    textEdit = new QTextEdit;
    textEdit->setFixedHeight(30);
    textEdit->document()->setMaximumBlockCount(1); //设置最大行数为1行
    textEdit->setAlignment(Qt::AlignCenter);
    QFont font(QString::fromLocal8Bit("Arial"),14);
    textEdit->setFont(font);

    combox = new QComboBox;
    combox->addItem(QIcon(rsrcPath + "/recognize.png"),tr("揭晓")); //0 识别
    if(currentWinType == New) {
        combox->addItem(QIcon(rsrcPath + "/check.png"),tr("校对")); //1
        combox->addItem(QIcon(rsrcPath + "/generate.png"),tr("更换"));//2
        combox->addItem(QIcon(rsrcPath + "/tooth.png"),tr("处理"));//3
    }
    else {
        combox->addItem(QIcon(rsrcPath + "/check.png"),tr("还原"));//1
        combox->addItem(QIcon(rsrcPath + "/generate.png"),tr("更换"));//2
    }
    combox->setFixedHeight(30);
    //connect(comboBox,SIGNAL(activated(int)),this,SLOT(textEdit->clear()));  //关联相应的槽函数
    functionBtn = new QPushButton(tr("OK"));
    functionBtn->setFixedHeight(30);
    connect(functionBtn,SIGNAL(clicked()),this,SLOT(tool()));
}
void SubWindow::newFile()
{
    toSaveIt = false;
    //新建验证码的默认命名验证码的字符值一致
    curPath = codeArea->getCode();
    setWindowTitle(curPath);
    //编辑框的内容被更改时发送contentsChange()信号来执行isModified()函数
    connect(this->textEdit->document(),SIGNAL(contentsChanged()),this,SLOT(isModified()));
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
    connect(this->textEdit->document(),SIGNAL(contentsChanged()),this,SLOT(isModified()));
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

        pixmapItem = scene->addPixmap(pixmap);                      //显示导入的验证码
        scene->setSceneRect(QRectF(pixmap.rect()));                 //设置显示大小

        //qDebug() << "depth:" << pixmap.depth();
        //qDebug() << "hasAlpha:" << pixmap.hasAlpha();
        setCurrentFile(fileName);
        size->setText("Dimensions:" + QString::number(pixmapItem->pixmap().width())
                      + " x " + QString::number(pixmapItem->pixmap().height()));
        //若编辑框的内容被更改时发送contentsChange()信号来执行isModified()函数
        connect(this->textEdit->document(), SIGNAL(contentsChanged()),this, SLOT(isModified()));
        return true;
    }
    return false;
}

void SubWindow::isModified()
{
    //编辑器是否被更改修改窗口状态setWindowModified()改为更新主窗口更新字体菜单
    if(this->textEdit->document()->isModified())
    {
        textUpdate();       //发送更新主窗体字体菜单状态栏的信号
    }
}

void SubWindow::replaceCode()
{
    //更新验证码
    codeArea->replaceCodePic();
}
//将结果送到编辑框，自定义生成的
void SubWindow::recognizeCode()
{
    this->textEdit->clear();
    textEdit->setText(codeArea->getCode());
    textEdit->setAlignment(Qt::AlignCenter);
}

//将识别结果送到编辑框，用dnn获得的
void SubWindow::recognizePic()
{
    this->textEdit->clear();
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
    std::string result = recognition(picture);
    if(result.size()<4 || result.size()>5)
        textEdit->setText(QString("Wrong"));
    else
        textEdit->setText(QString(result.c_str()));
    textEdit->setAlignment(Qt::AlignCenter);
}

//设置验证码
void SubWindow::setCode()
{
    QString strText = this->textEdit->toPlainText();
    QTextStream ts(&strText);
    /*while( ! ts.atEnd())
    {
        qDebug()<<ts.readLine();
    }*/
    QString code = ts.readLine();
    codeArea->setCode(code);
    newFile();
}
//认证检验
void SubWindow::checkCode()
{
    QString strText = this->textEdit->toPlainText();
    QTextStream ts(&strText);
    QString user = ts.readLine();
    QString code = "";
    for(int i=0; i<user.size();i++)
    {
        QChar c = user[i];
        if(c.isDigit()||c.isLetter())
            code += QString(c);
    }
    if(code.size() == DEF_CODECOUNT)
    {
        if (codeArea->checkCode(user))
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
}
//图片缩放
void SubWindow::zoomInOut(int value)   	//缩放
{
    qreal s = 1.0;
    if(value>zoom)                 		//放大
    {
        s=pow(1.02,(value-zoom));
    }
    else                             	//缩小
    {
        s=pow(1/1.02,(zoom-value));
    }
    view->scale(s,s);
    zoom = value;
}
void SubWindow::zoomIn()   	//缩放
{
    qreal s = slider->value();
    if(s < 99.0) s += 1.0;
    else s = 99.0;
    slider->setValue(s);
}

void SubWindow::zoomOut()   	//缩放
{
    qreal s = slider->value();
    if(s > 1.0) s -= 1.0;
    else s = 1.0;
    slider->setValue(s);
}
//还原
void SubWindow::resetZoom()   	//缩放
{
    qreal s = 1.0;
    qreal v = slider->value();
    if(v > 50.0)                 		//已放大
    {
        s=pow(1/1.02,(v-50.0));
    }
    else                             	//已缩小
    {
        s=pow(1.02,(50.0-v));
    }
    view->scale(s,s);
    zoom = 50.0;
    slider->setValue(zoom);
}
//
void SubWindow::tool()
{
    winType type = getCurrentWinType();
    int index = combox->currentIndex();
    switch(index)
    {
        case 0: {
                    if(type == New) recognizeCode();    //揭晓验证码的答案
                    else recognizePic();                //调用验证码识别功能
                    break;
                }
        case 1: {
                    if(type == New) checkCode();        //用户校对验证码的结果
                    else restoreImage();                //恢复图像
                    break;
                }
        case 2: {
                   if(type == New) setCode();           //读取编辑框的信息并生成对应验证码
                   else changeWindows();                //导入图片更换窗口
                   break;
                }
        case 3: {
                    if(type == New) processCodeArea();
                    else qDebug() << "Not support for Open here.";
                    break;
                }
        default:   qDebug() << "Nothing called.";
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
        if(type != New){
            success = pixmapItem->pixmap().save(fileName);
        }
        else{
            success = codeArea->saveCode(fileName);
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
    if(!pix.isNull()){
        pixmap = pix;
        pixmapItem = scene->addPixmap(pixmap);                      //显示导入的验证码
        scene->setSceneRect(QRectF(pixmap.rect()));                  //设置显示大小
        qDebug() << "depth:" << pixmap.depth();
        qDebug() << "hasAlpha:" << pixmap.hasAlpha();
        size->setText("Dimensions:" + QString::number(pixmapItem->pixmap().width())
                      + " x " + QString::number(pixmapItem->pixmap().height()));

        //connect(this->textEdit->document(), SIGNAL(contentsChanged()),this, SLOT(isModified()));
        connect(slider,SIGNAL(valueChanged(int)),this,SLOT(zoomInOut(int)));
        return true;
    }
    else return false;
}

/*
QPointF SubWindow::mapToMap(QPointF p)
{
    QPointF location;
    qreal ws =sceneRect().width();
    qreal hs =sceneRect().height();

    qreal wp =pixmapItem->pixmap().width();
    qreal hp =pixmapItem->pixmap().height();

    qreal y =hp-(hs/2+p.y())*hp/hs;
    qreal x =(ws/2+p.x())*wp/ws;

    location.setX(x);
    location.setY(y);

    return location;
}*/
