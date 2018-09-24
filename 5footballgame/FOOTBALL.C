/**********************************************************
Copyright (C) 2017 郭裕祺 黄杰豪.
File name:      football.c
Author:         郭裕祺
Version：        1.1
Date：           2017/9/23
Description：    实况足球游戏核心部分源文件。

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
	
	****************守门员状态实现函数******************		
	18.void TendgoalExecute(struct _GOALKEEPER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball);
		处于巡逻状态，每个时间步都要执行
		
	19.void ReturnhomeExecute(struct _GOALKEEPER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball); 
		处于回位状态，每个时间步都要执行
	
	****************初始化函数******************		
	20.void InitPlayer(struct _PLAYER * pplayer, int ID);
		初始化球员
		
	21.void InitBall(struct _BALL * pball);
		初始化足球
		
	22.void InitTeam(struct _TEAM * pteam, struct _PLAYER * players, int name);
		初始化球队
		
	23.void InitGkeeper(struct _GOALKEEPER * pplayer, int ID);
		初始化守门员
		
	****************驱动函数******************
	24._result FootballGame(struct _FIELDPLAYER fieldplayers, int gamemode, int botmode);
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

#include<math.h>
#include"football.h"
#include"MultiKey.h"
#include"tool.h"

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
void BallUpdate(struct _BALL * pball, struct _TEAM * pWteam, struct _TEAM * pBteam, struct _GOALKEEPER * Gkeepers)
{
    _player * pConPlayer = NULL;                       //指向持球队员，初始化为空
    pball->mv_oldpos.x = pball->mv_pos.x;              //更新oldpos
    pball->mv_oldpos.y = pball->mv_pos.y;
    //重画背景
    Circlefill((int)(pball->mv_pos.x), (int)(pball->mv_pos.y), 10, 0x19);
    ReBack((int)(pball->mv_oldpos.x)-10, (int)(pball->mv_oldpos.y)-10, 20, 20);

    //更新位置
    if(-1 == pball->Owner)                            //球未被持有
    {
        pball->mv_pos.x += pball->mv_velocity.x;                    //换算常数定为1
        pball->mv_pos.y += pball->mv_velocity.y;
        pball->mv_velocity.x -= BallDeceleration * GetDir(pball->mv_velocity.x, pball->mv_velocity.y).x;     
        pball->mv_velocity.y -= BallDeceleration * GetDir(pball->mv_velocity.x, pball->mv_velocity.y).y;
    }
    else                                            //球被持有
    {
		pball->mv_velocity.x = 0.0;            //速度设为0，跟着控球队员跑，自己的速度不参与运算
        pball->mv_velocity.y = 0.0;
		
        if(pball->Owner < 4)                    //持有者为白队
        {
            pConPlayer = pWteam->player[pball->Owner];
        }
        else if(pball->Owner < 8)               //持有者为蓝队
        {
            pConPlayer = pBteam->player[pball->Owner - 4];
        }
        else                                    //持有者为守门员
        {
            pConPlayer = &Gkeepers[pball->Owner - 8];
        }
		switch(pConPlayer->Dir)
		{
			case Right: pball->mv_pos.x = pConPlayer->mv_pos.x + 20.0;
						pball->mv_pos.y = pConPlayer->mv_pos.y;
						break;
			case Left:  pball->mv_pos.x = pConPlayer->mv_pos.x - 20.0;
						pball->mv_pos.y = pConPlayer->mv_pos.y;
						break;
			case Up:    pball->mv_pos.x = pConPlayer->mv_pos.x;
						pball->mv_pos.y = pConPlayer->mv_pos.y - 20.0;
						break;
			case Down:  pball->mv_pos.x = pConPlayer->mv_pos.x;
						pball->mv_pos.y = pConPlayer->mv_pos.y + 20.0;
						break;
			case RU:    pball->mv_pos.x = pConPlayer->mv_pos.x + 14.0;
						pball->mv_pos.y = pConPlayer->mv_pos.y - 14.0;
						break;
			case RD:    pball->mv_pos.x = pConPlayer->mv_pos.x + 14.0;
						pball->mv_pos.y = pConPlayer->mv_pos.y + 14.0;
						break;
			case LU:    pball->mv_pos.x = pConPlayer->mv_pos.x - 14.0;
						pball->mv_pos.y = pConPlayer->mv_pos.y - 14.0;
						break;
			case LD:    pball->mv_pos.x = pConPlayer->mv_pos.x - 14.0;
						pball->mv_pos.y = pConPlayer->mv_pos.y + 14.0;
						break;
		}

    }

    //更新时间记录器
    pball->TimeCount++;
    if(pball->TimeCount > 2500)     //超过2500，则重新开始记录
    {
        pball->TimeCount = pball->TimeCount % FPS;
    }

    //画球
    Putbmp256((int)(pball->mv_pos.x) - 8, (int)(pball->mv_pos.y) - 8, "bmp\\ball.bmp");
}

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
void TeamUpdate(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam)
{
    if(self->pcurrentstate != NULL)
    {
        self->pcurrentstate->Execute(self, pball, pOppteam);
    }
}

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
void PlayerUpdate(struct _PLAYER * self, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{

    float acc = 0.0;        //临时变量，用于表示体力条百分比
    //加速模式CD
    if(self->ID < 4 && self->cdmark > 0)
    {
        if(!KeyPress(KEY_LSHIFT))
        {
            self->cdmark--;
        }
        else if(self->pcurrentstate != &self->Dribble && self->pcurrentstate != &self->ReceivingBall &&  self->pcurrentstate != &self->ChaseBall)
        {
            self->cdmark--;
        }
    }
    else if(self->cdmark > 0)
    {
        if(!KeyPress(KEY_CONTROL))
        {
            self->cdmark--;
        }
        else if(self->pcurrentstate != &self->Dribble && self->pcurrentstate != &self->ReceivingBall &&  self->pcurrentstate != &self->ChaseBall)
        {
            self->cdmark--;
        }
    }

    //执行当前状态
    if(self->pcurrentstate != NULL)
    {
        self->pcurrentstate->Execute(self, key, pOurteam, pOppteam, pball);
    }

    //边界碰撞检测
    //若下一帧将超出边界，则削减速度
    PlayerBorder(self);

    if(self->mv_velocity.x != 0.0 || self->mv_velocity.y != 0.0)                   //球员位置发生改变
    {
        //重画背景
        Bar( (int)(self->mv_pos.x)-21, (int)(self->mv_pos.y)-21, (int)(self->mv_pos.x)+21, (int)(self->mv_pos.y)+21, 0x19);
        ReBack((int)(self->mv_pos.x)-21, (int)(self->mv_pos.y)-21, 42, 42);
        self->mv_pos.x += self->mv_velocity.x;
        self->mv_pos.y += self->mv_velocity.y;

        //更新朝向
        if(self->mv_velocity.x > 0.0 && 0.0 == self->mv_velocity.y)          //右
        {
            self->Dir = Right;
        }
        else if (self->mv_velocity.x < 0.0 && 0.0 == self->mv_velocity.y)    //左
        {
            self->Dir = Left;
        }
        else if (0.0 == self->mv_velocity.x && self->mv_velocity.y > 0.0)    //下
        {
            self->Dir = Down;
        }
        else if (0.0 == self->mv_velocity.x && self->mv_velocity.y < 0.0)    //上
        {
            self->Dir = Up;
        }
        else if (self->mv_velocity.x > 0.0 && self->mv_velocity.y > 0.0)     //右下
        {
            self->Dir = RD;
        }
        else if (self->mv_velocity.x > 0.0 && self->mv_velocity.y < 0.0)    //右上
        {
            self->Dir = RU;
        }
        else if (self->mv_velocity.x < 0.0 && self->mv_velocity.y > 0.0)    //左下
        {
            self->Dir = LD;
        }
        else if (self->mv_velocity.x < 0.0 && self->mv_velocity.y < 0.0)    //左上
        {
            self->Dir = LU;
        }

        if(0 == pOurteam->name)                                               //白队
        {
            //画球员白色
            DrawPlayer((int)(self->mv_pos.x),(int)(self->mv_pos.y), self->Dir, 0XF6);
        }
        else if(1 == pOurteam->name)                                             //蓝队
        {
            //画球员
            DrawPlayer((int)(self->mv_pos.x),(int)(self->mv_pos.y), self->Dir, LIGHTBLUE);
        }
    }
    else                                             //球员位置没有变
    {
        Bar( (int)(self->mv_pos.x)-21, (int)(self->mv_pos.y)-21, (int)(self->mv_pos.x)+21, (int)(self->mv_pos.y)+21, 0x19);
        if(0 == pOurteam->name)
        {
            DrawPlayer((int)(self->mv_pos.x),(int)(self->mv_pos.y), self->Dir, 0XF6);
        }
        else
        {
            DrawPlayer((int)(self->mv_pos.x),(int)(self->mv_pos.y), self->Dir, LIGHTBLUE);
        }
    }

    //给玩家控制的球员做个标记
    if(&self->Dribble == self->pcurrentstate || &self->ChaseBall == self->pcurrentstate || &self->ReceivingBall == self->pcurrentstate )
    {

        if(0 == pball->GameMode && 0 == pOurteam->name)                   //单人模式
        {
            Circlefill((int)(self->mv_pos.x), (int)(self->mv_pos.y), 5, RED);
        }
        else if (1 == pball->GameMode)
        {
            Circlefill((int)(self->mv_pos.x), (int)(self->mv_pos.y), 5, RED);
        }
    }

    //画加速模式体力条
    if(0 == self->cdmark)
    {
        acc = self->acctimecounter / (FPS*self->acc_time);
    }
    else
    {
        acc = self->cdmark / (FPS*self->acc_cd);
    }
    if(self->ID < 4)
    {
        Bar(168 + 130*(self->ID%2), 25 + 95*(self->ID/2), 176 + 130*(self->ID%2), 90 + 95*(self->ID/2), 0X19);
        Bar(168 + 130*(self->ID%2), 25 + 95*(self->ID/2) + acc*65, 176 + 130*(self->ID%2), 90 + 95*(self->ID/2), RED);
    }
    else
    {
        Bar(808 + 130*((self->ID-4)%2), 25 + 95*((self->ID-4)/2), 816 + 130*((self->ID-4)%2), 90 + 95*((self->ID-4)/2), 0X19);
        Bar(808 + 130*((self->ID-4)%2), 25 + 95*((self->ID-4)/2) + acc*65, 816 + 130*((self->ID-4)%2), 90 + 95*((self->ID-4)/2), RED);
    }
}


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
void PlayerChangeState(struct _PLAYER * self, struct PLAYER_STATE * pnewstate, struct _TEAM * pOurteam, struct _TEAM * pOppteam)
{
    if(self->pcurrentstate->Exit != NULL)
    {
        self->pcurrentstate->Exit(self, pOurteam, pOppteam);
    }

    self->pcurrentstate = pnewstate;
    if(self->pcurrentstate->Enter != NULL)
    {
        self->pcurrentstate->Enter(self, pOurteam, pOppteam);
    }
}

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
void TeamChangeState(struct _TEAM * self, struct TEAM_STATE * pnewstate)
{
    if(self->pcurrentstate->Exit !=NULL)
    {
        self->pcurrentstate->Exit(self);
    }

    self->pcurrentstate = pnewstate;

    if(self->pcurrentstate->Enter !=NULL)
    {
        self->pcurrentstate->Enter(self);
    }
}

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
void GkeeperUpdate(struct _GOALKEEPER * self, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{
    if(self->pcurrentstate != NULL)                           //执行当前状态
    {
        self->pcurrentstate->Execute(self, key, pOurteam, pOppteam, pball);
    }

    //限制守门员移动范围
    if(self->mv_pos.y - 20 + self->mv_velocity.y <= 395-20)             //下一帧与上边界碰撞
    {
        self->mv_velocity.y = 395-20 - (self->mv_pos.y - 20);
    }
    else if(self->mv_pos.y + 20 + self->mv_velocity.y >= 495+20)    //下一帧与下边界碰撞
    {
        self->mv_velocity.y = 495+20 - (self->mv_pos.y + 20);
    }

    if(self->mv_velocity.x != 0.0 || self->mv_velocity.y != 0.0)                   //球员位置发生改变，则需重画背景
    {
         //重画背景
        Bar( (int)(self->mv_pos.x)-21, (int)(self->mv_pos.y)-21, (int)(self->mv_pos.x)+21, (int)(self->mv_pos.y)+21, 0x19);
        ReBack((int)(self->mv_pos.x)-21, (int)(self->mv_pos.y)-21, 42, 42);
        self->mv_pos.x += self->mv_velocity.x;
        self->mv_pos.y += self->mv_velocity.y;
    }

    //画球员
    if(0 == pOurteam->name)                                               //白队
    {
        //画球员白色
        DrawPlayer((int)(self->mv_pos.x),(int)(self->mv_pos.y), self->Dir, 0XF6);
    }
    else if(1 == pOurteam->name)                                             //蓝队
    {
        //画球员
        DrawPlayer((int)(self->mv_pos.x),(int)(self->mv_pos.y), self->Dir, LIGHTBLUE);
    }
}

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
void GkeeperChangeState(struct _GOALKEEPER * self, struct PLAYER_STATE * pnewstate, struct _TEAM * pOurteam, struct _TEAM * pOppteam)
{
    if(self->pcurrentstate->Exit != NULL)
    {
        self->pcurrentstate->Exit(self, pOurteam, pOppteam);
    }

    self->pcurrentstate = pnewstate;

    if(self->pcurrentstate->Enter != NULL)
    {
        self->pcurrentstate->Enter(self, pOurteam, pOppteam);
    }
}



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
void AttackExecute(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam)
{
    //更新关键指针
    //ControllingPlayer的更改由球员自己触发
    int i;
    int No = 0;                        //最佳接应点编号
    double score = 0.0;
    double maxscore = 0.0;             //接应点最高得分
    SVector2D tarpos;                  //射门目标

    tarpos.y = 445.0;
    tarpos.x = (0 == self->name)? 960.0 : 60.0;

    //每一段时间计算一次最佳接应点
    //每隔1s计算一次
    if(pball->TimeCount % FPS == 0 && self->ControllingPlayer != -1)
    {

        for(i = 0; i < 30; i++)
        {
            //评估是否有利于传球
            score += 1.0 * CanSafePass(pball->mv_pos, self->SupPos[i], pOppteam);
            //评估是否有利于射门
            score += 1.0 * CanSafePass(self->SupPos[i], tarpos, pOppteam);
            //评估传球距离
            score -= 0.5 * abs(Distance(self->SupPos[i], pball->mv_pos) - 200.0);

            //找到得分最高的位置
            if(score > maxscore)
            {
                maxscore = score;
                No = i;
            }
            score = 0.0;
        }

        self->BestPos = No;

        //接应球员设置为离最佳接应点最近的那个
        self->SupportingPlayer = FindClosest(self->player, self->SupPos[self->BestPos], self->ControllingPlayer);
    }

    //接球队员在发生传球的时候设置(由踢球的队员来设置)
    //设置为传球方向上的那个

    //出口
    if(0 == self->IsControllBall)
    {
        TeamChangeState(self, &self->Defend);
    }

}

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
void DefendEnter(struct _TEAM * self)
{
    self->ReceivingPlayer = -1;
    self->ControllingPlayer = -1;
    self->SupportingPlayer = -1;
}

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
void DefendExecute(struct _TEAM * self, struct _BALL * pball, struct _TEAM * pOppteam)
{
    //更新关键指针
    self->ClosestPlayer = FindClosest(self->player, pball->mv_pos, -1);          //更新离球最近的队员标记

    //出口
    if(1 == self->IsControllBall)
    {
        TeamChangeState(self, &self->Attack);
    }

}




   /*状态部分*/

