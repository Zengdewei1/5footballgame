/**********************************************************
Copyright (C) 2017 郭裕祺 黄杰豪.
File name:      tool.h
Author:         郭裕祺
Version：        1.1
Date：           2017/9/23
Description：    实况足球游戏核心部分的辅助函数库头文件。

                对本文件的修改请及时按如下格式记录在下面（每个修改单独列出）！

Function List：
    1.void RewriteBack(int x, int y, int size_x, int size_y);
        恢复背景功能函数。用于恢复被球员、足球覆盖过的背景白线

    2.int GetBall(struct _PLAYER * pplayer, struct _BALL * pball);
        用于判定球员做出抢球动作时是否能抢到球。

    3.double Distance(SVector2D n1, SVector2D n2);
        计算并返回两个点之间的距离。

    4.SVector2D GetDir(double x, double y);
        计算并返回一个向量对应的单位向量

    5.int GetRecPlayer(SVector2D ball_v, SVector2D p_pos, struct _TEAM * pOurteam);
        计算得出一个合适的接球队员，并返回其队内编号

    6.void PlayerBorder(struct _PLAYER * pplayer);
        限制球员在场地范围内移动

    7.int FindClosest(struct _PLAYER ** players, SVector2D pos, int exID);
        找到除给定序号外离给定点最近的球员，并返回其队内编号

    8.SVector2D ShootBall(struct _BALL * pball, int TeamName);
        射门辅助函数，计算得出球员射门时应给球的速度

    9.int CanShoot(struct _PLAYER * pplayer, int TeamName);
        判定球员此时的朝向能否进行射门

    10.int BallDec(struct _BALL * pball);
       用于检测球是否出界或进球，若出界返回1，若白队进球返回2，若蓝队进球返回3，否则返回0

    11.void Restart(struct _TEAM * pWteam, struct _TEAM * pBteam, struct _BALL * pball, struct _GOALKEEPER * Gkeepers);
        重新开球，将场上所有球员以及足球恢复初始状态

    12.void DrawPlayer(int x, int y, enum _dir Dir, unsigned char color);
        在给定位置画出指定方向、颜色的球员

    13.void DrawNum(int x, int y, int size, unsigned char color, int num);
        在给定位置画出给定大小、颜色的数字（0到9）

    14.void Updategoal(int Wgoal, int Bgoal);
        更新球赛比分

    15.void UPdatetime(int time);
        更新倒计时
		
	16.int GamePause(void);
		暂停游戏
	
	17.double CanSafePass(SVector2D pos1, SVector2D tar_pos, struct _TEAM * pOppteam);
		判断能否安全的传球到指定位置
	
	18.double GetDisver(SVector2D pos1, SVector2D pos2, SVector2D pos3);
		计算pos3到pos1与pos2连线的距离
	
	19.int FindPass(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);
		找到最佳传球目标ID
	
	20.int IsDanger(struct _PLAYER * pplayer, struct _TEAM * pOppteam, double range);
		 检测带球队员有无危险，是否需要传球
		 
	21.void PlayerControl(struct _PLAYER * pplayer);
		根据键盘消息控制球员速度
		
	22.void DrawGameBK(void);
		画球场背景
	
	23.void DrawScoreBoard(int Wgoal, int Bgoal);
		画计分板
	
	24.void DrawTimer(int Time);
		画计时板
	
	25.void SetPlayersPara(struct _PLAYER Wplayers[], struct _PLAYER Bplayers[], struct _FIELDPLAYER fieldplayers);
		初始化球员能力参数
	
	26.void SetAIMode(struct _PLAYER Bplayers[], int gamemode, int botmode);
		设置AI难度
	
	27.void GetKeyboardAct(char *key);
		获取键盘传入的传球、射门动作
	
History：        对本文件的修改请及时按如下格式记录在下面（每个修改单独列出）！
    Example：    Date：
                Author：
                Version：
                Modification：

1.  Date：2017/9/23
        Author：郭裕祺
        Version：1.0
        Modification：此前注释并未形成规范，此次重新梳理了一遍，将版本号定为1.0
2.  Date：2017/10/3
        Author：郭裕祺
        Version：1.1
        Modification：加入了16至21号函数
**********************************************************/




#ifndef _TOOL_H
#define _TOOL_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<bios.h>
#include<graphics.h>
#include<math.h>
#include"MultiKey.h"
#include"football.h"



/**********************************************************
Function：      ReBack

Description：   恢复背景功能函数。用于恢复被球员、足球覆盖过的背景白线

Calls：         Putpixel256 (SVGAmode.c)

Called By：     BallUpdate (football.c)
                PlayerUpdate (football.c)
                GkeeperUpdate (football.c)

Input：     int x               待恢复区域左上角点的x坐标
            int y               待恢复区域左上角点的y坐标
            int size_x          待恢复区域的宽度
            int size_y          待恢复区域的高度

Output：     None

Return：     None
Others：     None
**********************************************************/
void ReBack(int x, int y, int size_x, int size_y);

