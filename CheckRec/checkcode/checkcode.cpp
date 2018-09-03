#include "checkcode.h"
#include "common.h"
#include <QTime>
#include <QPainter>
#include <QPaintEvent>

CodeArea::CodeArea(): activeConversed(true)
{
    initCodeRange();
    initCodeColorList();
    //setAutoFillBackground(true);    //对窗体背景色的设置
    //setPalette(QPalette(Qt::white));

    replaceCodePic();
}

CodeArea::~CodeArea()
{
    delete_s(perCodePic);
}

void CodeArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(event->rect(), QBrush(Qt::white));
    painter.translate(0, 0);
    painter.save();
    painter.translate(event->rect().center().x() - codePic.size() * 3, event->rect().center().y());
    for (int i = 0; i < codePic.size(); i++)
    {
        drawConversion(painter);
        drawCode(painter, i);
        painter.translate(15,0);
    }
    painter.restore();
    drawOutline(painter,event->rect().width(),event->rect().height());
    drawNoisyLine(painter,event->rect().width(),event->rect().height());
    activeConversed = false;
}

/* 初始化验证码范围 */
void CodeArea::initCodeRange()
{
    for(int i=0; i<10; i++)//0-9
        codeRange << QString(static_cast<QChar>(48+i));
    for(int i=0; i<26; i++)//A-Z
        codeRange << QString(static_cast<QChar>(65+i));
    for(int i=0; i<26; i++)//a-z
        codeRange << QString(static_cast<QChar>(97+i));
}

/* 初始化验证码可用颜色列表 */
void CodeArea::initCodeColorList()
{
    codeColor << Qt::darkRed << Qt::darkGreen << Qt::darkBlue << Qt::darkCyan
        << Qt::darkMagenta << Qt::darkYellow << Qt::darkGray;
}

/* 更换验证码图片 */
void CodeArea::replaceCodePic()
{
    updateLoginCode();
    updateCodePic();
}

/* 更新验证码图片 */
void CodeArea::updateCodePic()
{    
    codePic.clear();
    for (int i = 0; i < codeCount; i++)				 //默认字符个数
    {
        perCodePic = new QPainterPath(QPointF(0, 0));// 单个字符的验证码图片
        QFont font(tr("Comic Sans MS"));//tr("Microsoft YaHei"),QString::fromLocal8Bit("华文彩云")
        font.setBold(true);
        font.setPixelSize(33);
        QRect fontBoundingRect = QFontMetrics(font).boundingRect(loginCode[i]);
        perCodePic->addText(-QPointF(fontBoundingRect.center()), font, loginCode[i]);
        codePic << perCodePic;						// 验证码图片
    }
    setCodePic(codePic);
}

/* 设置验证码图片 */
void CodeArea::setCodePic(const QList<QPainterPath *> &lCodePic)
{
    this->codePic = lCodePic;
    activeConversed = true;
    update();
}

bool CodeArea::saveCode(QString& fileName)                       // 保存验证码
{
    if(fileName.isEmpty())
        return false;
    QImage image(150,60, QImage::Format_RGB32);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(QRect(0,0,150,60), QBrush(Qt::white));
    painter.translate(0, 0);
    painter.save();
    painter.translate(75 - codePic.size() * 3, 30);
    for (int i = 0; i < codePic.size(); i++)
    {
        drawConversion(painter);
        drawCode(painter, i);
        painter.translate(15,0);
    }
    painter.restore();
    drawNoisyLine(painter,150,60);
    return image.save(fileName);
}

/* 更新验证码 */
void CodeArea::updateLoginCode()
{
    loginCode.clear();
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
    loginCode << codeRange[qrand() % 62];
    for (int i = 1; i < codeCount; i++)
    {
        QString s = codeRange[qrand() % 62];
        if( s != loginCode[i-1])
            loginCode << s;
        else
            i -= 1;
    }
}

/* 更新用于与用户输入的验证码做比较的码 */
void CodeArea::updateCode()
{
    userCode = "";
    for (int i = 0; i < codeCount; i++)
    {
        userCode += loginCode[i];
    }
}

/* 设置验证码位数 */
void CodeArea::setCodeCount(int nCodeCount)
{
    codeCount = nCodeCount;
}

/* 设置噪点数量 */
void CodeArea::setNoisyPointCount(int nNoisyLineCount)
{
    noisyLineCount = nNoisyLineCount;
}

/* 检验验证码 */
bool CodeArea::checkCode(QString sCode)
{
    updateCode();
    return userCode == sCode;
}
/*获取当前验证码*/
QString CodeArea::getCode()
{
    updateCode();
    return userCode;
}
/* 设置验证码 */
void CodeArea::setCode(QString code)
{
    if(codeCount != code.size())
        replaceCodePic();
    else
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        loginCode.clear();
        for(int i = 0; i<code.size();i++)
        {
            QChar c = code[i];
            if(c.isDigit()||c.isLetter())
                loginCode << QString(c);
            else
                loginCode << codeRange[qrand() % 62];
        }
        updateCodePic();
    }
}

/* 绘制边缘虚线框 */
void CodeArea::drawOutline(QPainter &painter,int w, int h)
{
    painter.setPen(Qt::darkGreen);
    painter.setPen(Qt::DashLine);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, w, h);
}

/* 绘制单个字符 */
void CodeArea::drawCode(QPainter &painter, int nCodeIndex)
{
    painter.fillPath(*codePic[nCodeIndex], QBrush(codeColor[qrand() % 7]));
}

/* 绘制噪点线 */
void CodeArea::drawNoisyLine(QPainter &painter,int w, int h)
{
    painter.setRenderHint(QPainter::Antialiasing,true);
    for (int i = 0; i < noisyLineCount; i++)
    {
        knots[i].clear();
        for(int j=0; j<4; j++)
            knots[i] << QPointF(static_cast<int>(0.1*w)+qrand() % static_cast<int>(0.8*w), qrand() % h);
        QPainterPath path(knots[i][0]);
        for(int j=0; j<4-1; j++){
            QPointF sp = knots[i][j];
            QPointF ep = knots[i][j+1];
            QPointF cl = QPointF((sp.x()+ep.x())/2,sp.y());
            QPointF cr = QPointF((sp.x()+ep.x())/2,ep.y());
            path.cubicTo(cl,cr,ep);
        }
        painter.setPen(QPen(codeColor[qrand() % 7],2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));
        painter.drawPath(path);
    }
}

/* 做验证码形态转换 */
void CodeArea::drawConversion(QPainter &painter)
{
    if(activeConversed)
        for(int i=0; i<3; i++)
            converse[i] = qrand();
    painter.rotate(converse[0] %(2*converseRotate+1) - converseRotate);
    painter.scale((converse[1] % converseScale + (100.0 - converseScale))/100.0,
        (converse[2] % converseScale + (100.0 - converseScale))/100.0 );
}
