
/*˵��**************************************************************************
 ���״�����ʽ uchar code MusicName{���ߣ����������ߣ�����...., 0xff,0xff};ĩβ��ʾ����
 ��������λ������ɣ�
 		��λ�Ǳ�ʾ 1~7 ���߸����� 
 		ʮλ�Ǳ�ʾ�������ڵ�����:1-������2-������3-����;
 		��λ��ʾ��������Ƿ�Ҫ������: 0-������1-��������
    �����������λ������ɣ� 
 		��λ��ʾ������ʱֵ�����Ӧ��ϵ�ǣ� 
 			|��ֵ(n):  |0 |1 |2 |3 | 4 | 5 | 6 
 			|��������: |1 |2 |4 |8 |16 |32 |64 		����=2^n
 		ʮλ��ʾ����������Ч��(0-2):  0-��ͨ��1-������2-����
 		��λ�Ƿ���λ: 0-�޷��㣬1-�з���
 ���������ӳ���ĸ�ʽ
 Play(������,����,�����˶�,�����ٶ�);
	|������           : Ҫ���ŵ�����ָ��,��β��(0xff,0xff)����;
	|����(0-11)       :	��ָ���������ٸ���������;
	|�����˶�(1-3)	  : 1:���˶�, 2:��������, 3:���˶�;
	|�����ٶ�(1-12000):	ֵԽ���ٶ�Խ��;
*******************************************************************************/
#ifndef __playvideo_h_REVISION_FIRST__
#define __playvideo_h_REVISION_FIRST__
//****************************************************************************//
#include "Global.h"
#include "lcd.h"

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint 
#define uint unsigned int
#endif
#ifndef ulong
#define ulong unsigned long

#define N 3 //�ܹ�3��

