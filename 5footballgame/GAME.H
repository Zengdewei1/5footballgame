/*********************        H文件         ************************/

/**********************************************************

Copyright (C) 2017 郭裕祺 黄杰豪.
File name:      game.h
Author:         黄杰豪
Version：        1.1
Date：           2017/11/1
Description：    足球游戏外部功能所有函数库头文件。


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
		单机选择存档

	13.void single_enter(struct _GAME * game);
	14.void single_execute(struct _GAME * game);
	15.void single_exit(struct _GAME * game);
		单机存档内界面

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

#ifndef GAME_H
#define GAME_H

#include<stdio.h>
#include<stdlib.h>
#include<bios.h>
#include"tool.h"
#include"SVGAmode.h"
#include"football.h"
#include"HZDIS.h"
#include"HANZI.h"


//游戏状态机、存档包
typedef struct _STATE
{
	void (*Enter) (struct _GAME * game);                  //进入状态时调用
	void (*Execute) (struct _GAME * game);                //处于状态时调用
	void (*Exit) (struct _GAME * game);                   //退出状态时调用
}_state;

typedef struct _FILE
{
	int filenumber;                        //存档编号

	unsigned int money;                    //金钱
	int property[6];                       //拥有的道具
	int numbers;                           //已完成场数
	int winnum;                            //胜场数
	int losenum;                           //败场数
	int winmargin;                         //净胜球数
	char stars[31];                        //用于判断30位球员是否解锁，其中stars[0]为白板，里面的数据还有用于判断球员等级，0表示未拥有，1表示已拥有等级为0；234表示…………
	int surprise[5];                       //用于判断球队中是否出现了新球员
	int inuse[2];                          //正在使用中的道具号码
}_file;


//球员数据结构包
typedef struct _ABLITY
{
	float skill;                           //抢球率
	float speed;                           //速度
	float explodespeed;                    //爆发速度
	float persistence;                     //耐力
	float cd;                              //冷却时间
}_ablity;


//游戏传参
typedef struct _ELSETHING
{
	int gamemode;                          //游戏模式：单双人
	int botlevel;                          //机器人难度

                                 //双人
	int choose1;                         //用于记录一号玩家选人
	int choose2;                         //用于记录二号玩家选人
	int choose1before;                   //解决一号玩家选择画√问题
	int choose2before;                   //解决二号玩家选择画√问题

                                    //单人
	int shopstate;                       //用于判断商店处于哪一个界面：球员（1）和道具（2）
	int backpackstate;                   //用于判断仓库处于哪一个界面：球员（1）和道具（2）
	int country;                         //用于判断商店和仓库内国家，依次为12345
	int property;                        //用于判断商店和仓库内使用的道具的编号，依次为123456
	int propertybuynum[6];               //记录所购买道具的数量
	int propertybuycost[6];              //记录该道具购买的总价
	int propertyonecost[6];              //记录每个道具的单价，在另一处初始化

	int sin_teamchoose;                  //用于记录单机模式下玩家选自己队伍
	int sin_teamchoosebefore;            //用于解决单人模式选择自己队伍画√问题
	int sin_teamchoose2;                 //用于记录单机模式下玩家选对手队伍
	int sin_teamchoosebefore2;           //用于解决单人模式选择对手队伍画√问题

	int sin_menchoose[4];                //10选4中已经选择的人
	int sin_menchoosebefore[4];          //10选4记录已选择

	int botteam;                         //选择对手的队伍
	int unsigned gamble;                 //赌资
}_elsething;



//游戏内所有参数，打包在结构game内
typedef struct _GAME
{
	_state menu, doubleman, choose, single, achieve, shop, mything, myteamchoose, menchoose, levelchoose;            //状态命名
	struct _STATE * pcurrentstate;                                                                                   //指向当前状态

	int MouseState;                             //鼠标当前状态
	int MouseStateOld;                          //鼠标上一次的状态

	_file nowfile;                              //存档数据
	_elsething elsething;                       //难度及模式选择
	_fieldplayer playerchoose;                  //队伍选择及球员选择
	_result result;                             //足球比赛结果
}_game;



/**********************************************************
Function：      Update

Description：   状态机循环的函数。用于执行状态内循环

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void Update(struct _GAME * self);


/**********************************************************
Function：      changestate

Description：  改变状态机状态的函数。用于改变状态机状态

Input：     结构game地址、要改变的状态的指针

Output：     None

Return：     None

Others：     None
**********************************************************/
void changestate(struct _GAME * self, _state * pnewstate);


/**********************************************************
Function：      Update

Description：   状态机循环及改变状态执行的函数。用于执行状态内循环以及改变状态

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void initgame(struct _GAME * game);


/**********************************************************
Function：     menu_enter、menu_execute、menu_exit

Description：   进入主菜单的函数、状态主菜单循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void menu_enter(struct _GAME * game);
void menu_execute(struct _GAME * game);
void menu_exit(struct _GAME * game);


/**********************************************************
Function：      doubleman_enter、doubleman_execute、doubleman_exit

Description：   进入双人游戏的函数、状态双人游戏循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void doubleman_enter(struct _GAME * game);
void doubleman_execute(struct _GAME * game);
void doubleman_exit(struct _GAME * game);


/**********************************************************
Function：      choose_enter、choose_execute、choose_exit

Description：   进入存档选择的函数、状态存档选择循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     单人游戏第一个界面为存档选择，所以命名为choose
**********************************************************/
void choose_enter(struct _GAME * game);
void choose_execute(struct _GAME * game);
void choose_exit(struct _GAME * game);


