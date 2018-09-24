/**********************************************************
Copyright (C) 2017 郭裕祺 黄杰豪.
File name:      football.h
Author:         郭裕祺
Version：        1.1
Date：           2017/10/25
Description：    实况足球游戏核心部分头文件。

                对本文件的修改请及时按如下格式记录在下面（每个修改单独列出）！

Function List：
    1.void BallUpdate(struct _BALL * pball, struct _TEAM * pWteam, struct _TEAM * pBteam, struct _GOALKEEPER * Gkeepers);
		足球更新函数
	
	2.void TeamUpdate(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam);
		球队更新函数
		
	3.void PlayerUpdate(struct _PLAYER * self, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);
		球员更新函数
		
	4.void PlayerChangeState(struct _PLAYER * self, struct PLAYER_STATE * pnewstate, struct _TEAM * pOurteam, struct _TEAM * pOppteam);
		球员改变状态函数
		
	5.void TeamChangeState(struct _TEAM * self, struct TEAM_STATE * pnewstate);
		球队改变状态函数
		
	6.void GkeeperUpdate(struct _GOALKEEPER * self, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball); 
		守门员更新函数
		
	7.void GkeeperChangeState(struct _GOALKEEPER * self, struct PLAYER_STATE * pnewstate, struct _TEAM * pOurteam, struct _TEAM * pOppteam);
		守门员改变状态函数
	
	****************球队状态实现函数******************	
	8.void AttackExecute(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam);
		处于进攻状态，每个时间步都要执行
		
	9.void DefendEnter(struct _TEAM * self); 
		只在进入防守状态时执行一次
		
	10.void DefendExecute(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam);
		处于防守状态，每个时间步都要执行
	
	****************踢球的球员状态实现函数******************	
	11.void WaitEnter(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);
		只在进入待命状态时执行一次
		
	12.void WaitExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);
		处于待命状态，每个时间步都要执行
		
	13.void ReceivingBallExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball); 
		处于接球状态，每个时间步都要执行
		
	14.void ReceivingBallExit(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam); 
		只在退出接球状态时执行一次
		
	15.void DribbleExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);
		处于带球状态，每个时间步都要执行
		
	16.void ChaseBallExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);
		处于追球状态，每个时间步都要执行
		
	17.void SupportingAttackerExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball); 
		处于接应状态，每个时间步都要执行
		
	18.void SupportingAttackerExit(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);
		只在退出接应状态时执行一次
	
	****************守门员状态实现函数******************		
	19.void TendgoalExecute(struct _GOALKEEPER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);
		处于巡逻状态，每个时间步都要执行
		
	20.void ReturnhomeExecute(struct _GOALKEEPER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball); 
		处于回位状态，每个时间步都要执行
	
	****************初始化函数******************		
	21.void InitPlayer(struct _PLAYER * pplayer, int ID);
		初始化球员
		
	22.void InitBall(struct _BALL * pball);
		初始化足球
		
	23.void InitTeam(struct _TEAM * pteam, struct _PLAYER * players, int name);
		初始化球队
		
	24.void InitGkeeper(struct _GOALKEEPER * pplayer, int ID);
		初始化守门员
		
	****************驱动函数******************
	25._result FootballGame(struct _FIELDPLAYER fieldplayers, int gamemode, int botmode);
		驱动函数，负责调用其它部分
		
		
History：        对本文件的修改请及时按如下格式记录在下面（每个修改单独列出）！
    Example：    Date：
                Author：
                Version：
                Modification：

1.  Date：2017/9/23
        Author：郭裕祺
        Version：1.0
        Modification：此前注释并未形成规范，此次重新梳理了一遍，将版本号定为1.0

2.  Date：2017/10/25
        Author：郭裕祺
        Version：1.1
        Modification：修复了一些状态跳转逻辑上的小错误，调整了一下抢球的判定方式

**********************************************************/
#ifndef FOOTBALL_H
#define FOOTBALL_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<bios.h>
#include<graphics.h>
#include<math.h>
#include"SVGAmode.h"

//刷新延迟(ms)
#define TimeStep 12
//球场4条边界
#define Border_UP 200
#define Border_DOWN 690
#define Border_LEFT 60
#define Border_RIGHT 960

//球门数据
#define GoalUP 395
#define GoalDOWN 495

#define BallDeceleration 0.35        //足球的减速度
#define PlayerVelocity 2.5           //球员移速
#define BallPassV 2.0               //足球被传球时的速度参数（一个因子）
#define MaxShootL 250.0              //最大可控射球距离
#define GetballRate 0.8              //断球成功率
#define AccRate 1.5                  //加速倍率
#define AccTime 1.5                    //加速模式持续时间（单位 S）
#define AccCD 3.0                      //加速模式CD（单位 S）
#define Botrate 1.1
#define FPS 40                         //帧率