//球员状态
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
void WaitEnter(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam)
{
    pplayer->mv_velocity.y = 0;
    pplayer->mv_velocity.x = 0;
}

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
void WaitExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{
    double dx = 0.0,
           dy = 0.0;

    if(0 == pOurteam->IsControllBall)               //我方未控球时，采取防守策略
    {
        dx = pOurteam->DefendPos[pplayer->ID%4].x - pplayer->mv_pos.x;
        dy = pOurteam->DefendPos[pplayer->ID%4].y - pplayer->mv_pos.y;
		
        if(0 == pOurteam->name && pball->mv_pos.x < 355.0 && pball->mv_pos.y > 370.0 && pball->mv_pos.y < 520.0)         //白方355 665
        {
            if(pplayer->ID%4 == 2)
            {
                dy -= 75.0;
            }
            else if(pplayer->ID%4 == 3)
            {
                dy += 75.0;
            }
        }
        else if(1 == pOurteam->name && pball->mv_pos.x > 665.0 && pball->mv_pos.y > 370.0 && pball->mv_pos.y < 520.0)
        {
            if(pplayer->ID%4 == 2)
            {
                dy += 75.0;
            }
            else if(pplayer->ID%4 == 3)
            {
                dy -= 75.0;
            }
        }
        //若没到自己的防守区域，则移向防守区域
        if(abs(dx) > 5 || abs(dy) > 5)
        {
            if(sqrt(dx*dx+dy*dy) > 50.0)               //若离目标距离大于50像素，则以正常速度跑动
            {
                pplayer->mv_velocity.x = 1.2 * pplayer->Max_v * GetDir(dx, dy).x;
                pplayer->mv_velocity.y = 1.2 * pplayer->Max_v * GetDir(dx, dy).y;
            }
            else                                      //若离目标距离小于50像素，则应减速
            {
                pplayer->mv_velocity.x = pplayer->Max_v * GetDir(dx, dy).x * sqrt(dx*dx+dy*dy) / 50.0;
                pplayer->mv_velocity.y = pplayer->Max_v * GetDir(dx, dy).y * sqrt(dx*dx+dy*dy) / 50.0;
            }
        }
        else
        {
            pplayer->mv_velocity.x = 0.0;
            pplayer->mv_velocity.y = 0.0;
            if(0 == pOurteam->name)
            {
                pplayer->Dir = Right;
            }
            else
            {
                pplayer->Dir = Left;
            }
        }
    }
    else               //我方控球，采取进攻策略
    {
        dx = pOurteam->AttackPos[pplayer->ID%4].x - pplayer->mv_pos.x;
        dy = pOurteam->AttackPos[pplayer->ID%4].y - pplayer->mv_pos.y;

        //若没到自己的进攻区域，则移向进攻区域
        if(abs(dx) > 5 || abs(dy) > 5)
        {
            if(sqrt(dx*dx+dy*dy) > 50.0)    //若离目标距离大于50像素，则以正常速度跑动
            {
                pplayer->mv_velocity.x = pplayer->Max_v * GetDir(dx, dy).x;
                pplayer->mv_velocity.y = pplayer->Max_v * GetDir(dx, dy).y;
            }
            else               //若离目标距离小于50像素，则应减速
            {
                pplayer->mv_velocity.x = pplayer->Max_v * GetDir(dx, dy).x * sqrt(dx*dx+dy*dy) / 50.0;
                pplayer->mv_velocity.y = pplayer->Max_v * GetDir(dx, dy).y * sqrt(dx*dx+dy*dy) / 50.0;
            }
        }
        else
        {
            pplayer->mv_velocity.x = 0.0;
            pplayer->mv_velocity.y = 0.0;
            if(0 == pOurteam->name)
            {
                pplayer->Dir = Right;
            }
            else
            {
                pplayer->Dir = Left;
            }
        }
    }
	//出口
    //若我方队伍控球， 且自己被设置为了接应队员，则进入接应状态
    if((pplayer->ID % 4) == pOurteam->SupportingPlayer && pOurteam->ControllingPlayer != -1)
    {
        PlayerChangeState(pplayer, &pplayer->SupportingAttacker, pOurteam, pOppteam);
    }
    //若我方队伍未控球，且自己为离球最近的队员，守门员也没控球，则进入追球状态
    if((pplayer->ID % 4) == pOurteam->ClosestPlayer && 0 == pOurteam->IsControllBall && pball->Owner < 8)
    {
        PlayerChangeState(pplayer, &pplayer->ChaseBall, pOurteam, pOppteam);
    }
    //若我方队伍控球， 且自己被设置为了接球队员，则进入接球状态
    if((pplayer->ID % 4) == pOurteam->ReceivingPlayer && 1 == pOurteam->IsControllBall)
    {
        PlayerChangeState(pplayer, &pplayer->ReceivingBall, pOurteam, pOppteam);
    }
}



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
void ReceivingBallExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{
    if(0 == pOurteam->name)                    //白队，受玩家控制
    {
        //按键控制速度
        pplayer->mv_velocity.y = 0;
        pplayer->mv_velocity.x = 0;
        PlayerControl(pplayer);

        //加速模式
        if(0 == pplayer->cdmark && KeyPress(KEY_LSHIFT) && pplayer->acctimecounter < (int)(FPS * pplayer->acc_time) )
        {
            pplayer->mv_velocity.x *= pplayer->acc_rate;
            pplayer->mv_velocity.y *= pplayer->acc_rate;
            pplayer->acctimecounter++;
        }
        if(pplayer->acctimecounter >= (int)(FPS * pplayer->acc_time))
        {
            pplayer->cdmark = (int)(FPS * pplayer->acc_cd);
            pplayer->acctimecounter = 0;
        }
        if(!KeyPress(KEY_LSHIFT) && pplayer->cdmark > 0)
        {
            pplayer->cdmark--;
        }


        //出口

        if(GetBall(pplayer, pball))               //抢球
        {
            //判定是否抢到球
            //若抢到，则变为持球状态，且持球队员设为自己
            pOurteam->ControllingPlayer = pplayer->ID%4;
            pOurteam->IsControllBall = 1;                //我方控球
            pOppteam->ControllingPlayer = -1;           //将对方球队持球队员设为-1
            pOppteam->IsControllBall = 0;               //对方不再控球
            pball->Owner = pplayer->ID;                          //将ball中持球队员标记设为自己

            PlayerChangeState(pplayer, &pplayer->Dribble, pOurteam, pOppteam);

        }
    }
    else                            //蓝队
    {
        if(1 == pball->GameMode)        //双人模式
        {
            //按键控制速度
            pplayer->mv_velocity.y = 0;
            pplayer->mv_velocity.x = 0;
            PlayerControl(pplayer);

            //加速模式
            if(0 == pplayer->cdmark && KeyPress(KEY_CONTROL) && pplayer->acctimecounter < (int)(FPS * pplayer->acc_time))
            {
                pplayer->mv_velocity.x *= pplayer->acc_rate;
                pplayer->mv_velocity.y *= pplayer->acc_rate;
                pplayer->acctimecounter++;
            }
            if(pplayer->acctimecounter >= (int)(FPS * pplayer->acc_time))
            {
                pplayer->cdmark = (int)(FPS * pplayer->acc_cd);
                pplayer->acctimecounter = 0;
            }
            if(!KeyPress(KEY_CONTROL) && pplayer->cdmark > 0)
            {
                pplayer->cdmark--;
            }

            //出口

            if(GetBall(pplayer, pball))               //抢球
            {
                //判定是否抢到球
                //若抢到，则变为持球状态，且持球队员设为自己
                pOurteam->ControllingPlayer = pplayer->ID%4;
                pOurteam->IsControllBall = 1;                //我方控球
                pOppteam->ControllingPlayer = -1;           //将对方球队持球队员设为-1
                pOppteam->IsControllBall = 0;               //对方不再控球
                pball->Owner = pplayer->ID;                          //将ball中持球队员标记设为自己

                PlayerChangeState(pplayer, &pplayer->Dribble, pOurteam, pOppteam);

            }
        }
        else                       //单人模式
        {
            //简单版接球
            pplayer->mv_velocity.x = pplayer->Max_v * GetDir(pball->mv_pos.x - pplayer->mv_pos.x, pball->mv_pos.y - pplayer->mv_pos.y).x;
            pplayer->mv_velocity.y = pplayer->Max_v * GetDir(pball->mv_pos.y - pplayer->mv_pos.x, pball->mv_pos.y - pplayer->mv_pos.y).y;
            if( sqrt(pplayer->mv_velocity.x * pplayer->mv_velocity.x + pplayer->mv_velocity.y * pplayer->mv_velocity.y) < 0.5 * pplayer->Max_v)
            {
                pplayer->mv_velocity.x *= 2.0;
                pplayer->mv_velocity.y *= 2.0;
            }

            //抢球
            if(pball->TimeCount % 20 && Distance(pplayer->mv_pos, pball->mv_pos) <= 15.0 && GetBall(pplayer, pball))
            {
                //判定是否抢到球
                //若抢到，则变为持球状态，且持球队员设为自己
                pOurteam->ControllingPlayer = pplayer->ID%4;
                pOurteam->IsControllBall = 1;                //我方控球
                pOppteam->ControllingPlayer = -1;           //将对方球队持球队员设为-1
                pOppteam->IsControllBall = 0;               //对方不再控球
                pball->Owner = pplayer->ID;                 //将ball中持球队员标记设为自己
                PlayerChangeState(pplayer, &pplayer->Dribble, pOurteam, pOppteam);

            }
        }
    }

    //出口
    //若我方失去球权，转至wait状态
    if(0 == pOurteam->IsControllBall)
    {
        PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
    }
}

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
void ReceivingBallExit(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam)
{
    pOurteam->ReceivingPlayer = -1;
}

