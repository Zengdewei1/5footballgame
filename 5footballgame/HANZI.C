#include"hanzi.h"
#include<stdio.h>
#include"SVGAmode.h"
#include<string.h>
#include<stdlib.h>
#include<bios.h>
#include<dos.h>
#include<conio.h>

		
void text16(int ax,int ay,char *p,int colour,int ROW,int COL)                        //汉字显示
{
	FILE *fp;
	char buffer[32];
	register m,n,i,j,k;
	unsigned char qh,wh;
	unsigned long location;
	if((fp=fopen("HZK\\hzk16","rb"))==NULL)
	{
		printf("Can't open hzk16!");
		getch();
		exit(0);
	}
	while(*p)
	{
		qh=*p-0xa0;
		wh=*(p+1)-0xa0;
		location=(94*(qh-1)+(wh-1))*32L;
		fseek(fp,location,SEEK_SET);
		fread(buffer,32,1,fp);
		for(i=0;i<16;i++)
			for(n=0;n<ROW;n++)
				for(j=0;j<2;j++)
					for(k=0;k<8;k++)
						for(m=0;m<COL;m++)
							if(((buffer[i*2+j]>>(7-k))&0x1)!=NULL)
								Putpixel64k(ax+8*j*COL+k*COL+m,ay+i*ROW+n,colour);
		p+=2;
		ax+=16*COL;
	}
	rewind(fp);
	fclose(fp);
}

void printASC(char *s,int x,int y,int dx,int dy,short color)
{
	char *s1;
	unsigned long offset;
	FILE *fp;
	char buffer[16]; //buffer用来存储一个字符
	int m,n,i,j;
	if ((fp=fopen("hzk\\asc16","rb"))==NULL)
	{
		printf("Can't open asc16,Please add it");
		exit(1);
	}
	s1=s;
	while(*s)
	{
		offset=(*s)*16+1; //计算该字符在字库中偏移量
		fseek(fp,offset,SEEK_SET);
		fread(buffer,16,1,fp); //取出字符16字节的点阵字模存入buffer中

		for(n=0;n<dx;n++) //将16位字节的点阵按位在屏幕上打印出来(1:打印,0:不打印),显示字符
		{
			for(i=0;i<16;i++)
			{
						for(j=0;j<8;j++)
						{
							for(m=0;m<dy;m++)
							{
								if(((buffer[i]>>(7-j))&0x1)!=NULL)
								{
									Putpixel64k(x+j*2+n,y+i*2+m,color);
								}
							}
						}

			}
		}
		s++; //一个字符内码占用一个字节
		x+=8*dx;//字符间间隔
	}
	s=s1;
	fclose(fp);

}