/**********************************************************
Function：      single_enter、single_execute、single_exit

Description：   正式进入单人游戏的函数、状态单人游戏循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void single_enter(struct _GAME * game);
void single_execute(struct _GAME * game);
void single_exit(struct _GAME * game);


/**********************************************************
Function：      achieve_enter、achieve_execute、achieve_exit

Description：   进入单人成就的函数、状态单人成就循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void achieve_enter(struct _GAME * game);
void achieve_execute(struct _GAME * game);
void achieve_exit(struct _GAME * game);


/**********************************************************
Function：      shop_enter、shop_execute、shop_exit

Description：   进入商店的函数、状态商店循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void shop_enter(struct _GAME * game);
void shop_execute(struct _GAME * game);
void shop_exit(struct _GAME * game);


/**********************************************************
Function：      mything_enter、mything_execute、mything_exit

Description：   进入仓库的函数、状态仓库循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void mything_enter(struct _GAME * game);
void mything_execute(struct _GAME * game);
void mything_exit(struct _GAME * game);


/**********************************************************
Function：      myteamchoose_enter、myteamchoose_execute、myteamchoose_exit

Description：   进入队伍选择的函数、状态队伍选择循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void myteamchoose_enter(struct _GAME * game);
void myteamchoose_execute(struct _GAME * game);
void myteamchoose_exit(struct _GAME * game);


/**********************************************************
Function：      menchoose_enter、menchoose_execute、menchoose_exit

Description：   进入选择球员的函数、状态选择球员循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void menchoose_enter(struct _GAME * game);
void menchoose_execute(struct _GAME * game);
void menchoose_exit(struct _GAME * game);

/**********************************************************
Function：      levelchoose_enter、levelchoose_execute、levelchoose_exit

Description：   进入对手难度选择的函数、状态难度选择循环执行的函数、退出该状态时执行的函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void levelchoose_enter(struct _GAME * game);
void levelchoose_execute(struct _GAME * game);
void levelchoose_exit(struct _GAME * game);


/**********************************************************
Function：      pause

Description：   全局状态pause，检测是否按下右上角退出按键并给出相应的按键功能，执行操作

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void pause(struct _GAME * game);





/**********************************************************
Function：      normal

Description：   刷新界面时重画界面小组件

Input：      None

Output：     None

Return：     None

Others：     None
**********************************************************/
void normal();


/**********************************************************
Function：      readfile、writefile

Description：   读档、存档函数

Input：     结构game地址

Output：     None

Return：     None

Others：     None
**********************************************************/
void readfile(struct _GAME * game);
void writefile(struct _GAME * game);


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
void cleanfile(struct _GAME * game, int x);


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
void paintmoney(int x, int y, int size, unsigned char color, int num);


/**********************************************************
Function：      dou_paintyes

Description：   双人游戏选择队伍时候，根据玩家选择改变贴图

Input：     结构game地址

Output：     None

Return：     None

Others：     记录参数均包含在结构game中
**********************************************************/
void dou_paintyes(struct _GAME * game);


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
void shopphoto(struct _GAME * game);
void shopjudge(struct _GAME * game, int membernum);


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
void memberbuy(struct _GAME * game);
void toolbuy(struct _GAME * game, int x, int y);


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
void backpackphoto(struct _GAME * game);
void backpackjudge(struct _GAME * game, int membernum);


/**********************************************************
Function：      memberup、tooluse

Description：   仓库内球员升级和道具使用的函数

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
void memberup(struct _GAME * game);
void tooluse(struct _GAME * game);


/**********************************************************
Function：      toolhaved

Description：   显示拥有道具的数量

Input：     结构game

Output：     None

Return：     None

Others：     根据judge标志量判断商店还是仓库
**********************************************************/
void toolhaved(struct _GAME * game, int judge);


/**********************************************************
Function：      toolinuse1、toolinuse2

Description：   检测使用中的道具

Input：     结构game

Output：     None

Return：     None

Others：     显示使用中的道具，因临时改bug问题分成了两个函数
**********************************************************/
void toolinuse1(struct _GAME * game);
void toolinuse2(struct _GAME * game);


/**********************************************************
Function：      sin_paintyes1

Description：   单人游戏更改贴图显示已选择己方队伍

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
void sin_paintyes1(struct _GAME * game);


/**********************************************************
Function：      yourgamble

Description：   单人游戏选择队伍时，附有赌资输入，玩家根据需要输入加入赌注的金钱

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
void yourgamble(struct _GAME * game);


/**********************************************************
Function：      photo_6men、choose_4men、

Description：   在选好的队伍中根据球员是否解锁给出可选择球员，画○表示玩家已选择上场的球员

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
void photo_6men(struct _GAME * game);
void choose_4men(struct _GAME * game);


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
char *dealpath(struct _GAME * game, int x, char * finished);


/**********************************************************
Function：      sin_paintyes2

Description：   单人游戏更改贴图显示已选择对手的队伍

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
void sin_paintyes2(struct _GAME * game);


/**********************************************************
Function：      sin_paintcircle

Description：   画○函数，辅助上一个的画圈表示选择函数

Input：     int x, int y                     圆圈位置

Output：     None

Return：     None

Others：     用于photo_6men、choose_4men
**********************************************************/
void sin_paintcircle(int x, int y);


/**********************************************************
Function：      dealresult

Description：   处理比赛结果

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
void dealresult(struct _GAME * game);


/**********************************************************
Function：      dealachieve

Description：   更新成就，检查是否符合条件解锁新球员

Input：     结构game

Output：     None

Return：     None

Others：     None
**********************************************************/
void dealachieve(struct _GAME * game);


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
void paintstars(struct _GAME * game, int membernum, int x, int y, int size);



void checkmember(struct _GAME * game);
void message(struct _GAME * game, int number);

#endif