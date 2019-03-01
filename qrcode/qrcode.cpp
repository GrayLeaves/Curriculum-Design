#include "qrcode.h"

Qrcode::Qrcode()
{
    qr = nullptr;
}

Qrcode::~Qrcode()
{
    if(qr != nullptr)  //若二维码已生成则清理掉
    {
       QRcode_free(qr);
    }
}

bool Qrcode::saveImage(QString fileName, int size)
{
    if(size > 0 && !fileName.isEmpty())  //若图片规格为正且文件名不为空
    {
        QImage image(size, size, QImage::Format_Mono);
        QPainter painter(&image);
        QColor background(Qt::white);
        painter.setBrush(background);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, size, size);
        if(qr != nullptr)
        {
            draw(painter, size, size);  //根据图片的规格来绘制二维码
        }
        return image.save(fileName);    //返回是否成功保存二维码
    }
    else return false;
}

void Qrcode::generateString(QString str)
{
    string = str;
    if(qr != nullptr)    //二维码已存在则清理
    {
        QRcode_free(qr);
    }                    //绘制二维码类指针，包含二维码全部信息
    qr = QRcode_encodeString(string.toStdString().c_str(), 2, QR_ECLEVEL_Q, QR_MODE_8, 1);
    update();           //激活绘制事件paintEvent()
}

void Qrcode::draw(QPainter &painter, int width, int height)
{
    if(qr != nullptr){
        QColor foreground(Qt::black);
        painter.setBrush(foreground);
        const int qr_width = qr->width > 0 ? qr->width : 1; //二维码格点每行/列的总数
        double scale_x = width / qr_width;                  //实际绘制二维码各格点的宽高
        double scale_y = height / qr_width;
        for( int y = 0; y < qr_width; y ++)                 //遍历每个格点
        {
            for(int x = 0; x < qr_width; x++)
            {
                unsigned char b = qr->data[y * qr_width + x];//取格点值
                if(b & 0x01)    //判断最低位是否为1，若是则绘制黑色格点
                {
                    QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);//生成格点矩形
                    painter.drawRects(&r, 1);
                }
            }
        }
    }
}

void Qrcode::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor background(Qt::white);
    painter.setBrush(background);               //设置白色背景
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());  //绘制外框
    draw(painter, width(), height());
}