void out_hz(int x,int y, char *s, int mode, int size, int part, unsigned char color) //输出汉字
{
	FILE *hzk_p;
	unsigned char quma,weima;	
	unsigned long offset;		
	int i,j,k;
	
	switch (mode)												
	{
		case 1:
		{
		
			switch(size)
			{
				case 12:			
					{		
						char mat[24];   													
						hzk_p=fopen("HZK\\HZK12","rb");						
						if(hzk_p==NULL)
						{
							printf("\n Can't open file HZK ! Press any key to return.");
							getch();
							exit(1);
						}
						while(*s!='\0')														
						{
							quma=s[0]-128-32;
							weima=s[1]-128-32;							
							offset=(94 * (quma-1) + (weima-1)) * 24L;	
							fseek(hzk_p,offset,0);											
							fread(mat,24,1,hzk_p);						
							for(i=0;i<12;i++)							
							{
								for(j=0;j<2;j++)						
								{
									for(k=0;k<6;k++)					
									{
													if(((mat[i * 2 + j]>>(5-k))& 0x1) !=NULL)		
														Putpixel256(x+(6 * j + k) ,y + i ,color);		
									}
							
								}
							}
							s+=2;
							x+=part;
								
						}
						fclose(hzk_p);
						break;
					
					}
					
					
					
				case 16:
					{			
						char mat[32];													
						hzk_p=fopen("HZK\\HZK16","rb");				
						if(hzk_p==NULL)
						{
							printf("\n Can't open file HZK ! Press any key to return.");
							getch();
							exit(1);
						}
						while(*s!='\0')													
						{
							quma=s[0]-128-32;
							weima=s[1]-128-32;											
							offset=(94 * (quma-1) + (weima-1)) * 32L;
							fseek(hzk_p,offset,0);
							fread(mat,32,1,hzk_p);
							for(i=0;i<16;i++)
							{
								for(j=0;j<2;j++)
								{
									for(k=0;k<8;k++)
									{
													if(((mat[i * 2 + j]>>(7-k))& 0x1) !=NULL)
														Putpixel256(x+(8 * j + k) ,y + i  ,color);		
									}
							
								}
							}
							s+=2;
							x+=part;
								
						}
						fclose(hzk_p);
						break;
					}
					
					
					
				case 24:
				{		
						char mat[72];
						hzk_p=fopen("HZK\\HZK24","rb");
						if(hzk_p==NULL)
						{
							printf("\n Can't open file HZK ! Press any key to return.");
							getch();
							exit(1);
						}
						while(*s!='\0')
						{
							quma=s[0]-128-32;
							weima=s[1]-128-32;
							offset=(94 * (quma-1) + (weima-1)) * 72L;
							fseek(hzk_p,offset,0);
							fread(mat,72,1,hzk_p);
							for(i=0;i<24;i++)
							{
								for(j=0;j<3;j++)
								{
									for(k=0;k<8;k++)
									{
													if(((mat[i * 3 + j]>>(7-k))& 0x1) !=NULL)
														Putpixel256(x+(8 * j + k) ,y + i ,color);		
									}
							
								}
							}
							s+=2;
							x+=part;
								
						}
						fclose(hzk_p);
						break;
					}
					
				
					
				case 48:
				{		
						char mat[288];
						hzk_p=fopen("HZK\\HZK48","rb");
						if(hzk_p==NULL)
						{
							printf("\n Can't open file HZK ! Press any key to return.");
							getch();
							exit(1);
						}
						while(*s!='\0')
						{
							quma=s[0]-128-32;
							weima=s[1]-128-32;
							offset=(94 * (quma-1) + (weima-1)) * 288L;
							fseek(hzk_p,offset,0);
							fread(mat,288,1,hzk_p);
							for(i=0;i<48;i++)
							{
								for(j=0;j<6;j++)
								{
									for(k=0;k<8;k++)
									{
													if(((mat[i * 6 + j]>>(7-k))& 0x1) !=NULL)
														Putpixel256(x+(8 * j + k) ,y + i ,color);		
									}
							
								}
							}
							s+=2;
							x+=part;
								
						}
						fclose(hzk_p);
						break;
				}
					
			}
		}
	
		case 2:						
		{	
			switch(size)
				{
					case 12:				
						{		
							char mat[24];   													
							hzk_p=fopen("HZK\\HZK12","rb");						
							if(hzk_p==NULL)
							{
								printf("\n Can't open file HZK ! Press any key to return.");
								getch();
								exit(1);
							}
							while(*s!='\0')														
							{
								quma=s[0]-128-32;
								weima=s[1]-128-32;							
								offset=(94 * (quma-1) + (weima-1)) * 24L;	
								fseek(hzk_p,offset,0);										
								fread(mat,24,1,hzk_p);							
								for(i=0;i<12;i++)							
								{
									for(j=0;j<2;j++)						
									{
										for(k=0;k<6;k++)					
										{
														if(((mat[i * 2 + j]>>(5-k))& 0x1) !=NULL)		
															Putpixel64k(x+(6 * j + k) ,y + i  ,color);		
										}
								
									}
								}
								s+=2;
								x+=part;
									
							}
							fclose(hzk_p);
							break;
						
						}
						
						
						
					case 16:
						{			
							char mat[32];													
							hzk_p=fopen("HZK\\HZK16","rb");					
							if(hzk_p==NULL)
							{
								printf("\n Can't open file HZK ! Press any key to return.");
								getch();
								exit(1);
							}
							while(*s!='\0')													
							{
								quma=s[0]-128-32;
								weima=s[1]-128-32;											
								offset=(94 * (quma-1) + (weima-1)) * 32L;
								fseek(hzk_p,offset,0);
								fread(mat,32,1,hzk_p);
								for(i=0;i<16;i++)
								{
									for(j=0;j<2;j++)
									{
										for(k=0;k<8;k++)
										{
														if(((mat[i * 2 + j]>>(7-k))& 0x1) !=NULL)
															Putpixel64k(x+(8 * j + k) ,y + i  ,color);		
										}
								
									}
								}
								s+=2;
								x+=part;
									
							}
							fclose(hzk_p);
							break;
						}
						
						
						
					case 24:
					{		
							char mat[72];
							hzk_p=fopen("HZK\\HZK24","rb");
							if(hzk_p==NULL)
							{
								printf("\n Can't open file HZK ! Press any key to return.");
								getch();
								exit(1);
							}
							while(*s!='\0')
							{
								quma=s[0]-128-32;
								weima=s[1]-128-32;
								offset=(94 * (quma-1) + (weima-1)) * 72L;
								fseek(hzk_p,offset,0);
								fread(mat,72,1,hzk_p);
								for(i=0;i<24;i++)
								{
									for(j=0;j<3;j++)
									{
										for(k=0;k<8;k++)
										{
														if((((mat[i * 3 + j]>>(7-k))& 0x1) !=NULL)&&(x<1024))
															Putpixel64k(x+(8 * j + k) ,y + i  ,color);
														else if(x>=1024)
															Putpixel64k(x+(8 * j + k) ,y + i*96  ,color);		
										}
								
									}
								}
								s+=2;
								x+=part;
									
							}
							fclose(hzk_p);
							break;
						}
						
					
						
					case 48:
					{		
							char mat[288];
							hzk_p=fopen("HZK\\HZK48","rb");
							if(hzk_p==NULL)
							{
								printf("\n Can't open file HZK ! Press any key to return.");
								getch();
								exit(1);
							}
							while(*s!='\0')
							{
								quma=s[0]-128-32;
								weima=s[1]-128-32;
								offset=(94 * (quma-1) + (weima-1)) * 288L;
								fseek(hzk_p,offset,0);
								fread(mat,288,1,hzk_p);
								for(i=0;i<48;i++)
								{
									for(j=0;j<6;j++)
									{
										for(k=0;k<8;k++)
										{
														if(((mat[i * 6 + j]>>(7-k))& 0x1) !=NULL)
															Putpixel64k(x+(8 * j + k),y + i ,color);		
										}
								
									}
								}
								s+=2;
								x+=part;
									
							}
							fclose(hzk_p);
							break;
					}
						
				}
			
		}
		
		default :
			break;
	}
		
	
		
}

