#ifndef	HANZI_H
#define	HANZI_H
#include<stdio.h>
#include"SVGAmode.h"
#include<string.h>
#include<stdlib.h>
#include<bios.h>
#include<dos.h>
#include<conio.h>

void text16(int ax,int ay,char *p,int colour,int ROW,int COL) ;
void printASC(char *s,int x,int y,int dx,int dy,short color);
void out_hz(int x,int y, char *s, int mode, int size, int part, unsigned char color);
void out_ASC(int x, int y, char *s, int mode, int color);
void out_ASCplus(int x, int y, char *s, int mode,int col,int row,int d,int color);
void InputChar(int x, int y, char *s, int mode,int modeII,int col,int row,int d,int color,int limit);
#endif
