/***************************C文件**************************/

/**********************************************************

Copyright (C) 2017 郭裕祺 黄杰豪.
File name:      game.h
Author:         黄杰豪
Version：        1.1
Date：           2017/11/1
Description：    足球游戏外部功能所有函数库源文件。

Function List：
	1.void Update(struct _GAME * self);
	2.void changestate(struct _GAME * self, _state * pnewstate);
		状态机循环及改变状态执行的函数

	3.void initgame(struct _GAME * game);
		初始化结构game内参数

	4.void menu_enter(struct _GAME * game);
	5.void menu_execute(struct _GAME * game);
	6.void menu_exit(struct _GAME * game);
		状态主菜单三个函数

	7.void doubleman_enter(struct _GAME * game);
	8.void doubleman_execute(struct _GAME * game);
	9.void doubleman_exit(struct _GAME * game);
		双人游戏

	10.void choose_enter(struct _GAME * game);
	11.void choose_execute(struct _GAME * game);
	12.void choose_exit(struct _GAME * game);
		单人选择存档

	13.void single_enter(struct _GAME * game);
	14.void single_execute(struct _GAME * game);
	15.void single_exit(struct _GAME * game);
		单机存档内界面（单人游戏）

	16.void achieve_enter(struct _GAME * game);
	17.void achieve_execute(struct _GAME * game);
	18.void achieve_exit(struct _GAME * game);
		单机显示成就、解锁球员系统

	19.void shop_enter(struct _GAME * game);
	20.void shop_execute(struct _GAME * game);
	21.void shop_exit(struct _GAME * game);
		单机商店购买球员和道具

	22.void mything_enter(struct _GAME * game);
	23.void mything_execute(struct _GAME * game);
	24.void mything_exit(struct _GAME * game);
		单机仓库查看已购买、升级、道具等

	25.void myteamchoose_enter(struct _GAME * game);
	26.void myteamchoose_execute(struct _GAME * game);
	27.void myteamchoose_exit(struct _GAME * game);
		单机比赛开始选择自己队伍

	28.void menchoose_enter(struct _GAME * game);
	29.void menchoose_execute(struct _GAME * game);
	30.void menchoose_exit(struct _GAME * game);
		单机队伍里选人

	31.void levelchoose_enter(struct _GAME * game);
	32.void levelchoose_execute(struct _GAME * game);
	33.void levelchoose_exit(struct _GAME * game);
		单机选择对手难度

	34.void pause(struct _GAME * game);
		退出状态——全局



	35.void normal();
		每个界面右上角图标

	36.void readfile(struct _GAME * game);
		存档函数

	37.void writefile(struct _GAME * game);
		读档函数

	38.void cleanfile(struct _GAME * game, int x);
		询问是否清空存档

	39.void paintmoney(int x, int y, int size, unsigned char color, int num);
		画数字、数（主要是金钱），参数分别表示位置、大小、颜色、要画的数字

	40.void dou_paintyes(struct _GAME * game);
		双人模式选队伍时候，通过改变贴图√显示选择的队伍

	41.void shopphoto(struct _GAME * game);
		商店球员根据国家贴球员

	42.void shopjudge(struct _GAME * game, int membernum);
		商店球员根据是否购买选择贴价格或画数字，membernum用于判断球员编号

	43.void memberbuy(struct _GAME * game);
		商店球员购买

	44.void toolbuy(struct _GAME * game, int x, int y);
		商店道具购买

	45.void backpackphoto(struct _GAME * game);
		仓库球员根据国家贴球员

	46.void paintstars(struct _GAME * game, int membernum, int x, int y, int size);
		画星表示球员星数

	47.void backpackjudge(struct _GAME * game, int membernum);
		仓库球员根据是否满级贴图，membernum用于判断球员编号

	48.void memberup(struct _GAME * game);
		仓库球员升级

	49.void tooluse(struct _GAME * game);
		仓库道具使用

	50.void toolhaved(struct _GAME * game, int judge);
		商店和仓库里更新显示已有道具数量

	51.void toolinuse1(struct _GAME * game);
	55.void toolinuse2(struct _GAME * game);
		显示使用中的道具，因临时改bug问题分成了两个函数

	53.void sin_paintyes1(struct _GAME * game);
		单人模式选队伍贴图√问题

	54.void yourgamble(struct _GAME * game);
		赌资输入

	55.void photo_6men(struct _GAME * game);
		单人根据队伍选人贴图，除了默认四位白板队员之外，其余6位均要判断是否解锁

	56.void choose_4men(struct _GAME * game);
		球员10选4 画圈表示选择

	57.char *dealpath(struct _GAME * game, int x, char * finished);
		重复路径统一处理

	58.void sin_paintcircle(int x, int y);
		画○函数

	59.void sin_paintyes2(struct _GAME * game);
		单人游戏选对手队伍时候，通过改变贴图√显示选择的队伍

	60.void dealresult(struct _GAME * game);
		比赛结果处理（包括成就、金钱）

	61.void dealachieve(struct _GAME * game);
		成就系统更新解锁新球员


History：        对本文件的修改请及时按如下格式记录在下面（每个修改单独列出）！
    Example：    Date：
                Author：
                Version：
                Modification：

1.	Date：
	Author：
	Version：
	Modification：

**********************************************************/

#include"game.h"
#include<stdio.h>
#include<stdlib.h>
#include<bios.h>
#include"SVGAmode.h"
#include"mouse.h"
#include"tool.h"
#include"football.h"
#include"HZDIS.h"
#include"HANZI.h"


/*********************结构game内函数**********************/

/**********************************************************
Function：      Update

Description：   状态机循环的函数。用于执行状态内循环

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void Update(struct _GAME * self)
{
	self->MouseStateOld = self->MouseState;
	if( self->pcurrentstate!=NULL )
	{
		self->pcurrentstate->Execute(self);
	}

	if ( Mouse_press(978, 0, 1023, 48)==1 )
	{
		pause(self);       //全局状态，每一个界面都会判断是否点击退出按钮，
	}

	//更新鼠标状态标记

	if ( Mouse_press(0,0,1024,768)==1 )
	{
		self->MouseState = 1;
	}
	else
	{
		self->MouseState = 0;
	}

}


/**********************************************************
Function：      changestate

Description：  改变状态机状态的函数。用于改变状态机状态

Input：     结构game地址、要改变的状态的指针

Output：     None

Return：     None

Others：     None
**********************************************************/
void changestate(struct _GAME * self, _state * pnewstate)
{
	if( self->pcurrentstate->Exit!=NULL )
	{
		self->pcurrentstate->Exit(self);            //调用当前状态的退出方法
	}

	self->pcurrentstate = pnewstate;                       //改变状态到新状态
	if( self->pcurrentstate->Enter!=NULL )
	{
		self->pcurrentstate->Enter(self);             //调用新状态的进入方法
	}
}

/***********************初始化****************************/

/**********************************************************
Function：      Update

Description：   状态机循环及改变状态执行的函数。用于执行状态内循环以及改变状态

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//初始化game所有参数
void initgame(struct _GAME * game)
{
	int i;

	game->menu.Enter = menu_enter;
	game->menu.Execute = menu_execute;
	game->menu.Exit = NULL;

	game->doubleman.Enter = doubleman_enter;
	game->doubleman.Execute = doubleman_execute;
	game->doubleman.Exit = NULL;

	game->choose.Enter = choose_enter;
	game->choose.Execute = choose_execute;
	game->choose.Exit = NULL;

	game->single.Enter = single_enter;
	game->single.Execute = single_execute;
	game->single.Exit = NULL;

	game->achieve.Enter = achieve_enter;
	game->achieve.Execute = achieve_execute;
	game->achieve.Exit = NULL;

	game->shop.Enter = shop_enter;
	game->shop.Execute = shop_execute;
	game->shop.Exit = NULL;

	game->mything.Enter = mything_enter;
	game->mything.Execute = mything_execute;
	game->mything.Exit = NULL;

	game->myteamchoose.Enter = myteamchoose_enter;
	game->myteamchoose.Execute = myteamchoose_execute;
	game->myteamchoose.Exit = NULL;

	game->menchoose.Enter = menchoose_enter;
	game->menchoose.Execute = menchoose_execute;
	game->menchoose.Exit = NULL;

	game->levelchoose.Enter = levelchoose_enter;
	game->levelchoose.Execute = levelchoose_execute;
	game->levelchoose.Exit = NULL;

	game->pcurrentstate = &game->menu;

	game->MouseState = 0;
	game->MouseStateOld = 0;

	game->nowfile.filenumber = 0;
	game->nowfile.money = 0;
	for (i=0;i<6;i++)
	{
		game->nowfile.property[i]=0;
	}
	game->nowfile.numbers = 0;
	game->nowfile.winnum = 0;
	game->nowfile.losenum = 0;
	game->nowfile.winmargin = 0;
	strcpy(game->nowfile.stars, "0000000000000000000000000000000");
	for (i=0;i<5;i++)
	{
		game->nowfile.surprise[i]=0;
	}
	game->nowfile.inuse[0]=0;
	game->nowfile.inuse[1]=0;
	
	game->elsething.gamemode = 0;
	game->elsething.botlevel = 0;
	game->elsething.choose1 = 0;
	game->elsething.choose2 = 0;
	game->elsething.choose1before = 0;
	game->elsething.choose2before = 0;
	game->elsething.country = 0;
	game->elsething.shopstate = 0;
	game->elsething.backpackstate = 0;
	game->elsething.sin_teamchoose = 0;
	game->elsething.sin_teamchoosebefore = 0;
	game->elsething.sin_teamchoose2 = 0;
	game->elsething.sin_teamchoosebefore2 = 0;
	for (i=0;i<4;i++)
	{
		game->elsething.sin_menchoosebefore[i]=0;
		game->elsething.sin_menchoose[i]=0;
	}
	for (i=0;i<6;i++)
	{
		game->elsething.propertybuynum[i]=0;
		game->elsething.propertybuycost[i]=0;
	}
	game->elsething.botteam = 0;

	game->elsething.propertyonecost[0]=100;
	game->elsething.propertyonecost[1]=150;
	game->elsething.propertyonecost[2]=200;
	game->elsething.propertyonecost[3]=150;
	game->elsething.propertyonecost[4]=200;
	game->elsething.propertyonecost[5]=250;
}


/**************************各个状态的函数*****************************/