/****************结构定义部分*******************/
typedef struct _RESULT
{
    int Wscore;
    int Bscore;
}_result;

typedef struct _FIELDPLAYER
{
    int WplayerID[4];              //白队上场队员ID
    int Wstars[4];                 //白队上场队员的星级
    int BplayerID[4];              //蓝队上场队员ID
    int Bstars[4];                 //蓝队上场队员的星级
	int Tool;                      //使用的道具编号，0为未使用
}_fieldplayer;

enum _dir
{
    Up,                 //上
    Down,               //下
    Right,              //右
    Left,               //左
    RU,                 //右上
    LU,                 //左上
    RD,                 //右下
    LD                  //左下
};

//2维矢量，单位1像素
typedef struct _sv2d          
{
    double x, y;
}SVector2D;

typedef struct PLAYER_STATE
{
    void (*Enter) (struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);                      //进入状态时调用
    void (*Execute) (struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);          //处于状态时调用
    void (*Exit) (struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);                       //退出状态时调用
} player_state;
//球队状态
typedef struct TEAM_STATE
{
    void (*Enter) (struct _TEAM * self);                      //进入状态时调用
    void (*Execute) (struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam);                   //处于状态时调用
    void (*Exit) (struct _TEAM * self);                      //退出状态时调用
} team_state;

typedef struct _TEAM
{
    struct _PLAYER * player[4];
    int ReceivingPlayer;              //接球队员
    int ClosestPlayer;                //离球最近的队员
    int ControllingPlayer;            //控球队员,  由ball更新函数来更改
    int SupportingPlayer;             //接应队员
    int IsControllBall;               //是否控球
    SVector2D DefendPos[4];           //防守点位
    SVector2D AttackPos[4];           //进攻点位
    int BestPos;                      //最佳接应点编号
    SVector2D SupPos[30];             //备选接应点
    int name;                         //用于区分两队

    struct TEAM_STATE * pcurrentstate;               //指向当前状态
    struct TEAM_STATE Attack, Defend;                //状态，进攻，防守
} _team;

typedef struct _BALL
{
    SVector2D mv_velocity;        //速度
    SVector2D mv_pos;             //位置
    SVector2D mv_oldpos;          //上一次的位置
    int GameMode;

    unsigned int TimeCount;       //时间记录器

    int Owner;                    //持球者的ID，若无人持球，则设为-1
                                  //0到3是白队球员，4到7是蓝队球员，8是白队守门员，9是蓝队守门员

} _ball;

typedef struct _PLAYER
{
    SVector2D mv_velocity;        //速度
    SVector2D mv_pos;             //位置
    enum _dir Dir;             //朝向，8种朝向
    int ID;                       //编号
    double Max_v;              //最大速度
    double get_rate;           //断球成功率
    double acc_rate;          //加速倍率
    double acc_time;             //加速持续时间
    int acctimecounter;       //加速时间累加器
    int cdmark;               //CD计数器
    double acc_cd;               //加速冷却时间
    struct PLAYER_STATE * pcurrentstate;                      //指向当前状态
    struct PLAYER_STATE Wait, ReceivingBall, Dribble,
                        ChaseBall, SupportingAttacker;            //5种状态
} _player;

typedef struct _GOALKEEPER
{
    SVector2D mv_velocity;        //速度
    SVector2D mv_pos;             //位置
    enum _dir Dir;             //朝向，8种朝向
    int ID;                       //编号
    struct PLAYER_STATE * pcurrentstate;                      //指向当前状态
    struct PLAYER_STATE Tendgoal, Returnhome;            //2种状态，守门，回位开球
} _goalkeeper;

/****************函数原型部分*******************/

/**********************************************************
Function：      BallUpdate

Description：   足球更新函数

Input：     struct _BALL * pball               指向足球的指针
            struct _TEAM * pWteam              指向白队的指针
            struct _TEAM * pBteam              指向蓝队的指针
            struct _GOALKEEPER * Gkeepers      指向守门员的指针

Output：     None

Return：     None
Others：     足球更新函数在驱动函数的主循环中执行。若球未被任何
			球员持有，则根据当前足球的速度更新足球的位置，足球会
			以一定的减速度减速直到速度为零，以此模拟真实情况下的
			阻力；若球被球员持有，则足球的位置由持球的球员位置决定。
			此外，封装于足球结构中的时间计数器也会进行更新。
**********************************************************/
void BallUpdate(struct _BALL * pball, struct _TEAM * pWteam, struct _TEAM * pBteam, struct _GOALKEEPER * Gkeepers);