//Dribble部分(持球)
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
void DribbleExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{
    int mark = 0;                                 //传球目标编号
    double v = 0.0;                             //传球速度
    SVector2D tarpos;
    if(0 == pOurteam->name)                    //白队，受玩家控制
    {
        //按键控制速度
        pplayer->mv_velocity.y = 0;
        pplayer->mv_velocity.x = 0;
		PlayerControl(pplayer);

        //加速模式
        if(0 == pplayer->cdmark && KeyPress(KEY_LSHIFT) && pplayer->acctimecounter < (int)(FPS * pplayer->acc_time))
        {
            pplayer->mv_velocity.x *= pplayer->acc_rate;
            pplayer->mv_velocity.y *= pplayer->acc_rate;
            pplayer->acctimecounter++;
        }
        if(pplayer->acctimecounter >= (int)(FPS * pplayer->acc_time))
        {
            pplayer->cdmark = (int)(FPS * pplayer->acc_cd);
            pplayer->acctimecounter = 0;
        }

        //出口
        if('g' == key)               //传球
        {
            //找到合适的传球目标
            if((mark = FindPass(pplayer, pOurteam, pOppteam)) != -1)
            {
                v = sqrt(2*BallDeceleration*Distance(pball->mv_pos, pOurteam->player[mark]->mv_pos));
                //给球一个速度
                pball->mv_velocity.x = v * GetDir(pOurteam->player[mark]->mv_pos.x - pball->mv_pos.x , pOurteam->player[mark]->mv_pos.y - pball->mv_pos.y).x;
                pball->mv_velocity.y = v * GetDir(pOurteam->player[mark]->mv_pos.x - pball->mv_pos.x , pOurteam->player[mark]->mv_pos.y - pball->mv_pos.y).y;
            }
            else
            {
                //给球一个速度
                pball->mv_velocity.x = BallPassV * GetDir(pplayer->mv_velocity.x, pplayer->mv_velocity.y).x;
                pball->mv_velocity.y = BallPassV * GetDir(pplayer->mv_velocity.x, pplayer->mv_velocity.y).y;
            }

            //重置控球队员标记
            pball->Owner = -1;
            pOurteam->ControllingPlayer = -1;
            //设置接球队员
            //接球队员为 距足球 预计落点最近的队员
            pOurteam->ReceivingPlayer = GetRecPlayer(pball->mv_velocity, pplayer->mv_pos, pOurteam);
            //改变状态到wait
            PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);

        }

        if('f' == key)                            //射门
        {
            //判断球员朝向是否能射门
            if(CanShoot(pplayer, 0))
            {
                //给球一个合适的速度
                pball->mv_velocity.x = ShootBall(pball, 0).x;
                pball->mv_velocity.y = ShootBall(pball, 0).y;

                //重置控球队员标记
                pball->Owner = -1;
                pOurteam->ControllingPlayer = -1;
                //队伍不再控球
                pOurteam->IsControllBall = 0;

                //改变状态到wait
                PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
            }
        }
    }
    else                                      //蓝队
    {
        if(1 == pball->GameMode)        //双人模式
        {
            //按键控制速度
            pplayer->mv_velocity.y = 0;
            pplayer->mv_velocity.x = 0;           
			PlayerControl(pplayer);

            //加速模式
            if(0 == pplayer->cdmark && KeyPress(KEY_CONTROL) && pplayer->acctimecounter < (int)(FPS * pplayer->acc_time))
            {
                pplayer->mv_velocity.x *= pplayer->acc_rate;
                pplayer->mv_velocity.y *= pplayer->acc_rate;
                pplayer->acctimecounter++;
            }
            if(pplayer->acctimecounter >= (int)(FPS * pplayer->acc_time))
            {
                pplayer->cdmark = (int)(FPS * pplayer->acc_cd);
                pplayer->acctimecounter = 0;
            }

            //出口s
            if('n' == key)               //传球
            {
                //找到合适的传球目标
                if((mark = FindPass(pplayer, pOurteam, pOppteam)) != -1)
                {
                    v = sqrt(2*BallDeceleration*Distance(pball->mv_pos, pOurteam->player[mark]->mv_pos));
                    //给球一个速度
                    pball->mv_velocity.x = v * GetDir(pOurteam->player[mark]->mv_pos.x - pball->mv_pos.x , pOurteam->player[mark]->mv_pos.y - pball->mv_pos.y).x;
                    pball->mv_velocity.y = v * GetDir(pOurteam->player[mark]->mv_pos.x - pball->mv_pos.x , pOurteam->player[mark]->mv_pos.y - pball->mv_pos.y).y;
                }
                else
                {
                    //给球一个速度
                    pball->mv_velocity.x = BallPassV * GetDir(pplayer->mv_velocity.x, pplayer->mv_velocity.y).x;
                    pball->mv_velocity.y = BallPassV * GetDir(pplayer->mv_velocity.x, pplayer->mv_velocity.y).y;
                }
                //重置控球队员标记
                pball->Owner = -1;
                pOurteam->ControllingPlayer = -1;

                //设置接球队员
                //接球队员为 距足球 预计落点最近的队员
                pOurteam->ReceivingPlayer = GetRecPlayer(pball->mv_velocity, pplayer->mv_pos, pOurteam);

                //改变状态到wait
                PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);

            }

            if('m' == key)                            //射门
            {
                //判断球员朝向是否能射门
                if(CanShoot(pplayer, 1))
                {
                    //给球一个合适的速度
                    pball->mv_velocity.x = ShootBall(pball, 1).x;
                    pball->mv_velocity.y = ShootBall(pball, 1).y;

                    //重置控球队员标记
                    pball->Owner = -1;
                    pOurteam->ControllingPlayer = -1;
                    //队伍不再控球
                    pOurteam->IsControllBall = 0;

                    //改变状态到wait
                    PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
                }
            }
        }
        else            //单人模式
        {
            //简单版，添加蛇皮走位
            pplayer->mv_velocity.x = -Botrate * pplayer->Max_v;
            pplayer->mv_velocity.y = ( (pball->TimeCount / 30) % 2 == 0 )? 0.6* Botrate * pplayer->Max_v : -0.6 * Botrate * pplayer->Max_v;

            tarpos.x = 60.0;
            tarpos.y = 445.0;

            //检测有没有机会射门
            if(Distance(pplayer->mv_pos, tarpos) <= MaxShootL && CanSafePass(pplayer->mv_pos, tarpos, pOppteam))
            {
                //给球一个合适的速度
                pball->mv_velocity.x = ShootBall(pball, 1).x;
                pball->mv_velocity.y = ShootBall(pball, 1).y;

                //重置控球队员标记
                pball->Owner = -1;
                pOurteam->ControllingPlayer = -1;
                //队伍不再控球
                pOurteam->IsControllBall = 0;

                //改变状态到wait
                PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
            }

            //检测有无威胁，若有威胁则传球
            if(pball->TimeCount % 10 == 0 && IsDanger(pplayer, pOppteam, 45.0))
            {
                //找到合适的传球目标
                if((mark = FindPass(pplayer, pOurteam, pOppteam)) != -1)
                {
                    v = sqrt(2*BallDeceleration*Distance(pball->mv_pos, pOurteam->player[mark]->mv_pos));
                    //给球一个速度
                    pball->mv_velocity.x = v * GetDir(pOurteam->player[mark]->mv_pos.x - pball->mv_pos.x , pOurteam->player[mark]->mv_pos.y - pball->mv_pos.y).x;
                    pball->mv_velocity.y = v * GetDir(pOurteam->player[mark]->mv_pos.x - pball->mv_pos.x , pOurteam->player[mark]->mv_pos.y - pball->mv_pos.y).y;
                }
                else
                {
                    //给球一个速度
                    pball->mv_velocity.x = BallPassV * GetDir(pplayer->mv_velocity.x, pplayer->mv_velocity.y).x;
                    pball->mv_velocity.y = BallPassV * GetDir(pplayer->mv_velocity.x, pplayer->mv_velocity.y).y;
                }
                //重置控球队员标记
                pball->Owner = -1;
                pOurteam->ControllingPlayer = -1;

                //设置接球队员
                //接球队员为 距足球飞行直线最近的，且方向要对
                pOurteam->ReceivingPlayer = GetRecPlayer(pball->mv_velocity, pplayer->mv_pos, pOurteam);

                //改变状态到wait
                PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
            }
        }
    }
    if(pplayer->ID != pball->Owner)
    {
        PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
    }
}



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
void ChaseBallExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{
    if(0 == pOurteam->name)                    //白队，受玩家控制
    {
        //按键控制速度
        pplayer->mv_velocity.y = 0;
        pplayer->mv_velocity.x = 0;
        PlayerControl(pplayer);

        //加速模式
        if(0 == pplayer->cdmark && KeyPress(KEY_LSHIFT) && pplayer->acctimecounter < (int)(FPS * pplayer->acc_time) )
        {
            pplayer->mv_velocity.x *= pplayer->acc_rate;
            pplayer->mv_velocity.y *= pplayer->acc_rate;
            pplayer->acctimecounter++;
        }
        if(pplayer->acctimecounter >= (int)(FPS * pplayer->acc_time))
        {
            pplayer->cdmark = (int)(FPS * pplayer->acc_cd);
            pplayer->acctimecounter = 0;
        }
        if(!KeyPress(KEY_LSHIFT) && pplayer->cdmark > 0)
        {
            pplayer->cdmark--;
        }

        //出口

        if( ('f' == key || (!IsDanger(pplayer, pOppteam, 65.0) && pball->Owner == -1)) && GetBall(pplayer, pball))               //抢球
        {
            //判定是否抢到球
            //若抢到，则变为持球状态，且持球队员设为自己
            pOurteam->ControllingPlayer = pplayer->ID%4;
            pOurteam->IsControllBall = 1;                //我方控球
            pOppteam->ControllingPlayer = -1;           //将对方球队持球队员设为-1
            pOppteam->IsControllBall = 0;               //对方不再控球
            pball->Owner = pplayer->ID;                          //将ball中持球队员标记设为自己

            PlayerChangeState(pplayer, &pplayer->Dribble, pOurteam, pOppteam);


        }
        //若自己已不是离球最近的，切换为wait状态
        if(pOurteam->ClosestPlayer != pplayer->ID%4)
        {
            PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
        }
    }
    else                          //蓝队
    {
        if(1 == pball->GameMode)     //双人模式
        {
            pplayer->mv_velocity.y = 0;
            pplayer->mv_velocity.x = 0;
            if(KeyPress(KEY_UP))
            {
                pplayer->mv_velocity.y = -pplayer->Max_v;

            }
            else if(KeyPress(KEY_DOWN))
            {
                pplayer->mv_velocity.y = pplayer->Max_v;

            }

            if(KeyPress(KEY_LEFT))
            {

                pplayer->mv_velocity.x = -pplayer->Max_v;
            }
            else if(KeyPress(KEY_RIGHT))
            {

                pplayer->mv_velocity.x = pplayer->Max_v;
            }

            //加速模式
            if(0 == pplayer->cdmark && KeyPress(KEY_CONTROL) && pplayer->acctimecounter < (int)(FPS * pplayer->acc_time) )
            {
                pplayer->mv_velocity.x *= pplayer->acc_rate;
                pplayer->mv_velocity.y *= pplayer->acc_rate;
                pplayer->acctimecounter++;
            }
            if(pplayer->acctimecounter >= (int)(FPS * pplayer->acc_time))
            {
                pplayer->cdmark = (int)(FPS * pplayer->acc_cd);
                pplayer->acctimecounter = 0;
            }
            if(!KeyPress(KEY_CONTROL) && pplayer->cdmark > 0)
            {
                pplayer->cdmark--;
            }
            //出口

            if(('m' == key || (!IsDanger(pplayer, pOppteam, 65.0) && pball->Owner == -1)) && GetBall(pplayer, pball))               //抢球
            {
                //判定是否抢到球
                //若抢到，则变为持球状态，且持球队员设为自己
                pOurteam->ControllingPlayer = pplayer->ID%4;
                pOurteam->IsControllBall = 1;                //我方控球
                pOppteam->ControllingPlayer = -1;           //将对方球队持球队员设为-1
                pOppteam->IsControllBall = 0;               //对方不再控球
                pball->Owner = pplayer->ID;                          //将ball中持球队员标记设为自己

                PlayerChangeState(pplayer, &pplayer->Dribble, pOurteam, pOppteam);


            }
            //若自己已不是离球最近的，切换为wait状态
            if(pOurteam->ClosestPlayer != pplayer->ID%4)
            {
                PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
            }



        }
        else                       //单人模式
        {
            //追球（简单版）
            pplayer->mv_velocity.x = Botrate * pplayer->Max_v * GetDir(pball->mv_pos.x - pplayer->mv_pos.x, pball->mv_pos.y - pplayer->mv_pos.y).x;
            pplayer->mv_velocity.y = Botrate * pplayer->Max_v * GetDir(pball->mv_pos.y - pplayer->mv_pos.x, pball->mv_pos.y - pplayer->mv_pos.y).y;
            if( sqrt(pplayer->mv_velocity.x * pplayer->mv_velocity.x + pplayer->mv_velocity.y * pplayer->mv_velocity.y) < 0.5 * pplayer->Max_v)
            {
                pplayer->mv_velocity.x *= 3.0;
                pplayer->mv_velocity.y *= 3.0;
            }

            //抢球，范围更小
            if(pball->TimeCount % FPS && Distance(pplayer->mv_pos, pball->mv_pos) <= 15.0 && GetBall(pplayer, pball))
            {
                //判定是否抢到球
                //若抢到，则变为持球状态，且持球队员设为自己
                pOurteam->ControllingPlayer = pplayer->ID%4;
                pOurteam->IsControllBall = 1;                //我方控球
                pOppteam->ControllingPlayer = -1;           //将对方球队持球队员设为-1
                pOppteam->IsControllBall = 0;               //对方不再控球
                pball->Owner = pplayer->ID;                 //将ball中持球队员标记设为自己

                PlayerChangeState(pplayer, &pplayer->Dribble, pOurteam, pOppteam);

            }

            //若自己已不是离球最近的，切换为wait状态
            if(pOurteam->ClosestPlayer != pplayer->ID%4)
            {
                PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
            }
        }
    }

    //若守门员控球，跳至wait
    if(pball->Owner >= 8)
    {
        PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
    }
}



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
void SupportingAttackerExecute(struct _PLAYER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{
    double dx = 0.0,
           dy = 0.0;
    dx = pOurteam->SupPos[pOurteam->BestPos].x - pplayer->mv_pos.x;
    dy = pOurteam->SupPos[pOurteam->BestPos].y - pplayer->mv_pos.y;

    //若没到最佳接应点，则移向接应点
    if(abs(dx) > 5 || abs(dy) > 5)
    {
        if(sqrt(dx*dx+dy*dy) > 50.0)               //若离目标距离大于50像素，则以正常速度跑动
        {
            pplayer->mv_velocity.x = 1.2 * pplayer->Max_v * GetDir(dx, dy).x;
            pplayer->mv_velocity.y = 1.2 * pplayer->Max_v * GetDir(dx, dy).y;
        }
        else                                      //若离目标距离小于50像素，则应减速
        {
            pplayer->mv_velocity.x = 1.2 * pplayer->Max_v * GetDir(dx, dy).x * sqrt(dx*dx+dy*dy) / 50.0;
            pplayer->mv_velocity.y = 1.2 * pplayer->Max_v * GetDir(dx, dy).y * sqrt(dx*dx+dy*dy) / 50.0;
        }
    }
    else            //若已到达，跳到wait状态
    {
        pplayer->mv_velocity.x = 0.0;
        pplayer->mv_velocity.y = 0.0;
        if(0 == pOurteam->name)
        {
            pplayer->Dir = Right;
        }
        else
        {
            pplayer->Dir = Left;
        }

        //PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
    }

    //若自己不再是接应队员则跳转至待命状态
    if(pplayer->ID%4 != pOurteam->SupportingPlayer)
    {
        PlayerChangeState(pplayer, &pplayer->Wait, pOurteam, pOppteam);
    }

    //若我方队伍控球， 且自己被设置为了接球队员，则进入接球状态
    if((pplayer->ID % 4) == pOurteam->ReceivingPlayer && 1 == pOurteam->IsControllBall)
    {
        PlayerChangeState(pplayer, &pplayer->ReceivingBall, pOurteam, pOppteam);
    }


    //通过全局状态可跳出
}

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
void SupportingAttackerExit(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam)
{
    pOurteam->BestPos = 0;
    pOurteam->SupportingPlayer = -1;
}

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
void TendgoalExecute(struct _GOALKEEPER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{
    SVector2D DefendPos;
    double y = 445.0;              //目标位置的y坐标
    double dx = 1.0, dy = 1.0;

    pplayer->mv_velocity.y = 0.0;       //速度归零

    DefendPos.y = 445.0;
    DefendPos.x = (0 == pOurteam->name)? 60.0 : 960.0;

    dx = GetDir(pball->mv_pos.x - DefendPos.x, pball->mv_pos.y - DefendPos.y).x;
    dy = GetDir(pball->mv_pos.x - DefendPos.x, pball->mv_pos.y - DefendPos.y).y;

    //算得目标位置的y坐标
    y = (0 == pOurteam->name)? 445.0 + dy / dx * 50.0 : 445.0 - dy / dx * 50.0;

    //让守门员犯一点错
    y += rand()%40 - 20.0;

    if(Distance(pball->mv_pos, DefendPos) <= MaxShootL - 20.0 || pball->TimeCount % 10 == 0)
    {
        //移向目标点
        if(abs(y - pplayer->mv_pos.y) > 1.0)
        {
            pplayer->mv_velocity.y = (y - pplayer->mv_pos.y > 0.0)? 1.5*PlayerVelocity : -1.5*PlayerVelocity;
        }
        else
        {
            pplayer->mv_velocity.y = 0.0;
        }
    }

    if(Distance(pball->mv_pos, pplayer->mv_pos) <= 10.0)
    {
        pball->Owner = pplayer->ID;
		//将双方队伍都设置为未控球
        pOppteam->IsControllBall = 0;
		pOurteam->IsControllBall = 0;
        //转至Returnhome状态
        GkeeperChangeState(pplayer, &pplayer->Returnhome, pOurteam, pOppteam);
    }


}


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
void ReturnhomeExecute(struct _GOALKEEPER * pplayer, char key, struct _TEAM * pOurteam, struct _TEAM * pOppteam, struct _BALL * pball)
{
    double v = 0.0;
    double distance = 0.0;
    int mark = -1;           //传球目标编号
    int canpass = 1;         //能否开球的标志
    int i;

    if(abs(pplayer->mv_pos.y - 445.0) > 2.0)
    {
        pplayer->mv_velocity.y = (445.0 - pplayer->mv_pos.y > 0.0)? 0.75*PlayerVelocity : -0.75*PlayerVelocity;
    }
    else
    {
        pplayer->mv_velocity.y = 0.0;

        for(i = 0; i < 4; i++)
        {
            //若对方球员还没离开我方半场，则不开球
            if(0 == pOurteam->name && pOppteam->player[i]->mv_pos.x <= 507.0)
            {
                canpass = 0;
            }
            else if (1 == pOurteam->name && pOppteam->player[i]->mv_pos.x >= 507.0)
            {
                canpass = 0;
            }
        }

        if(canpass)
        {
            //找到合适的传球目标
            if((mark = FindPass(pplayer, pOurteam, pOppteam)) != -1)
            {
                v = sqrt(2*BallDeceleration*Distance(pball->mv_pos, pOurteam->player[mark]->mv_pos));
                //给球一个速度
                pball->mv_velocity.x = v * GetDir(pOurteam->player[mark]->mv_pos.x - pball->mv_pos.x , pOurteam->player[mark]->mv_pos.y - pball->mv_pos.y).x;
                pball->mv_velocity.y = v * GetDir(pOurteam->player[mark]->mv_pos.x - pball->mv_pos.x , pOurteam->player[mark]->mv_pos.y - pball->mv_pos.y).y;
            }
            else
            {
                //传给2号
                //然后游戏继续进行
                distance = Distance(pplayer->mv_pos, pOurteam->player[1]->mv_pos);
                v = sqrt( abs(2 * BallDeceleration * distance) );
                pball->mv_velocity.x = v * GetDir(pOurteam->player[1]->mv_pos.x - pplayer->mv_pos.x, pOurteam->player[1]->mv_pos.y - pplayer->mv_pos.y).x;
                pball->mv_velocity.y = v * GetDir(pOurteam->player[1]->mv_pos.x - pplayer->mv_pos.x, pOurteam->player[1]->mv_pos.y - pplayer->mv_pos.y).y;
            }

            pball->Owner = -1;
            //转至Tendgoal状态
            GkeeperChangeState(pplayer, &pplayer->Tendgoal, pOurteam, pOppteam);
        }
    }

}

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
void InitPlayer(struct _PLAYER * pplayer, int ID)
{
    pplayer->ID = ID;
    pplayer->mv_velocity.x = 0.0;
    pplayer->mv_velocity.y = 0.0;
    pplayer->mv_pos.x = 0;
    pplayer->mv_pos.y = 0;
    pplayer->pcurrentstate = &pplayer->Wait;

    pplayer->Max_v = PlayerVelocity;
    pplayer->get_rate = GetballRate;
    pplayer->acc_rate = AccRate;
    pplayer->acc_time = AccTime;
    pplayer->acc_cd = AccCD;

    pplayer->acctimecounter = 0;
    pplayer->cdmark = 0;


    pplayer->Wait.Enter = WaitEnter;
    pplayer->Wait.Execute = WaitExecute;
    pplayer->Wait.Exit = NULL;

    pplayer->ReceivingBall.Enter = NULL;
    pplayer->ReceivingBall.Execute = ReceivingBallExecute;
    pplayer->ReceivingBall.Exit = ReceivingBallExit;

    pplayer->Dribble.Enter = NULL;
    pplayer->Dribble.Execute = DribbleExecute;
    pplayer->Dribble.Exit = NULL;

    pplayer->ChaseBall.Enter = NULL;
    pplayer->ChaseBall.Execute = ChaseBallExecute;
    pplayer->ChaseBall.Exit = NULL;

    pplayer->SupportingAttacker.Enter = NULL;
    pplayer->SupportingAttacker.Execute = SupportingAttackerExecute;
    pplayer->SupportingAttacker.Exit = SupportingAttackerExit;

    if(ID < 4)
    {
        pplayer->Dir = Right;
    }
    else
    {
        pplayer->Dir = Left;
    }

}


/**********************************************************
Function：      InitBall

Description：   足球初始化函数

Input：     struct _BALL * pball       指向足球的指针

Output：     None
Return：     None
Others：     None
**********************************************************/
void InitBall(struct _BALL * pball)
{
    pball->mv_velocity.x = 0.0;
    pball->mv_velocity.y = 0.0;
    pball->mv_pos.x = 512.0;
    pball->mv_pos.y = 445.0;
    pball->mv_oldpos.x = 512.0;
    pball->mv_oldpos.y = 445.0;
    pball->Owner = -1;
    pball->TimeCount = 1;

    pball->GameMode = 0;                    //设置游戏模式，0单人，1双人


}


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
void InitTeam(struct _TEAM * pteam, struct _PLAYER * players, int name)
{
    int i = 0;
    for(i = 0; i < 4; i++)
    {
        pteam->player[i] = &players[i];
    }

    pteam->ClosestPlayer = 0;
    pteam->ReceivingPlayer = -1;
    pteam->ControllingPlayer = -1;
    pteam->SupportingPlayer = -1;
    pteam->name = name;
    pteam->IsControllBall = 0;
    pteam->pcurrentstate = &pteam->Defend;
    pteam->BestPos = 0;

    pteam->Attack.Enter = NULL;
    pteam->Attack.Execute = AttackExecute;
    pteam->Attack.Exit = NULL;

    pteam->Defend.Enter = DefendEnter;
    pteam->Defend.Execute = DefendExecute;
    pteam->Defend.Exit = NULL;


    //初始化队员位置，和进攻、防守点位
    if(0 == name)               //我方球队（白队）
    {
        pteam->player[0]->mv_pos.x = 450.0;
        pteam->player[0]->mv_pos.y = 370.0;

        pteam->player[1]->mv_pos.x = pteam->DefendPos[1].x = 350.0;
        pteam->player[1]->mv_pos.y = pteam->DefendPos[1].y = 560.0;

        pteam->player[2]->mv_pos.x = pteam->DefendPos[2].x = 230.0;
        pteam->player[2]->mv_pos.y = pteam->DefendPos[2].y = 560.0;

        pteam->player[3]->mv_pos.x = pteam->DefendPos[3].x = 230.0;
        pteam->player[3]->mv_pos.y = pteam->DefendPos[3].y = 330.0;

        pteam->DefendPos[0].x = 350.0;
        pteam->DefendPos[0].y = 330.0;

        //进攻点位
        pteam->AttackPos[0].x = 730.0;
        pteam->AttackPos[0].y = 330.0;

        pteam->AttackPos[1].x = 730.0;
        pteam->AttackPos[1].y = 560.0;

        pteam->AttackPos[2].x = 440.0;
        pteam->AttackPos[2].y = 560.0;

        pteam->AttackPos[3].x = 300.0;
        pteam->AttackPos[3].y = 445.0;


        for (i = 0; i < 4; i++)
        {
            DrawPlayer((int)(pteam->player[i]->mv_pos.x), (int)(pteam->player[i]->mv_pos.y), pteam->player[i]->Dir, 0XF6);
        }

        //设置备选接应点
        for(i = 0; i < 30; i++)
        {
            pteam->SupPos[i].x = 560.0 + (i%5) * 70.0;
            pteam->SupPos[i].y = 225.0 + (i/5) * 80.0;
        }

    }
    else if(1 == name)
    {
        pteam->player[0]->mv_pos.x = 564.0;
        pteam->player[0]->mv_pos.y = 520.0;

        pteam->player[1]->mv_pos.x = pteam->DefendPos[1].x = 670.0;
        pteam->player[1]->mv_pos.y = pteam->DefendPos[1].y = 330.0;

        pteam->player[2]->mv_pos.x = pteam->DefendPos[2].x = 790.0;
        pteam->player[2]->mv_pos.y = pteam->DefendPos[2].y = 330.0;

        pteam->player[3]->mv_pos.x = pteam->DefendPos[3].x = 790.0;
        pteam->player[3]->mv_pos.y = pteam->DefendPos[3].y = 560.0;

        pteam->DefendPos[0].x = 670.0;
        pteam->DefendPos[0].y = 560.0;

        //进攻点位
        pteam->AttackPos[0].x = 290.0;
        pteam->AttackPos[0].y = 560.0;

        pteam->AttackPos[1].x = 290.0;
        pteam->AttackPos[1].y = 330.0;

        pteam->AttackPos[2].x = 580.0;
        pteam->AttackPos[2].y = 330.0;

        pteam->AttackPos[3].x = 720.0;
        pteam->AttackPos[3].y = 445.0;

        for (i = 0; i < 4; i++)
        {
            DrawPlayer((int)(pteam->player[i]->mv_pos.x), (int)(pteam->player[i]->mv_pos.y), pteam->player[i]->Dir, LIGHTBLUE);
        }

        //设置备选接应点
        for(i = 0; i < 30; i++)
        {
            pteam->SupPos[i].x = 454.0 - (i%5) * 70.0;
            pteam->SupPos[i].y = 225.0 + (i/5) * 80.0;
        }


    }


}


/**********************************************************
Function：      InitGkeeper

Description：   守门员初始化函数

Input：     struct _GOALKEEPER * pplayer       指向自身代表的球员的指针
			int ID                             球员ID

Output：     None
Return：     None
Others：     None
**********************************************************/
void InitGkeeper(struct _GOALKEEPER * pplayer, int ID)
{
    pplayer->ID = ID;
    pplayer->mv_velocity.x = 0.0;
    pplayer->mv_velocity.y = 0.0;

    pplayer->pcurrentstate = &pplayer->Tendgoal;


    pplayer->Tendgoal.Enter = NULL;
    pplayer->Tendgoal.Execute = TendgoalExecute;
    pplayer->Tendgoal.Exit = NULL;

    pplayer->Returnhome.Enter = NULL;
    pplayer->Returnhome.Execute = ReturnhomeExecute;
    pplayer->Returnhome.Exit = NULL;


    if(8 == ID)       //白队守门员
    {
        pplayer->Dir = Right;
        pplayer->mv_pos.x = 80.0;
        pplayer->mv_pos.y = 445.0;
        DrawPlayer((int)(pplayer->mv_pos.x), (int)(pplayer->mv_pos.y), pplayer->Dir, 0XF6);
    }
    else if(9 == ID)   //蓝队守门员
    {
        pplayer->Dir = Left;
        pplayer->mv_pos.x = 940.0;
        pplayer->mv_pos.y = 445.0;
        DrawPlayer((int)(pplayer->mv_pos.x), (int)(pplayer->mv_pos.y), pplayer->Dir, LIGHTBLUE);
    }

}



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
_result FootballGame(struct _FIELDPLAYER fieldplayers, int gamemode, int botmode)
{   
	/*******各变量定义********/
	struct _BALL ball;
    struct _PLAYER Wplayers[4];
    struct _PLAYER Bplayers[4];
    struct _GOALKEEPER goalkeepers[2];
    struct _TEAM Wteam;
    struct _TEAM Bteam;

    int i, n = 0;
    int Wgoal = 0,             //白队得分
        Bgoal = 0;             //蓝队得分
    int gameover = 0;          //游戏是否正常结束
    int Time = 300;            //300秒，5分钟的时间限制
    _result Result;	
    int controlbefore = -1;           //上一次控球的队员
    char key = '\0';
	
	/***********初始化**********/
    SetSVGA256();
    InstallKeyboard();        //开启多按键检测
    Result.Wscore = 0;
    Result.Bscore = 0;

    //产生随机数种子
    srand((unsigned) time(NULL));

    //贴出键位指引图片
    Putbmp256(0, 0, "bmp\\jianwei.bmp");
    while(!GetKey(KEY_ENTER))
    {
        continue;
    }
    //画背景
	DrawGameBK();
    //画计分板
    DrawScoreBoard(Wgoal, Bgoal);
    //画计时器
    DrawTimer(Time);

	//初始化足球
    InitBall(&ball);
	//初始化白队球员
    for(i = 0; i < 4; i++)
    {
        InitPlayer(&Wplayers[i], i);
    }
	//初始化蓝队球员
    for(i = 0; i < 4; i++)
    {
        InitPlayer(&Bplayers[i], i + 4);
    }
	//初始化两个守门员
    for(i = 0; i < 2; i++)
    {
        InitGkeeper(&goalkeepers[i], i + 8);
    }	
	//初始化白队
    InitTeam(&Wteam, Wplayers, 0);	
	//初始化蓝队
    InitTeam(&Bteam, Bplayers, 1);

    //根据传入的参数进行一些设置
    //设置游戏模式（单人/双人）
    if(0 == gamemode || 1 == gamemode)
    {
        ball.GameMode = gamemode;
    }
    else
    {
        exit(1);
    }
    //设置各队球员能力
    SetPlayersPara(Wplayers, Bplayers, fieldplayers);
    //设置bot难度
    SetAIMode(Bplayers, gamemode, botmode);

	/***********主循环**********/
    while(1)
    {
		//检测传球/射门按键动作
        GetKeyboardAct(&key);
        //白队更新
        TeamUpdate(&Wteam, &ball, &Bteam);
        //蓝队更新
        TeamUpdate(&Bteam, &ball, &Wteam);
		//两队球员更新
        for (i = 0; i < 4; i++)
        {
            PlayerUpdate(&Wplayers[i], key, &Wteam, &Bteam, &ball);
            PlayerUpdate(&Bplayers[i], key, &Bteam, &Wteam, &ball);

        }
        //守门员更新
        GkeeperUpdate(&goalkeepers[0], key, &Wteam, &Bteam, &ball);
        GkeeperUpdate(&goalkeepers[1], key, &Bteam, &Wteam, &ball);
		//足球更新
        BallUpdate(&ball, &Wteam, &Bteam, goalkeepers);
        //标记控球队员
        if(ball.Owner != controlbefore)
        {
            if(controlbefore < 4 && controlbefore > -1 )
            {
                Bar(60 + 130*(controlbefore%2), 50 + 95*(controlbefore/2), 85 + 130*(controlbefore%2), 75 + 95*(controlbefore/2), 0X19);
            }
            else if(controlbefore < 8 && controlbefore > 3)
            {
                Bar(700 + 130*((controlbefore-4)%2), 50 + 95*((controlbefore-4)/2), 725 + 130*((controlbefore-4)%2), 75 + 95*((controlbefore-4)/2), 0X19);
            }
            if(ball.Owner < 4 && ball.Owner > -1 )
            {
                Putbmp256(60 + 130*(ball.Owner%2), 50 + 95*(ball.Owner/2) , "bmp\\gou.bmp");
                controlbefore = ball.Owner;
            }
            else if(ball.Owner < 8 && ball.Owner > 3)
            {
                Putbmp256(700 + 130*((ball.Owner-4)%2), 50 + 95*((ball.Owner-4)/2) , "bmp\\gou.bmp");
                controlbefore = ball.Owner;
            }
        }

		//出界/进球检测
        if(BallDec(&ball))
        {
            if(BallDec(&ball) == 2)      //白队得分
            {
                Wgoal++;
                Updategoal(Wgoal, Bgoal);
            }
            else if(BallDec(&ball) == 3)    //蓝队得分
            {
                Bgoal++;
                Updategoal(Wgoal, Bgoal);
            }
            Restart(&Wteam, &Bteam, &ball, goalkeepers);
            if(controlbefore < 4 && controlbefore > -1 )
            {
                Bar(60 + 130*(controlbefore%2), 50 + 95*(controlbefore/2), 85 + 130*(controlbefore%2), 75 + 95*(controlbefore/2), 0X19);
            }
            else if(controlbefore < 8 && controlbefore > 3)
            {
                Bar(700 + 130*((controlbefore-4)%2), 50 + 95*((controlbefore-4)/2), 725 + 130*((controlbefore-4)%2), 75 + 95*((controlbefore-4)/2), 0X19);
            }
            controlbefore = -1;
        }

        //更新时间
        if(ball.TimeCount % FPS == 0)
        {
            Time--;
            UPdatetime(Time);
        }
        key = '\0';		
		//暂停
        if(GamePause())
        {
            break;
        }
        //时间到了5分钟，结束比赛
        if(Time <= 0)
        {
            gameover = 1;
            break;
        }
        delay(TimeStep);
        n++;
    }

	/***********结算比分**************/
    if(1 == gameover)
    {
        Putbmp256(255, 200, "bmp\\gameend.bmp");
        //画最终比分
        if(Wgoal / 10 > 0)
        {
            DrawNum(305, 320, 50, 0XF6, Wgoal / 10);
        }
        DrawNum(375, 320, 50, 0XF6, Wgoal % 10);
        if(Bgoal / 10 > 0)
        {
            DrawNum(585, 320, 50, 0XF6, Bgoal / 10);
            DrawNum(655, 320, 50, 0XF6, Bgoal % 10);
        }
        else
        {
            DrawNum(585, 320, 50, 0XF6, Bgoal % 10);
        }
        while(!GetKey(KEY_ENTER))
        {
            continue;
        }
        //记录最后比分
        Result.Wscore = Wgoal;
        Result.Bscore = Bgoal;
    }
    else
    {
        Result.Wscore = -1;
        Result.Bscore = -1;
    }
    ShutDownKeyboard();                  //退出多按键检测
    return Result;
}