/**********************************************************
Function：     menu_enter、menu_execute、menu_exit

Description：   进入主菜单的函数、状态主菜单循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//主菜单界面
void menu_enter(struct _GAME * game)
{
	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	Bar(237, 147, 484, 266, 0x000000);
	Bar(240, 150, 481, 263, 0xFFFFFF);
	Bar(566, 144, 812, 266, 0x000000);
	Bar(569, 147, 809, 263, 0xFFFFFF);

	out_hz(270,180,"单人模式",1,48,48,0x000000);
	out_hz(590,180,"双人模式",1,48,48,0x000000);

	NewBK();
}

void menu_execute(struct _GAME * game)
{
	if ( Mouse_press(240,150,481,263)==1 )
	{
		changestate(game, &game->choose);
	}
	else if ( Mouse_press(569,147,809,263)==1 )
	{
		changestate(game, &game->doubleman);
	}
}

void menu_exit(struct _GAME * game)
{

}


/**********************************************************
Function：      doubleman_enter、doubleman_execute、doubleman_exit

Description：   进入双人游戏的函数、状态双人游戏循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//双人游戏选择界面
void doubleman_enter(struct _GAME * game)
{
	int i;
	for (i=0; i<4; i++)      //锁定双人球员等级
	{
		game->playerchoose.Wstars[i]=1;
		game->playerchoose.Bstars[i]=1;
	}
	game->playerchoose.Tool = 0;
	game->elsething.gamemode = 1;                               //确定游戏模式
	game->elsething.botlevel = 1;

	game->elsething.choose1 = 0;
	game->elsething.choose2 = 0;
	game->elsething.choose1before = 0;
	game->elsething.choose2before = 0;

	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	Putbmp256(182, 435, "bmp\\team.bmp");
	Putbmp256(94, 256, "bmp\\team.bmp");

	Bar(788-2, 637-2, 980+2, 717+2, 0x000000);
	Bar(788, 637, 980, 717, 0xFFFFFF);

	out_hz(100,100,"请双方各选择一支队伍进行比赛",1,48,48,0x000000);
	out_hz(870,270,"一号",1,48,48,0x000000);
	out_hz(870,320,"玩家",1,48,48,0x000000);
	out_hz(50,450,"二号",1,48,48,0x000000);
	out_hz(50,500,"玩家",1,48,48,0x000000);
	out_hz(790,650,"开始游戏",1,48,48,0x000000);
	
	NewBK();
}

void doubleman_execute(struct _GAME * game)
{

	if (Mouse_press(931,0,976,48)==1)
	{
		changestate(game, &game->menu);
	}

	//确定双方所选队伍
	if ( Mouse_press(114,271,230,390)==1 )
	{
		game->elsething.choose1 = 1;
		game->playerchoose.WplayerID[0] = 1;
		game->playerchoose.WplayerID[1] = 2;
		game->playerchoose.WplayerID[2] = 3;
		game->playerchoose.WplayerID[3] = 4;
	}
	else if ( Mouse_press(262,271,381,390)==1 )
	{
		game->elsething.choose1 = 2;
		game->playerchoose.WplayerID[0] = 7;
		game->playerchoose.WplayerID[1] = 8;
		game->playerchoose.WplayerID[2] = 9;
		game->playerchoose.WplayerID[3] = 10;
	}
	else if ( Mouse_press(412,271,531,390)==1 )
	{
		game->elsething.choose1 = 3;
		game->playerchoose.WplayerID[0] = 13;
		game->playerchoose.WplayerID[1] = 14;
		game->playerchoose.WplayerID[2] = 15;
		game->playerchoose.WplayerID[3] = 16;
	}
	else if ( Mouse_press(562,271,681,390)==1 )
	{
		game->elsething.choose1 = 4;
		game->playerchoose.WplayerID[0] = 19;
		game->playerchoose.WplayerID[1] = 20;
		game->playerchoose.WplayerID[2] = 21;
		game->playerchoose.WplayerID[3] = 22;
	}
	else if ( Mouse_press(713,271,832,390)==1 )
	{
		game->elsething.choose1 = 5;
		game->playerchoose.WplayerID[0] = 25;
		game->playerchoose.WplayerID[1] = 26;
		game->playerchoose.WplayerID[2] = 27;
		game->playerchoose.WplayerID[3] = 28;
	}


	if ( Mouse_press(199,450,318,569)==1 )
	{
		game->elsething.choose2 = 1;
		game->playerchoose.BplayerID[0] = 1;
		game->playerchoose.BplayerID[1] = 2;
		game->playerchoose.BplayerID[2] = 3;
		game->playerchoose.BplayerID[3] = 4;
	}
	else if ( Mouse_press(350,450,469,569)==1 )
	{
		game->elsething.choose2 = 2;
		game->playerchoose.BplayerID[0] = 7;
		game->playerchoose.BplayerID[1] = 8;
		game->playerchoose.BplayerID[2] = 9;
		game->playerchoose.BplayerID[3] = 10;
	}
	else if ( Mouse_press(500,450,619,569)==1 )
	{
		game->elsething.choose2 = 3;
		game->playerchoose.BplayerID[0] = 13;
		game->playerchoose.BplayerID[1] = 14;
		game->playerchoose.BplayerID[2] = 15;
		game->playerchoose.BplayerID[3] = 16;
	}
	else if ( Mouse_press(650,450,769,569)==1 )
	{
		game->elsething.choose2 = 4;
		game->playerchoose.BplayerID[0] = 19;
		game->playerchoose.BplayerID[1] = 20;
		game->playerchoose.BplayerID[2] = 21;
		game->playerchoose.BplayerID[3] = 22;
	}
	else if ( Mouse_press(801,450,920,569)==1 )
	{
		game->elsething.choose2 = 5;
		game->playerchoose.BplayerID[0] = 25;
		game->playerchoose.BplayerID[1] = 26;
		game->playerchoose.BplayerID[2] = 27;
		game->playerchoose.BplayerID[3] = 28;
	}

	dou_paintyes(game);

	if ( game->elsething.choose1!=0 && game->elsething.choose2!=0)      //只有当双方均已选择才能开始游戏
	{
		if ( Mouse_press(788,637,980,717)==1 )
		{
			FootballGame(game->playerchoose, game->elsething.gamemode, game->elsething.botlevel);
			game->pcurrentstate->Enter(game);
		}
	}
}

void doubleman_exit(struct _GAME * game)
{

}


/**********************************************************
Function：      choose_enter、choose_execute、choose_exit

Description：   进入存档选择的函数、状态存档选择循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     单人游戏第一个界面为存档选择，所以命名为choose
**********************************************************/
//单机选择存档界面
void choose_enter(struct _GAME * game)
{
	game->elsething.gamemode = 0;

	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	Bar(83, 153, 415, 241, 0x000000);
	Bar(85, 155, 413, 239, 0xFFFFFF);
	Bar(319, 178, 384, 219, 0x000000);
	Bar(321, 180, 382, 217, 0xFFFFFF);

	Bar(83, 327, 415, 415, 0x000000);
	Bar(85, 329, 413, 413, 0xFFFFFF);
	Bar(319, 353, 384, 394, 0x000000);
	Bar(321, 355, 382, 392, 0xFFFFFF);

	Bar(83, 496, 415, 584, 0x000000);
	Bar(85, 498, 413, 582, 0xFFFFFF);
	Bar(319, 521, 384, 562, 0x000000);
	Bar(321, 523, 382, 560, 0xFFFFFF);

	out_hz(115,165,"存档一",1,48,48,0x000000);
	out_hz(115,345,"存档二",1,48,48,0x000000);
	out_hz(115,515,"存档三",1,48,48,0x000000);
	out_hz(325,185,"清空",1,24,24,0x000000);
	out_hz(325,360,"清空",1,24,24,0x000000);
	out_hz(325,530,"清空",1,24,24,0x000000);

	NewBK();
}

void choose_execute(struct _GAME * game)
{
	//关于存档的选择以及清空处理
	if (Mouse_press(83,153,415,241)==1 && Mouse_press(319,178,384,219)!=1)                                     //读存档一
	{
		game->nowfile.filenumber = 1;
		readfile(game);
		changestate(game, &game->single);
	}
	else if (Mouse_press(319,178,384,219)==1)
	{
		cleanfile(game, 1);
	}

	if (Mouse_press(83,327,415,415)==1 && Mouse_press(319,353,384,394)!=1)                                //读存档二
	{
		game->nowfile.filenumber = 2;
		readfile(game);
		changestate(game, &game->single);
	}
	else if (Mouse_press(319,353,384,394)==1)
	{
		cleanfile(game, 2);
	}

	if (Mouse_press(83,496,415,584)==1 && Mouse_press(319,521,384,562)!=1)                                //读存档三
	{
		game->nowfile.filenumber = 3;
		readfile(game);
		changestate(game, &game->single);
	}
	else if (Mouse_press(319,521,384,562)==1)
	{
		cleanfile(game, 3);
	}
	
	if (Mouse_press(931,0,976,48)==1)                            //返回上一个界面
	{
		changestate(game, &game->menu);
	}
}

void choose_exit(struct _GAME * game)
{

}