/**********************************************************
Function：      TeamUpdate

Description：   球队更新函数

Input：     struct _TEAM * self                指向自身球队的指针
            struct _BALL * pball               指向足球的指针
            struct _TEAM * pOppteam			   指向对方球队的指针

Output：     None

Return：     None
Others：     球队更新函数在驱动函数的主循环中执行。此函数通过
			球队结构内的指向球队当前状态的指针调用该状态的Execute
			函数。
**********************************************************/
void TeamUpdate(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam);

/**********************************************************
Function：      PlayerUpdate

Description：   球员更新函数

Input：     struct _PLAYER * self              指向自身代表的球员的指针
			char key                           传球/射门动作标志量
			struct _TEAM * pOurteam			   指向自身所在球队的指针
			struct _TEAM * pOppteam			   指向对方球队的指针
            struct _BALL * pball               指向足球的指针
            
Output：     None

Return：     None
Others：     球员更新函数在驱动函数的主循环中执行。此函数通过
			球员结构内的指向球员当前状态的指针调用该状态的Execute
			函数，更新球员速度等参数，根据速度来更新球员的新位置、
			朝向。此外，还要给玩家控制的球员做标记、绘制球员头像旁
			的体力条。
**********************************************************/
void PlayerUpdate(struct _PLAYER * self, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);       

/**********************************************************
Function：      PlayerChangeState

Description：   球员改变状态函数

Input：     struct _PLAYER * self              指向自身代表的球员的指针
			struct PLAYER_STATE * pnewstate    指向新状态的指针
			struct _TEAM * pOurteam			   指向自身所在球队的指针
			struct _TEAM * pOppteam			   指向对方球队的指针
            
Output：     None
Return：     None
Others：     球员状态需要改变时应通过调用此函数实现。此函数通过
			函数指针（检测非空后）先执行球员当前状态的Exit函数，
			然后给指向球员当前状态的指针赋新的值，接着执行新状态
			的Enter函数。
**********************************************************/
void PlayerChangeState(struct _PLAYER * self, struct PLAYER_STATE * pnewstate, struct _TEAM * pOurteam, struct _TEAM * pOppteam);   

/**********************************************************
Function：      TeamChangeState

Description：   球队改变状态函数

Input：     struct _TEAM * self                指向自身球队的指针
			struct TEAM_STATE * pnewstate      指向新状态的指针

Output：     None
Return：     None
Others：     球队状态需要改变时应通过调用此函数实现。此函数通过
			函数指针（检测非空后）先执行球队当前状态的Exit函数，
			然后给指向球队当前状态的指针赋新的值，接着执行新状态
			的Enter函数。
**********************************************************/
void TeamChangeState(struct _TEAM * self, struct TEAM_STATE * pnewstate);   

/**********************************************************
Function：      GkeeperUpdate

Description：   守门员更新函数

Input：     struct _PLAYER * self              指向自身代表的球员的指针
			char key                           传球/射门动作标志量
			struct _TEAM * pOurteam			   指向自身所在球队的指针
			struct _TEAM * pOppteam			   指向对方球队的指针
            struct _BALL * pball               指向足球的指针
            
Output：     None

Return：     None
Others：     守门员更新函数在驱动函数的主循环中执行。此函数通过
			球员结构内的指向球员当前状态的指针调用该状态的Execute
			函数，更新守门员速度等参数，根据速度来更新守门员的新位置。
**********************************************************/
void GkeeperUpdate(struct _GOALKEEPER * self, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);       

/**********************************************************
Function：      GkeeperChangeState

Description：   守门员改变状态函数

Input：     struct _GOALKEEPER * self              	指向自身代表的球员的指针
			struct PLAYER_STATE * pnewstate    		指向新状态的指针
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
            
Output：     None
Return：     None
Others：     守门员状态需要改变时应通过调用此函数实现。此函数通过
			函数指针（检测非空后）先执行守门员当前状态的Exit函数，
			然后给指向守门员当前状态的指针赋新的值，接着执行新状态
			的Enter函数。
**********************************************************/
void GkeeperChangeState(struct _GOALKEEPER * self, struct PLAYER_STATE * pnewstate, struct _TEAM * pOurteam, struct _TEAM * pOppteam);  


   /*球队状态部分*/
