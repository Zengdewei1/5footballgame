/******************************************************************************
*�������Ƽ����ܽ��ͣ�
*          1.   void open_hzk()  
*               �ص㣺�޲������޷���ֵ�� 
*               ��;���򿪺��ֿ��ļ�  
*          2.   void get_hz(char incode[],char bytes[])
*               �ص㣺char incode[]:���ڴ��뺺������
*                     char bytes[]:�������������ģ
*               ��;����һ�����ֵ�����ת������λ�룬��������ֵ���ģ
*          3.   void dishz(int x,int y,char code[],int color)
*               �ص㣺int x,int y:��ʾһ�����ֵ����Ͻǵ�����
*                     char code[]:���ֵ���ģ
*                     int color:��ʾ���ֵ���ɫ
*               ��;����ʾһ������
*          4.   void disques(int x,int y,char *Question)
*               �ص㣺int x,int yͬ��
*                     char *Question:��ʾ�������������
*               ��;����ʾһ�������ľ���
*          5.   void getHZ(char *filename,char *Question)
*               �ص㣺char *filename:����·��
*                     char *Question:�����ַ���
*               ��;�����ļ��е���һ�������Question�ַ���
* ����������ͣ�Ԥ�����е�LEFTTOPX��LEFTTOPY��������������Ͻ�����
*               RIGHTBORDER����������Ҷ˵Ľ���
*******************************************************************************/

/*�Զ����Ʒ�ʽ�򿪺��ֿ��ļ�hzk16X*/
# include"HZDIS.h"
# include"SVGAmode.h"

FILE* hzk_p;
FILE* asc_p;

/*ͨ�����ֵ�����ת������λ�룬������
bytes[]�����з��� */
void get_hz(char incode[],char bytes[])
{
	unsigned char qh,wh;
	unsigned long offset;
	if((hzk_p = fopen("HZK\\hzk16","rb"))==NULL)	//�ļ���ȡ���
	{
		puts("cannot open file hzk16");
		exit(1);
	}
	qh = incode[0] - 0xa0;                  //�õ�����
	wh = incode[1] - 0xa0;                  //�õ�λ��
	offset = (94*(qh-1)+(wh -1))*32L;       //�õ�ƫ��λ��
	fseek(hzk_p,offset,SEEK_SET);           //�ƶ��ļ�ָ�뵽Ҫ��ȡ�ĺ�����ģ��
	fread(bytes,1,32,hzk_p);                   //��ȡ32���ֽڵĺ�����ģ�������أ�
}

/*�õ��ַ�����ģ*/
void get_asc(char code,char bytes[])
{
	unsigned long offset;
	if((asc_p = fopen(ASC16_PATH,"rb")) == NULL)
	{
		puts("cannot open file asc16");
		exit(1);
	}
	offset = (unsigned long)(code*16);
	fseek(asc_p,offset,SEEK_SET);
	fread(bytes,16,1,asc_p);
}

/*��x��y�������һ���ֵ���ģ������ʾ */
/*���һ��Ҫ�ر��ļ�*/
void dishz(int x0,int y0,char code[],unsigned color) //code[]Ϊ����
{
	unsigned char mask[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
			  /*������ģÿ�и�λ������ */
	register int i,j,x,y,pos;
	char mat[32];
	get_hz(code,mat);
	y=y0;
	for(i=0;i<16;++i)
	{
		x=x0;
		pos = 2*i;
		for(j=0;j<16;++j)
		{
			if((mask[j%8]&mat[pos+j/8])!=NULL)    /*���γ�������ģ��һ��λ*/
			{
				Putpixel256(x,y,color);
				//getch();                         //test
			}
			x++;
		}
		y++;
	}
	fclose(hzk_p);
}

/*��ʾһ���ַ�*/
void disasc(int x,int y,char code,unsigned color)
{
	register int i,j;
	char mat[16];
	get_asc(code,mat);
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if((mat[i]>>(7-j))&0x1)    	//Ϊ1��λ��ʾ
			{
				Putpixel256(x+j,y+i,color);
			}
		}
	}
	fclose(asc_p);
}
	
/*�������ַ�����ӡ����Ļ�� */
void dis_string(int x,int y,char *string,unsigned color,int Left,int Right)
{
	while(*string != NULL)
	{
		while((x < Right)&&(*string != NULL))
		{
			dishz(x,y,string,color);
			x+=16;
			string+=2;
		}
		x=Left;
		y+=16;
	}
}
/*���ַ��ַ�����ӡ����Ļ��*/
void disasc_str(int x,int y,char *string,unsigned color,int Left,int Right)
{
	while(*string != NULL)
	{
		while(x<Right&&*string != NULL)
		{
			disasc(x,y,string[0],color);
			x+=8;
			string++;
		}
		x = Left;
		y+=14;
	}
}

void mix(int x,int y,char *string,unsigned color,int Left,int Right)
{
	while(*string != NULL)
	{
		while((x<Right)&&(*string != NULL))
		{
			if(string[0] > 0&&string[0] < 127)
			{
				disasc(x,y+3,string[0],color);
				x+=8;
				string++;
				continue;
			}
			dishz(x,y,string,color);
			x+= 16;
			string+=2;
		}
		x = Left;
		y+= 16;
	}
}
			
	
	
	
	
	
	
	
/*�ڴ򿪵��ļ��ж���һ����Ŀ����sָ���� */
/*��������⣺fgets()�ַ����������棡 */
void getHZ(char* filename,char *Question)
{
	FILE *fp;
	if((fp = fopen(filename,"r")) == NULL)
	{
		printf("File can not be opened!\n");
		getch();
		exit(1);
	}
	fgets(Question,120,fp);
}
