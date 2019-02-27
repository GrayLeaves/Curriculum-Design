#include "mainwindow.h"
#include "subwindow.h"
#include "recentopened/recentopened.h"
#include "histogram/histogram.h"
#include "proc.h"

void SubWindow::cleanImage()
{
    //清除子窗口的已生成的验证码,
    scene->clear();
    view->resetTransform();
}

void SubWindow::updateImage(const QPixmap &pix)
{
   //更新图片
   cleanImage();
   pixmapItem = scene->addPixmap(pix);                      //显示导入的验证码
   scene->setSceneRect(QRectF(pix.rect()));                  //设置显示大小
   view->setScene(scene);
   qDebug() << "depth:" << pix.depth();
   qDebug() << "hasAlpha:" << pix.hasAlpha();
   qDebug() << "repaintScene"  << scene->items().count();
}

void SubWindow::restoreImage()
{
    if(!toSaveIt) return;
    updateImage(pixmap);
    toSaveIt = false;
}

void SubWindow::changeWindows()
{
    winType type = getCurrentWinType();
    //从文件浏览器导入图片无需保存但截屏需要
    toSaveIt = (type == Open)? false : true;
    reOpenImage(); //使用关闭并重新打开文件的信号
}

void SubWindow::processCodeArea()
{
    winType type = getCurrentWinType();
    if(type == New){
        //qDebug() << "Emit reopen signal.";
        reOpenImage(); //使用关闭并重新打开文件的信号
    }
}

void MainWindow::imageProcTriggered(QImage (*handle)(const QImage &),const QString &message)
{
    if (activeSubWindow() && activeSubWindow()->Item())
    {
        activeSubWindow()->resetZoom();     //还原大小再操作，防止过分变大或缩小
        QPixmap pix = activeSubWindow()->Item()->pixmap();
        if(!pix.isNull())
        {
            QImage image = handle(pix.toImage());
            pix.convertFromImage(image);
            activeSubWindow()->updateImage(pix);
            activeSubWindow()->setImageStatus(true);
            statusBar()->showMessage(message, 2000);
        }
    }
}
//gray scale
void MainWindow::greyScale()
{
    imageProcTriggered(proc::GreyScale,tr("greyscale"));
}
//binaryzation
void MainWindow::binaryzation()
{
    imageProcTriggered(proc::Binaryzation,tr("binaryzation"));
}
//inverse
void MainWindow::invert()
{
    imageProcTriggered(proc::Invert,tr("inverse"));
}
//laplace sharpen
void MainWindow::laplace()
{
    imageProcTriggered(proc::LaplaceSharpen,tr("laplace"));
}
//sobel Edge detective
void MainWindow::sobel()
{
    imageProcTriggered(proc::SobelEdge,tr("sobel"));
}

void MainWindow::NewImage()
{
    winType type = New;
    SubWindow *subwindow = new SubWindow(type);           //创建子窗口部件
    //subwindow->setFixedWidth(160);
    mdiArea->addSubWindow(subwindow);               //多文档区域添加子窗口，作为中心部件
    //关联子窗体的信号：关闭原有的窗口，重新导入同类型文件
    connect(subwindow,&SubWindow::reOpenImage,this,&MainWindow::reLoadImage);
    //根据QTextEdit类鉴别复制、剪切和复制是否可用
    connect(subwindow->getTextEdit(), SIGNAL(copyAvailable(bool)),cutAct, SLOT(setEnabled(bool)));
    connect(subwindow->getTextEdit(), SIGNAL(copyAvailable(bool)),copyAct, SLOT(setEnabled(bool)));
    subwindow->newFile();
    subwindow->show();
    setEnabledText(true);              //使得字体设置菜单可用
}

