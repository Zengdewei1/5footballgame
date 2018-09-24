
#include"MOUSE.h"

union REGS regs;

int mx=0,my=0,buttons=0;
unsigned mouse_sq[16][16]={0};
int default1=0;



int Liney(int x1,int y1,int x2, int y2,int color);       //´Ë´¦¿ÉÖØÐÂÐ´¹ý
void Cursor(int x,int y,int color)
{

    Liney(x,y,x,y+15,color);
    Liney(x+1,y+2,x+1,y+13,color);
    Liney(x+2,y+3,x+2,y+11,color);
    Liney(x+3,y+4,x+3,y+9,color);
    Liney(x+4,y+5,x+4,y+9,color);
    Liney(x+5,y+7,x+5,y+10,color);
    Liney(x+6,y+8,x+6,y+10,color);
    Liney(x+7,y+9,x+7,y+11,color);
    Liney(x+8,y+11,x+8,y+11,color);
    Liney(x+9,y+12,x+9,y+12,color);

    Liney(x+1,y+3,x+1,y+12,0xFFFFFF);
    Liney(x+2,y+4,x+2,y+10,0xFFFFFF);
    Liney(x+3,y+5,x+3,y+8,0xFFFFFF);
    Liney(x+4,y+6,x+4,y+8,0xFFFFFF);
    Liney(x+5,y+8,x+5,y+9,0xFFFFFF);
    Liney(x+6,y+9,x+6,y+9,0xFFFFFF);
    Liney(x+7,y+10,x+7,y+10,0xFFFFFF);

}


int Initmouse(int xmi,int xma,int ymi,int yma)
{
    int retcode;
    regs.x.ax=0;
    int86(51,&regs,&regs);
    retcode=regs.x.ax;
    if(retcode==0)
    {
  	   return 0;
    }
    regs.x.ax=7;
    regs.x.cx=xmi;
    regs.x.dx=xma;
    int86(51,&regs,&regs);
    regs.x.ax=8;
    regs.x.cx=ymi;
    regs.x.dx=yma;
    //regs.x.bx=0;
    int86(51,&regs,&regs);
    default1=0;
    return retcode;
}
/*??È¡????Ì¬???*/
int Readmouse()
{
    int xx0=mx,yy0=my;
    int xnew = 0, ynew = 0;

    regs.x.ax=3;
    int86(51,&regs,&regs);
    xnew=regs.x.cx;
    ynew=regs.x.dx;
    buttons=regs.x.bx;
    delay(10);

    if(mx == xnew && my == ynew)
    {
        return 0;              //Êó±êÃ»ÒÆ¶¯
    }
    else
    {
        mx=xnew;
        my=ynew;
        return 1;              //Êó±êÒÆ¶¯ÁË
    }



}
/*???????????*/
void Newxy()
{
    int xx0=mx,yy0=my;
    if(Readmouse() == 1)
    {
        if(default1)
            Mouse_putbk(xx0,yy0);
        Mouse_savebk(mx,my);

        Cursor(mx,my,BLACK);

    }

    default1=1;

}
//Ã¿´ÎÌùÒ»ÕÅÐÂÍ¼¶¼ÒªÖØÐÂ±£´æÊó±êBK
void NewBK()
{
        Mouse_savebk(mx,my);
}


/*??????Ç±??????*/
void Mouse_savebk(int x, int y)
{
    int i,j;
    for(i=0;i<=15;i++)
        for(j=0;j<=15;j++)
            mouse_sq[i][j]=Getpixel256(x+i,y+j);
}

/*?Ö¸????Ç±??????*/
void Mouse_putbk(int x,int y)
{
    int i,j;
    for(i=0;i<=15;i++)
        for(j=0;j<=15;j++)
            Putpixel256(x+i,y+j,mouse_sq[i][j]);
}
/*??????????????*/
int Mouse_press(int x1,int y1,int x2,int y2)
{
    if(mx>=x1&&mx<=x2&&my>=y1&&my<=y2&&buttons)
    return(1);
    else
    return(0);
}
/*?????????Ä³?????*/
int Mouse_above(int x1,int y1,int x2,int y2)
{
    if(mx>=x1&&mx<=x2&&my>=y1&&my<=y2)
    return(1);
    else
    return(0);
}


int Liney(int x1,int y1,int x2, int y2,int color)
{
  int i;
  if(x1>=1024||x2>=1024||y1>=768||y2>=768)
  return(0);

  if(x1<=0||x2<=0||y1<=0||y2<=0)
  return(0);

  if(x1!=x2||y1>y2)             //x1Òª±£Ö¤ºÍx2ÏàµÈ
  return(0);
  for(i=y1;i<=y2;i++)
  Putpixel256(x1,i,color);
}