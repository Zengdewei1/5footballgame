#ifndef	HZDIS_H
#define	HZDIS_H

/*Í·ÎÄ¼þ*/
# include<stdio.h>
# include<conio.h>
# include<graphics.h>
# include<stdlib.h>
# include<dos.h>
#include"SVGAmode.h"

# define ASC16_PATH "HZK\\ASC16"


void get_asc(char code,char bytes[]);
void disasc(int x,int y,char code,unsigned color);
void get_hz(char incode[],char bytes[]);
void dishz(int x,int y,char code[],unsigned color);
void dis_string(int x,int y,char *string,unsigned color,int Left,int Right);
void getHZ(char *filename,char *Question);
void disasc_str(int x,int y,char *string,unsigned color,int Left,int Right);
void mix(int x,int y,char *string,unsigned color,int Left,int Right);

#endif