//Attack部分
/**********************************************************
Function：      AttackExecute

Description：   球队处于进攻状态，每个时间步都要执行

Input：     struct _TEAM * self                指向自身球队的指针
            struct _BALL * pball               指向足球的指针
            struct _TEAM * pOppteam			   指向对方球队的指针
            
Output：     None
Return：     None

Others：     球队处于进攻状态时，需要每隔一段时间根据我方带球队员位置、敌方球员位置
			  计算出最佳的接应位置，并将离最佳接应位置最近的球员设为接应队员。
			  若我方队伍不再控球，则调用TeamChangeState()函数跳转至防守状态
**********************************************************/
void AttackExecute(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam);   
//Defend部分
/**********************************************************
Function：      DefendEnter

Description：   球队进入防守状态，只在进入时执行一次

Input：     struct _TEAM * self                指向自身球队的指针
                     
Output：     None
Return：     None

Others：     球队进入防守状态时，将接应队员、控球队员、接球队员
			都设为-1
**********************************************************/
void DefendEnter(struct _TEAM * self);                    

/**********************************************************
Function：      DefendExecute

Description：   球队处于防守状态，每个时间步都要执行

Input：     struct _TEAM * self                指向自身球队的指针
            struct _BALL * pball               指向足球的指针
            struct _TEAM * pOppteam			   指向对方球队的指针
            
Output：     None
Return：     None

Others：     球队处于防守状态时，需要实时计算离球最近的队员。
			若我方队伍开始控球，则调用TeamChangeState()函数跳转至进攻状态
**********************************************************/
void DefendExecute(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam);     



   /*球员状态部分*/
//Wait部分
/**********************************************************
Function：      WaitEnter

Description：   球员进入待命状态，只在进入时执行一次

Input：     struct _PLAYER * pplayer              	指向自身代表的球员的指针
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
            
Output：     None
Return：     None

Others：     球员进入待命状态时，将球员速度归零
**********************************************************/
void WaitEnter(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);

/**********************************************************
Function：      WaitExecute

Description：   球员处于待命状态，每个时间步都要执行

Input：     struct _PLAYER * pplayer              	指向自身代表的球员的指针
			char key								传球/射门动作标志量
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
			struct _BALL * pball               		指向足球的指针
            
Output：     None
Return：     None

Others：     球员处于待命状态时，我方队伍可能控球，也可能不控球；
			若我方队伍不控球，则球员移向预设的防守位置，后卫在敌方
			深入时会收缩防线；若我方队伍控球，则球员移向预设的进攻
			位置。若检测到自己被设为接应/接球/追球队员，则调用改变
			状态函数跳转至相应的状态。
**********************************************************/                     
void WaitExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);   
       
//ReceivingBall部分
/**********************************************************
Function：      ReceivingBallExecute

Description：   球员处于接球状态，每个时间步都要执行

Input：     struct _PLAYER * pplayer              	指向自身代表的球员的指针
			char key								传球/射门动作标志量
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
			struct _BALL * pball               		指向足球的指针
            
Output：     None
Return：     None

Others：     球员处于接球状态时，若是玩家方则由玩家控制该球员，
			若是电脑方则由程序自动控制球员去接球。当成功接到球
			时，跳转至带球状态；若球被对方抢去，则跳转至待命状态
**********************************************************/
void ReceivingBallExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);

/**********************************************************
Function：      ReceivingBallExit

Description：   球员退出接球状态，只在退出时执行一次

Input：     struct _PLAYER * pplayer              	指向自身代表的球员的指针
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
            
Output：     None
Return：     None

Others：     球员退出接球状态时，将接球队员标志设为-1
**********************************************************/         
void ReceivingBallExit(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);   
                   
//Dribble部分
/**********************************************************
Function：      DribbleExecute

Description：   球员处于带球状态，每个时间步都要执行

Input：     struct _PLAYER * pplayer              	指向自身代表的球员的指针
			char key								传球/射门动作标志量
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
			struct _BALL * pball               		指向足球的指针
            
Output：     None
Return：     None

Others：     球员处于带球状态时，若是玩家方则由玩家控制该球员，
			玩家控制球员移动、加速、传球、射门；若是电脑方则由
			程序自动控制球员。传球时程序会自动计算出最佳的传球
			目标并给球一个合适的速度，之后将相应的标志量设置好；
			射门时程序会随机的以球门线上一点为目标，给球一个合
			适的速度射门。传球/射门动作执行完或者被对方球员抢去
			球，则跳转至待命状态。
**********************************************************/
void DribbleExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);     
     
//ChaseBall部分
/**********************************************************
Function：      ChaseBallExecute

Description：   球员处于追球状态，每个时间步都要执行

Input：     struct _PLAYER * pplayer              	指向自身代表的球员的指针
			char key								传球/射门动作标志量
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
			struct _BALL * pball               		指向足球的指针
            
Output：     None
Return：     None

Others：     球员处于追球状态时，若是玩家方则由玩家控制该球员，
			玩家控制球员移动、加速、抢球；若是电脑方则由
			程序自动控制球员。若成功抢到球，则跳转至带球状态，
			并将球队结构中标志控球与否的量设为1
**********************************************************/
void ChaseBallExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);     
     
