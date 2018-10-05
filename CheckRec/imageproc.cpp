#include "mainwindow.h"
#include "subwindow.h"
#include "recentopened.h"
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
//PrewittEdge
void MainWindow::prewitt()
{
    imageProcTriggered(proc::PrewittEdge,tr("prewitt"));
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
//binaryzation 二值化
QImage proc::Binaryzation(const QImage &origin)
{
    int width = origin.width();
    int height = origin.height();
    int gray, newGray;
    QImage newImg = QImage(width, height, QImage::Format_RGB888);
    for (int x=0; x<width; x++) {
        for(int y=0; y<height; y++) {
            gray = qGray(origin.pixel(x,y));
            if (gray > 128)
                newGray = 255;
            else
                newGray = 0;
            newImg.setPixel(x,y,qRgb(newGray, newGray, newGray));
        }
    }
    return newImg;
}
//invert 反色相
QImage proc::Invert(const QImage &origin)
{
    int width = origin.width();
    int height = origin.height();
    int gray, newGray;
    QImage newImg = QImage(width, height, QImage::Format_RGB888);
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
    QImage *newImage = new QImage(origin.width(), origin.height(),
                                   QImage::Format_ARGB32);
    QColor oldColor;
    int average;
    for (int x=0; x<newImage->width(); x++) {
        for (int y=0; y<newImage->height(); y++) {
            oldColor = QColor(origin.pixel(x,y));
            average = (oldColor.red()*299 + oldColor.green()*587 + oldColor.blue()*114 + 500)/1000;
            newImage->setPixel(x,y,static_cast<int>(qRgb(average,average,average)));
        }
    }
    return *newImage;
}
//laplace 拉普拉斯锐化
QImage proc::LaplaceSharpen(const QImage &origin)
{
    int width = origin.width();
    int height = origin.height();
    QImage newImage = QImage(width, height,QImage::Format_RGB888);
    int window[3][3] = {0,-1,0,-1,4,-1,0,-1,0};
    int sumR, sumG, sumB, old_r, old_g, old_b;

    for (int x=1; x<width; x++) {
        for(int y=1; y<height; y++) {
            sumR = 0; sumG = 0; sumB = 0;
            //对每一个像素使用模板
            for(int m=x-1; m<=x+1; m++) {
                for(int n=y-1; n<=y+1; n++) {
                    if(m>=0 && m<width && n<height) {
                        sumR += QColor(origin.pixel(m,n)).red()*window[n-y+1][m-x+1];
                        sumG += QColor(origin.pixel(m,n)).green()*window[n-y+1][m-x+1];
                        sumB += QColor(origin.pixel(m,n)).blue()*window[n-y+1][m-x+1];
                    }
                }
            }
            old_r = QColor(origin.pixel(x,y)).red();
            sumR += old_r;
            sumR = qBound(0, sumR, 255);
            old_g = QColor(origin.pixel(x,y)).green();
            sumG += old_g;
            sumG = qBound(0, sumG, 255);
            old_b = QColor(origin.pixel(x,y)).blue();
            sumB += old_b;
            sumB = qBound(0, sumB, 255);
            newImage.setPixel(x,y, qRgb(sumR, sumG, sumB));
        }
    }

    return newImage;
}
//Sobel Edge Detector
QImage proc::SobelEdge(const QImage &origin)
{
    /* Sobel */
    double Gx[9] = { 1.0, 0.0,-1.0, 2.0,0.0,-2.0, 1.0,0.0,-1.0};
    double Gy[9] = {-1.0,-2.0,-1.0, 0.0,0.0,0.0, 1.0,2.0,1.0};

    QRgb pixel;
    QImage grayImage = GreyScale(origin);
    int height = grayImage.height();
    int width = grayImage.width();
    QImage newImage = QImage(width, height,QImage::Format_RGB888);

    float sobel_norm[width*height];
    float max = 0.0;
    QColor my_color;
    double value_gx, value_gy;

    for (int x=0; x < width; x++) {
        for( int y=0; y < height; y++) {
            value_gx = 0.0;
            value_gy = 0.0;
            for (int k=0; k<3;k++) {
                for(int p=0; p<3; p++) {
                    pixel=grayImage.pixel(x+1+1-k,y+1+1-p);
                    value_gx += Gx[p*3+k] * qRed(pixel);
                    value_gy += Gy[p*3+k] * qRed(pixel);
                }
//                sobel_norm[x+y*width] = sqrt(value_gx*value_gx + value_gy*value_gy)/1.0;
                sobel_norm[x+y*width] = abs(value_gx) + abs(value_gy);
                max=sobel_norm[x+y*width]>max ? sobel_norm[x+y*width] : max;
            }
        }
    }

    for(int i=0;i<width;i++) {
        for(int j=0;j<height;j++) {
            my_color.setHsv( 0 ,0, 255-int(255.0*sobel_norm[i + j * width]/max));
            newImage.setPixel(i,j,my_color.rgb());
        }
    }
    return newImage;
}
//Prewitt Edge Detector
QImage proc::PrewittEdge(const QImage &origin)
{
    /* Sobel */
    double Gx[9] = {-1.0,0.0,1.0, -1.0,0.0,1.0, -1.0,0.0,1.0};
    double Gy[9] = { 1.0,1.0,1.0, 0.0,0.0,0.0, -1.0,-1.0,-1.0};

    QRgb pixel;
    QImage grayImage = GreyScale(origin);
    int height = grayImage.height();
    int width = grayImage.width();
    QImage newImage = QImage(width, height,QImage::Format_RGB888);

    float sobel_norm[width*height];
    float max = 0.0;
    QColor my_color;
    double value_gx, value_gy;

    for (int x=0; x<width; x++) {
        for( int y=0; y<height; y++) {
            value_gx = 0.0;
            value_gy = 0.0;
            for (int k=0; k<3;k++) {
                for(int p=0; p<3; p++) {
                    pixel=grayImage.pixel(x+1+1-k,y+1+1-p);
                    value_gx += Gx[p*3+k] * qRed(pixel);
                    value_gy += Gy[p*3+k] * qRed(pixel);
                }
//                sobel_norm[x+y*width] = sqrt(value_gx*value_gx + value_gy*value_gy)/1.0;
                sobel_norm[x+y*width] = abs(value_gx) + abs(value_gy);
                max=sobel_norm[x+y*width]>max ? sobel_norm[x+y*width] : max;
            }
        }
    }

    for(int i=0;i<width;i++) {
        for(int j=0;j<height;j++){
            my_color.setHsv( 0 ,0, 255-int(255.0*sobel_norm[i + j * width]/max));
            newImage.setPixel(i,j,my_color.rgb());
        }
    }
    return newImage;
}