/**********************************************************
Function：      GetBall

Description：   用于判定球员做出抢球动作时是否能抢到球。

Calls：         Distance
				rand

Called By：     ReceivingBallExecute (football.c)
				ChaseBallExecute (football.c)

Input：    		struct _PLAYER * pplayer          指向球员的指针
				struct _BALL * pball              指向足球的指针

Output：     None

Return：     0 不能抢到球
             1 能抢到球

Others：     None
**********************************************************/
int GetBall(struct _PLAYER * pplayer, struct _BALL * pball);

/**********************************************************
Function：      Distance

Description：   计算并返回两个点之间的距离。

Calls：         sqrt

Called By： 		GetBall
					GetRecPlayer
					FindClosest
					ShootBall
					TendgoalExecute (football.c)
					ReturnhomeExecute (football.c)

Input：     SVector2D n1          第一个点
            SVector2D n2          第二个点

Output：     None

Return：     两个点之间的距离
Others：     None
**********************************************************/
double Distance(SVector2D n1, SVector2D n2);

/**********************************************************
Function：      GetDir

Description：   计算并返回一个向量对应的单位向量

Calls：         sqrt

Called By： BallUpdate (football.c)
			WaitExecute (football.c)
			DribbleExecute (football.c)
			SupportingAttackerExecute (football.c)
			TendgoalExecute (football.c)
			ReturnhomeExecute (football.c)

Input：     double x             向量的x分量
            double y             向量的y分量

Output：     None

Return：     给定向量对应的单位向量；若模为0，则返回（0,0）

Others：     None
**********************************************************/
SVector2D GetDir(double x, double y);

/**********************************************************
Function：      GetRecPlayer

Description：   计算得出一个合适的接球队员，并返回其队内编号

Calls：         abs
                Distance

Called By：     DribbleExecute (football.c)

Input：     	SVector2D ball_v           足球的速度
				SVector2D p_pos            传出足球的球员位置
				struct _TEAM * pOurteam    指向我方球队的指针

Output：     None

Return：     合适的接球队员的队内编号

Others：     None
**********************************************************/
int GetRecPlayer(SVector2D ball_v, SVector2D p_pos, struct _TEAM * pOurteam);

/**********************************************************
Function：     PlayerBorder

Description：   球员与边界的碰撞处理函数（完全非弹性碰撞）

Input：		struct _PLAYER * pplayer           指向球员的指针
            
Output：     None

Return：     None

Others：     此函数通过修改球员速度来将球员限制在屏幕的
				   固定范围内。
**********************************************************/
void PlayerBorder(struct _PLAYER * pplayer);

/**********************************************************
Function：     FindClosest

Description：   返回几个球员中离某个位置最近的一个,第3个参数可以将特定球员排除

Input：	   	struct _PLAYER * pplayer           指向球员的指针
			SVector2D pos                           给定的位置
			int exID                                       需要排除的球员ID，-1表示无
			
Output：     None

Return：     几个球员中离给定点最近的球员编号

Others：     None
**********************************************************/
int FindClosest(struct _PLAYER ** players, SVector2D pos, int exID);

/**********************************************************
Function：     ShootBall

Description：   射门辅助函数，计算得出球员射门时应给球的速度

Input：	   	struct _BALL * pball          			指向球的指针
			int TeamName                            队伍名
			
Output：     None

Return：     该给球的速度

Others：     None
**********************************************************/
SVector2D ShootBall(struct _BALL * pball, int TeamName);

/**********************************************************
Function：     CanShoot

Description：   用于判断球员朝向能否射门

Input：	    struct _PLAYER * pplayer           指向球员的指针
			int TeamName                          队伍名
			
Output：     None

Return：     1表示可以射门，0表示当前朝向无法射门

Others：     None
**********************************************************/
int CanShoot(struct _PLAYER * pplayer, int TeamName);

/**********************************************************
Function：     BallDec

Description：   用于检测球是否出界或进球

Input：	   	struct _BALL * pball          			指向球的指针
			
Output：     None

Return：     若出界返回1，若白队进球返回2，若蓝队进球返回3，
				   若以上情况都没发生返回0

Others：     None
**********************************************************/
int BallDec(struct _BALL * pball);

/**********************************************************
Function：     Restart

Description：   重新开球

Input：	   		struct _TEAM * pWteam             指向白队的指针
				struct _TEAM * pBteam			    指向蓝队的指针
				struct _BALL * pball          			指向球的指针
				struct _GOALKEEPER * Gkeepers     指向守门员的指针
			
Output：     None

Return：    None

Others：     让各个球员回到原位，重新开球
**********************************************************/
void Restart(struct _TEAM * pWteam, struct _TEAM * pBteam, struct _BALL * pball, struct _GOALKEEPER * Gkeepers);

