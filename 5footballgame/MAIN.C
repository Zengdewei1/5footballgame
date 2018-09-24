#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include"SVGAmode.h"
#include"game.h"
#include"mouse.h"
#include<conio.h>
#include<bios.h>
#include"tool.h"
#include"football.h"
#include"HZDIS.h"
#include"HANZI.h"

void main ()
{
	_game game;

	SetSVGA256();
	Initmouse(0, 1024, 0, 768);

	initgame(&game);
	
	Putbmp256(0, 0, "bmp\\Credis.bmp");

	while(bioskey(1)==0 && Mouse_press(0,0,1024,768)==0)
	{
		Newxy();
		delay(5);
	}
	menu_enter(&game);

	while(1)
	{
		Newxy();
		Update(&game);
		delay(5);
	}

}