//*****************************************************************************//
#define OSC 		12000000	      //���徧��Ƶ��12MHZ
#define SOUND_SPACE 	4/5 	    //������ͨ��������ĳ��ȷ���,//ÿ4���������
//****************************����ģ�鶨������*********************************//
sbit IRIN=P3^2;
uchar IrValue[6];          //���ȡ��������
uchar Time;
bit Next=0;
//*****************************************************************
bit Status=1;                            //0,��ͣ,1,����
bit IsMusic=0;                           //�Ƿ�Ÿ���,����T0�Ÿ��ѡ����
char musicNum;                           //��Ŀ����(�ɸ�)
uint  code FreTab[12]  = { 262,277,294,311,330,349,369,392,415,440,466,494 }; //1-7����ԭʼƵ�ʱ�
uchar code SignTab[7]  = { 0,2,4,5,7,9,11 }; 								  //1~7��Ƶ�ʱ��е�λ��
uchar code LengthTab[7]= { 1,2,4,8,16,32,64 };						
uchar Sound_Temp_TH0,Sound_Temp_TL0;	//������ʱ����ֵ�ݴ� 
uchar Sound_Temp_TH1,Sound_Temp_TL1;	//������ʱ����ֵ�ݴ�
//*******************�����б�*************************
void Initial(void);//��ʼ������
void BeepT0(void);//�����ж�
void Playvideo(uchar num,uchar Signature,uchar Octachord,uint Speed);//������
void UDshow(uchar number);
//******************�˿�λ����*************************
sbit BeepIO=P3^7;		     //������������ܽ�
//******************��������***************************
uchar code Music_hzcb[]={//���ų���Ů��
   0x17,0x02, 0x17,0x03, 0x18,0x03, 0x19,0x02, 0x15,0x03,
   0x16,0x03, 0x17,0x03, 0x17,0x03, 0x17,0x02, 0x18,0x03,
   0x19,0x02, 0x16,0x03, 0x17,0x03, 0x18,0x02, 0x18,0x03,
   0x17,0x03, 0x15,0x02, 0x18,0x03, 0x17,0x03, 0x18,0x02,
   0x10,0x03, 0x15,0x03, 0x16,0x02, 0x15,0x03, 0x16,0x03,
   0x17,0x02, 0x17,0x03, 0x18,0x03, 0x19,0x02, 0x1A,0x03,
   0x1B,0x03, 0x1F,0x03, 0x1F,0x03, 0x17,0x03, 0x18,0x03,
   0x19,0x02, 0x16,0x03, 0x17,0x03, 0x18,0x03, 0x17,0x03,
   0x18,0x03, 0x1F,0x03, 0x1F,0x02, 0x16,0x03, 0x17,0x03,
   0x18,0x03, 0x17,0x03, 0x18,0x03, 0x20,0x03, 0x20,0x02,
   0x1F,0x03, 0x1B,0x03, 0x1F,0x66, 0x20,0x03, 0x21,0x03,
   0x20,0x03, 0x1F,0x03, 0x1B,0x03, 0x1F,0x66, 0x1F,0x03,
   0x1B,0x03, 0x19,0x03, 0x19,0x03, 0x15,0x03, 0x1A,0x66,
   0x1A,0x03, 0x19,0x03, 0x15,0x03, 0x15,0x03, 0x17,0x03,
   0x16,0x66, 0x17,0x04, 0x18,0x04, 0x18,0x03, 0x19,0x03,
   0x1F,0x03, 0x1B,0x03, 0x1F,0x66, 0x20,0x03, 0x21,0x03,
   0x20,0x03, 0x1F,0x03, 0x1B,0x03, 0x1F,0x66, 0x1F,0x03,
   0x1B,0x03, 0x19,0x03, 0x19,0x03, 0x15,0x03, 0x1A,0x66,
   0x1A,0x03, 0x19,0x03, 0x19,0x03, 0x1F,0x03, 0x1B,0x03,
   0x1F,0x00, 0x1A,0x03, 0x1A,0x03, 0x1A,0x03, 0x1B,0x03,
   0x1B,0x03, 0x1A,0x03, 0x19,0x03, 0x19,0x02, 0x17,0x03,
   0x15,0x17, 0x15,0x03, 0x16,0x03, 0x17,0x03, 0x18,0x03,
   0x17,0x04, 0x18,0x0E, 0x18,0x03, 0x17,0x04, 0x18,0x0E,
   0x18,0x66, 0x17,0x03, 0x18,0x03, 0x17,0x03, 0x18,0x03,
   0x20,0x03, 0x20,0x02, 0x1F,0x03, 0x1B,0x03, 0x1F,0x66,
   0x20,0x03, 0x21,0x03, 0x20,0x03, 0x1F,0x03, 0x1B,0x03,
   0x1F,0x66, 0x1F,0x04, 0x1B,0x0E, 0x1B,0x02, 0x19,0x03,
   0x19,0x03, 0x15,0x03, 0x1A,0x66, 0x1A,0x03, 0x19,0x03,
   0x15,0x03, 0x15,0x03, 0x17,0x03, 0x16,0x66, 0x17,0x04,
   0x18,0x04, 0x18,0x03, 0x19,0x03, 0x1F,0x03, 0x1B,0x03,
   0x1F,0x66, 0x20,0x03, 0x21,0x03, 0x20,0x03, 0x1F,0x03,
   0x1B,0x03, 0x1F,0x66, 0x1F,0x03, 0x1B,0x03, 0x19,0x03,
   0x19,0x03, 0x15,0x03, 0x1A,0x66, 0x1A,0x03, 0x19,0x03,
   0x19,0x03, 0x1F,0x03, 0x1B,0x03, 0x1F,0x00, 0x18,0x02,
   0x18,0x03, 0x1A,0x03, 0x19,0x0D, 0x15,0x03, 0x15,0x02,
   0x18,0x66, 0x16,0x02, 0x17,0x02, 0x15,0x00, 0xff,0xff
                               };