/**********************************************************
Function：      single_enter、single_execute、single_exit

Description：   正式进入单人游戏的函数、状态单人游戏循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//单机存档内界面
void single_enter(struct _GAME * game)
{
	char rule[221];
	FILE *fp;
	int x;
	x = game->nowfile.money;
	dealachieve(game);

	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	Bar(74,125,373,245, 0x000000);
	Bar(74,325,373,445, 0x000000);
	Bar(74,524,373,644, 0x000000);
	Bar(682,530,938,668, 0x000000);

	Bar(76,127,371,243, 0xFFFFFF);
	Bar(76,327,371,443, 0xFFFFFF);
	Bar(76,526,371,642, 0xFFFFFF);
	Bar(684,532,936,666, 0xFFFFFF);

	Putbmp256(80, 0, "bmp\\money.bmp");

	out_hz(140,160,"仓库",1,48,48,0x000000);
	out_hz(140,360,"商店",1,48,48,0x000000);
	out_hz(140,560,"成就",1,48,48,0x000000);
	out_hz(705,570,"开始游戏",1,48,48,0x000000);

	paintmoney(180, 20, 30, 0x000000, x);

	fp = fopen("rule.txt", "r");
	if ( fp != NULL )
	{
		fgets (rule,221,fp);
	}
	fclose(fp);
	mix(600,200,rule,0x000000,600,800);

	//判断球队中是否出现了新球员
	if ( game->nowfile.surprise[0] || game->nowfile.surprise[1] || game->nowfile.surprise[2] || game->nowfile.surprise[3] || game->nowfile.surprise[4])
	{
		Putbmp256(344, 102, "bmp\\thereis.bmp");
	}

	NewBK();
}

void single_execute(struct _GAME * game)
{
	//转到不同的功能界面
	if (Mouse_press(76,127,371,243)==1)
	{
		changestate(game, &game->mything);
	}
	else if (Mouse_press(76,327,371,443)==1)
	{
		changestate(game, &game->shop);
	}
	else if (Mouse_press(76,526,371,642)==1)
	{
		changestate(game, &game->achieve);
	}
	else if (Mouse_press(684,532,936,666)==1)
	{
		changestate(game, &game->myteamchoose);
	}
	else if (Mouse_press(931,0,976,48)==1)
	{
		changestate(game, &game->choose);
		writefile(game);
	}
}

void single_exit(struct _GAME * game)
{

}



/**********************************************************
Function：      shop_enter、shop_execute、shop_exit

Description：   进入商店的函数、状态商店循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//单机商店
void shop_enter(struct _GAME * game)
{
	int x;

	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	Bar(69,101,244,177, 0x000000);
	Bar(72,104,241,174, 0xFFFFFF);
	Bar(290,101,465,177, 0x000000);
	Bar(293,104,462,174, 0xFFFFFF);
	Putbmp256(49, 199, "bmp\\shopman.bmp");

	out_hz(300,20,"商店",1,48,48,0x000000);
	out_hz(100,115,"球员",1,48,48,0x000000);
	out_hz(320,115,"道具",1,48,48,0x000000);
	out_hz(515,140,"你拥有的金钱数：",1,48,48,0x000000);
	//out_hz(100,700,"附：球员详细资料请到仓库中查看，球员升级也在仓库中进行",1,24,24,0x000000);

	game->elsething.country = 1;
	game->elsething.shopstate = 1;
	shopphoto(game);
	game->elsething.propertyonecost[0]=100;
	game->elsething.propertyonecost[1]=150;
	game->elsething.propertyonecost[2]=200;
	game->elsething.propertyonecost[3]=150;
	game->elsething.propertyonecost[4]=200;
	game->elsething.propertyonecost[5]=250;

	x = game->nowfile.money;
	paintmoney(880, 146, 20, 0x000000, x);

	NewBK();
}

void shop_execute(struct _GAME * game)
{
	int i;
	int all;
	while(bioskey(1))
	getch();
	if (Mouse_press(931,0,976,48)==1)
	{
		changestate(game, &game->single);                                //返回到存档界面
	}

	//商店道具和球员界面选择贴图
	if ( Mouse_press(71,103,242,175)==1 )
	{
		game->elsething.shopstate = 1;
		game->elsething.country = 1;
		Putbmp256(49, 199, "bmp\\shopman.bmp");
		//out_hz(100,700,"附：球员详细资料请到仓库中查看，球员升级也在仓库中进行",1,24,24,0x000000);
		shopphoto(game);
	}
	else if ( Mouse_press(292,103,463,175)==1 )
	{
		game->elsething.shopstate = 2;
		game->elsething.property = 0;
		shopphoto(game);
		toolhaved(game,1);
	}

	if ( game->elsething.shopstate == 1 )
	{
		//商店选看各个国家的球员
		if (Mouse_press(82,220,231,319)==1)
		{
			game->elsething.country = 1;
			shopphoto(game);
		}
		else if (Mouse_press(260,220,409,319)==1)
		{
			game->elsething.country = 2;
			shopphoto(game);
		}
		else if (Mouse_press(438,220,587,319)==1)
		{
			game->elsething.country = 3;
			shopphoto(game);
		}
		else if (Mouse_press(617,220,766,319)==1)
		{
			game->elsething.country = 4;
			shopphoto(game);
		}
		else if (Mouse_press(796,220,945,319)==1)
		{
			game->elsething.country = 5;
			shopphoto(game);
		}
		//商店购买球员
		memberbuy(game);
	}
	else if ( game->elsething.shopstate == 2 )
	{
		//商店购买道具
		if (Mouse_press(296,358,435,468)==1)
		{
			game->elsething.property = 1;
			toolbuy(game,296,358);
		}
		else if (Mouse_press(514,358,653,468)==1)
		{
			game->elsething.property = 2;
			toolbuy(game,514,358);
		}
		else if (Mouse_press(720,358,859,468)==1)
		{
			game->elsething.property = 3;
			toolbuy(game,720,358);
		}
		else if (Mouse_press(296,605,435,715)==1)
		{
			game->elsething.property = 4;
			toolbuy(game,296,605);
		}
		else if (Mouse_press(514,605,653,715)==1)
		{
			game->elsething.property = 5;
			toolbuy(game,514,605);
		}
		else if (Mouse_press(720,605,859,715)==1)
		{
			game->elsething.property = 6; 
			toolbuy(game,720,605);
		}
		
		//一键购买处理
		if (Mouse_press(84,625,221,665)==1)
		{
			for (i=0,all=0;i<6;i++)
			{
				all += game->elsething.propertybuycost[i];
			}
			if ( all<=game->nowfile.money )
			{
				for (i=0;i<6;i++)
				{
					game->nowfile.property[i] += game->elsething.propertybuynum[i];
					game->nowfile.money -= game->elsething.propertybuycost[i];
					game->elsething.propertybuynum[i]=0;
					game->elsething.propertybuycost[i]=0;
				}
				shopphoto(game);
				Bar(867,145,1023,196, 0xFFFFFF);
				paintmoney(880, 146, 20, 0x000000, game->nowfile.money);
				NewBK;
			}
			else 
			{
				out_hz(50,700,"钱不够哦！",1,48,48,0x000000);
				delay(800);
				out_hz(50,700,"钱不够哦！",1,48,48,0xFFFFFF);
			}
		}
	}

}

void shop_exit(struct _GAME * game)
{

}


/**********************************************************
Function：      mything_enter、mything_execute、mything_exit

Description：   进入仓库的函数、状态仓库循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//单机查看已购买、升级、道具等（仓库背包）
void mything_enter(struct _GAME * game)
{
	int x;

	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	Bar(69,101,244,177, 0x000000);
	Bar(72,104,241,174, 0xFFFFFF);
	Bar(290,101,465,177, 0x000000);
	Bar(293,104,462,174, 0xFFFFFF);
	Putbmp256(49, 199, "bmp\\bagman.bmp");

	out_hz(300,20,"仓库",1,48,48,0x000000);
	out_hz(100,115,"球员",1,48,48,0x000000);
	out_hz(320,115,"道具",1,48,48,0x000000);
	out_hz(495,140,"你拥有的金钱数：",1,48,48,0x000000);
	out_hz(100,700,"附：球员详细资料点击球员头像查看，箭头表示可升级",1,24,24,0x000000);

	game->elsething.country = 1;
	game->elsething.backpackstate = 1;
	backpackphoto(game);

	x = game->nowfile.money;
	paintmoney(860, 146, 20, 0x000000, x);

	NewBK();
}

void mything_execute(struct _GAME * game)
{
	if (Mouse_press(931,0,976,48)==1)
	{
		changestate(game, &game->single);                                //返回到存档界面
	}

	//仓库球员和道具界面选择贴图
	if ( Mouse_press(71,103,242,175)==1 )
	{
		game->elsething.backpackstate = 1;
		Putbmp256(49, 199, "bmp\\bagman.bmp");
		out_hz(100,700,"附：球员详细资料点击球员头像查看，箭头表示可升级",1,24,24,0x000000);
		backpackphoto(game);
	}
	else if ( Mouse_press(292,103,463,175)==1 )
	{
		game->elsething.backpackstate = 2;
		backpackphoto(game);
	}


	if ( game->elsething.backpackstate == 1 )
	{
		//仓库选看各个国家的球员
		if (Mouse_press(82,220,231,319)==1)
		{
			game->elsething.country = 1;
			backpackphoto(game);
		}
		else if (Mouse_press(260,220,409,319)==1)
		{
			game->elsething.country = 2;
			backpackphoto(game);
		}
		else if (Mouse_press(438,220,587,319)==1)
		{
			game->elsething.country = 3;
			backpackphoto(game);
		}
		else if (Mouse_press(617,220,766,319)==1)
		{
			game->elsething.country = 4;
			backpackphoto(game);
		}
		else if (Mouse_press(796,220,945,319)==1)
		{
			game->elsething.country = 5;
			backpackphoto(game);
		}
		//仓库升级球员
		memberup(game);
		//仓库查看球员
		checkmember(game);
	}
	else if ( game->elsething.backpackstate == 2 )
	{
		//仓库使用道具
		tooluse(game);
	}
}

void mything_exit(struct _GAME * game)
{

}




/**********************************************************
Function：      achieve_enter、achieve_execute、achieve_exit

Description：   进入单人成就的函数、状态单人成就循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//单机展示成就系统
void achieve_enter(struct _GAME * game)
{
	int x;

	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	out_hz(285,30,"已完成场数：",1,48,48,0x000000);
	out_hz(285,100,"胜场数：",1,48,48,0x000000);
	out_hz(285,170,"败场数：",1,48,48,0x000000);
	out_hz(285,240,"净胜球数：",1,48,48,0x000000);
	out_hz(100,740,"附：当某项成就达到一定值时候，你的仓库中会解锁相应的秘密球员", 1,24,24,0x000000);

	x = game->nowfile.numbers;
	paintmoney(560, 30, 20, BLACK, x);
	x = game->nowfile.winnum;
	paintmoney(470, 100, 20, BLACK, x);
	x = game->nowfile.losenum;
	paintmoney(470, 170, 20, BLACK, x);
	x = game->nowfile.winmargin;
	paintmoney(515, 240, 20, BLACK, x);

	NewBK();
}
void achieve_execute(struct _GAME * game)
{
	if (Mouse_press(931,0,976,48)==1)
	{
		changestate(game, &game->single);
	}
}

void achieve_exit(struct _GAME * game)
{

}





/**********************************************************
Function：      myteamchoose_enter、myteamchoose_execute、myteamchoose_exit

Description：   进入队伍选择的函数、状态队伍选择循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//单机选择自己队伍
void myteamchoose_enter(struct _GAME * game)
{
	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	out_hz(190,220,"请选择您的队伍：",1,48,48,0x000000);
	Putbmp256(150, 335, "bmp\\team.bmp");
	Bar(719-2,596-2,948+2,672+2,0x000000);
	Bar(719,596,948,672,0xFFFFFF);
	out_hz(780,610,"继续",1,48,48,0x000000);
	out_hz(50,600,"请输入赌资：",1,48,48,0x000000);
	Line(350,650,500,650, 0x000000);
	game->elsething.gamble = 0;

	game->elsething.gamemode = 0;                                              //确定游戏模式
	game->elsething.botlevel = 0;                                              //初始化难度

	game->elsething.sin_teamchoose = 0;
	game->elsething.sin_teamchoosebefore = 0;

	NewBK();
}
void myteamchoose_execute(struct _GAME * game)
{

	if (Mouse_press(931,0,976,48)==1)
	{
		changestate(game, &game->single);
	}

	//单人游戏鼠标点击选择队伍
	if ( Mouse_press(167,350,286,469)==1 )
	{
		game->elsething.sin_teamchoose = 1;
	}
	else if ( Mouse_press(318,350,437,469)==1 )
	{
		game->elsething.sin_teamchoose = 2;
	}
	else if ( Mouse_press(468,350,587,469)==1 )
	{
		game->elsething.sin_teamchoose = 3;
	}
	else if ( Mouse_press(618,350,737,469)==1 )
	{
		game->elsething.sin_teamchoose = 4;
	}
	else if ( Mouse_press(769,350,888,469)==1 )
	{
		game->elsething.sin_teamchoose = 5;
	}

	sin_paintyes1(game);

	//赌资输入
	if ( Mouse_press(350,600,500,650)==1 )
	{
		yourgamble(game);
	}

	if ( game->elsething.sin_teamchoose != 0 )
	{
		if ( Mouse_press(719,596,948,672)==1 )
		{
			if(game->nowfile.money >= game->elsething.gamble)
			{
				changestate(game, &game->menchoose);
			}
			else
			{
				out_hz(100,700,"钱不够哦",1,48,48,0x000000);
				delay(500);
				out_hz(100,700,"钱不够哦",1,48,48,0xFFFFFF);
			}
		}
	}
}

void myteamchoose_exit(struct _GAME * game)
{

}




/**********************************************************
Function：      menchoose_enter、menchoose_execute、menchoose_exit

Description：   进入选择球员的函数、状态选择球员循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//单机队伍里选人
void menchoose_enter(struct _GAME * game)
{
	Bar(0,0,1024,768,0xFFFFFF);
	normal();

	Putbmp256(320, 49, "bmp\\blank.bmp");
	Putbmp256(478, 49, "bmp\\blank.bmp");
	Putbmp256(635, 49, "bmp\\blank.bmp");
	Putbmp256(791, 49, "bmp\\blank.bmp");
	Putbmp256(398, 405, "bmp\\hhhhh.bmp");
	photo_6men(game);
	out_hz(75,150,"请选择",1,48,48,0x000000);
	out_hz(75,220,"球员：",1,48,48,0x000000);
	Bar(704-2,624-2,982+2,715+2, 0x000000);
	Bar(704,624,982,715, 0xFFFFFF);
	out_hz(790,650,"继续",1,48,48,0x000000);

	game->elsething.sin_menchoose[0]=0;
	game->elsething.sin_menchoose[1]=0;
	game->elsething.sin_menchoose[2]=0;
	game->elsething.sin_menchoose[3]=0;
	game->elsething.sin_menchoosebefore[0]=0;
	game->elsething.sin_menchoosebefore[1]=0;
	game->elsething.sin_menchoosebefore[2]=0;
	game->elsething.sin_menchoosebefore[3]=0;

	sin_paintcircle(320, 49);
	sin_paintcircle(478, 49);
	sin_paintcircle(635, 49);
	sin_paintcircle(791, 49);

	NewBK();
}

void menchoose_execute(struct _GAME * game)
{
	int i;
	int wid;
	int wstar;
	char lv;

	int num;
	num = 6*(game->elsething.sin_teamchoose-1);

	if (Mouse_press(931,0,976,48)==1)
	{
		changestate(game, &game->myteamchoose);
	}
	
	//记录所选上场队员
	if ( Mouse_press(320,49,439,198)==1 )
	{
		game->elsething.sin_menchoose[0] = 0;
	}
	else if ( Mouse_press(478,49,597,198)==1 )
	{
		game->elsething.sin_menchoose[1] = 0;
	}
	else if ( Mouse_press(635,49,754,198)==1 )
	{
		game->elsething.sin_menchoose[2] = 0;
	}
	else if ( Mouse_press(791,49,910,198)==1 )
	{
		game->elsething.sin_menchoose[3] = 0;
	}
	else if ( Mouse_press(320,230,439,379)==1 && game->nowfile.stars[num+1]!='0')
	{ 
		game->elsething.sin_menchoose[0]=num+1;
	}
	else if ( Mouse_press(478,230,597,379)==1 && game->nowfile.stars[num+2]!='0')
	{
		game->elsething.sin_menchoose[1]=num+2;
	}
	else if ( Mouse_press(635,230,754,379)==1 && game->nowfile.stars[num+3]!='0')
	{
		game->elsething.sin_menchoose[2]=num+3;
	}
	else if ( Mouse_press(791,230,910,379)==1 && game->nowfile.stars[num+4]!='0')
	{
		game->elsething.sin_menchoose[3]=num+4;
	}
	else if ( Mouse_press(635,405,754,554)==1 && game->nowfile.stars[num+5]!='0')
	{
		game->elsething.sin_menchoose[2]=num+5;
	}
	else if ( Mouse_press(791,405,910,554)==1 && game->nowfile.stars[num+6]!='0')
	{
		game->elsething.sin_menchoose[3]=num+6;
	}

	choose_4men(game);

	//进入下一个界面前所做工作
	if ( Mouse_press(704,624,982,715)==1 )
	{
		game->playerchoose.WplayerID[0] = game->elsething.sin_menchoose[0];
		game->playerchoose.WplayerID[1] = game->elsething.sin_menchoose[1];
		game->playerchoose.WplayerID[2] = game->elsething.sin_menchoose[2];
		game->playerchoose.WplayerID[3] = game->elsething.sin_menchoose[3];

		for (i=0; i<4; i++)
		{
			wid = game->playerchoose.WplayerID[i];
			lv = game->nowfile.stars[wid];
			wstar = lv - '0';			
			game->playerchoose.Wstars[i] = wstar;
		}
		changestate(game, &game->levelchoose);
	}
}

void menchoose_exit(struct _GAME * game)
{

}


/**********************************************************
Function：      levelchoose_enter、levelchoose_execute、levelchoose_exit

Description：   进入对手难度选择的函数、状态难度选择循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//单机对手选择难度
void levelchoose_enter(struct _GAME * game)
{
	Bar(0,0,1024,768,0xFFFFFF);
	Putbmp256(0, 0, "bmp\\botlevel.bmp");
	normal();

	out_hz(530,500,"简单（赔率一）",1,48,48,0x000000);
	out_hz(260,245,"中等（赔率二）",1,48,48,0x000000);
	out_hz(530,20,"困难（赔率三）",1,48,48,0x000000);
	out_hz(50,90,"请选择你",1,48,48,0x000000);
	out_hz(50,140,"的对手：",1,48,48,0x000000);

	game->elsething.gamemode = 0;
	game->elsething.botlevel = 3;
	game->elsething.sin_teamchoosebefore2=0;
	game->elsething.sin_teamchoose2=0;
	game->elsething.botteam = 0;

	NewBK();
}

void levelchoose_execute(struct _GAME * game)
{
	int i;
	int bid;
	int bstar;
	char lv;

	FILE * fp;
	
	if (Mouse_press(931,0,976,48)==1)
	{
		changestate(game, &game->menchoose);
	}

	//所选对手国家
	if ( Mouse_press(340,97,459,216)==1 || Mouse_press(61,325,180,444)==1 || Mouse_press(317,580,436,699)==1 )
	{
		game->elsething.botteam = 1;
		game->playerchoose.BplayerID[0] = 1;
		game->playerchoose.BplayerID[1] = 2;
		game->playerchoose.BplayerID[2] = 3;
		game->playerchoose.BplayerID[3] = 4;
	}
	else if ( Mouse_press(350,97,594,216)==1 || Mouse_press(196,325,315,444)==1 || Mouse_press(452,580,571,699)==1 )
	{
		game->elsething.botteam = 2;
		game->playerchoose.BplayerID[0] = 7;
		game->playerchoose.BplayerID[1] = 8;
		game->playerchoose.BplayerID[2] = 9;
		game->playerchoose.BplayerID[3] = 10;
	}
	else if ( Mouse_press(500,97,730,216)==1 || Mouse_press(332,325,451,444)==1 || Mouse_press(588,580,707,699)==1 )
	{
		game->elsething.botteam = 3;
		game->playerchoose.BplayerID[0] = 13;
		game->playerchoose.BplayerID[1] = 14;
		game->playerchoose.BplayerID[2] = 15;
		game->playerchoose.BplayerID[3] = 16;
	}
	else if ( Mouse_press(650,97,867,216)==1 || Mouse_press(469,325,588,444)==1 || Mouse_press(725,580,844,699)==1 )
	{
		game->elsething.botteam = 4;
		game->playerchoose.BplayerID[0] = 19;
		game->playerchoose.BplayerID[1] = 20;
		game->playerchoose.BplayerID[2] = 21;
		game->playerchoose.BplayerID[3] = 22;
	}
	else if ( Mouse_press(801,97,1007,216)==1 || Mouse_press(609,325,728,444)==1 || Mouse_press(865,580,984,699)==1 )
	{
		game->elsething.botteam = 5;
		game->playerchoose.BplayerID[0] = 25;
		game->playerchoose.BplayerID[1] = 26;
		game->playerchoose.BplayerID[2] = 27;
		game->playerchoose.BplayerID[3] = 28;
	}

	//所选电脑难度
	if ( Mouse_press(340,97,459,216)==1 || Mouse_press(350,97,594,216)==1 || Mouse_press(500,97,730,216)==1 || Mouse_press(650,97,867,216)==1 || Mouse_press(801,97,1007,216)==1)
	{
		game->elsething.botlevel = 2;
	}
	else if ( Mouse_press(61,325,180,444)==1 || Mouse_press(196,325,315,444)==1 || Mouse_press(332,325,451,444)==1 || Mouse_press(469,325,588,444)==1 || Mouse_press(609,325,728,444)==1 )
	{
		game->elsething.botlevel = 1;
	}
	else if ( Mouse_press(317,580,436,699)==1 || Mouse_press(452,580,571,699)==1 || Mouse_press(588,580,707,699)==1 || Mouse_press(725,580,844,699)==1 || Mouse_press(865,580,984,699)==1 )
	{
		game->elsething.botlevel = 0;
	}

	game->elsething.sin_teamchoose2 = 5*(2-game->elsething.botlevel) + game->elsething.botteam;

	sin_paintyes2(game);

	if ( Mouse_press(783,348,985,429)==1 && game->elsething.botteam!=0 && game->elsething.botlevel!=4 )
	{
		for (i=0; i<4; i++)
		{
			game->playerchoose.Bstars[i]=1;
		}
		game->playerchoose.Tool = game->nowfile.inuse[0];
		game->result = FootballGame(game->playerchoose, game->elsething.gamemode, game->elsething.botlevel);
		dealresult(game);
		dealachieve(game);
		changestate(game, &game->single);
	}
}

void levelchoose_exit(struct _GAME * game)
{

}



/**********************************************************
Function：      pause

Description：   全局状态pause，检测是否按下右上角退出按键并给出相应的按键功能，执行操作

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//退出状态________全局
void pause(struct _GAME * game)
{
	Putbmp256(309, 231, "bmp\\pause.bmp");

	while (1)
	{
		Newxy();
		if ( Mouse_press(388, 254, 633, 319) ==1 )         //返回
		{
			game->pcurrentstate->Enter(game);
			NewBK();
			break;
		}
		else if ( Mouse_press(388, 346, 633, 411) ==1 )    //主菜单
		{
			writefile(game);
			changestate(game, &game->menu);
			break;
		}
		else if ( Mouse_press(388, 436, 633, 501) ==1 )    //保存退出
		{
			writefile(game);
			exit(1);
		}

	delay(5);
	}
}





































/**********************************************************
Function：      normal

Description：   刷新界面时重画界面小组件

Input：      None

Output：     None

Return：     None

Others：     None
**********************************************************/
//每个界面右上角图标
void normal()
{
	int i;

	Line(931, 0, 1023, 0, 0x000000);
	Line(931, 1, 1023, 1, 0x000000);
	Line(931, 47, 1023, 47, 0x000000);
	Line(931, 48, 1023, 48, 0x000000);

	Line(931, 0, 931, 48, 0x000000);
	Line(932, 0, 932, 48, 0x000000);
	Line(1022, 0, 1022, 48, 0x000000);
	Line(1023, 0, 1023, 48, 0x000000);
	Line(975, 0, 975, 48, 0x000000);
	Line(976, 0, 976, 48, 0x000000);
	Line(978, 0, 978, 48, 0x000000);
	Line(979, 0, 979, 48, 0x000000);

	for (i=0;i<6;i++)                     //画×
	{
		Line(982+i, 11-i, 1013+i, 43-i, 0x000000);
		Line(982+i, 38+i, 1013+i, 6+i, 0x000000);
		Line(982+i+1, 11-i, 1013+i+1, 43-i, 0x000000);
		Line(982+i+1, 38+i, 1013+i+1, 6+i, 0x000000);
	}

	for (i=0;i<10;i++)                    //画箭头
	{
		Line(940, 20+i, 975, 20+i, 0x000000);
	}
	for (i=0;i<12;i++)
	{
		Line(932+i, 24, 954+i, 2, 0x000000);
		Line(932+i, 24, 954+i, 46, 0x000000);
	}
}



