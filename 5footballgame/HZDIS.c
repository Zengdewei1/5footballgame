/******************************************************************************
*函数名称及功能解释：
*          1.   void open_hzk()  
*               特点：无参数，无返回值； 
*               用途：打开汉字库文件  
*          2.   void get_hz(char incode[],char bytes[])
*               特点：char incode[]:用于传入汉字内码
*                     char bytes[]:用于输出汉字字模
*               用途：将一个汉字的内码转换成区位码，并输出该字的字模
*          3.   void dishz(int x,int y,char code[],int color)
*               特点：int x,int y:显示一个汉字的左上角的坐标
*                     char code[]:汉字的字模
*                     int color:显示汉字的颜色
*               用途：显示一个汉字
*          4.   void disques(int x,int y,char *Question)
*               特点：int x,int y同上
*                     char *Question:显示的完整问题句子
*               用途：显示一个完整的句子
*          5.   void getHZ(char *filename,char *Question)
*               特点：char *filename:问题路径
*                     char *Question:问题字符串
*               用途：从文件中导出一个问题给Question字符串
* 其他问题解释：预定义中的LEFTTOPX和LEFTTOPY是文字输出的左上角坐标
*               RIGHTBORDER是文字输出右端的界限
*******************************************************************************/

/*以二进制方式打开汉字库文件hzk16X*/
# include"HZDIS.h"
# include"SVGAmode.h"

FILE* hzk_p;
FILE* asc_p;

/*通过汉字的内码转化成区位码，并放入
bytes[]数组中返回 */
void get_hz(char incode[],char bytes[])
{
	unsigned char qh,wh;
	unsigned long offset;
	if((hzk_p = fopen("HZK\\hzk16","rb"))==NULL)	//文件读取检测
	{
		puts("cannot open file hzk16");
		exit(1);
	}
	qh = incode[0] - 0xa0;                  //得到区号
	wh = incode[1] - 0xa0;                  //得到位号
	offset = (94*(qh-1)+(wh -1))*32L;       //得到偏移位置
	fseek(hzk_p,offset,SEEK_SET);           //移动文件指针到要读取的汉子字模处
	fread(bytes,1,32,hzk_p);                   //读取32个字节的汉子字模（并返回）
}

/*得到字符的字模*/
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

/*在x，y对输入的一个字的字模进行显示 */
/*最后一定要关闭文件*/
void dishz(int x0,int y0,char code[],unsigned color) //code[]为内码
{
	unsigned char mask[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
			  /*屏蔽字模每行各位的数组 */
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
			if((mask[j%8]&mat[pos+j/8])!=NULL)    /*屏蔽出汉子字模的一个位*/
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

/*显示一个字符*/
void disasc(int x,int y,char code,unsigned color)
{
	register int i,j;
	char mat[16];
	get_asc(code,mat);
	for(i=0;i<16;i++)
	{
		for(j=0;j<8;j++)
		{
			if((mat[i]>>(7-j))&0x1)    	//为1的位显示
			{
				Putpixel256(x+j,y+i,color);
			}
		}
	}
	fclose(asc_p);
}
	
/*将汉字字符串打印到屏幕上 */
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
/*将字符字符串打印在屏幕上*/
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
			
	
	
	
	
	
	
	
/*在打开的文件中读入一个题目放入s指针中 */
/*待解决问题：fgets()字符输出溢出警告！ */
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