uchar code Music_tyshg[]={ //ͬһ�׸�
  0x0F,0x01, 0x15,0x02, 0x16,0x02, 0x17,0x66, 0x18,0x03,
  0x17,0x02, 0x15,0x02, 0x16,0x01, 0x15,0x02, 0x10,0x02,
  0x15,0x00, 0x0F,0x01, 0x15,0x02, 0x16,0x02, 0x17,0x02,
  0x17,0x03, 0x18,0x03, 0x19,0x02, 0x15,0x02, 0x18,0x66,
  0x17,0x03, 0x19,0x02, 0x16,0x03, 0x17,0x03, 0x16,0x00,
  0x17,0x01, 0x19,0x02, 0x1B,0x02, 0x1B,0x70, 0x1A,0x03,
  0x1A,0x01, 0x19,0x02, 0x19,0x03, 0x1A,0x03, 0x1B,0x02,
  0x1A,0x0D, 0x19,0x03, 0x17,0x00, 0x18,0x66, 0x18,0x03,
  0x19,0x02, 0x1A,0x02, 0x19,0x0C, 0x18,0x0D, 0x17,0x03,
  0x16,0x01, 0x11,0x02, 0x11,0x03, 0x10,0x03, 0x0F,0x0C,
  0x10,0x02, 0x15,0x00, 0x1F,0x01, 0x1A,0x01, 0x18,0x66,
  0x19,0x03, 0x1A,0x01, 0x1B,0x02, 0x1B,0x03, 0x1B,0x03,
  0x1B,0x0C, 0x1A,0x0D, 0x19,0x03, 0x17,0x00, 0x1F,0x01,
  0x1A,0x01, 0x18,0x66, 0x19,0x03, 0x1A,0x01, 0x10,0x02,
  0x10,0x03, 0x10,0x03, 0x1A,0x0C, 0x18,0x0D, 0x17,0x03,
  0x16,0x00, 0x0F,0x01, 0x15,0x02, 0x16,0x02, 0x17,0x70,
  0x18,0x03, 0x17,0x02, 0x15,0x03, 0x15,0x03, 0x16,0x66,
  0x16,0x03, 0x16,0x02, 0x16,0x03, 0x15,0x03, 0x10,0x02,
  0x10,0x01, 0x11,0x01, 0x11,0x66, 0x10,0x03, 0x0F,0x0C,
  0x1A,0x02, 0x19,0x02, 0x16,0x03, 0x16,0x03, 0x18,0x66,
  0x18,0x03, 0x18,0x02, 0x17,0x03, 0x16,0x03, 0x19,0x00,
  0xff,0xff  //0xff��ֹ����
                         };

