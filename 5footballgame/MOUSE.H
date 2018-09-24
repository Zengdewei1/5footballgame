#ifndef _MOUSE_H_
#define _MOUSE_H_

#include"SVGAmode.h"
#include<dos.h>

void Cursor(int x,int y,int color);
int Initmouse(int xmi,int xma,int ymi,int yma);
int Readmouse();
void Newxy();
void Mouse_savebk(int x,int y);
void Mouse_putbk(int x,int y);
int Mouse_press(int x1,int y1,int x2,int y2);
int Mouse_above(int x1,int y1,int x2,int y2);

//每次贴一张新图都要重新保存鼠标BK
void NewBK();


#endif