void MainWindow::reLoadImage()
{
    SubWindow * sub = activeSubWindow();
    if(sub)
    {
        winType type = sub->getCurrentWinType();
        if(type == Open)
        {
            mdiArea->closeActiveSubWindow();
            OpenImage();
        }
        else
        {
            if(type == Cut){
                mdiArea->closeActiveSubWindow();
                CutImage();
            }
            else{
                QString fileName; //将当前图片导入到Open窗口里
                if(sub->saveAs(fileName)){ //先保存（不能先关闭窗口，否则资源会被销毁），跳出文件浏览器窗口
                    statusBar()->showMessage(tr("保存成功"), 2000);
                    mdiArea->setActiveSubWindow(findSubWindow(fileName)); //找到这个窗口并设置为当前活动窗口
                    mdiArea->closeActiveSubWindow(); //再把它关闭才成功
                    openFile(fileName); //在打开
                }
            }
        }
    }
}

/*要打开单个文件应先判断该文件是否已被打开，即先遍历多文档子窗口中的文件，若找到则以该子窗口为活动窗口，否则加载要打开的文件*/
void MainWindow::OpenImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open image"), getUserPath() + "/Pictures",
                tr("All Files (*);;" "All Images (*.bpm *.gif *.jpg *.jpeg *.png *.ppm *.xbm *.xpm);;"
                   "Image BPM (*.bpm);;" "Image GIF (*.gif);;" "Image JPG (*.jpg);;" "Image JPEG (*.jpeg);;"
                   "Image PNG (*.png);;" "Image PPM (*.ppm);;" "Image XBM (*.xbm);;" "Image XPM (*.xpm);;"));

    openFile(fileName);
}

void MainWindow::openFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findSubWindow(fileName);
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            qDebug() << "Windows already exists.";
            return;
        }

        SubWindow *subwindow = createSubWindow();
        if (subwindow->loadFile(fileName)) {
            statusBar()->showMessage(tr("验证码已载入"), 2000);
            this->recent->updateList(fileName);
            subwindow->show();
            setEnabledText(true);          //使得字体设置菜单可用
            //关联子窗体的信号：关闭原有的窗口，重新导入同类型文件
            connect(subwindow,&SubWindow::reOpenImage,this,&MainWindow::reLoadImage);
            //关联子窗体信号，根据字体来实时更新主窗体字体菜单状态栏
            connect(subwindow,&SubWindow::textUpdate,this,&MainWindow::textChangedUpdate);
        } else {
            qDebug() << "Fail to open this file.";
            subwindow->close();
        }
    }
}

void MainWindow::DrawPic(){
    winType type = Draw;
    SubWindow *subwindow = createSubWindow(type);
    //subwindow->setFixedWidth(220);
    statusBar()->showMessage(tr("已生成绘制窗口，欢迎使用"), 2000);
    subwindow->show();
    setEnabledText(true);          //使得字体设置菜单可用
    subwindow->drawFile();
    //关联子窗体的信号：关闭原有的窗口，重新导入同类型文件
    //connect(subwindow,&SubWindow::reOpenImage,this,&MainWindow::reLoadImage);
    //关联子窗体信号，根据字体来实时更新主窗体字体菜单状态栏
    connect(subwindow,&SubWindow::textUpdate,this,&MainWindow::textChangedUpdate);
}

void MainWindow::SaveImage()
{
    if (activeSubWindow() && activeSubWindow()->save())
        statusBar()->showMessage(tr("保存成功"), 2000);
}

void MainWindow::SaveImageAs()
{
    if (activeSubWindow() && activeSubWindow()->saveAs())
    {
        statusBar()->showMessage(tr("保存成功"), 2000);
    }
}

