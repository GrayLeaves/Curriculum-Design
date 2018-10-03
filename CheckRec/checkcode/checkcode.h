#ifndef CHECKCODE_H
#define CHECKCODE_H

#include <QList>
#include <QPainterPath>
#include <QRect>
#include <QWidget>
#include <QVector>
#include <QPixmap>
//#include <random> //提供随机数引擎

#define DEF_CODECOUNT 5					//默认字符个数
#define DEF_NOISYLINECOUNT 2			// 噪点线数量
#define DEF_CONVERSEROTATE 10			// 转换角度范围度数
#define DEF_CONVERSESCALE 15			// 转换大小范围百分比

class CodeArea : public QWidget
{
    Q_OBJECT

public:
    CodeArea();
    ~CodeArea();
    void replaceCodePic();                          // 更换验证码图片
    void setCodeCount(int nCodeCount)
        { codeCount = nCodeCount; }                 // 设置验证码位数
    void setNoisyPointCount(int nNoisyLineCount)
        {noisyLineCount = nNoisyLineCount;}         // 设置噪点数量
    bool checkCode(QString sCode);                  // 检验验证码
    void setCode(QString code);
    QString getCode();                              // 获取验证码
    bool saveCode(QString& fileName);               // 保存验证码
    //QImage getImage() {return img;}                 // 获得验证码整幅图片
protected:
    void paintEvent(QPaintEvent *event);

private:
    void initCodeRange();                    // 初始化验证码范围
    void initCodeColorList();                // 初始化验证码可用颜色列表
    void generateImage(QPaintEvent *event);  // 绘制图片，防止更新过频与及记录参数
    void updateLoginCode();                  // 更新验证码
    void updateCodePic();                    // 更新验证码图片
    void updateCode();                       // 更新用于与用户输入的验证码做比较的码
    void drawOutline(QPainter &painter,int w, int h);       // 绘制边缘虚线框
    void drawCode(QPainter &painter, int nCodeIndex);       // 绘制验证码
    void drawNoisyLine(QPainter &painter,int w, int h);     // 绘制噪点
    void drawConversion(QPainter &painter);       // 做验证码形态转换
    void setCodePic(const QList<QPainterPath *> &lCodePic); // 设置验证码图片

    QImage img;                             // 保存绘制好的验证码
    QString userCode = "";					// 用于与用户输入的验证码做比较的码
    QStringList codeRange;					// 验证码生成范围
    QStringList loginCode;					// 验证码列表,用于生成验证码图片
    QPainterPath *perCodePic;				// 单个字符的验证码图片
    QList<QPointF> knots[DEF_NOISYLINECOUNT];// 连续的点
    QList<QPainterPath *> codePic;			// 验证码图片
    QList<Qt::GlobalColor> codeColor;		// 验证码可用颜色列表
    bool activeConversed;                   // 是否激活重新绘制
    QVector<int> converse = {0,0,0};
    int noisyLineCount = DEF_NOISYLINECOUNT;
    int converseRotate = DEF_CONVERSEROTATE;
    int converseScale = DEF_CONVERSESCALE;
    int codeCount = DEF_CODECOUNT;
    //std::random_device rd; //随机数引擎，效果还不太好
};
#endif // CHECKCODE_H