void out_ASC(int x, int y, char *s, int mode, int color) //输出字母 
{
	FILE *hzk_p;
	unsigned long offset;		
	int i,j;
	char mat[16];
	switch (mode)
	{
		case 1:
		{
			char mat[16];													
			hzk_p=fopen("HZK\\ASC16","rb");					
			if(hzk_p==NULL)
			{
				printf("\n Can't open file HZK ! Press any key to return.");
				getch();
				exit(1);
			}
			while(*s!='\0')
			{
				offset=(*s)*16+1;			 
				fseek(hzk_p,offset,SEEK_SET);
				fread(mat,16,1,hzk_p); 			
				for(i=0;i<16;i++)
				{
					for(j=0;j<8;j++)
					{
						if(((mat[i]>>(7-j))&0x1)!=NULL)
						{
							Putpixel256(x+j,y+i,color);
						}
					}
				}
		
				s++; 
				x+=8;
			}	
			fclose(hzk_p);
			
		}
			
		
		case 2:
		{
			char mat[16];													
			hzk_p=fopen("HZK\\ASC16","rb");					
			if(hzk_p==NULL)
			{
				printf("\n Can't open file HZK ! Press any key to return.");
				getch();
				exit(1);
			}
			while(*s!='\0')
			{
				offset=(*s)*16+1;			
				fseek(hzk_p,offset,SEEK_SET);
				fread(mat,16,1,hzk_p); 			
				for(i=0;i<16;i++)
				{
					for(j=0;j<8;j++)
					{
						if(((mat[i]>>(7-j))&0x1)!=NULL)
						{
							Putpixel64k(x+j,y+i,color);
						}
					}
				}
		
				s++;
				x+=8;
			}	
			fclose(hzk_p);
			
		}
		default:
			break;
	}
}