/**********************************************************
Function：      readfile、writefile

Description：   读档、存档函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
//读档
void readfile( struct _GAME * game )
{

	FILE *fp;

	if ( game->nowfile.filenumber==0 )
	{
		fp = fopen("file\\file0.txt", "r");
	}
	else if ( game->nowfile.filenumber==1 )
	{
		fp = fopen("file\\file1.txt", "r");
	}
	else if ( game->nowfile.filenumber==2 )
	{
		fp = fopen("file\\file2.txt", "r");
	}
	else if ( game->nowfile.filenumber==3 )
	{
		fp = fopen("file\\file3.txt", "r");
	}

	if ( fp != NULL )
	{
		fscanf (fp, "%d\n", &game->nowfile.filenumber);

		fscanf (fp, "%d\n", &game->nowfile.money);
		fscanf (fp, "%d %d %d %d %d %d\n", &game->nowfile.property[0], &game->nowfile.property[1], &game->nowfile.property[2], &game->nowfile.property[3], &game->nowfile.property[4], &game->nowfile.property[5]);
		fscanf (fp, "%d\n", &game->nowfile.numbers);
		fscanf (fp, "%d\n", &game->nowfile.winnum);
		fscanf (fp, "%d\n", &game->nowfile.losenum);
		fscanf (fp, "%d\n", &game->nowfile.winmargin);
		fscanf (fp, "%s\n", &game->nowfile.stars);
		fscanf (fp, "%d %d %d %d %d\n", &game->nowfile.surprise[0], &game->nowfile.surprise[2], &game->nowfile.surprise[2], &game->nowfile.surprise[3], &game->nowfile.surprise[4]);
		fscanf (fp, "%d %d\n", &game->nowfile.inuse[0], &game->nowfile.inuse[1]);
	}
	fclose(fp);
}

//存档
void writefile( struct _GAME * game )
{

	FILE *fp;

	if ( game->nowfile.filenumber == 1 )
	{
		fp = fopen("file\\file1.txt", "w");
	}
	else if ( game->nowfile.filenumber == 2 )
	{
		fp = fopen("file\\file2.txt", "w");
	}
	else if ( game->nowfile.filenumber == 3 )
	{
		fp = fopen("file\\file3.txt", "w");
	}

	if ( fp != NULL )
	{

		rewind(fp);
		fprintf (fp, "%d\n", game->nowfile.filenumber);

		fprintf (fp, "%d\n", game->nowfile.money);
		fprintf (fp, "%d %d %d %d %d %d\n", game->nowfile.property[0], game->nowfile.property[1], game->nowfile.property[2], game->nowfile.property[3], game->nowfile.property[4], game->nowfile.property[5]);
		fprintf (fp, "%d\n", game->nowfile.numbers);
		fprintf (fp, "%d\n", game->nowfile.winnum);
		fprintf (fp, "%d\n", game->nowfile.losenum);
		fprintf (fp, "%d\n", game->nowfile.winmargin);
		fprintf (fp, "%s\n", game->nowfile.stars);
		fprintf (fp, "%d %d %d %d %d\n", game->nowfile.surprise[0], game->nowfile.surprise[2], game->nowfile.surprise[2], game->nowfile.surprise[3], game->nowfile.surprise[4]);
		fprintf (fp, "%d %d\n", game->nowfile.inuse[0], game->nowfile.inuse[1]);
	}
	else
	{
		exit(0);
	}
	fclose(fp);
}


/**********************************************************
Function：      cleanfile

Description：   询问玩家是否清空存档

Input：
		结构game
		int x             所要清空存档的编号

Output：     None

Return：     None

Others：     None
**********************************************************/
//询问是否清空存档
void cleanfile(struct _GAME * game, int x)
{
	out_hz(50,650,"你确定要清空此存档吗？",1,48,48,0x000000);
	game->nowfile.filenumber = 0;
	readfile(game);
	Bar(610-2, 650-2, 759+2, 709+2, 0x000000);
	Bar(610, 650, 759, 709, 0xFFFFFF);
	Bar(800-2, 650-2, 949+2, 709+2, 0x000000);
	Bar(800, 650, 949, 709, 0xFFFFFF);
	out_hz(630,655,"确定",1,48,48,0x000000);
	out_hz(820,655,"取消",1,48,48,0x000000);
	while(1)
	{
		Newxy();
		if(Mouse_press(610, 650, 759, 709)==1)
		{
			game->nowfile.filenumber = x;
			writefile(game);
			break;
		}
		else if(Mouse_press(800, 650, 949, 709)==1)
		{
			break;
		}
	}
	Bar(0, 600, 1023, 767, 0xFFFFFF);
	NewBK();
}




/**********************************************************
Function：      paintmoney

Description：    画数字

Input：
			int x、int y               数的位置
			int size                   数字字号大小
			unsigned char color        颜色
			int num                    数的大小

Output：     None

Return：     None

Others：     一开始只用于花金钱，故命名为paintmoney
**********************************************************/
//画数字（主要是金钱）
void paintmoney(int x, int y, int size, unsigned char _color, int num)
{
	FILE *fp;
	int everynum[6];                //最多不超过的位数
	int a=0;                        //数字编号
	int i, j;                       //数字所处位数
	int ten;

	everynum[5]=0;
	for (i=0; i<5; i++)
	{
		j=0;
		ten=1;
		while(j<i)
		{
			ten*=10;
			j++;
		}
		everynum[4-i] = ((num/ten))%10;
	}

	for (i=0; i<5; i++)
	{
		if ( everynum[i]==0 && a==0 )
		{
			if ( everynum[i+1]!=0 )
			{
				a++;
			}
		}
		else
		{
			DrawNum(x, y, size, _color, everynum[i]);
			x += 6*size/5;
			a++;
		}
	}

	if ( a==0 )
	{
		DrawNum(x, y, size, _color, 0);
	}
}





/**********************************************************
Function：      dou_paintyes

Description：   双人游戏选择队伍时候，根据玩家选择改变贴图

Input：     结构game地址

Output：     None

Return：     None

Others：     记录参数均包含在结构game中
**********************************************************/
//双人模式选队伍贴图√问题
void dou_paintyes( struct _GAME * game )
{
	if ( game->elsething.choose1before != game->elsething.choose1 )
	{
		if ( game->elsething.choose1before == 1)
		{
			Putbmp256(111, 271, "bmp\\agt.bmp");
		}
		else if ( game->elsething.choose1before == 2)
		{
			Putbmp256(262, 271, "bmp\\bx.bmp");
		}
		else if ( game->elsething.choose1before == 3)
		{
			Putbmp256(412, 271, "bmp\\dg.bmp");
		}
		else if ( game->elsething.choose1before == 4)
		{
			Putbmp256(562, 271, "bmp\\hl.bmp");
		}
		else if ( game->elsething.choose1before == 5)
		{
			Putbmp256(713, 271, "bmp\\xby.bmp");
		}

		game->elsething.choose1before = game->elsething.choose1;

		if ( game->elsething.choose1 == 1 )
		{
			Putbmp256(111, 271, "bmp\\agtYes.bmp");
		}
		else if ( game->elsething.choose1 == 2)
		{
			Putbmp256(262, 271, "bmp\\bxYes.bmp");
		}
		else if ( game->elsething.choose1 == 3)
		{
			Putbmp256(412, 271, "bmp\\dgYes.bmp");
		}
		else if ( game->elsething.choose1 == 4)
		{
			Putbmp256(562, 271, "bmp\\hlYes.bmp");
		}
		else if ( game->elsething.choose1 == 5)
		{
			Putbmp256(713, 271, "bmp\\xbyYes.bmp");
		}
		//保存新的鼠标背景
		NewBK();
	}

	if ( game->elsething.choose2before != game->elsething.choose2 )
	{
		if ( game->elsething.choose2before == 1)
		{
			Putbmp256(199, 450, "bmp\\agt.bmp");
		}
		else if ( game->elsething.choose2before == 2)
		{
			Putbmp256(350, 450, "bmp\\bx.bmp");
		}
		else if ( game->elsething.choose2before == 3)
		{
			Putbmp256(500, 450, "bmp\\dg.bmp");
		}
		else if ( game->elsething.choose2before == 4)
		{
			Putbmp256(650, 450, "bmp\\hl.bmp");
		}
		else if ( game->elsething.choose2before == 5)
		{
			Putbmp256(801, 450, "bmp\\xby.bmp");
		}

		game->elsething.choose2before = game->elsething.choose2;

		if ( game->elsething.choose2 == 1 )
		{
			Putbmp256(199, 450, "bmp\\agtYes.bmp");
		}
		else if ( game->elsething.choose2 == 2)
		{
			Putbmp256(350, 450, "bmp\\bxYes.bmp");
		}
		else if ( game->elsething.choose2 == 3)
		{
			Putbmp256(500, 450, "bmp\\dgYes.bmp");
		}
		else if ( game->elsething.choose2 == 4)
		{
			Putbmp256(650, 450, "bmp\\hlYes.bmp");
		}
		else if ( game->elsething.choose2 == 5)
		{
			Putbmp256(801, 450, "bmp\\xbyYes.bmp");
		}
		//保存新的鼠标背景
		NewBK();
	}
}



/**********************************************************
Function：      shopphoto、shopjudge

Description：   商店改变国家选择时改变球员头像贴图，同时改变下方购买图标

Input：     
		结构game地址
		int membernum      对应位置相应球员编号

Output：     None

Return：     None

Others：     商店球员贴头像，商店球员是否以购买，贴价格画数字或已购买，membernum用于判断球员编号
**********************************************************/
//商店球员贴图函数
void shopphoto(struct _GAME * game)
{
	if ( game->elsething.shopstate == 1 )
	{
		//商店选看各个国家的球员
		if (game->elsething.country == 1)
		{
			Putbmp256(140, 377, "bmp\\members\\1.bmp");
			Putbmp256(346, 377, "bmp\\members\\2.bmp");
			Putbmp256(552, 377, "bmp\\members\\3.bmp");
			Putbmp256(759, 377, "bmp\\members\\4.bmp");
			shopjudge(game,1);
			shopjudge(game,2);
			shopjudge(game,3);
			shopjudge(game,4);
		}
		else if (game->elsething.country == 2)
		{
			Putbmp256(140, 377, "bmp\\members\\7.bmp");
			Putbmp256(346, 377, "bmp\\members\\8.bmp");
			Putbmp256(552, 377, "bmp\\members\\9.bmp");
			Putbmp256(759, 377, "bmp\\members\\10.bmp");
			shopjudge(game,7);
			shopjudge(game,8);
			shopjudge(game,9);
			shopjudge(game,10);
		}
		else if (game->elsething.country == 3)
		{
			Putbmp256(140, 377, "bmp\\members\\13.bmp");
			Putbmp256(346, 377, "bmp\\members\\14.bmp");
			Putbmp256(552, 377, "bmp\\members\\15.bmp");
			Putbmp256(759, 377, "bmp\\members\\16.bmp");
			shopjudge(game,13);
			shopjudge(game,14);
			shopjudge(game,15);
			shopjudge(game,16);
		}
		else if (game->elsething.country == 4)
		{
			Putbmp256(140, 377, "bmp\\members\\19.bmp");
			Putbmp256(346, 377, "bmp\\members\\20.bmp");
			Putbmp256(552, 377, "bmp\\members\\21.bmp");
			Putbmp256(759, 377, "bmp\\members\\22.bmp");
			shopjudge(game,19);
			shopjudge(game,20);
			shopjudge(game,21);
			shopjudge(game,22);
		}
		else if (game->elsething.country == 5)
		{
			Putbmp256(140, 377, "bmp\\members\\25.bmp");
			Putbmp256(346, 377, "bmp\\members\\26.bmp");
			Putbmp256(552, 377, "bmp\\members\\27.bmp");
			Putbmp256(759, 377, "bmp\\members\\28.bmp");
			shopjudge(game,25);
			shopjudge(game,26);
			shopjudge(game,27);
			shopjudge(game,28);
		}
	}
	else if ( game->elsething.shopstate == 2 )
	{
		//商店道具界面
		Putbmp256(49, 199, "bmp\\shoptool.bmp");
		toolhaved(game,1);
	}
}