//SupportingAttacker部分
/**********************************************************
Function：      SupportingAttackerExecute

Description：   球员处于接应状态，每个时间步都要执行

Input：     struct _PLAYER * pplayer              	指向自身代表的球员的指针
			char key								传球/射门动作标志量
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
			struct _BALL * pball               		指向足球的指针
            
Output：     None
Return：     None

Others：     球员处于接应状态时，球员有程序自动控制，球员会
			移向最佳接应位置并等待队员传球。若到达指定位置或
			自己不再是接应队员，则跳转至待命状态；若带球的队
			员将球传出且自己被设为接球队员，则跳转至接球状态。
**********************************************************/
void SupportingAttackerExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);          

/**********************************************************
Function：      SupportingAttackerExit

Description：   球员退出接应状态，只在退出时执行一次

Input：     struct _PLAYER * pplayer              	指向自身代表的球员的指针
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
            
Output：     None
Return：     None

Others：     球员退出接应状态时，将最佳接应点设为0，将接应队员标志设为-1
**********************************************************/
void SupportingAttackerExit(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam);

    /*守门员状态部分*/
//Tendgoal部分
/**********************************************************
Function：      TendgoalExecute

Description：   守门员处于巡逻状态，每个时间步都要执行

Input：     struct _GOALKEEPER * pplayer            指向自身代表的球员的指针
			char key								传球/射门动作标志量
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
			struct _BALL * pball               		指向足球的指针
            
Output：     None
Return：     None

Others：     守门员处于巡逻状态时，守门员会根据球的位置不停移动
			保持挡在球与己方球门之间，程序中设置了一个随机量来使
			守门员犯一点错误。若守门员截住球，则跳转至回位状态
**********************************************************/
void TendgoalExecute(struct _GOALKEEPER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);          

//Returnhome部分
/**********************************************************
Function：      ReturnhomeExecute

Description：   守门员处于回位状态，每个时间步都要执行

Input：     struct _GOALKEEPER * pplayer            指向自身代表的球员的指针
			char key								传球/射门动作标志量
			struct _TEAM * pOurteam			   		指向自身所在球队的指针
			struct _TEAM * pOppteam			   		指向对方球队的指针
			struct _BALL * pball               		指向足球的指针
            
Output：     None
Return：     None

Others：     守门员处于回位状态时，守门员会带着球回到球门中央并
			等待对方球员离开己方半场，然后将球传给合适的己方队员，
			之后跳转至巡逻状态
**********************************************************/
void ReturnhomeExecute(struct _GOALKEEPER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);         



    /*初始化函数*/
	
/**********************************************************
Function：      InitPlayer

Description：   球员初始化函数

Input：     struct _PLAYER * pplayer       指向自身代表的球员的指针
			int ID                         球员ID

Output：     None
Return：     None
Others：     None
**********************************************************/
void InitPlayer(struct _PLAYER * pplayer, int ID);

/**********************************************************
Function：      InitBall

Description：   足球初始化函数

Input：     struct _BALL * pball       指向足球的指针

Output：     None
Return：     None
Others：     None
**********************************************************/
void InitBall(struct _BALL * pball);

/**********************************************************
Function：      InitTeam

Description：   球队初始化函数

Input：     struct _TEAM * pteam             指向自身球队的指针
			struct _PLAYER * players         指向球员组的指针
			int name                         队名标记

Output：     None
Return：     None
Others：     None
**********************************************************/
void InitTeam(struct _TEAM * pteam, struct _PLAYER * players, int name);

/**********************************************************
Function：      InitGkeeper

Description：   守门员初始化函数

Input：     struct _GOALKEEPER * pplayer       指向自身代表的球员的指针
			int ID                             球员ID

Output：     None
Return：     None
Others：     None
**********************************************************/
void InitGkeeper(struct _GOALKEEPER * pplayer, int ID);



/**********************************************************
Function：      FootballGame

Description：   驱动函数，负责调用其它部分

Input：     struct _FIELDPLAYER fieldplayers       储存上场球员信息的结构
			int gamemode                           游戏模式，0单人，1多人
			int botmode                            AI难度级别，0到2共三级

Output：     None
Return：     比赛结果，封装在_result结构中
Others：     None
**********************************************************/
_result FootballGame(struct _FIELDPLAYER fieldplayers, int gamemode, int botmode);






#endif