/**********************************************************
Function：     DrawPlayer

Description：   画球员

Input：	   		int x            	 						球员左上角x坐标
				int y			    						球员左上角y坐标
				enum _dir Dir          			球员朝向
				unsigned char color     		球员颜色
			
Output：     None

Return：    None

Others：     None
**********************************************************/
void DrawPlayer(int x, int y, enum _dir Dir, unsigned char color);

/**********************************************************
Function：     DrawNum

Description：   画数字

Input：	   		int x            	 						数字左上角x坐标
				int y			    						数字左上角y坐标
				int size         						数字宽度
				unsigned char color     		数字颜色
				int num								给定数字（0~9）
			
Output：     None

Return：    None

Others：    数字的高度是宽度的两倍。
**********************************************************/
void DrawNum(int x, int y, int size, unsigned char color, int num);

/**********************************************************
Function：     Updategoal

Description：   更新比分

Input：	   		int Wgoal            	 白队得分
				int Bgoal		    		 蓝队得分
			
Output：     None

Return：    None

Others：    None
**********************************************************/
void Updategoal(int Wgoal, int Bgoal);

/**********************************************************
Function：     UPdatetime

Description：   更新倒计时

Input：	   		int time            	 剩余时间（单位S）
			
Output：     None

Return：    None

Others：    None
**********************************************************/
void UPdatetime(int time);

/**********************************************************
Function：      GamePause

Description：   暂停游戏界面，子选项可以恢复比赛或退出

Input：      None

Output：     None

Return：      1  用户想退出游戏
			  0  用户想恢复游戏

Others：     None
**********************************************************/
int GamePause(void);

/**********************************************************
Function：     CanSafePass

Description：   判定能否安全传球

Input：	   		SVector2D pos1            	           当前位置
				SVector2D tar_pos						目标位置
				struct _TEAM * pOppteam			指向对方球员的指针
			
Output：     None

Return：   若可能会被对方球员截住球，则返回0；
				 否则返回离传球线最近球员离传球线的距离

Others：    None
**********************************************************/
double CanSafePass(SVector2D pos1, SVector2D tar_pos, struct _TEAM * pOppteam);

/**********************************************************
Function：     GetDisver

Description：   算点到直线距离

Input：	   		SVector2D pos1            	           位置1
				SVector2D pos2						   位置2
				SVector2D pos3						   位置3
			
Output：     None

Return：   pos3到pos1与pos2连线的距离

Others：    None
**********************************************************/
double GetDisver(SVector2D pos1, SVector2D pos2, SVector2D pos3);

/**********************************************************
Function：     FindPass

Description：   找到合适的传球目标，并返回其队内编号

Input：	   		struct _PLAYER * pplayer            	           指向持球队员的指针
				struct _TEAM * pOurteam						   指向我方队伍的指针
				struct _TEAM * pOppteam					       指向对方队伍的指针
			
Output：     None

Return：   最合适的传球目标ID

Others：    None
**********************************************************/
int FindPass(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);

/**********************************************************
Function：    IsDanger

Description：   检测带球队员有无危险，是否需要传球

Input：	   		struct _PLAYER * pplayer            	           指向持球队员的指针
				struct _TEAM * pOppteam					           指向对方队伍的指针
				double range                                       判定范围半径
			
Output：     None

Return：   0表示无危险，1表示有危险

Others：    None
**********************************************************/
int IsDanger(struct _PLAYER * pplayer, struct _TEAM * pOppteam, double range);

/**********************************************************
Function：    PlayerControl

Description：   根据键盘消息控制球员速度

Input：	   		struct _PLAYER * pplayer            	           指向持球队员的指针
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void PlayerControl(struct _PLAYER * pplayer);

/**********************************************************
Function：    DrawGameBK

Description：   画球场背景

Input：	   	None
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void DrawGameBK(void);

/**********************************************************
Function：    DrawScoreBoard

Description：   画计分板

Input：	   	None
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void DrawScoreBoard(int Wgoal, int Bgoal);

/**********************************************************
Function：    DrawTimer

Description：   画计时板

Input：	   	None
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void DrawTimer(int Time);

/**********************************************************
Function：    SetPlayersPara

Description：   初始化球员能力参数

Input：	   	struct _PLAYER Wplayers[]                   白队球员数组
			struct _PLAYER Bplayers[]                   蓝队球员数组
			struct _FIELDPLAYER fieldplayers            上场球员信息
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void SetPlayersPara(struct _PLAYER Wplayers[], struct _PLAYER Bplayers[], struct _FIELDPLAYER fieldplayers);

/**********************************************************
Function：    SetAIMode

Description：   设置AI难度

Input：	   	struct _PLAYER Bplayers[]                   蓝队球员数组
			int gamemode                                游戏模式，0单人，1双人
			int botmode                                 AI难度，0到2共三个难度
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void SetAIMode(struct _PLAYER Bplayers[], int gamemode, int botmode);

/**********************************************************
Function：    GetKeyboardAct

Description：   获取键盘传入的传球、射门动作

Input：	   	char *key         动作标记      
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void GetKeyboardAct(char *key);

#endif