//商店每个国家的球员是否已购买问题
void shopjudge( struct _GAME * game, int membernum )
{
	int teamnumber;
	teamnumber = membernum % 6;

	if ( teamnumber == 1 )
	{
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(140, 572, "bmp\\buynow.bmp");
			paintmoney(180, 577, 20, BLACK, 100);
		}
		else
		{
			Putbmp256(140, 572, "bmp\\bought.bmp");
		}
	}
	else if ( teamnumber == 2 )
	{
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(346, 572, "bmp\\buynow.bmp");
			paintmoney(386, 577, 20, BLACK, 100);
		}
		else
		{
			Putbmp256(346, 572, "bmp\\bought.bmp");
		}
	}
	else if ( teamnumber == 3 )
	{
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(553, 572, "bmp\\buynow.bmp");
			paintmoney(593, 577, 20, BLACK, 100);
		}
		else
		{
			Putbmp256(553, 572, "bmp\\bought.bmp");
		}
	}
	else if ( teamnumber == 4 )
	{
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(759, 572, "bmp\\buynow.bmp");
			paintmoney(799, 577, 20, BLACK, 100);
		}
		else
		{
			Putbmp256(759, 572, "bmp\\bought.bmp");
		}
	}
}


/**********************************************************
Function：      memberbuy、toolbuy

Description：   商店内球员和道具的购买函数

Input：
		结构game
		int x, int y           用于判断所要购买道具

Output：     None

Return：     None

Others：     None
**********************************************************/
//商店球员购买
void memberbuy(struct _GAME * game)
{
	int num;
	int x;
	int country;

	country = game->elsething.country;

	//球员购买（四个购买位）
	if( Mouse_press(140,572,259,621) == 1 )
	{
		num = 6*(game->elsething.country-1)+1;
		if ( game->nowfile.stars[num]=='0' && game->nowfile.money>=100 )
		{
			game->nowfile.money -= 100;
			game->nowfile.stars[num]++;
			game->nowfile.surprise[country-1]++;
			Putbmp256(140, 572, "bmp\\bought.bmp");

			Bar(867,145,1023,196, 0xFFFFFF);;
			x = game->nowfile.money;
			paintmoney(880, 146, 20, 0x000000, x);

			NewBK();
		}
	}
	else if( Mouse_press(346,572,465,621) == 1 )
	{
		num = 6*(game->elsething.country-1)+2;
		if ( game->nowfile.stars[num]=='0' && game->nowfile.money>=100 )
		{
			game->nowfile.money -= 100;
			game->nowfile.stars[num]++;
			game->nowfile.surprise[country-1]++;
			Putbmp256(346, 572, "bmp\\bought.bmp");

			Bar(867,145,1023,196, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(880, 146, 20, 0x000000, x);

			NewBK();
		}
	}
	else if( Mouse_press(553,572,672,621) == 1 )
	{
		num = 6*(game->elsething.country-1)+3;
		if ( game->nowfile.stars[num]=='0' && game->nowfile.money>=100 )
		{
			game->nowfile.money -= 100;
			game->nowfile.stars[num]++;
			game->nowfile.surprise[country-1]++;
			Putbmp256(553, 572, "bmp\\bought.bmp");

			Bar(867,145,1023,196, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(880, 146, 20, 0x000000, x);

			NewBK();
		}
	}
	else if( Mouse_press(759,572,878,621) == 1 )
	{
		num = 6*(game->elsething.country-1)+4;
		if ( game->nowfile.stars[num]=='0' && game->nowfile.money>=100 )
		{
			game->nowfile.money -= 100;
			game->nowfile.stars[num]++;
			game->nowfile.surprise[country-1]++;
			Putbmp256(759, 572, "bmp\\bought.bmp");

			Bar(867,145,1023,196, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(880, 146, 20, 0x000000, x);

			NewBK();
		}
	}
}

//商店道具购买
void toolbuy(struct _GAME * game, int x, int y)
{
	int press;                                     //检测按键
	int onenumber[4]={-2,-2,-2};                   //记录每一位数字
	int i=0;                                       //记录位数，(i+1)为位数
	int j=0;                                       //辅助最终计算
	int t=0;                                       //记录当前10的n次方

	int propertynum;                               //当前选择购买的道具的编号
	int buynumber;                                 //当前选择购买的道具的数量
	int onecost;                                   //当前选择的道具单价

	int x0;
	int y0;
	int a;
	int b;

	int m;                                //光标位置
	int n;                                //光标位置
	int q=0;                              //光标更新时间计数
	int p=0;                              //光标黑白状态判定

	x0=x+36;
	y0=y+2;
	a=x+36+2;
	b=y+2;
	m=x0;
	n=y0;

	propertynum = game->elsething.property - 1;
	onecost = game->elsething.propertyonecost[propertynum];

	Bar(x+36, y, x+103, y+35, 0xFFFFFF);
	Bar(x+30, y+70, x+99, y+110, 0xFFFFFF);

	while(1)
	{
		Newxy();
		delay(5);
		if (q==0)
		{
			Line(m, n, m, n+30, 0x000000);
			Line(m+1, n, m+1, n+30, 0x000000);
			p++;
			if(p==40)
			{
				q=1;
			}
		}
		else if (q==1)
		{
			Line(m, n, m, n+30, 0xFFFFFF);
			Line(m+1, n, m+1, n+30, 0xFFFFFF);
			p--;
			if(p==0)
			{
				q=0;
			}
		}
		if (bioskey(1) && i<=3)
		{
			press=(int)bioskey(0);
			
			if (press==0x0231)
			{
				onenumber[i]=1;
			}
			else if (press==0x0332)
			{
				onenumber[i]=2;
			}
			else if (press==0x0433)
			{
				onenumber[i]=3;
			}
			else if (press==0x0534)
			{
				onenumber[i]=4;
			}
			else if (press==0x0635)
			{
				onenumber[i]=5;
			}
			else if (press==0x0736)
			{
				onenumber[i]=6;
			}
			else if (press==0x0837)
			{
				onenumber[i]=7;
			}
			else if (press==0x0938)
			{
				onenumber[i]=8;
			}
			else if (press==0x0a39)
			{
				onenumber[i]=9;
			}
			else if (press==0x0b30)
			{
				onenumber[i]=0;
			}
			else if (press==0x0e08 && i>0)   //退格键
			{
				onenumber[i]=-1;
			}
			else if (press==0x011b)          //ESC键
			{
				for(;i>=0;i--)
				{
					DrawNum(a, b, 15, 0xFFFFFF, 8);
					a-=20;
				}
				Line(m, n, m, n+30, 0xFFFFFF);
				Line(m+1, n, m+1, n+30, 0xFFFFFF);
				game->elsething.property = 0;
				break;
			}
			else if (press==0x1c0d)          //ENTER键
			{
				//计算返回然后break
				for(buynumber=0,i--;j<=i;j++)
				{
					t = (int)pow(10.0, (double)(i-j));
					buynumber += onenumber[j]*t;
				}
				game->elsething.propertybuynum[propertynum]=buynumber;
				game->elsething.propertybuycost[propertynum]=buynumber*onecost;
				game->elsething.property = 0;
				if (game->elsething.propertybuycost[propertynum]<=20000)
				{
					paintmoney(x0-5, y0+80, 10, BLACK, game->elsething.propertybuycost[propertynum]);
				}
				else
				{
					//钱太多
					out_hz(50,700,"钱超两万",1,48,48,0x000000);
					delay(500);
					out_hz(50,700,"钱超两万",1,48,48,0xFFFFFF);
				}
				Line(m, n, m, n+30, 0xFFFFFF);
				Line(m+1, n, m+1, n+30, 0xFFFFFF);
				break;
			}

			//根据按键所进行的处理
			if (onenumber[i]>=0 && onenumber[i]<=9 && i<=2)
			{
				DrawNum(a, b, 15, BLACK, onenumber[i]);
				i++;
				a+=20;
				Line(m, n, m, n+30, 0xFFFFFF);
				Line(m+1, n, m+1, n+30, 0xFFFFFF);
				m+=20;
			}
			else if (onenumber[i]==-1)
			{
				a-=20;
				Line(m, n, m, n+30, 0xFFFFFF);
				Line(m+1, n, m+1, n+30, 0xFFFFFF);
				m-=20;
				i--;
				onenumber[i]=-2;
				DrawNum(a, b, 15, 0xFFFFFF, 8);
			}
		}
	}
}



/**********************************************************
Function：      backpackphoto、backpackjudge

Description：   仓库改变国家选择时改变球员头像贴图，同时改变下方升级图标

Input：      
		结构game地址
		int membernum      对应位置相应球员编号

Output：     None

Return：     None

Others：     仓库球员贴头像，仓库球员是否解锁、画升级所需金钱或已满级，membernum用于判断球员编号
**********************************************************/
//仓库球员贴图函数
void backpackphoto(struct _GAME * game)
{
	if ( game->elsething.backpackstate == 1 )
	{
		Bar(54, 534, 54+918-1, 534+57-1, 0xFFFFFF);
		//仓库选看各个国家的球员
		if (game->elsething.country == 1)
		{
			game->nowfile.surprise[0]=0;
			Putbmp256(92, 382, "bmp\\members\\1.bmp");
			Putbmp256(239, 382, "bmp\\members\\2.bmp");
			Putbmp256(386, 382, "bmp\\members\\3.bmp");
			Putbmp256(534, 382, "bmp\\members\\4.bmp");
			if ( game->nowfile.stars[5] == '0' )
			{
				Putbmp256(681, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(681, 382, "bmp\\members\\5.bmp");
			}
			if ( game->nowfile.stars[6] == '0' )
			{
				Putbmp256(827, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(827, 382, "bmp\\members\\6.bmp");
			}
			backpackjudge(game,1);
			backpackjudge(game,2);
			backpackjudge(game,3);
			backpackjudge(game,4);
			backpackjudge(game,5);
			backpackjudge(game,6);
		}
		else if (game->elsething.country == 2)
		{
			game->nowfile.surprise[1]=0;
			Putbmp256(92, 382, "bmp\\members\\7.bmp");
			Putbmp256(239, 382, "bmp\\members\\8.bmp");
			Putbmp256(386, 382, "bmp\\members\\9.bmp");
			Putbmp256(534, 382, "bmp\\members\\10.bmp");
			if ( game->nowfile.stars[11] == '0' )
			{
				Putbmp256(681, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(681, 382, "bmp\\members\\11.bmp");
			}
			if ( game->nowfile.stars[12] == '0' )
			{
				Putbmp256(827, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(827, 382, "bmp\\members\\12.bmp");
			}
			backpackjudge(game,7);
			backpackjudge(game,8);
			backpackjudge(game,9);
			backpackjudge(game,10);
			backpackjudge(game,11);
			backpackjudge(game,12);
		}
		else if (game->elsething.country == 3)
		{
			game->nowfile.surprise[2]=0;
			Putbmp256(92, 382, "bmp\\members\\13.bmp");
			Putbmp256(239, 382, "bmp\\members\\14.bmp");
			Putbmp256(386, 382, "bmp\\members\\15.bmp");
			Putbmp256(534, 382, "bmp\\members\\16.bmp");
			if ( game->nowfile.stars[17] == '0' )
			{
				Putbmp256(681, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(681, 382, "bmp\\members\\17.bmp");
			}
			if ( game->nowfile.stars[18] == '0' )
			{
				Putbmp256(827, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(827, 382, "bmp\\members\\18.bmp");
			}
			backpackjudge(game,13);
			backpackjudge(game,14);
			backpackjudge(game,15);
			backpackjudge(game,16);
			backpackjudge(game,17);
			backpackjudge(game,18);
		}
		else if (game->elsething.country == 4)
		{
			game->nowfile.surprise[3]=0;
			Putbmp256(92, 382, "bmp\\members\\19.bmp");
			Putbmp256(239, 382, "bmp\\members\\20.bmp");
			Putbmp256(386, 382, "bmp\\members\\21.bmp");
			Putbmp256(534, 382, "bmp\\members\\22.bmp");
			if ( game->nowfile.stars[23] == '0' )
			{
				Putbmp256(681, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(681, 382, "bmp\\members\\23.bmp");
			}
			if ( game->nowfile.stars[24] == '0' )
			{
				Putbmp256(827, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(827, 382, "bmp\\members\\24.bmp");
			}
			backpackjudge(game,19);
			backpackjudge(game,20);
			backpackjudge(game,21);
			backpackjudge(game,22);
			backpackjudge(game,23);
			backpackjudge(game,24);
		}
		else if (game->elsething.country == 5)
		{
			game->nowfile.surprise[4]=0;
			Putbmp256(92, 382, "bmp\\members\\25.bmp");
			Putbmp256(239, 382, "bmp\\members\\26.bmp");
			Putbmp256(386, 382, "bmp\\members\\27.bmp");
			Putbmp256(534, 382, "bmp\\members\\28.bmp");
			if ( game->nowfile.stars[29] == '0' )
			{
				Putbmp256(681, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(681, 382, "bmp\\members\\29.bmp");
			}
			if ( game->nowfile.stars[30] == '0' )
			{
				Putbmp256(827, 382, "bmp\\members\\secret.bmp");
			}
			else
			{
				Putbmp256(827, 382, "bmp\\members\\30.bmp");
			}
			backpackjudge(game,25);
			backpackjudge(game,26);
			backpackjudge(game,27);
			backpackjudge(game,28);
			backpackjudge(game,29);
			backpackjudge(game,30);
		}
	}
	else if ( game->elsething.backpackstate == 2 )
	{
		//仓库道具界面
		Putbmp256(49, 199, "bmp\\bagtool.bmp");
		toolhaved(game,2);
		toolinuse1(game);
		toolinuse2(game);
	}
}

//仓库球员贴图时升级满级
void backpackjudge( struct _GAME * game, int membernum)
{
	int cost;
	
	int teamnumber;
	teamnumber = membernum % 6;

	//根据球队内球员位置编号贴图处理
	if ( teamnumber == 1 )
	{
		cost = 100*(game->nowfile.stars[membernum]-'0')+100;
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(92, 593, "bmp\\manlock.bmp");
		}
		else if ( game->nowfile.stars[membernum] <= '3' )
		{
			Putbmp256(92, 593, "bmp\\levelup.bmp");
			paintmoney(127, 598, 20, BLACK, cost);
		}
		else
		{
			Putbmp256(92, 593, "bmp\\toplevel.bmp");
		}
		paintstars(game, membernum, 92, 540, 40);
	}
	else if ( teamnumber == 2 )
	{
		cost = 100*(game->nowfile.stars[membernum]-'0')+100;
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(239, 593, "bmp\\manlock.bmp");
		}
		else if ( game->nowfile.stars[membernum] <= '3' )
		{
			Putbmp256(239, 593, "bmp\\levelup.bmp");
			paintmoney(274, 598, 20, BLACK, cost);
		}
		else
		{
			Putbmp256(239, 593, "bmp\\toplevel.bmp");
		}
		paintstars(game, membernum, 239, 540, 40);
	}
	else if ( teamnumber == 3 )
	{
		cost = 100*(game->nowfile.stars[membernum]-'0')+100;
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(385, 593, "bmp\\manlock.bmp");
		}
		else if ( game->nowfile.stars[membernum] <= '3' )
		{
			Putbmp256(385, 593, "bmp\\levelup.bmp");
			paintmoney(420, 598, 20, BLACK, cost);
		}
		else
		{
			Putbmp256(385, 593, "bmp\\toplevel.bmp");
		}
		paintstars(game, membernum, 385, 540, 40);
	}
	else if ( teamnumber == 4 )
	{
		cost = 100*(game->nowfile.stars[membernum]-'0')+100;
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(534, 593, "bmp\\manlock.bmp");
		}
		else if ( game->nowfile.stars[membernum] <= '3' )
		{
			Putbmp256(534, 593, "bmp\\levelup.bmp");
			paintmoney(569, 598, 20, BLACK, cost);
		}
		else
		{
			Putbmp256(534, 593, "bmp\\toplevel.bmp");
		}
		paintstars(game, membernum, 534, 540, 40);
	}
	else if ( teamnumber == 5 )
	{
		cost = 100*(game->nowfile.stars[membernum]-'0')+100;
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(681, 593, "bmp\\manlock.bmp");
		}
		else if ( game->nowfile.stars[membernum] <= '3' )
		{
			Putbmp256(681, 593, "bmp\\levelup.bmp");
			paintmoney(716, 598, 20, BLACK, cost);
		}
		else
		{
			Putbmp256(681, 593, "bmp\\toplevel.bmp");
		}
		paintstars(game, membernum, 681, 540, 40);
	}
	else if ( teamnumber == 0 )
	{
		cost = 100*(game->nowfile.stars[membernum]-'0')+100;
		if ( game->nowfile.stars[membernum] == '0' )
		{
			Putbmp256(827, 593, "bmp\\manlock.bmp");
		}
		else if ( game->nowfile.stars[membernum] <= '3' )
		{
			Putbmp256(827, 593, "bmp\\levelup.bmp");
			paintmoney(862, 598, 20, BLACK, cost);
		}
		else
		{
			Putbmp256(827, 593, "bmp\\toplevel.bmp");
		}
		paintstars(game, membernum, 827, 540, 40);
	}
}



/**********************************************************
Function：      memberup、tooluse

Description：   仓库内球员升级和道具使用的函数

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
//仓库球员升级
void memberup(struct _GAME * game)
{
	int num = 0;
	int cost;
	int x;

	//球员升级
	if( Mouse_press(92,593,211,642) == 1 && 0 == game->MouseStateOld)
	{
		num = 6*(game->elsething.country-1) +1;
		cost = 100*(game->nowfile.stars[num]-'0')+100;
		if ( (game->nowfile.stars[num]>='1') && (game->nowfile.stars[num]<='3') && game->nowfile.money>=cost )
		{
			game->nowfile.money -= cost;
			game->nowfile.stars[num]++;
			cost = 100*(game->nowfile.stars[num]-'0')+100;
			if ( game->nowfile.stars[num] == '4' )
			{
				Putbmp256(92, 593, "bmp\\toplevel.bmp");
			}
			else
			{
				Putbmp256(92, 593, "bmp\\levelup.bmp");
				paintmoney(127, 598, 20, BLACK, cost);
			}

			Bar(849,138,849+175-1,138+58-1, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(860, 146, 20, 0x000000, x);
			paintstars(game, num, 92, 540, 40);

			NewBK();
		}
	}
	if( Mouse_press(239,593,358,642) == 1 && 0 == game->MouseStateOld )
	{
		num = 6*(game->elsething.country-1) +2;
		cost = 100*(game->nowfile.stars[num]-'0')+100;
		if ( (game->nowfile.stars[num]>='1') && (game->nowfile.stars[num]<='3') && (game->nowfile.money>=cost) )
		{
			game->nowfile.money -= cost;
			game->nowfile.stars[num]++;
			cost = 100*(game->nowfile.stars[num]-'0')+100;
			if ( game->nowfile.stars[num] == '4' )
			{
				Putbmp256(239, 593, "bmp\\toplevel.bmp");
			}
			else
			{
				Putbmp256(239, 593, "bmp\\levelup.bmp");
				paintmoney(274, 598, 20, BLACK, cost);
			}

			Bar(849,138,849+175-1,138+58-1, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(860, 146, 20, 0x000000, x);
			paintstars(game, num, 239, 540, 40);

			NewBK();
		}
	}
	if( Mouse_press(385,593,504,642) == 1 && 0 == game->MouseStateOld )
	{
		num = 6*(game->elsething.country-1) +3;
		cost = 100*(game->nowfile.stars[num]-'0')+100;
		if ( (game->nowfile.stars[num]>='1') && (game->nowfile.stars[num]<='3') && (game->nowfile.money>=cost) )
		{
			game->nowfile.money -= cost;
			game->nowfile.stars[num]++;
			cost = 100*(game->nowfile.stars[num]-'0')+100;
			if ( game->nowfile.stars[num] == '4' )
			{
				Putbmp256(385, 593, "bmp\\toplevel.bmp");
			}
			else
			{
				Putbmp256(385, 593, "bmp\\levelup.bmp");
				paintmoney(420, 598, 20, BLACK, cost);
			}

			Bar(849,138,849+175-1,138+58-1, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(860, 146, 20, 0x000000, x);
			paintstars(game, num, 385, 540, 40);

			NewBK();
		}
	}
	if( Mouse_press(534,593,653,642) == 1 && 0 == game->MouseStateOld )
	{
		num = 6*(game->elsething.country-1) +4;
		cost = 100*(game->nowfile.stars[num]-'0')+100;
		if ( (game->nowfile.stars[num]>='1') && (game->nowfile.stars[num]<='3') && (game->nowfile.money>=cost) )
		{
			game->nowfile.money -= cost;
			game->nowfile.stars[num]++;
			cost = 100*(game->nowfile.stars[num]-'0')+100;
			if ( game->nowfile.stars[num] == '4' )
			{
				Putbmp256(534, 593, "bmp\\toplevel.bmp");
			}
			else
			{
				Putbmp256(534, 593, "bmp\\levelup.bmp");
				paintmoney(569, 598, 20, BLACK, cost);
			}

			Bar(849,138,849+175-1,138+58-1, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(860, 146, 20, 0x000000, x);
			paintstars(game, num, 534, 540, 40);

			NewBK();
		}
	}
	if( Mouse_press(681,593,800,642) == 1 && 0 == game->MouseStateOld )
	{
		num = 6*(game->elsething.country-1) +5;
		cost = 200*(game->nowfile.stars[num]-'0');
		if ( (game->nowfile.stars[num]>='1') && (game->nowfile.stars[num]<='3') && (game->nowfile.money>=cost) )
		{
			game->nowfile.money -= cost;
			game->nowfile.stars[num]++;
			cost = 200*(game->nowfile.stars[num]-'0');
			if ( game->nowfile.stars[num] == '4' )
			{
				Putbmp256(681, 593, "bmp\\toplevel.bmp");
			}
			else
			{
				Putbmp256(681, 593, "bmp\\levelup.bmp");
				paintmoney(716, 598, 20, BLACK, cost);
			}

			Bar(849,138,849+175-1,138+58-1, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(860, 146, 20, 0x000000, x);
			paintstars(game, num, 681, 540, 40);

			NewBK();
		}
	}
	if( Mouse_press(827,593,946,642) == 1 && 0 == game->MouseStateOld )
	{
		num = 6*(game->elsething.country-1) +6;
		cost = 200*(game->nowfile.stars[num]-'0');
		if ( (game->nowfile.stars[num]>='1') && (game->nowfile.stars[num]<='3') && (game->nowfile.money>=cost) )
		{
			game->nowfile.money -= cost;
			game->nowfile.stars[num]++;
			cost = 200*(game->nowfile.stars[num]-'0');
			if ( game->nowfile.stars[num] == '4' )
			{
				Putbmp256(827, 593, "bmp\\toplevel.bmp");
			}
			else
			{
				Putbmp256(827, 593, "bmp\\levelup.bmp");
				paintmoney(862, 598, 20, BLACK, cost);
			}

			Bar(849,138,849+175-1,138+58-1, 0xFFFFFF);
			x = game->nowfile.money;
			paintmoney(860, 146, 20, 0x000000, x);
			paintstars(game, num, 827, 540, 40);

			NewBK();
		}
	}
}

//仓库道具使用
void tooluse(struct _GAME * game)
{
	//加速道具
	if ( game->nowfile.inuse[0]==0 )
	{
		if (Mouse_press(297,359,416,408)==1 && game->nowfile.property[0]>0)
		{
			game->nowfile.property[0]--;
			game->nowfile.inuse[0]=1;
			Bar(330, 417, 384, 466, 0xFFFFFF);
			paintmoney(297+36, 417+8, 15, BLACK, game->nowfile.property[0]);
		}
		if (Mouse_press(514,359,633,408)==1 && game->nowfile.property[1]>0)
		{
			game->nowfile.property[1]--;
			game->nowfile.inuse[0]=2;
			Bar(547, 417, 601, 466, 0xFFFFFF);
			paintmoney(514+36, 417+8, 15, BLACK, game->nowfile.property[1]);
		}
		if (Mouse_press(720,359,839,408)==1 && game->nowfile.property[2]>0)
		{
			game->nowfile.property[2]--;
			game->nowfile.inuse[0]=3;
			Bar(753, 417, 807, 466, 0xFFFFFF);
			paintmoney(720+36, 417+8, 15, BLACK, game->nowfile.property[2]);
		}
		toolinuse1(game);
	}
	//金钱道具
	if ( game->nowfile.inuse[1]==0 )
	{
		if (Mouse_press(297,605,416,654)==1 && game->nowfile.property[3]>0)
		{
			game->nowfile.property[3]--;
			game->nowfile.inuse[1]=4;
			Bar(330, 663, 384, 712, 0xFFFFFF);
			paintmoney(297+36, 663+8, 15, BLACK, game->nowfile.property[3]);
		}
		if (Mouse_press(514,605,633,654)==1 && game->nowfile.property[4]>0)
		{
			game->nowfile.property[4]--;
			game->nowfile.inuse[1]=5;
			Bar(547, 663, 601, 712, 0xFFFFFF);
			paintmoney(514+36, 663+8, 15, BLACK, game->nowfile.property[4]);
		}
		if (Mouse_press(720,605,839,654)==1 && game->nowfile.property[5]>0)
		{
			game->nowfile.property[5]--;
			game->nowfile.inuse[1]=6;
			Bar(753, 663, 807, 712, 0xFFFFFF);
			paintmoney(720+36, 663+8, 15, BLACK, game->nowfile.property[5]);
		}
		toolinuse2(game);
	}
}



/**********************************************************
Function：      toolhaved

Description：   显示拥有道具的数量

Input：     结构game

Output：     None

Return：     None

Others：     根据judge标志量判断商店还是仓库
**********************************************************/
//商店里更新显示已有道具数量
void toolhaved(struct _GAME * game, int judge)
{
	if (judge==1)               //商店
	{
		Bar(296+67, 358+39, 296+67+40, 358+39+29, 0xFFFFFF);
		Bar(514+67, 358+39, 514+67+40, 358+39+29, 0xFFFFFF);
		Bar(720+67, 358+39, 720+67+40, 358+39+29, 0xFFFFFF);
		Bar(296+67, 605+39, 296+67+40, 605+39+29, 0xFFFFFF);
		Bar(514+67, 605+39, 514+67+40, 605+39+29, 0xFFFFFF);
		Bar(720+67, 605+39, 720+67+40, 605+39+29, 0xFFFFFF);

		paintmoney(296+68,358+46, 10, BLACK, game->nowfile.property[0]);
		paintmoney(514+68,358+46, 10, BLACK, game->nowfile.property[1]);
		paintmoney(720+68,358+46, 10, BLACK, game->nowfile.property[2]);
		paintmoney(296+68,605+46, 10, BLACK, game->nowfile.property[3]);
		paintmoney(514+68,605+46, 10, BLACK, game->nowfile.property[4]);
		paintmoney(720+68,605+46, 10, BLACK, game->nowfile.property[5]);
	}
	else if (judge==2)               //仓库
	{
		Bar(330, 417, 384, 466, 0xFFFFFF);
		Bar(547, 417, 601, 466, 0xFFFFFF);
		Bar(753, 417, 807, 466, 0xFFFFFF);
		Bar(330, 663, 384, 712, 0xFFFFFF);
		Bar(547, 663, 601, 712, 0xFFFFFF);
		Bar(753, 663, 807, 712, 0xFFFFFF);

		paintmoney(297+36, 417+8, 15, BLACK, game->nowfile.property[0]);
		paintmoney(514+36, 417+8, 15, BLACK, game->nowfile.property[1]);
		paintmoney(720+36, 417+8, 15, BLACK, game->nowfile.property[2]);
		paintmoney(297+36, 663+8, 15, BLACK, game->nowfile.property[3]);
		paintmoney(514+36, 663+8, 15, BLACK, game->nowfile.property[4]);
		paintmoney(720+36, 663+8, 15, BLACK, game->nowfile.property[5]);
	}
}



/**********************************************************
Function：      toolinuse1、toolinuse2

Description：   检测使用中的道具

Input：     结构game

Output：     None

Return：     None

Others：     显示使用中的道具，因临时改bug问题分成了两个函数
**********************************************************/
//显示使用中的道具
void toolinuse1(struct _GAME * game)
{
	if ( game->nowfile.inuse[0]==1 )
	{
		Bar(297,359,297+120-1,359+50-1, 0xFFFFFF);
		out_hz(297+15,359+15,"使用中",1,24,36,0x000000);
		NewBK();
	}
	else if ( game->nowfile.inuse[0]==2 )
	{
		Bar(514,359,514+120-1,359+50-1, 0xFFFFFF);
		out_hz(514+15,359+15,"使用中",1,24,36,0x000000);
		NewBK();
	}
	else if ( game->nowfile.inuse[0]==3 )
	{
		Bar(720,359,720+120-1,359+50-1, 0xFFFFFF);
		out_hz(720+15,359+15,"使用中",1,24,36,0x000000);
		NewBK();
	}
}
void toolinuse2(struct _GAME * game)
{
	if ( game->nowfile.inuse[1]==4 )
	{
		Bar(297,605,297+120-1,605+50-1, 0xFFFFFF);
		out_hz(297+15,605+15,"使用中",1,24,36,0x000000);
		NewBK();
	}
	else if ( game->nowfile.inuse[1]==5 )
	{
		Bar(514,605,514+120-1,605+50-1, 0xFFFFFF);
		out_hz(514+15,605+15,"使用中",1,24,36,0x000000);
		NewBK();
	}
	else if ( game->nowfile.inuse[1]==6 )
	{
		Bar(720,605,720+120-1,605+50-1, 0xFFFFFF);
		out_hz(720+15,605+15,"使用中",1,24,36,0x000000);
		NewBK();
	}
}



/**********************************************************
Function：      sin_paintyes1

Description：   单人游戏更改贴图显示已选择己方队伍

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
//单人模式选队伍贴图√问题
void sin_paintyes1(struct _GAME * game)
{
	if ( game->elsething.sin_teamchoosebefore != game->elsething.sin_teamchoose )
	{
		if ( game->elsething.sin_teamchoosebefore == 1)
		{
			Putbmp256(167, 350, "bmp\\agt.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore == 2)
		{
			Putbmp256(318, 350, "bmp\\bx.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore == 3)
		{
			Putbmp256(468, 350, "bmp\\dg.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore == 4)
		{
			Putbmp256(618, 350, "bmp\\hl.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore == 5)
		{
			Putbmp256(769, 350, "bmp\\xby.bmp");
		}

		game->elsething.sin_teamchoosebefore = game->elsething.sin_teamchoose;

		if ( game->elsething.sin_teamchoose == 1 )
		{
			Putbmp256(167, 350, "bmp\\agtYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose == 2)
		{
			Putbmp256(318, 350, "bmp\\bxYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose == 3)
		{
			Putbmp256(468, 350, "bmp\\dgYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose == 4)
		{
			Putbmp256(618, 350, "bmp\\hlYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose == 5)
		{
			Putbmp256(769, 350, "bmp\\xbyYes.bmp");
		}
		//保存新的鼠标背景
		NewBK();
	}
}



/**********************************************************
Function：      yourgamble

Description：   单人游戏选择队伍时，附有赌资输入，玩家根据需要输入加入赌注的金钱

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
//赌资输入
void yourgamble(struct _GAME * game)
{
	int press;                                     //检测按键
	int onenumber[5]={-2,-2,-2,-2};                //记录每一位数字
	int i=0;                                       //记录位数，(i+1)为位数
	int j=0;                                       //辅助最终计算
	int t=0;                                       //记录当前10的n次方
	int input;

	int a=350;
	int b=605;

	int m=a-2;                                //光标位置
	int n=b-2;                                //光标位置
	int q=0;                                //光标更新时间计数
	int p=0;                                //光标黑白状态判定

	Bar(350,600,500,649,0xFFFFFF);
	NewBK();
	game->elsething.gamble=0;

	while(1)
	{
		Newxy();
		delay(5);
		if (q==0)
		{
			Line(m, n, m, n+40, 0x000000);
			Line(m+1, n, m+1, n+40, 0x000000);
			p++;
			if(p==40)
			{
				q=1;
			}
		}
		else if (q==1)
		{
			Line(m, n, m, n+40, 0xFFFFFF);
			Line(m+1, n, m+1, n+40, 0xFFFFFF);
			p--;
			if(p==0)
			{
				q=0;
			}
		}
		if (bioskey(1) && i<=4)
		{
			press=(int)bioskey(0);
			
			if (press==0x0231)
			{
				onenumber[i]=1;
			}
			else if (press==0x0332)
			{
				onenumber[i]=2;
			}
			else if (press==0x0433)
			{
				onenumber[i]=3;
			}
			else if (press==0x0534)
			{
				onenumber[i]=4;
			}
			else if (press==0x0635)
			{
				onenumber[i]=5;
			}
			else if (press==0x0736)
			{
				onenumber[i]=6;
			}
			else if (press==0x0837)
			{
				onenumber[i]=7;
			}
			else if (press==0x0938)
			{
				onenumber[i]=8;
			}
			else if (press==0x0a39)
			{
				onenumber[i]=9;
			}
			else if (press==0x0b30)
			{
				onenumber[i]=0;
			}
			else if (press==0x0e08 && i>0)    //退格键
			{
				onenumber[i]=-1;
			}
			else if (press==0x011b)           //ESC键
			{
				for(;i>=0;i--)
				{
					DrawNum(a, b, 20, 0xFFFFFF, 8);
					a-=25;
				}
				Line(m, n, m, n+40, 0xFFFFFF);
				Line(m+1, n, m+1, n+40, 0xFFFFFF);
				a+=25;
				DrawNum(a, b, 20, 0x000000, 0);
				game->elsething.gamble=0;
				break;
			}
			else if (press==0x1c0d)            //ENTER键
			{
				//计算返回然后break
				for(input=0,i--;j<=i;j++)
				{
					t = (int)pow(10.0, (double)(i-j));
					input += onenumber[j]*t;
				}
				game->elsething.gamble=input;
				Line(m, n, m, n+40, 0xFFFFFF);
				Line(m+1, n, m+1, n+40, 0xFFFFFF);
				if (i-1<0)
				{
					DrawNum(a, b, 20, 0x000000, 0);
				}
				break;
			}

			if (onenumber[i]>=0 && onenumber[i]<=9 && i<=3)
			{
				DrawNum(a, b, 20, BLACK, onenumber[i]);
				i++;
				a+=25;
				Line(m, n, m, n+40, 0xFFFFFF);
				Line(m+1, n, m+1, n+40, 0xFFFFFF);
				m+=25;
			}
			else if (onenumber[i]==-1)
			{
				a-=25;
				Line(m, n, m, n+40, 0xFFFFFF);
				Line(m+1, n, m+1, n+40, 0xFFFFFF);
				m-=25;
				i--;
				onenumber[i]=-2;
				DrawNum(a, b, 20, 0xFFFFFF, 8);
			}
		}
	}
}


/**********************************************************
Function：      photo_6men、choose_4men、

Description：   在选好的队伍中根据球员是否解锁给出可选择球员，画○表示玩家已选择上场的球员

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
//单人队伍里选人根据队伍贴图
void photo_6men(struct _GAME * game)
{
	int num;
	char path[30] = "/0";

	num = 6*(game->elsething.sin_teamchoose-1);
	dealpath(game,num+1,path);
	Putbmp256(320, 230, path);
	dealpath(game,num+2,path);
	Putbmp256(478, 230, path);
	dealpath(game,num+3,path);
	Putbmp256(635, 230, path);
	dealpath(game,num+4,path);
	Putbmp256(791, 230, path);
	dealpath(game,num+5,path);
	Putbmp256(635, 405, path);
	dealpath(game,num+6,path);
	Putbmp256(791, 405, path);
}

//球员10选4
void choose_4men(struct _GAME * game)
{
	char path[30] = "/0";
	int num;
	num = 6*(game->elsething.sin_teamchoose-1);

	if ( game->elsething.sin_menchoosebefore[0] != game->elsething.sin_menchoose[0] )
	{
		if ( game->elsething.sin_menchoosebefore[0] == 0)
		{
			Putbmp256(320, 49, "bmp\\blank.bmp");
		}
		else if ( game->elsething.sin_menchoosebefore[0] == num+1)
		{
			dealpath(game,num+1,path);
			Putbmp256(320, 230, path);
		}

		game->elsething.sin_menchoosebefore[0] = game->elsething.sin_menchoose[0];

		if ( game->elsething.sin_menchoose[0] == 0 )
		{
			sin_paintcircle(320, 49);
		}
		else if ( game->elsething.sin_menchoose[0]==num+1 && game->nowfile.stars[num+1]!='0')
		{
			sin_paintcircle(320, 230);	
		}
	}
	if ( game->elsething.sin_menchoosebefore[1] != game->elsething.sin_menchoose[1] )
	{
		if ( game->elsething.sin_menchoosebefore[1] == 0)
		{
			Putbmp256(478, 49, "bmp\\blank.bmp");
		}
		else if ( game->elsething.sin_menchoosebefore[1] == num+2)
		{
			dealpath(game,num+2,path);
			Putbmp256(478, 230, path);
		}

		game->elsething.sin_menchoosebefore[1] = game->elsething.sin_menchoose[1];

		if ( game->elsething.sin_menchoose[1] == 0 )
		{
			sin_paintcircle(478, 49);
		}
		else if ( game->elsething.sin_menchoose[1]==num+2 && game->nowfile.stars[num+2]!='0')
		{
			sin_paintcircle(478, 230);
		}
	}
	if ( game->elsething.sin_menchoosebefore[2] != game->elsething.sin_menchoose[2] )
	{
		if ( game->elsething.sin_menchoosebefore[2] == 0)
		{
			Putbmp256(635, 49, "bmp\\blank.bmp");
		}
		else if ( game->elsething.sin_menchoosebefore[2] == num+3)
		{
			dealpath(game,num+3,path);
			Putbmp256(635, 230, path);
		}
		else if ( game->elsething.sin_menchoosebefore[2] == num+5)
		{
			dealpath(game,num+5,path);
			Putbmp256(635, 405, path);
		}

		game->elsething.sin_menchoosebefore[2] = game->elsething.sin_menchoose[2];

		if ( game->elsething.sin_menchoose[2] == 0 )
		{
			sin_paintcircle(635, 49);
		}
		else if ( game->elsething.sin_menchoose[2]==num+3 && game->nowfile.stars[num+3]!='0')
		{
			sin_paintcircle(635, 230);
		}
		else if ( game->elsething.sin_menchoose[2]==num+5 && game->nowfile.stars[num+5]!='0')
		{
			sin_paintcircle(635, 405);
		}
	}
	if ( game->elsething.sin_menchoosebefore[3] != game->elsething.sin_menchoose[3] )
	{
		if ( game->elsething.sin_menchoosebefore[3] == 0)
		{
			Putbmp256(791, 49, "bmp\\blank.bmp");
		}
		else if ( game->elsething.sin_menchoosebefore[3] == num+4)
		{
			dealpath(game,num+4,path);
			Putbmp256(791, 230, path);
		}
		else if ( game->elsething.sin_menchoosebefore[3] == num+6)
		{
			dealpath(game,num+6,path);
			Putbmp256(791, 405, path);
		}

		game->elsething.sin_menchoosebefore[3] = game->elsething.sin_menchoose[3];

		if ( game->elsething.sin_menchoose[3] == 0 )
		{
			sin_paintcircle(791, 49);
		}
		else if ( game->elsething.sin_menchoose[3]==num+4 && game->nowfile.stars[num+4]!='0')
		{
			sin_paintcircle(791, 230);
		}
		else if ( game->elsething.sin_menchoose[3]==num+6 && game->nowfile.stars[num+6]!='0')
		{
			sin_paintcircle(791, 405);
		}
	}
}


/**********************************************************
Function：      dealpath

Description：   状态机循环及改变状态执行的函数。用于执行状态内循环以及改变状

Input：
		结构game
		int x                        地址字符串中有关数字的字符
		char * finished              做好的地址字符串保存的地址

Output：     None

Return：     改好的地址字符串所存地址

Others：     None
**********************************************************/
//路径处理（某部分重复路径太多，用此函数统一处理）
char *dealpath(struct _GAME * game, int x, char * finished)
{	
	
	char front[] = "bmp\\members\\";                          //前缀
	char end[] = ".bmp";                                      //后缀
	char middle[10];

	char lock[] = "lock";
	char secret[] = "secret";
	char membernum[3];                                        //用字符用于记录编号x

	int innumber;                                             //队伍内球员编号

	innumber = (x-1)%6+1;
	itoa (x, membernum, 10);
	if ( game->nowfile.stars[x] == '0')
	{
		if ( innumber<=4 )
		{
			strcpy(middle,lock);
		}
		else
		{
			strcpy(middle,secret);
		}
	}
	else 
	{
		strcpy( middle, membernum );
	}
	strcpy(finished, front);
	strcat(finished, middle);
	strcat(finished, end);

	return finished;
}



/**********************************************************
Function：      sin_paintyes2

Description：   单人游戏更改贴图显示已选择对手的队伍

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
//单人模式选对手队伍贴图√问题
void sin_paintyes2(struct _GAME * game)
{
	if ( game->elsething.sin_teamchoosebefore2 != game->elsething.sin_teamchoose2 )
	{
		if ( game->elsething.sin_teamchoosebefore2 == 1)
		{
			Putbmp256(340, 97, "bmp\\agt.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 2)
		{
			Putbmp256(475, 97, "bmp\\bx.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 3)
		{
			Putbmp256(611, 97, "bmp\\dg.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 4)
		{
			Putbmp256(748, 97, "bmp\\hl.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 5)
		{
			Putbmp256(888, 97, "bmp\\xby.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 6)
		{
			Putbmp256(61, 325, "bmp\\agt.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 7)
		{
			Putbmp256(196, 325, "bmp\\bx.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 8)
		{
			Putbmp256(332, 325, "bmp\\dg.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 9)
		{
			Putbmp256(469, 325, "bmp\\hl.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 10)
		{
			Putbmp256(609, 325, "bmp\\xby.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 11)
		{
			Putbmp256(317, 580, "bmp\\agt.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 12)
		{
			Putbmp256(452, 580, "bmp\\bx.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 13)
		{
			Putbmp256(588, 580, "bmp\\dg.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 14)
		{
			Putbmp256(725, 580, "bmp\\hl.bmp");
		}
		else if ( game->elsething.sin_teamchoosebefore2 == 15)
		{
			Putbmp256(865, 580, "bmp\\xby.bmp");
		}

		game->elsething.sin_teamchoosebefore2 = game->elsething.sin_teamchoose2;

		if ( game->elsething.sin_teamchoose2 == 1 )
		{
			Putbmp256(340, 97, "bmp\\agtYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 2)
		{
			Putbmp256(475, 97, "bmp\\bxYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 3)
		{
			Putbmp256(611, 97, "bmp\\dgYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 4)
		{
			Putbmp256(748, 97, "bmp\\hlYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 5)
		{
			Putbmp256(888, 97, "bmp\\xbyYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 6)
		{
			Putbmp256(61, 325, "bmp\\agtYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 7)
		{
			Putbmp256(196, 325, "bmp\\bxYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 8)
		{
			Putbmp256(332, 325, "bmp\\dgYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 9)
		{
			Putbmp256(469, 325, "bmp\\hlYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 10)
		{
			Putbmp256(609, 325, "bmp\\xbyYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 11)
		{
			Putbmp256(317, 580, "bmp\\agtYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 12)
		{
			Putbmp256(452, 580, "bmp\\bxYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 13)
		{
			Putbmp256(588, 580, "bmp\\dgYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 14)
		{
			Putbmp256(725, 580, "bmp\\hlYes.bmp");
		}
		else if ( game->elsething.sin_teamchoose2 == 15)
		{
			Putbmp256(865, 580, "bmp\\xbyYes.bmp");
		}
		//保存新的鼠标背景
		NewBK();
	}
}


/**********************************************************
Function：      sin_paintcircle

Description：   画○函数，辅助上一个的画圈表示选择函数

Input：     int x, int y                     圆圈位置

Output：     None

Return：     None

Others：     用于photo_6men、choose_4men
**********************************************************/
//单人模式选队伍贴图○问题
void sin_paintcircle(int x, int y)
{
	int xcircle;
	int ycircle;
	xcircle = x+60;
	ycircle = y+75;
	Circle(xcircle, ycircle, 40, RED);
	Circle(xcircle, ycircle, 41, RED);
	Circle(xcircle, ycircle, 42, RED);
	Circle(xcircle, ycircle, 43, RED);
	Circle(xcircle, ycircle, 44, RED);
	Circle(xcircle, ycircle, 45, RED);
}



/**********************************************************
Function：      dealresult

Description：   处理比赛结果

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
//比赛结果处理（包括成就、金钱）
void dealresult(struct _GAME * game)
{
	int addmoney=0;
	float tooladd;
	int x, y;
	x = game->result.Wscore;
	y = game->result.Bscore;
	if ( x>=0 )
	{
		game->nowfile.numbers++;
		if (x>y)
		{
			game->nowfile.winnum++;
			game->nowfile.winmargin += x-y;
			addmoney += 100*(game->elsething.botlevel+1);
			if (game->elsething.gamble>0)
			{
				addmoney += game->elsething.gamble*(game->elsething.botlevel+1);
			}

			if (game->nowfile.inuse[1]==4)
			{
				tooladd = 1.5;
			}
			else if (game->nowfile.inuse[1]==5)
			{
				tooladd = 2.0;
			}
			else if (game->nowfile.inuse[1]==6)
			{
				tooladd = 2.5;
			}
			game->nowfile.money += (int)addmoney*tooladd;
		}
		else if (x<y)
		{
			game->nowfile.losenum++;
			if (game->elsething.gamble>0)
			{
				game->nowfile.money -= game->elsething.gamble;
			}
		}
		game->nowfile.inuse[0]=0;
		game->nowfile.inuse[1]=0;
	}

}



/**********************************************************
Function：      dealachieve

Description：   更新成就，检查是否符合条件解锁新球员

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
//成就系统更新解锁球员
void dealachieve(struct _GAME * game)
{
	if ( game->nowfile.numbers>=1 && game->nowfile.stars[5]=='0')
	{
		game->nowfile.stars[5]++;
		game->nowfile.surprise[0]++;
	}
	if ( game->nowfile.numbers>=30 && game->nowfile.stars[6]=='0')
	{
		game->nowfile.stars[6]++;
		game->nowfile.surprise[0]++;
	}

	if ( game->nowfile.losenum>=1 && game->nowfile.stars[11]=='0')
	{
		game->nowfile.stars[11]++;
		game->nowfile.surprise[1]++;
	}
	if ( game->nowfile.winnum>=10 && game->nowfile.stars[12]=='0')
	{
		game->nowfile.stars[12]++;
		game->nowfile.surprise[1]++;
	}

	if ( game->nowfile.numbers>=5 && game->nowfile.stars[17]=='0')
	{
		game->nowfile.stars[17]++;
		game->nowfile.surprise[2]++;
	}
	if ( game->nowfile.winmargin>=20 && game->nowfile.stars[18]=='0')
	{
		game->nowfile.stars[18]++;
		game->nowfile.surprise[2]++;
	}

	if ( game->nowfile.winnum>=3 && game->nowfile.stars[23]=='0')
	{
		game->nowfile.stars[23]++;
		game->nowfile.surprise[3]++;
	}
	if ( game->nowfile.losenum>=3 && game->nowfile.stars[24]=='0')
	{
		game->nowfile.stars[24]++;
		game->nowfile.surprise[3]++;
	}

	if ( game->nowfile.winmargin>=5 && game->nowfile.stars[29]=='0')
	{
		game->nowfile.stars[29]++;
		game->nowfile.surprise[4]++;
	}
	if ( game->nowfile.numbers>=20 && game->nowfile.stars[30]=='0')
	{
		game->nowfile.stars[30]++;
		game->nowfile.surprise[4]++;
	}
}




/**********************************************************
Function：      paintstars

Description：   仓库中画星函数，用于表示球员星级

Input：
		结构game
		int membernum                     球员编号
		int x, int y                      星星所在位置
		int size                          星星大小

Output：     None

Return：     None

Others：     None
**********************************************************/
//画星表示球员星数函数
void paintstars(struct _GAME * game, int membernum, int x, int y, int size)
{
	int starnum;
	starnum = game->nowfile.stars[membernum]-'1';
	for ( ;starnum>0;starnum--)
	{
		Line(x, y+2*size/5-1, x+size, y+2*size/5-1, BLACK);
		Line(x, y+2*size/5, x+size, y+2*size/5, BLACK);
		Line(x, y+2*size/5+1, x+size, y+2*size/5+1, BLACK);

		Line(x+size/2-1, y, x+size/5-1, y+size, BLACK);
		Line(x+size/2, y, x+size/5, y+size, BLACK);
		Line(x+size/2+1, y, x+size/5+1, y+size, BLACK);

		Line(x, y+2*size/5-1, x+4*size/5+1, y+size, BLACK);
		Line(x, y+2*size/5, x+4*size/5, y+size, BLACK);
		Line(x, y+2*size/5+1, x+4*size/5-1, y+size, BLACK);

		Line(x+size/2-1, y, x+4*size/5-1, y+size, BLACK);
		Line(x+size/2, y, x+4*size/5, y+size, BLACK);
		Line(x+size/2+1, y, x+4*size/5+1, y+size, BLACK);

		Line(x+size/5-1, y+size, x+size, y+2*size/5-1, BLACK);
		Line(x+size/5, y+size, x+size, y+2*size/5, BLACK);
		Line(x+size/5+1, y+size, x+size, y+2*size/5+1, BLACK);

		x += 11*size/10;
	}
}





void checkmember(struct _GAME * game)
{
	int number;
	if (Mouse_press(92,382,211,531)==1)
	{
		number = 6*(game->elsething.country-1)+1;
		message(game,number);
	}
	else if (Mouse_press(239,382,358,531)==1)
	{
		number = 6*(game->elsething.country-1)+2;
		message(game,number);
	}
	else if (Mouse_press(386,382,505,531)==1)
	{
		number = 6*(game->elsething.country-1)+3;
		message(game,number);
	}
	else if (Mouse_press(534,382,653,531)==1)
	{
		number = 6*(game->elsething.country-1)+4;
		message(game,number);
	}
	else if (Mouse_press(681,382,800,531)==1)
	{
		number = 6*(game->elsething.country-1)+5;
		if(game->nowfile.stars[number]>='1')
		{
			message(game,number);
		}
	}
	else if (Mouse_press(827,382,946,531)==1)
	{
		number = 6*(game->elsething.country-1)+6;
		if(game->nowfile.stars[number]>='1')
		{
			message(game,number);
		}
	}
}


void message(struct _GAME * game, int number)
{
	FILE * fdata = NULL;
	char path[30] = "/0";
	//球员参数临时变量
    float PlayerV;       //基础速度
    float get_rate;      //抢断率
    float acc_rate;      //加速倍率
    float acc_time;      //加速持续时间
    float acc_cd;        //加速CD
    char IDtemp[3] = "\0";     //用于定位数据文档中指定的球员66
    int PlayerID = -1;         //指定球员ID
	char name[30];
	
	int a, b, c, d, e;
	
	//打开球员数据文档
	if((fdata = fopen("data\\players.txt", "r")) == NULL)
    {
        exit(1);
    }
	rewind(fdata);
    while( !(strcmp(IDtemp, "ID") == 0 && PlayerID == number) )
    {
        fscanf(fdata, "%s", IDtemp);
        if(strcmp(IDtemp, "ID") == 0)
        {
            fscanf(fdata, "%d", &PlayerID);
        }
    }
	
    fscanf(fdata, "%f", &PlayerV);
    fscanf(fdata, "%f", &get_rate);
    fscanf(fdata, "%f", &acc_rate);
    fscanf(fdata, "%f", &acc_time);
    fscanf(fdata, "%f", &acc_cd);
    fscanf(fdata, "%s", &name);
	fscanf(fdata, "%s", &name);
		
	fclose(fdata);
		
	Bar(0,0,1024,768,0xFFFFFF);
	
	out_hz(200,200+15,"基础速度",1,24,24,0x000000);
	out_hz(200,250+15,"抢断率",1,24,24,0x000000);
	out_hz(200,300+15,"加倍速率",1,24,24,0x000000);
	out_hz(200,350+15,"加速持续时间",1,24,24,0x000000);
	out_hz(200,400+15,"加速冷却",1,24,24,0x000000);
	
	paintmoney(400, 200+5, 20, 0x000000, (int)(10.0*PlayerV));
	paintmoney(400, 250+5, 20, 0x000000, (int)(10.0*get_rate));
	paintmoney(400, 300+5, 20, 0x000000, (int)(10.0*acc_rate));
	paintmoney(400, 350+5, 20, 0x000000, (int)(10.0*acc_time));
	paintmoney(400, 400+5, 20, 0x000000, (int)(10.0*acc_cd));
	
	a=(int)(100.0*PlayerV);
	b=(int)(100.0*get_rate);
	c=(int)(100.0*acc_rate);
	d=(int)(100.0*acc_time);
	e=(int)(100.0*acc_cd);
	
	Bar(500, 200+5, 500+a, 250-5, 0x000000);
	Bar(500, 250+5, 500+b, 300-5, 0x000000);
	Bar(500, 300+5, 500+c, 350-5, 0x000000);
	Bar(500, 350+5, 500+d, 400-5, 0x000000);
	Bar(500, 400+5, 500+e, 450-5, 0x000000);
	
	game->nowfile.stars[number]++;
	dealpath(game,number,path);
	Putbmp256(50, 250, path);
	game->nowfile.stars[number]--;
	out_hz(50,400,name,1,24,24,0x000000);
	NewBK();
	
	while(1)
	{	
		Newxy();
		delay(5);
		if(Mouse_press(0,0,1024,768)==1) 
		{
			break;
		}
	}
	
	Bar(0,0,1024,768,0xFFFFFF);
	normal();
	Bar(69,101,244,177, 0x000000);
	Bar(72,104,241,174, 0xFFFFFF);
	Bar(290,101,465,177, 0x000000);
	Bar(293,104,462,174, 0xFFFFFF);
	Putbmp256(49, 199, "bmp\\bagman.bmp");
	out_hz(300,20,"仓库",1,48,48,0x000000);
	out_hz(100,115,"球员",1,48,48,0x000000);
	out_hz(320,115,"道具",1,48,48,0x000000);
	out_hz(495,140,"你拥有的金钱数：",1,48,48,0x000000);
	out_hz(100,700,"附：球员详细资料点击球员头像查看，箭头表示可升级",1,24,24,0x000000);
	backpackphoto(game);
	paintmoney(860, 146, 20, 0x000000, game->nowfile.money);
	NewBK();
}