uchar code Music_lzhd[]={  //��ֻ����
  0x17,0x03, 0x16,0x03, 0x17,0x01, 0x16,0x03, 0x17,0x03,
  0x16,0x03, 0x15,0x01, 0x10,0x03, 0x15,0x03, 0x16,0x02,
  0x16,0x0D, 0x17,0x03, 0x16,0x03, 0x15,0x03, 0x10,0x03,
  0x10,0x0E, 0x15,0x04, 0x0F,0x01, 0x17,0x03, 0x16,0x03,
  0x17,0x01, 0x16,0x03, 0x17,0x03, 0x16,0x03, 0x15,0x01,
  0x10,0x03, 0x15,0x03, 0x16,0x02, 0x16,0x0D, 0x17,0x03,
  0x16,0x03, 0x15,0x03, 0x10,0x03, 0x15,0x03, 0x16,0x01,
  0x17,0x03, 0x16,0x03, 0x17,0x01, 0x16,0x03, 0x17,0x03,
  0x16,0x03, 0x15,0x01, 0x10,0x03, 0x15,0x03, 0x16,0x02,
  0x16,0x0D, 0x17,0x03, 0x16,0x03, 0x15,0x03, 0x10,0x03,
  0x10,0x0E, 0x15,0x04, 0x0F,0x01, 0x17,0x03, 0x19,0x03,
  0x19,0x01, 0x19,0x03, 0x1A,0x03, 0x19,0x03, 0x17,0x01,
  0x16,0x03, 0x16,0x03, 0x16,0x02, 0x16,0x0D, 0x17,0x03,
  0x16,0x03, 0x15,0x03, 0x10,0x03, 0x10,0x0D, 0x15,0x00,
  0x19,0x03, 0x19,0x03, 0x1A,0x03, 0x1F,0x03, 0x1B,0x03,
  0x1B,0x03, 0x1A,0x03, 0x17,0x0D, 0x16,0x03, 0x16,0x03,
  0x16,0x0D, 0x17,0x01, 0x17,0x03, 0x17,0x03, 0x19,0x03,
  0x1A,0x02, 0x1A,0x02, 0x10,0x03, 0x17,0x0D, 0x16,0x03,
  0x16,0x01, 0x17,0x03, 0x19,0x03, 0x19,0x03, 0x17,0x03,
  0x19,0x02, 0x1F,0x02, 0x1B,0x03, 0x1A,0x03, 0x1A,0x0E,
  0x1B,0x04, 0x17,0x02, 0x1A,0x03, 0x1A,0x03, 0x1A,0x0E,
  0x1B,0x04, 0x1A,0x03, 0x19,0x03, 0x17,0x03, 0x16,0x03,
  0x17,0x0D, 0x16,0x03, 0x17,0x03, 0x19,0x01, 0x19,0x03,
  0x19,0x03, 0x1A,0x03, 0x1F,0x03, 0x1B,0x03, 0x1B,0x03,
  0x1A,0x03, 0x17,0x0D, 0x16,0x03, 0x16,0x03, 0x16,0x03,
  0x17,0x01, 0x17,0x03, 0x17,0x03, 0x19,0x03, 0x1A,0x02,
  0x1A,0x02, 0x10,0x03, 0x17,0x0D, 0x16,0x03, 0x16,0x01,
  0x17,0x03, 0x19,0x03, 0x19,0x03, 0x17,0x03, 0x19,0x03,
  0x1F,0x02, 0x1B,0x03, 0x1A,0x03, 0x1A,0x0E, 0x1B,0x04,
  0x17,0x02, 0x1A,0x03, 0x1A,0x03, 0x1A,0x0E, 0x1B,0x04,
  0x17,0x16, 0x1A,0x03, 0x1A,0x03, 0x1A,0x0E, 0x1B,0x04,
  0x1A,0x03, 0x19,0x03, 0x17,0x03, 0x16,0x03, 0x0F,0x02,
  0x10,0x03, 0x15,0x00, 0xff,0xff
                              };
uchar *Sound[N]={Music_hzcb,Music_tyshg,Music_lzhd};          //ָ����������ָ��,��Ϊmusic.Name[N]
uchar *MusicName[N]={"hzcb ","tyshg","lzhd "};                //��������,����Ϊ5,LCD1602��ʾ��
//uint SoundLength[N];                                        //��Ӧ�����ĳ���,��Ϊmusic.Len[N]
uint Point=0;                                                 //����������λ��,����				