void out_ASCplus(int x, int y, char *s, int mode,int col,int row,int d,int color) //放大字母 
{
	FILE *hzk_p;
	unsigned long offset;		
	int i,j,m,n;
	char mat[16];
	switch (mode)
	{
		case 1:
		{
			char mat[16];													
			hzk_p=fopen("HZK\\ASC16","rb");					
			if(hzk_p==NULL)
			{
				printf("\n Can't open file HZK ! Press any key to return.");
				getch();
				exit(1);
			}
			while(*s!='\0')
			{
				offset=(*s)*16+1;			
				fseek(hzk_p,offset,SEEK_SET);
				fread(mat,16,1,hzk_p); 			
				for(i=0;i<16;i++)
				{
					for(j=0;j<8;j++)
					{
						if(((mat[i]>>(7-j))&0x1)!=NULL)
						{
							for(m=0;m<col;m++)
							{
								for(n=0;n<row;n++)
								{
										Putpixel256(x + j * col + m,y + i * row + n,color);
								}
							}
						}
					}
				}
		
				s++; 
				x+=d;
			}
			fclose(hzk_p);
		}
			
		
		case 2:
		{
			char mat[16];													
			hzk_p=fopen("HZK\\ASC16","rb");					
			if(hzk_p==NULL)
			{
				printf("\n Can't open file HZK ! Press any key to return.");
				getch();
				exit(1);
			}
			while(*s!='\0')
			{
				offset=(*s)*16+1;			 
				fseek(hzk_p,offset,SEEK_SET);
				fread(mat,16,1,hzk_p); 			
				for(i=0;i<16;i++)
				{
					for(n=0;n<row;n++)
					{
						for(j=0;j<8;j++)
						{
							for(m=0;m<col;m++)
							{
								if(((mat[i]>>(7-j))&0x1)!=NULL)
								{       
										Putpixel64k(x + j * col + m,y + i * row + n,color);
								}
							}
						}
					}
				}
				s++; 
				x+=d;
			}	
			fclose(hzk_p);
			
			
		}
		default:
			break;
	}
}

/*
void InputChar(int x, int y, char *s, int mode,int modeII,int col,int row,int d,int color,int limit) //x,y代表左边第一个字符的坐标，s代表输入字符串，mode代表显示模式，modeII代表只允许输入数字或无限制或输出暗文，col，row代表横向纵向放大倍数，d代表间距，color代表颜色，limit代表只最多输入的字符数
{
	int i=0;
	switch(modeII)
	{
		case 1: //	只能输入数字
		{

			while((s[i]=bioskey(0))!=ENTER) //获取键盘输入字符，当按下回车键时，结束输入
			{
				if((s[i]!=BACKSPACE)&&(i<limit)&&(s[i]<='9')&&(s[i]>='0')) //限制只能输入数字
				{
					out_ASCplus(x,y,&s[i],mode,col,row,0,color); //一个一个地显示字符
					x+=d; //输出完一个字符后向左移动20
					++i;
				}
				if((i!=0)&&(s[i]==BACKSPACE)) //文本框里又不为空时，才能按下
				{
					s[i]='\0';
					--i;
					x-=d;
					out_ASCplus(x,y,&s[i],mode,col,row,0,0xffffff);//覆盖原字符以删除
				}
			}
			s[i]='\0';
		}
		break;
		case 2: //	不加任何限制
		{
			while((s[i]=bioskey(0))!=ENTER) //获取键盘输入字符，当按下回车键时，结束输入
			{
				if((s[i]!=BACKSPACE)&&(i<limit)) //将字符数限制在limit以下
				{
					out_ASCplus(x,y,&s[i],mode,col,row,0,color); //一个一个地显示字符
					x+=d; //输出完一个字符后向左移动20
					++i;
				}
				if((i!=0)&&(s[i]==BACKSPACE)) //文本框里又不为空时，才能按下
				{
					s[i]='\0';
					--i;
					x-=d;
					out_ASCplus(x,y,&s[i],mode,col,row,0,0xffffff);//覆盖原字符以删除
				}
			}
			s[i]='\0';
		}
		break;
		case 3: //输出暗文
		{
			while((s[i]=bioskey(0))!=ENTER) //获取键盘输入字符，当按下回车键时，结束输入
			{
				if((s[i]!=BACKSPACE)&&(i<limit)) //将字符数限制在limit以下
				{
					out_ASCplus(x,y,"*",mode,col,row,0,color); //一个一个地显示"*"
					x+=d; //输出完一个字符后向左移动20
					++i;
				}
				if((i!=0)&&(s[i]==BACKSPACE)) //文本框里又不为空时，才能按下
				{
					s[i]='\0';
					--i;
					x-=d;
					out_ASCplus(x,y,"*",mode,col,row,0,0xffffff);//覆盖原字符以删除
				}
			}
			s[i]='\0';
		}
		break;
	}
}

*/