//******************************Namespace proc********************************/
//绘制直方图
void MainWindow::drawHistogram()                   //绘制直方图
{
    if (activeSubWindow() && activeSubWindow()->Item())
    {
        activeSubWindow()->resetZoom(); //  还原大小再操作，防止过分变大或缩小
        QPixmap pix = activeSubWindow()->Item()->pixmap();
        if(!pix.isNull())
        {
            QDialog *hstgrmDialog = new QDialog(this);
            QScrollArea *scrollArea = new QScrollArea(hstgrmDialog);
            Histogram *hstgrm = new Histogram(scrollArea);

            hstgrm->computeHstgrm(pix.toImage());
            scrollArea->setWidget(hstgrm);
            QHBoxLayout * layout = new QHBoxLayout;
            layout->addWidget(scrollArea);
            hstgrmDialog->setLayout(layout);
            hstgrm->resize(820, 780);
            hstgrmDialog->setFixedWidth(820);
            scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            scrollArea->adjustSize();
            hstgrmDialog->setWindowTitle("Image - Histogram - Recognition");
            //QWidget::update()或者QWidget::repaint()
            hstgrmDialog->show();
        }
    }
}
cv::Mat proc::QImageToMat(QImage image)
{
    cv::Mat mat;
    switch (image.format())
    {
        case QImage::Format_ARGB32:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32_Premultiplied:
            mat = cv::Mat(image.height(), image.width(), CV_8UC4,
                          (void*)image.constBits(), image.bytesPerLine());
            break;
        case QImage::Format_RGB888:
            mat = cv::Mat(image.height(), image.width(), CV_8UC3,
                          (void*)image.constBits(), image.bytesPerLine());
            cv::cvtColor(mat, mat, CV_BGR2RGB);
            break;
        case QImage::Format_Indexed8:
            mat = cv::Mat(image.height(), image.width(), CV_8UC1,
                          (void*)image.constBits(), image.bytesPerLine());
            break;
        default: ;
     }
    return mat;
}

QImage proc::MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        // qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
//binaryzation 二值化
QImage proc::Binaryzation(const QImage &origin)
{
    cv::Mat mat = QImageToMat(origin);
    cv::Mat res;
    cv::threshold(mat, res, 125, 200.0, cv::THRESH_BINARY);
    return MatToQImage(res);
}
//invert 反色相
QImage proc::Invert(const QImage &origin)
{
    /*cv::Mat mat = QImageToMat(origin);
    cv::bitwise_not(mat, mat);
    return MatToQImage(mat);*/

    int width = origin.width();
    int height = origin.height();
    int gray, newGray;
    QImage newImg = QImage(width, height, QImage::Format_RGB32);
    for (int x=0; x<width; x++) {
        for(int y=0; y<height; y++) {
            gray = qGray(origin.pixel(x,y));
            newGray = 255 - gray;
            newImg.setPixel(x,y,qRgb(newGray, newGray, newGray));
        }
    }
    return newImg;
}
//GreyScale 灰度化
QImage proc::GreyScale(const QImage &origin)
{
    cv::Mat mat = QImageToMat(origin);
    cv::cvtColor(mat, mat, CV_BGR2GRAY);
    return MatToQImage(mat);
}
//laplace 拉普拉斯锐化
QImage proc::LaplaceSharpen(const QImage &origin)
{
    cv::Mat mat = QImageToMat(origin);
    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    cv::GaussianBlur(mat, mat, cv::Size(3, 3), 0, 0, BORDER_DEFAULT);
    cv::cvtColor(mat, mat, COLOR_BGR2GRAY);
    int ddepth = CV_16S, kernel_size = 3, scale = 1, delta = 0;
    cv::Mat dst, abs_dst;
    cv::Laplacian( mat, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
    // converting back to CV_8U
    cv::convertScaleAbs( dst, abs_dst );
    return MatToQImage(abs_dst);
}
//Sobel Edge Detector
QImage proc::SobelEdge(const QImage &origin)
{
    int ddepth = CV_16S, ksize = 3, scale = 1, delta = 0;
    cv::Mat mat = QImageToMat(origin);
    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    cv::GaussianBlur(mat, mat, cv::Size(3, 3), 0, 0, BORDER_DEFAULT);
    cv::cvtColor(mat, mat, COLOR_BGR2GRAY);
    cv::Mat grad, grad_x, grad_y, abs_grad_x, abs_grad_y;
    cv::Sobel(mat, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
    cv::Sobel(mat, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    return MatToQImage(grad);
}
