#ifndef __AT89C02_H__
#define __AT89C02_H__

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint 
#define uint unsigned int
#endif
#ifndef ulong
#define ulong unsigned long
//
sbit SCL=P2^1;
sbit SDA=P2^0;
//******************************************************/
void I2cStart();
void I2cStop();
uchar I2cSendByte(uchar dat);
uchar I2cReadByte();
void At24c02Write(uchar addr,uchar dat);
uchar At24c02Read(uchar addr);
/*******************************************************************************
* 函数名         : Delay10us()
* 函数功能		   : 延时10us
*******************************************************************************/
void Delay10us()
{
	uchar a,b;
	for(a=1;a>0;a--)
		for(b=2;b>0;b--);
}
/*******************************************************************************
* 函数名         : I2cStart()
* 函数功能		   : 起始信号：在SCL时钟信号在高电平期间SDA信号产生一个下降沿
* 备注           : 起始之后SDA和SCL都为0
*******************************************************************************/
void I2cStart()
{
	SDA=1;
	Delay10us();
	SCL=1;
	Delay10us();//建立时间是SDA保持时间>4.7us
	SDA=0;
	Delay10us();//保持时间是>4us
	SCL=0;			
	Delay10us();		
}
/*******************************************************************************
* 函数名         : I2cStop()
* 函数功能		 : 终止信号：在SCL时钟信号高电平期间SDA信号产生一个上升沿
* 备注           : 结束之后保持SDA和SCL都为1；表示总线空闲
*******************************************************************************/
void I2cStop()
{
	SDA=0;
	Delay10us();
	SCL=1;
	Delay10us();    //建立时间大于4.7us
	SDA=1;
	Delay10us();		
}
/*******************************************************************************
* 函数名         : I2cSendByte(unsigned char dat)
* 函数功能		   : 通过I2C发送一个字节。在SCL时钟信号高电平期间，保持发送信号SDA保持稳定
* 输入           : num
* 输出         	 : 0或1。发送成功返回1，发送失败返回0
* 备注           : 发送完一个字节SCL=0,SDA=1
*******************************************************************************/
unsigned char I2cSendByte(uchar dat)
{
	uchar a=0,b=0;//最大255，一个机器周期为1us，最大延时255us。		
	for(a=0;a<8;a++)//要发送8位，从最高位开始
	{
		SDA=dat>>7;	 //起始信号之后SCL=0，所以可以直接改变SDA信号
		dat=dat<<1;
		Delay10us();
		SCL=1;
		Delay10us();//建立时间>4.7us
		SCL=0;
		Delay10us();//时间大于4us		
	}
	SDA=1;
	Delay10us();
	SCL=1;
	while(SDA)//等待应答，也就是等待从设备把SDA拉低
	{
		b++;
		if(b>200)	 //如果超过2000us没有应答发送失败，或者为非应答，表示接收结束
		{
			SCL=0;
			Delay10us();
			return 0;
		}
	}
	SCL=0;
	Delay10us();
 	return 1;		
}
/*******************************************************************************
* 函数名         : I2cReadByte()
* 函数功能		   : 使用I2c读取一个字节
* 输入           : 无
* 输出         	 : dat
* 备注           : 接收完一个字节SCL=0,SDA=1.
*******************************************************************************/
uchar I2cReadByte()
{
	uchar a=0,dat=0;
	SDA=1;			//起始和发送一个字节之后SCL都是0
	Delay10us();
	for(a=0;a<8;a++)//接收8个字节
	{
		SCL=1;
		Delay10us();
		dat<<=1;
		dat|=SDA;
		Delay10us();
		SCL=0;
		Delay10us();
	}
	return dat;		
}
/*******************************************************************************
* 函数名         : void At24c02Write(unsigned char addr,unsigned char dat)
* 函数功能		   : 往24c02的一个地址写入一个数据
*******************************************************************************/
void At24c02Write(uchar addr,uchar dat)
{
	I2cStart();
	I2cSendByte(0xa0);//发送写器件地址
	I2cSendByte(addr);//发送要写入内存地址
	I2cSendByte(dat);	//发送数据
	I2cStop();
}
/*******************************************************************************
* 函数名         : unsigned char At24c02Read(unsigned char addr)
* 函数功能		   : 读取24c02的一个地址的一个数据
*******************************************************************************/
uchar At24c02Read(uchar addr)
{
	uchar num;
	I2cStart();
	I2cSendByte(0xa0); //发送写器件地址
	I2cSendByte(addr); //发送要读取的地址
	I2cStart();
	I2cSendByte(0xa1); //发送读器件地址
	num=I2cReadByte(); //读取数据
	I2cStop();
	return num;	
}
#endif