//****************************************************************
void Initial(void)
{
	BeepIO = 0;
	Sound_Temp_TH1 = (65535-(1/1200)*OSC)/256;	// ����TL1Ӧװ��ĳ�ֵ 	(10ms�ĳ�װֵ)
	Sound_Temp_TL1 = (65535-(1/1200)*OSC)%256;	// ����TH1Ӧװ��ĳ�ֵ 
	TH1 = Sound_Temp_TH1;
	TL1 = Sound_Temp_TL1;
	TR1 = 1;                                    //--- ������ʱ��T1 ---
  ET1 = 0;                                    //--- �رն�ʱ��T1����ж�Դ---
  IT1 = 1;                                    //--- INT1�ⲿ�ж�Դ�½��ش��� ---
  EX1 = 1;                                    //--- ʹ��INT1�ⲿ�ж�Դ ---
  PT1 = 1;                                    //--- ����T1�����ȼ��ж� ---
}
//*****************************�Ÿ�״̬��ʾ����****************************
void UDshow(uchar number)
{
	uchar up,down;
	
	if(number==0)
   {
     up=N-1;
		 down=1; 
   }
	if(number>0)
	 {
		 up=number-1;
		 down=number+1;
	 }
	if(number==(N-1))
	 {
     up=N-2;
		 down=0;
	 }
	 LcdShowStr(2,0,MusicName[up]);
	 delayms(10);
	 LcdShowStr(10,0,MusicName[down]);
}
//******�ⲿ�ж�ԴINT1���жϷ������ѡ����һ��������������ʹ����������һ***************
void Int1(void ) interrupt 2                                      //�ӳ�Լ0.5s
{
    EX1 = 0;                                                      //��ֹINT1�ж�Դ
	  if(IsMusic)
		{
      musicNum++;                                                 //��һ��
      if(N==musicNum)musicNum=0;                                  //���������������,ѭ��ѡ��
      while(0 == INT1);                                       		//�ȴ������ͷ�
	    Point=0;                                                   	//����λ������
			Next=1;
		}
    EX1 = 1;                                                      //ʹ��INT1�ж�Դ
    IE1 = 0;                                                      //���жϱ�־ 
}
//**********************�����ٺ����ֺз�����***********************************
void BeepT0(void) interrupt 1                                   	//���������ж�
{
	if(IsMusic)
	 {
	   TH0 = Sound_Temp_TH0;
 	   TL0 = Sound_Temp_TL0;
	 }
	 else
	 {
		 TH0 = (Tonetab[ALL])/256;
     TL0 = (Tonetab[ALL])%256;       //��װֵ
   }
	 BeepIO = !BeepIO;
}
//***************************���Ÿ账����*********************************
void Playvideo(uchar num,uchar Signature,uchar Octachord,uint Speed)
{
	uint LDiv,LDiv0,LDiv1,LDiv2,LDiv4,CurrentFre,Temp_T;
  uchar Tone,Length,SL,SH,SM,SLen,XG,FD;
	uint NewFreTab[12];													//�µ�Ƶ�ʱ�
	
	IsMusic=1;
	BeepIO=0;
	Status=1;                                   //Ĭ���ǿɲ���,��ʼ��
	musicNum=num%N;                             //��ѡ������Ϊ��ʼ                     
	ET1=0;                                      //��ֹ����100Hz������
	LcdShowStr(0,0,udstr);                      //��ʾ��ǰ���������������
	delayms(10);
	UDshow(musicNum);                           //��ʾ��������
	delayms(10);
	LcdShowStr(0,1,Playing);                    //��ʾ��ǰ�Ÿ����
	delayms(10);
	DisplayOneChar(0,1,ASCII[musicNum]);        //��ʾ��ǰ�������
	delayms(10);
	LcdShowStr(2,1,MusicName[musicNum]);        //��ʾ��ǰ������
	
	for(i=0;i<12;i++) 	                  			// ���ݵ��ż������˶��������µ�Ƶ�ʱ� 
	 {
		j = i + Signature;
		if(j > 11)
		 {
			j = j-12;
			NewFreTab[i] = FreTab[j]*2;
		 }
		else
			NewFreTab[i] = FreTab[j];

		if(Octachord == 1)
			NewFreTab[i]>>=2;
		else if(Octachord == 3)
			NewFreTab[i]<<=2;
	 }				
/*	for(i=0;i<N;i++)
	{
		SoundLength[i] = 0;
	  while(Sound[i][SoundLength[i]] != 0xff)	//�����������
	   {
		  SoundLength[i]+=2;
	   }
  }*/
	   Point = 0;                          //�����׸�λ��
	   Tone   = Sound[musicNum][Point];	
	   Length = Sound[musicNum][Point+1];  // ������һ����������ʱʱֵ
	   LDiv0 = 12000/Speed;				         // ���1�������ĳ���(����10ms) 	
	   LDiv4 = LDiv0/4; 					         // ���4�������ĳ��� 
	   LDiv4 = LDiv4-LDiv4*SOUND_SPACE; 	 // ��ͨ��������׼ 
	   TR0	 = 0;                          // �رշ���
     TR1   = 1;                          // ��ʱ10ms
	
	   while(Sound[musicNum][Point]!=0xff) //�Ƿ񳬳�����������Ч�߽�
	     {
				 if(Next)                        //�մ��ж��л�������
				  {
						Next=0;
				 		UDshow(musicNum);            //��ʾ��������
						DisplayOneChar(0,1,ASCII[musicNum]);//��ʾ�������
						delayms(10);
		        LcdShowStr(2,1,MusicName[musicNum]);//��ʾ������
				  }
        //******************��ͣ/����ת������**********************
				 if(K2==0)			
	        {
		        delayms(10);
		        if(K2==0)
		          {
							  while(!K2);
			          if(!Status)
		              { 
	                  Status=1;
										LcdShowStr(0,1,Playing);    //��ʾ����״̬
										delayms(10);
										DisplayOneChar(0,1,ASCII[musicNum]);//��ʾ�������
	                  delayms(10);
								    LcdShowStr(2,1,MusicName[musicNum]);//��ʾ������
	                }                           //������ͣ״̬�򣬽�������״̬
                else                          //���������ͣ״̬
                  {
                    TR0 = 0;
                    BeepIO = 1;
                    Status = 0;               //����λ���ܸĶ���ֵ
										LcdShowStr(0,1,Pause);    //��ʾ��ͣ״̬
										delayms(10);
										DisplayOneChar(0,1,ASCII[musicNum]);//���
	                  delayms(10);
								    LcdShowStr(2,1,MusicName[musicNum]);//����
                  }
		          }
	       }
       //**********************���Ÿ����*************************
			  if(Status)                          //�Ƿ�ɲ���
				 {
						 SL=Tone%10; 								    //��������� 
						 LED=Tab[Tone%8];               //LED������ʾ
						 SM=Tone/10%10; 								//������ߵ��� 
						 SH=Tone/100; 							    //������Ƿ����� 
						 CurrentFre = NewFreTab[SignTab[SL-1]+SH]; 	//�����Ӧ������Ƶ�� 	
						 if(SL!=0)
							 {
								 if (SM==1) CurrentFre >>= 2; 		//���� 
								 if (SM==3) CurrentFre <<= 2; 		//����
								 Temp_T = 65536-(50000/CurrentFre)*10/(12000000/OSC);//�����������ֵ
								 Sound_Temp_TH0 = Temp_T/256; 
								 Sound_Temp_TL0 = Temp_T%256; 
								 TH0 = Sound_Temp_TH0;  
								 TL0 = Sound_Temp_TL0 + 12; //��12�Ƕ��ж���ʱ�Ĳ��� 
							 }
						 SLen=LengthTab[Length%10]; 	  //����Ǽ�������
						 XG=Length/10%10; 			        //�����������(0��ͨ1����2����) 
						 FD=Length/100;
						 LDiv=LDiv0/SLen; 		        	//���������������ĳ���(���ٸ�10ms)
						 if(FD==1) 
							 LDiv=LDiv+LDiv/2;
						 if(XG!=1)	
							 if(XG==0) 				           //�����ͨ���������೤�� 
								 if(SLen<=4)	
									 LDiv1=LDiv-LDiv4;
								else
									 LDiv1=LDiv*SOUND_SPACE;
							else
								 LDiv1=LDiv/2; 	         	//������������೤�� 
						else
							LDiv1=LDiv;
						if(SL==0) LDiv1=0;
							LDiv2=LDiv-LDiv1; 	      	//����������ĳ��� 
						if (SL!=0)
							{
								TR0=1;                    //��������
								for(i=LDiv1;i>0;i--)    	//���涨���ȵ���,ʹ�ö�ʱ��T1
									{
										while(TF1==0);
										TH1 = Sound_Temp_TH1;
										TL1 = Sound_Temp_TL1;
										TF1 = 0;
									}
							}
						if(LDiv2!=0)
							{
								TR0=0; BeepIO=0;       //�رշ���
								for(i=LDiv2;i>0;i--) 	//������ļ��
									{
										while(TF1==0);
										TH1 = Sound_Temp_TH1;
										TL1 = Sound_Temp_TL1;
										TF1=0;
									}
							}
							
						Point+=2;                  //������λ��ָ���¸�����
						Tone=Sound[musicNum][Point];
						Length=Sound[musicNum][Point+1];
		    }
	//*****************************��ͣ�м���˳��ĳ���************************************
			  else
				 {
					  if(Next)                        //�մ��ж��л���
				      {
								Next=0;
								UDshow(musicNum);           //��ʾ��������
								delayms(10);
								LcdShowStr(0,1,Pause);      //��ʾ��ͣ״̬
								delayms(10);
								DisplayOneChar(0,1,ASCII[musicNum]);//���
	              delayms(10);
								LcdShowStr(2,1,MusicName[musicNum]);//����
//								delayms(800);               //����Ӱ���˳�,���ɷ�ֹlcd1602ˢ�¹���
				      }
						if(K3==0)			
							{
								delayms(10);
								if(K3==0)
									 {
										 while(!K3);
										 break;
									 }
						  }
           if(K1==0)			
							{
								delayms(10);
								if(K1==0)
									 {
										 while(!K1);
										 musicNum--;                             //��һ��
										 if(musicNum==(-1))musicNum=N-1;         //0--,�ص�N-1,ѭ��ѡ��
										 Point=0;                                //����λ������
										 UDshow(musicNum);                       //��ʾ��������
										 delayms(10);
										 DisplayOneChar(0,1,ASCII[musicNum]);    //���
	                   delayms(10);
										 LcdShowStr(2,1,MusicName[musicNum]);    //����
									 }
						  }
				 }			            
		  }			//�˳��Ÿ�
			IsMusic=0;                         //����T0�Ÿ��ѡ����
			BeepIO=0;
			Point=0;
			LcdShowStr(0,0,welcome);
			delayms(10);
			LcdShowStr(0,1,leave);
}
/*******************************************************************************
* �� �� ��       : READIR_QUIT(void )
* ��������		   : ������˳�
*******************************************************************************/
void READIR(void ) interrupt 0
{
//************************������յĳ���*********************************
	 uint err;
	 Time=0;
	 IRIN=1;//��ʼ���˿�

	 delayus(700);                      	//7ms
	 if(IRIN==0)		                      //ȷ���Ƿ���Ľ��յ���ȷ���ź�
		{	 
			err=1000;				                  //10ms,����˵�����յ�������ź�
			while((IRIN==0)&&(err>0))	       //�ȴ�ǰ��9ms�ĵ͵�ƽ��ȥ  		
			 {			
				 delayus(1);
				 err--;
			 }
			 
			if(IRIN==1)			                 //�����ȷ�ȵ�9ms�͵�ƽ
			 {
					err=500;
					while((IRIN==1)&&(err>0))     //�ȴ�4.5ms����ʼ�ߵ�ƽ��ȥ
							{
							 delayus(1);
							 err--;
							}
		     for(k=0;k<4;k++)		          //����4������
				  {				
						for(j=0;j<8;j++)	          //����һ������
								{
									err=60;		
									while((IRIN==0)&&(err>0)) //�ȴ��ź�ǰ���560us�͵�ƽ��ȥ
											{
											 delayus(1);
											 err--;
											}
									err=500;
									while((IRIN==1)&&(err>0))	 //����ߵ�ƽ��ʱ�䳤�ȡ�
											{
											 delayus(10);	           //0.1ms
											 Time++;
											 err--;
											 if(Time>30)
											 {return;}
											}
									IrValue[k]>>=1;	          //k��ʾ�ڼ�������
									if(Time>=8)			          //����ߵ�ƽ���ִ���565us,��ô��1
											{
											 IrValue[k]|=0x80;
											}
									Time=0;		                //����ʱ��Ҫ���¸�ֵ							
								}
			   }
			}
		 if(IrValue[2]!=~IrValue[3])           //У��
			 {return;}
	 }
}
#endif