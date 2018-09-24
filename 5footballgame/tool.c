/**********************************************************
Copyright (C) 2017 郭裕祺 黄杰豪.
File name:      tool.c
Author:         郭裕祺
Version：        1.1
Date：           2017/9/23
Description：    实况足球游戏核心部分的辅助函数库源文件。

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
3.  Date：2017/11/9
        Author：郭裕祺
        Version：1.2
        Modification：加入了21至27号函数，将一些小功能封装了一下
**********************************************************/



#include"football.h"
#include"SVGAmode.h"
#include"tool.h"

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
void ReBack(int x, int y, int size_x, int size_y)
{
    int i, j;
    int mark = 0;

    for(i = x; i < x + size_x; i++)
    {
        if( (i<=64 && i>=60) || (i<=510 && i>=506) || (i<=961 && i>=957))
        {
            mark = 1;
        }

        for(j = y; j < y + size_y + 1; j++)
        {
            if(0 == mark)
            {
                if(i > 60 && i < 961 && ((j<=202 && j>=198) || (j<=691 && j>=686)) )
                {
                    mark = 1;
                }
                else if( j>202 && j<686 && (i >= 434 && i <= 582) && ((long int)(i-508)*(i-508)+(long int)(j-445)*(j-445)) <= 74*74 && ((long int)(i-508)*(i-508)+(long int)(j-445)*(j-445)) >= 70*70)
                {
                    mark = 1;
                }
                else if( (i>64 && i <= 210) && ((long int)(i-60)*(i-60)+(long int)(j-445)*(j-445)) <= 150*150 && ((long int)(i-60)*(i-60)+(long int)(j-445)*(j-445)) >= 146*146 )
                {
                    mark = 1;
                }
                else if( (i<957 && i >= 811) && ((long int)(i-961)*(i-961)+(long int)(j-445)*(j-445)) <= 150*150 && ((long int)(i-961)*(i-961)+(long int)(j-445)*(j-445)) >= 146*146 )
                {
                    mark = 1;
                }
                else if( (i < 60 || i > 961) && ( (j >= 390 && j < 394) || (j > 494 && j <= 498) ) )
                {
                    mark = 1;
                }



                if(1 == mark)
                {
                    Putpixel256(i, j, 0xF6);
                    mark = 0;
                }
            }
            else if(j >198 && j < 691)
            {
                Putpixel256(i, j, 0xF6);
            }

        }
        mark = 0;
    }
}

/**********************************************************
Function：      GetBall

Description：   用于判定球员做出抢球动作时是否能抢到球。

Calls：         Distance
					rand

Called By：     ReceivingBallExecute (football.c)
						ChaseBallExecute (football.c)

Input：    struct _PLAYER * pplayer          指向球员的指针
				struct _BALL * pball              指向足球的指针

Output：     None

Return：     0 不能抢到球
             1 能抢到球

Others：     None
**********************************************************/
int GetBall(struct _PLAYER * pplayer, struct _BALL * pball)
{
    int n = 0;           //用作概率事件的辅助变量
    if(Distance(pplayer->mv_pos, pball->mv_pos) <= 30.0)
    {
        if(pball->Owner >= 8)              //球的持有者是守门员
        {
            return 0;
        }
        else if(-1 == pball->Owner)         //球没被人持有
        {
            return 1;
        }
        else                               //球被在场球员持有（还有抢断几率没写）
        {
            n = rand()%100 + 1;

            return (n >= 100*GetballRate)? 1 : 0;
        }


    }
    else
    {
        return 0;
    }
}

/**********************************************************
Function：      Distance

Description：   计算并返回两个点之间的距离。

Calls：         sqrt

Called By： GetBall
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
double Distance(SVector2D n1, SVector2D n2)
{
    double D = 0.0;

    D = (n1.x - n2.x) * (n1.x - n2.x) + (n1.y - n2.y) * (n1.y - n2.y);
    D = sqrt(D);

    return D;
}

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
SVector2D GetDir(double x, double y)
{
    SVector2D dir;
    double z = sqrt(x*x + y*y);         //模长
    if(z > 0.0)
    {
        dir.x = x / z;
        dir.y = y / z;
    }
    else
    {
        dir.x = 0;
        dir.y = 0;
    }

    return dir;

}

/**********************************************************
Function：      GetRecPlayer

Description：   计算得出一个合适的接球队员，并返回其队内编号

Calls：         abs
                Distance

Called By：     DribbleExecute (football.c)

Input：SVector2D ball_v           足球的速度
            SVector2D p_pos            传出足球的球员位置
            struct _TEAM * pOurteam    指向我方球队的指针

Output：     None

Return：     合适的接球队员的队内编号

Others：     None
**********************************************************/
int GetRecPlayer(SVector2D ball_v, SVector2D p_pos, struct _TEAM * pOurteam)
{
    SVector2D ball_nextpos;
    int i, mark = 0;

    //用速度位移公式求出足球落点
    ball_nextpos.x = p_pos.x + ball_v.x * abs(ball_v.x) / 2.0 / BallDeceleration;
    ball_nextpos.y = p_pos.y + ball_v.y * abs(ball_v.y) / 2.0 / BallDeceleration;

    for(i = 0; i < 4; i++)
    {
        //选出离球落点最近的球员
        if( Distance(pOurteam->player[i]->mv_pos, ball_nextpos) < Distance(pOurteam->player[mark]->mv_pos, ball_nextpos))
        {
            mark = i;
        }
    }

    return mark;
}


/**********************************************************
Function：     PlayerBorder

Description：   球员与边界的碰撞处理函数（完全非弹性碰撞）

Input：		struct _PLAYER * pplayer           指向球员的指针
            
Output：     None

Return：     None

Others：     此函数通过修改球员速度来将球员限制在屏幕的
				   固定范围内。
**********************************************************/
void PlayerBorder(struct _PLAYER * pplayer)
{
    if(pplayer->mv_pos.x - 20 + pplayer->mv_velocity.x <= Border_LEFT-20)             //下一帧与左边界碰撞
    {
        pplayer->mv_velocity.x = Border_LEFT-20 - (pplayer->mv_pos.x - 20);
    }
    else if(pplayer->mv_pos.x + 20 + pplayer->mv_velocity.x >= Border_RIGHT+20) //下一帧与右边界碰撞
    {
        pplayer->mv_velocity.x = Border_RIGHT+20 - (pplayer->mv_pos.x + 20);
    }

    if(pplayer->mv_pos.y - 20 + pplayer->mv_velocity.y <= Border_UP-20)             //下一帧与上边界碰撞
    {
        pplayer->mv_velocity.y = Border_UP-20 - (pplayer->mv_pos.y - 20);
    }
    else if(pplayer->mv_pos.y + 20 + pplayer->mv_velocity.y >= Border_DOWN+20) //下一帧与下边界碰撞
    {
        pplayer->mv_velocity.y = Border_DOWN+20 - (pplayer->mv_pos.y + 20);
    }
}

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
int FindClosest(struct _PLAYER ** players, SVector2D pos, int exID)
{
    int mark, i;
    mark = (0 == exID)? 1 : 0;
    for(i = 0; i < 4; i++)
    {
        if(i != exID && Distance(players[i]->mv_pos, pos) < Distance(players[mark]->mv_pos, pos) )   //排除指定球员
        {
            mark = i;
        }

    }

    return mark;
}

/**********************************************************
Function：     ShootBall

Description：   射门辅助函数，计算得出球员射门时应给球的速度

Input：	   	struct _BALL * pball          			指向球的指针
					int TeamName                            队伍名
			
Output：     None

Return：     该给球的速度

Others：     None
**********************************************************/
SVector2D ShootBall(struct _BALL * pball, int TeamName)
{
    SVector2D ShootV;
    SVector2D TarPos;
    double distance = 0.0;
    double v = 0.0;

    ShootV.x = 0.0;
    ShootV.y = 0.0;

    if(0 == TeamName)
    {
        TarPos.x = Border_RIGHT;
    }
    else
    {
        TarPos.x = Border_LEFT;
    }

    TarPos.y = GoalUP + rand() % 80 + 10;

    distance = Distance(pball->mv_pos, TarPos);
    if(distance > MaxShootL)         //限制射球距离
    {
        distance = MaxShootL;
    }

    v = sqrt( abs(2 * BallDeceleration * distance) ) + 1;
    if(distance < 150.0)         //限制射球距离
    {
        v += 5.0;
    }

    ShootV.x = v * GetDir(TarPos.x - pball->mv_pos.x, TarPos.y - pball->mv_pos.y).x;
    ShootV.y = v * GetDir(TarPos.x - pball->mv_pos.x, TarPos.y - pball->mv_pos.y).y;

    return ShootV;
}

/**********************************************************
Function：     CanShoot

Description：   用于判断球员朝向能否射门

Input：	   struct _PLAYER * pplayer           指向球员的指针
					int TeamName                          队伍名
			
Output：     None

Return：     1表示可以射门，0表示当前朝向无法射门

Others：     None
**********************************************************/
int CanShoot(struct _PLAYER * pplayer, int TeamName)
{
    int mark = 0;
    if(0 == TeamName)      //白队
    {
        if(pplayer->mv_pos.y < GoalUP && RD == pplayer->Dir)
        {
            mark = 1;
        }
        if(pplayer->mv_pos.y > GoalDOWN && RU == pplayer->Dir)
        {
            mark = 1;
        }
        if(pplayer->mv_pos.y > GoalUP && pplayer->mv_pos.y < GoalDOWN && (Right == pplayer->Dir || RD == pplayer->Dir || RU == pplayer->Dir) )
        {
            mark = 1;
        }
    }
    else                    //蓝队
    {
        if(pplayer->mv_pos.y < GoalUP && LD == pplayer->Dir)
        {
            mark = 1;
        }
        if(pplayer->mv_pos.y > GoalDOWN && LU == pplayer->Dir)
        {
            mark = 1;
        }
        if(pplayer->mv_pos.y > GoalUP && pplayer->mv_pos.y < GoalDOWN && (Left == pplayer->Dir || LD == pplayer->Dir || LU == pplayer->Dir))
        {
            mark = 1;
        }
    }
    return mark;
}

/**********************************************************
Function：     BallDec

Description：   用于检测球是否出界或进球

Input：	   struct _BALL * pball          			指向球的指针
			
Output：     None

Return：     若出界返回1，若白队进球返回2，若蓝队进球返回3，
				   若以上情况都没发生返回0

Others：     None
**********************************************************/
int BallDec(struct _BALL * pball)
{
    int mark = 0;


    if(pball->mv_pos.x < Border_LEFT && pball->mv_oldpos.x >= Border_LEFT)
    {
        if(pball->mv_pos.y > GoalUP && pball->mv_pos.y < GoalDOWN)  //蓝队进球
        {
            mark = 3;
        }
        else
        {
            mark = 1;
        }
    }
    if(pball->mv_pos.x > Border_RIGHT && pball->mv_oldpos.x <= Border_RIGHT)
    {
        if(pball->mv_pos.y > GoalUP && pball->mv_pos.y < GoalDOWN)  //白队进球
        {
            mark = 2;
        }
        else
        {
            mark = 1;
        }

    }
    if(pball->mv_pos.y < Border_UP && pball->mv_oldpos.x >= Border_UP)
    {
        mark = 1;
    }
    if(pball->mv_pos.y > Border_DOWN && pball->mv_oldpos.y <= Border_DOWN)
    {
        mark = 1;
    }

    return mark;

}

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
void Restart(struct _TEAM * pWteam, struct _TEAM * pBteam, struct _BALL * pball, struct _GOALKEEPER * Gkeepers)
{

    int i;
    //足球复位
    Circlefill((int)(pball->mv_oldpos.x), (int)(pball->mv_oldpos.y), 25, 0x19);
    ReBack((int)(pball->mv_oldpos.x)-26, (int)(pball->mv_oldpos.y)-26, 52, 52);
    pball->mv_velocity.x = 0.0;
    pball->mv_velocity.y = 0.0;
    pball->mv_pos.x = 512.0;
    pball->mv_pos.y = 445.0;
    pball->mv_oldpos.x = 512.0;
    pball->mv_oldpos.y = 445.0;
    pball->Owner = -1;

    //白队复位
    pWteam->ClosestPlayer = 0;
    pWteam->ReceivingPlayer = -1;
    pWteam->ControllingPlayer = -1;
    pWteam->SupportingPlayer = -1;
    pWteam->IsControllBall = 0;
    pWteam->pcurrentstate = &pWteam->Defend;

    for(i = 0; i < 4; i++)
    {
        Bar( (int)(pWteam->player[i]->mv_pos.x)-21, (int)(pWteam->player[i]->mv_pos.y)-21, (int)(pWteam->player[i]->mv_pos.x)+21, (int)(pWteam->player[i]->mv_pos.y)+21, 0x19);
        ReBack((int)(pWteam->player[i]->mv_pos.x)-20,(int)(pWteam->player[i]->mv_pos.y)-20, 40, 40);
        pWteam->player[i]->mv_velocity.x = 0.0;
        pWteam->player[i]->mv_velocity.y = 0.0;
        pWteam->player[i]->pcurrentstate = &pWteam->player[i]->Wait;
        pWteam->player[i]->Dir = Right;
    }

    pWteam->player[0]->mv_pos.x = 450.0;
    pWteam->player[0]->mv_pos.y = 370.0;

    pWteam->player[1]->mv_pos.x = 350.0;
    pWteam->player[1]->mv_pos.y = 560.0;

    pWteam->player[2]->mv_pos.x = 230.0;
    pWteam->player[2]->mv_pos.y = 560.0;

    pWteam->player[3]->mv_pos.x = 230.0;
    pWteam->player[3]->mv_pos.y = 330.0;


    //蓝队复位
    pBteam->ClosestPlayer = 0;
    pBteam->ReceivingPlayer = -1;
    pBteam->ControllingPlayer = -1;
    pBteam->SupportingPlayer = -1;
    pBteam->IsControllBall = 0;
    pBteam->pcurrentstate = &pBteam->Defend;

    for(i = 0; i < 4; i++)
    {
        Bar((int)(pBteam->player[i]->mv_pos.x)-21, (int)(pBteam->player[i]->mv_pos.y)-21, (int)(pBteam->player[i]->mv_pos.x)+21, (int)(pBteam->player[i]->mv_pos.y)+21, 0x19);
        ReBack((int)(pBteam->player[i]->mv_pos.x)-20,(int)(pBteam->player[i]->mv_pos.y)-20, 40, 40);
        pBteam->player[i]->mv_velocity.x = 0.0;
        pBteam->player[i]->mv_velocity.y = 0.0;
        pBteam->player[i]->pcurrentstate = &pBteam->player[i]->Wait;
        pBteam->player[i]->Dir = Left;
    }

    pBteam->player[0]->mv_pos.x = 564.0;
    pBteam->player[0]->mv_pos.y = 520.0;

    pBteam->player[1]->mv_pos.x = 670.0;
    pBteam->player[1]->mv_pos.y = 330.0;

    pBteam->player[2]->mv_pos.x = 790.0;
    pBteam->player[2]->mv_pos.y = 330.0;

    pBteam->player[3]->mv_pos.x = 790.0;
    pBteam->player[3]->mv_pos.y = 560.0;

    //守门员复位
    Bar( (int)(Gkeepers[0].mv_pos.x)-21, (int)(Gkeepers[0].mv_pos.y)-21, (int)(Gkeepers[0].mv_pos.x)+21, (int)(Gkeepers[0].mv_pos.y)+21, 0x19);
    ReBack((int)(Gkeepers[0].mv_pos.x)-20,(int)(Gkeepers[0].mv_pos.y)-20, 40, 40);
    Gkeepers[0].mv_velocity.x = 0.0;
    Gkeepers[0].mv_velocity.y = 0.0;
    Gkeepers[0].mv_pos.x = 80.0;
    Gkeepers[0].mv_pos.y = 445.0;
    DrawPlayer((int)(Gkeepers[0].mv_pos.x), (int)(Gkeepers[0].mv_pos.y), Right, 0XF6);

    Bar( (int)(Gkeepers[1].mv_pos.x)-21, (int)(Gkeepers[1].mv_pos.y)-21, (int)(Gkeepers[1].mv_pos.x)+21, (int)(Gkeepers[1].mv_pos.y)+21, 0x19);
    ReBack((int)(Gkeepers[1].mv_pos.x)-20,(int)(Gkeepers[1].mv_pos.y)-20, 40, 40);
    Gkeepers[1].mv_velocity.x = 0.0;
    Gkeepers[1].mv_velocity.y = 0.0;
    Gkeepers[1].mv_pos.x = 940.0;
    Gkeepers[1].mv_pos.y = 445.0;
    DrawPlayer((int)(Gkeepers[1].mv_pos.x), (int)(Gkeepers[1].mv_pos.y), Left, LIGHTBLUE);
}


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
void DrawPlayer(int x, int y, enum _dir Dir, unsigned char color)
{

    Circlefill(x, y, 10, color);

    switch(Dir)
    {
        case Up:
                Line(x - 20, y - 5, x + 20, y - 5, color);
                Line(x - 14, y + 5, x + 14, y + 5, color);
                Line(x - 20, y - 5, x - 14, y + 5, color);
                Line(x + 20, y - 5, x + 14, y + 5, color);
                break;
        case Down:
                Line(x - 20, y + 5, x + 20, y + 5, color);
                Line(x - 14, y - 5, x + 14, y - 5, color);
                Line(x - 20, y + 5, x - 14, y - 5, color);
                Line(x + 20, y + 5, x + 14, y - 5, color);
                break;
        case Right:
                Line(x + 5, y - 20, x + 5, y + 20, color);
                Line(x - 5, y - 14, x - 5, y + 14, color);
                Line(x + 5, y - 20, x - 5, y - 14, color);
                Line(x + 5, y + 20, x - 5, y + 14, color);
                break;
        case Left:
                Line(x - 5, y - 20, x - 5, y + 20, color);
                Line(x + 5, y - 14, x + 5, y + 14, color);
                Line(x - 5, y - 20, x + 5, y - 14, color);
                Line(x - 5, y + 20, x + 5, y + 14, color);
                break;
        case RU:
                Line(x + 18, y + 11, x - 11, y - 18, color);
                Line(x + 6, y + 13, x - 13, y - 6, color);
                Line(x + 18, y + 11, x + 6, y + 13, color);
                Line(x - 11, y - 18, x - 13, y - 6, color);
                break;
        case RD:
                Line(x + 18, y - 11, x - 11, y + 18, color);
                Line(x + 6, y - 13, x - 13, y + 6, color);
                Line(x + 18, y - 11, x + 6, y - 13, color);
                Line(x - 11, y + 18, x - 13, y + 6, color);
                break;
        case LU:
                Line(x - 18, y + 11, x + 11, y - 18, color);
                Line(x - 6, y + 13, x + 13, y - 6, color);
                Line(x - 18, y + 11, x - 6, y + 13, color);
                Line(x + 11, y - 18, x + 13, y - 6, color);
                break;
        case LD:
                Line(x - 18, y - 11, x + 11, y + 18, color);
                Line(x - 6, y - 13, x + 13, y + 6, color);
                Line(x - 18, y - 11, x - 6, y - 13, color);
                Line(x + 11, y + 18, x + 13, y + 6, color);
                break;
    }

}


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
void DrawNum(int x, int y, int size, unsigned char color, int num)
{

    if(size > 120)
    {
        exit(0);
    }
    switch(num)
    {
        case 0: Bar(x+27*size/100, y+0*size/100, x+73*size/100, y+18*size/100, color);
                Circlefill(x+27*size/100, y+9*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+9*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+182*size/100, x+73*size/100, y+200*size/100, color);
                Circlefill(x+27*size/100, y+191*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+191*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+29*size/100, x+18*size/100, y+81*size/100, color);
                Circlefill(x+9*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+119*size/100, x+18*size/100, y+171*size/100, color);
                Circlefill(x+9*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+171*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+29*size/100, x+100*size/100, y+81*size/100, color);
                Circlefill(x+91*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;
        case 1: Bar(x+82*size/100, y+29*size/100, x+100*size/100, y+81*size/100, color);
                Circlefill(x+91*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;
        case 2: Bar(x+27*size/100, y+0*size/100, x+73*size/100, y+18*size/100, color);
                Circlefill(x+27*size/100, y+9*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+9*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+91*size/100, x+73*size/100, y+109*size/100, color);
                Circlefill(x+27*size/100, y+100*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+100*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+182*size/100, x+73*size/100, y+200*size/100, color);
                Circlefill(x+27*size/100, y+191*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+191*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+119*size/100, x+18*size/100, y+171*size/100, color);
                Circlefill(x+9*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+171*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+29*size/100, x+100*size/100, y+81*size/100, color);
                Circlefill(x+91*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+81*size/100, 9*size/100, color);
                break;
        case 3: Bar(x+27*size/100, y+0*size/100, x+73*size/100, y+18*size/100, color);
                Circlefill(x+27*size/100, y+9*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+9*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+91*size/100, x+73*size/100, y+109*size/100, color);
                Circlefill(x+27*size/100, y+100*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+100*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+182*size/100, x+73*size/100, y+200*size/100, color);
                Circlefill(x+27*size/100, y+191*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+191*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+29*size/100, x+100*size/100, y+81*size/100, color);
                Circlefill(x+91*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;
        case 4: Bar(x+27*size/100, y+91*size/100, x+73*size/100, y+109*size/100, color);
                Circlefill(x+27*size/100, y+100*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+100*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+29*size/100, x+18*size/100, y+81*size/100, color);
                Circlefill(x+9*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+29*size/100, x+100*size/100, y+81*size/100, color);
                Circlefill(x+91*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;
        case 5: Bar(x+27*size/100, y+0*size/100, x+73*size/100, y+18*size/100, color);
                Circlefill(x+27*size/100, y+9*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+9*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+91*size/100, x+73*size/100, y+109*size/100, color);
                Circlefill(x+27*size/100, y+100*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+100*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+182*size/100, x+73*size/100, y+200*size/100, color);
                Circlefill(x+27*size/100, y+191*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+191*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+29*size/100, x+18*size/100, y+81*size/100, color);
                Circlefill(x+9*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;
        case 6: Bar(x+27*size/100, y+0*size/100, x+73*size/100, y+18*size/100, color);
                Circlefill(x+27*size/100, y+9*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+9*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+91*size/100, x+73*size/100, y+109*size/100, color);
                Circlefill(x+27*size/100, y+100*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+100*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+182*size/100, x+73*size/100, y+200*size/100, color);
                Circlefill(x+27*size/100, y+191*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+191*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+29*size/100, x+18*size/100, y+81*size/100, color);
                Circlefill(x+9*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+119*size/100, x+18*size/100, y+171*size/100, color);
                Circlefill(x+9*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+171*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;
        case 7: Bar(x+27*size/100, y+0*size/100, x+73*size/100, y+18*size/100, color);
                Circlefill(x+27*size/100, y+9*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+9*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+29*size/100, x+100*size/100, y+81*size/100, color);
                Circlefill(x+91*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;
        case 8: Bar(x+27*size/100, y+0*size/100, x+73*size/100, y+18*size/100, color);
                Circlefill(x+27*size/100, y+9*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+9*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+91*size/100, x+73*size/100, y+109*size/100, color);
                Circlefill(x+27*size/100, y+100*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+100*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+182*size/100, x+73*size/100, y+200*size/100, color);
                Circlefill(x+27*size/100, y+191*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+191*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+29*size/100, x+18*size/100, y+81*size/100, color);
                Circlefill(x+9*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+119*size/100, x+18*size/100, y+171*size/100, color);
                Circlefill(x+9*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+171*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+29*size/100, x+100*size/100, y+81*size/100, color);
                Circlefill(x+91*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;
        case 9: Bar(x+27*size/100, y+0*size/100, x+73*size/100, y+18*size/100, color);
                Circlefill(x+27*size/100, y+9*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+9*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+91*size/100, x+73*size/100, y+109*size/100, color);
                Circlefill(x+27*size/100, y+100*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+100*size/100, 9*size/100, color);

                Bar(x+27*size/100, y+182*size/100, x+73*size/100, y+200*size/100, color);
                Circlefill(x+27*size/100, y+191*size/100, 9*size/100, color);
                Circlefill(x+73*size/100, y+191*size/100, 9*size/100, color);

                Bar(x+0*size/100, y+29*size/100, x+18*size/100, y+81*size/100, color);
                Circlefill(x+9*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+9*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+29*size/100, x+100*size/100, y+81*size/100, color);
                Circlefill(x+91*size/100, y+29*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+81*size/100, 9*size/100, color);

                Bar(x+82*size/100, y+119*size/100, x+100*size/100, y+171*size/100, color);
                Circlefill(x+91*size/100, y+119*size/100, 9*size/100, color);
                Circlefill(x+91*size/100, y+171*size/100, 9*size/100, color);
                break;

    }

}

/**********************************************************
Function：     Updategoal

Description：   更新比分

Input：	   		int Wgoal            	 白队得分
						int Bgoal		    		 蓝队得分
			
Output：     None

Return：    None

Others：    None
**********************************************************/
void Updategoal(int Wgoal, int Bgoal)
{
    //白队得分
    Bar(445, 95, 485, 175, 0X86);
    DrawNum(445, 95, 40, 0X0E, Wgoal%10);    //个位
    Bar(395, 95, 435, 175, 0X86);
    DrawNum(395, 95, 40, 0X0E, Wgoal/10);    //十位

    //蓝队得分
    Bar(585, 95, 625, 175, 0X86);
    DrawNum(585, 95, 40, 0X0E, Bgoal%10);    //个位
    Bar(535, 95, 575, 175, 0X86);
    DrawNum(535, 95, 40, 0X0E, Bgoal/10);    //十位

}

/**********************************************************
Function：     UPdatetime

Description：   更新倒计时

Input：	   		int time            	 剩余时间（单位S）
			
Output：     None

Return：    None

Others：    None
**********************************************************/
void UPdatetime(int time)
{
    int min = 0,       //分
        sec = 0;       //秒

    min = time / 60;
    sec = time - min * 60;

    //画秒
    Bar(543, 35, 561, 71, 0X86);
    DrawNum(543, 35, 18, 0X0E, sec%10);    //个位
    Bar(517, 35, 535, 71, 0X86);
    DrawNum(517, 35, 18, 0X0E, sec/10);    //十位

    //画分钟(10分钟以内)
    Bar(480, 35, 498, 71, 0X86);
    DrawNum(480, 35, 18, 0X0E, min);

}

/**********************************************************
Function：      GamePause

Description：   暂停游戏界面，子选项可以恢复比赛或退出

Input：      None

Output：     None

Return：     1  用户想退出游戏
             0  用户想恢复游戏

Others：     None
**********************************************************/
int GamePause(void)
{
    if(GetKey(KEY_ESC))
    {
        //贴出界面
        Putbmp256(310, 220, "bmp\\pause2.bmp");
		//消除之前按下的Y
		GetKey(KEY_Y);

        //循环检测按键动作
        while(1)
        {
            if(GetKey(KEY_Y))            //退出游戏
            {
                return 1;
            }
            else if(GetKey(KEY_ESC))     //恢复游戏
            {
                Bar(310, 220, 710, 470, 0X19);
                ReBack(310, 220, 400, 250);
                return 0;
            }
        }
    }
    return 0;
}

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
double CanSafePass(SVector2D pos1, SVector2D tar_pos, struct _TEAM * pOppteam)
{
    int i;
    double score = 1.0;
    double disdot = 0.0;             //点乘结果
    double disver = 0.0;           //垂直距离
    double distance = 0.0;         //传球距离
    double mimdisver = 1000.0;        //最小垂直距离
    double v = 0.0;                    //传球速度
    double Time = 0.0;                 //截球所需时间
    double MaxTime = 0.0;

    distance = Distance(pos1, tar_pos);
    v = sqrt(2 * distance * BallDeceleration);
    MaxTime = v / BallDeceleration;

    for(i = 0; i < 4; i++)
    {
        disdot = (pOppteam->player[i]->mv_pos.x-pos1.x)*(tar_pos.x-pos1.x) + (pOppteam->player[i]->mv_pos.y-pos1.y)*(tar_pos.y-pos1.y);
        disdot /= sqrt((tar_pos.x-pos1.x)*(tar_pos.x-pos1.x) + (tar_pos.y-pos1.y)*(tar_pos.y-pos1.y));
        if(disdot > 0.0 && disdot < distance)             //点乘小于0或大于传球距离的不计入威胁范围
        {
            disver = GetDisver(pos1, tar_pos, pOppteam->player[i]->mv_pos);
            disdot = sqrt(Distance(pos1, pOppteam->player[i]->mv_pos)*Distance(pos1, pOppteam->player[i]->mv_pos) - disver*disver);
            Time = disver / pOppteam->player[i]->Max_v;         //计算截球时间
            if(Time < MaxTime && Time * v - 0.5 * BallDeceleration * Time * Time < disdot)                        //球能被截
            {
                score = 0.0;
            }
            if(disver < mimdisver)                          //记录最短垂直距离
            {
                mimdisver = disver;
            }

        }
        disver = 0;
        disdot = 0;           //复位
    }

    if(score != 0.0)
    {
        score = mimdisver;
    }

    return score;

}

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
double GetDisver(SVector2D pos1, SVector2D pos2, SVector2D pos3)
{
    double S = 0.0;

    S = abs(pos1.x*pos2.y - pos2.x*pos1.y + pos2.x*pos3.y - pos3.x*pos2.y + pos3.x*pos1.y - pos1.x*pos3.y);

    return S/Distance(pos1, pos2);

}

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
int FindPass(struct _PLAYER * pplayer, struct _TEAM * pOurteam, struct _TEAM * pOppteam)
{
    int i;
    int mark = -1;
    for(i = 0; i < 4; i++)
    {
        //排除自己；排除距离大于250的；排除不能安全传球的
        if(i != pplayer->ID%4 && Distance(pplayer->mv_pos, pOurteam->player[i]->mv_pos) <= 300.0 && CanSafePass(pplayer->mv_pos, pOurteam->player[i]->mv_pos, pOppteam))
        {
            //找出离对方球门最近的
            if(-1 == mark)
            {
                mark = i;
            }
            else if(0 == pOurteam->name && pOurteam->player[i]->mv_pos.x > pOurteam->player[mark]->mv_pos.x)               //白队
            {
                mark = i;
            }
            else if(1 == pOurteam->name && pOurteam->player[i]->mv_pos.x < pOurteam->player[mark]->mv_pos.x)               //蓝队
            {
                mark = i;
            }
        }
    }

    return mark;

}


/**********************************************************
Function：    IsDanger

Description：   检测带球队员有无危险，是否需要传球

Input：	   		struct _PLAYER * pplayer            	           指向持球队员的指针
						struct _TEAM * pOppteam					       指向对方队伍的指针
						double range                                              判定范围半径
			
Output：     None

Return：   0表示无危险，1表示有危险

Others：    None
**********************************************************/
int IsDanger(struct _PLAYER * pplayer, struct _TEAM * pOppteam, double range)
{
    int mark = 0;
    int i;

    for(i = 0; i < 4; i++)
    {
        if(Distance(pplayer->mv_pos, pOppteam->player[i]->mv_pos) <= range)
        {
            mark = 1;
        }
    }

    return mark;
}


/**********************************************************
Function：    PlayerControl

Description：   根据键盘消息控制球员速度

Input：	   		struct _PLAYER * pplayer            	           指向持球队员的指针
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void PlayerControl(struct _PLAYER * pplayer)
{
	if(pplayer->ID < 4)
	{
		if(KeyPress(KEY_W))
		{
			pplayer->mv_velocity.y = -pplayer->Max_v;

		}
		else if(KeyPress(KEY_S))
		{
			pplayer->mv_velocity.y = pplayer->Max_v;

		}

		if(KeyPress(KEY_A))
		{

			pplayer->mv_velocity.x = -pplayer->Max_v;
		}
		else if(KeyPress(KEY_D))
		{

			pplayer->mv_velocity.x = pplayer->Max_v;
		}
	}
	else 
	{
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
	}
	
}

/**********************************************************
Function：    DrawGameBK

Description：   画球场背景

Input：	   	None
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void DrawGameBK(void)
{
	Bar(0, 0, 1024, 768, 0X19);
	
    Bar(18, 390, 64, 498, 0XF6);
    Bar(22, 394, 60, 494, 0X19);

    Bar(958, 390, 1002, 498, 0XF6);
    Bar(962, 394, 998, 494, 0X19);
    ReBack(60, 198, 904, 493);
}

/**********************************************************
Function：    DrawScoreBoard

Description：   画计分板

Input：	   	None
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void DrawScoreBoard(int Wgoal, int Bgoal)
{
	Bar(387, 82, 633, 183, 0XF6);
    Bar(390, 85, 630, 180, 0X86);
    Circlefill(507, 115, 8, 0X0E);
    Circlefill(507, 155, 8, 0X0E);
    Updategoal(Wgoal, Bgoal);
}

/**********************************************************
Function：    DrawTimer

Description：   画计时板

Input：	   	None
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void DrawTimer(int Time)
{
	Bar(444, 27, 570, 78, 0XF6);
    Bar(447, 30, 567, 75, 0X86);
    Circlefill(507, 38, 4, 0X0E);
    Circlefill(507, 65, 4, 0X0E);
    UPdatetime(Time);
}

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
void SetPlayersPara(struct _PLAYER Wplayers[], struct _PLAYER Bplayers[], struct _FIELDPLAYER fieldplayers)
{
	int i;
	FILE * fdata = NULL;
	
	//球员参数临时变量
    float PlayerV = 0.0;       //基础速度
    float get_rate = 0.8;      //抢断率
    float acc_rate = 1.0;      //加速倍率
    float acc_time = 1.5;      //加速持续时间
    float acc_cd = 3.0;        //加速CD
    char IDtemp[3] = "\0";     //用于定位数据文档中指定的球员
    int PlayerID = -1;         //指定球员ID
    char photopath[40] = "\0";        //球员头像路径
	
	//打开球员数据文档
	if((fdata = fopen("data\\players.txt", "r")) == NULL)
    {
        exit(1);
    }
	
	
	for(i = 0; i < 4; i++)
    {
        rewind(fdata);
        //白队
        //定位到指定球员
        while( !(strcmp(IDtemp, "ID") == 0 && PlayerID == fieldplayers.WplayerID[i]) )
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
        fscanf(fdata, "%s", photopath);
		
		//贴出球员头像
        if(fieldplayers.WplayerID[i] != 0)
        {
            Putbmp256(90 + 130*(i%2), 25 + 95*(i/2) , photopath);
        }
        else
        {
            Putbmp256(90 + 130*(i%2), 25 + 95*(i/2) , "bmp\\mini\\0.bmp");
        }

		//根据球员星级加强能力参数
        Wplayers[i].Max_v = PlayerV * (fieldplayers.Wstars[i]*0.1+1);
        Wplayers[i].get_rate = get_rate * (fieldplayers.Wstars[i]*0.1+1);
        Wplayers[i].acc_rate = acc_rate;
        Wplayers[i].acc_time = acc_time * (fieldplayers.Wstars[i]*0.1+1);
        Wplayers[i].acc_cd = acc_cd;
		
		//判断是否使用了速度加成道具，并更新球员速度参数
		if(fieldplayers.Tool > 0 && fieldplayers.Tool <= 3)
		{
			Wplayers[i].Max_v *= 1.0 + fieldplayers.Tool*0.1;
		}
    }

    for(i = 0; i < 4; i++)
    {
        rewind(fdata);
        //蓝队
        //定位到指定球员
        while( !(strcmp(IDtemp, "ID") == 0 && PlayerID == fieldplayers.BplayerID[i]) )
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
        fscanf(fdata, "%s", photopath);

        if(fieldplayers.BplayerID[i] != 0)
        {
            Putbmp256(730 + 130*(i%2), 25 + 95*(i/2) , photopath);
        }
        else
        {
            Putbmp256(730 + 130*(i%2), 25 + 95*(i/2) , "bmp\\mini\\0.bmp");
        }

		//根据球员星级加强能力参数
        Bplayers[i].Max_v = PlayerV * (fieldplayers.Bstars[i]*0.1+1);
        Bplayers[i].get_rate = get_rate * (fieldplayers.Bstars[i]*0.1+1);
        Bplayers[i].acc_rate = acc_rate;
        Bplayers[i].acc_time = acc_time * (fieldplayers.Bstars[i]*0.1+1);
        Bplayers[i].acc_cd = acc_cd;
    }

	fclose(fdata);
}

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
void SetAIMode(struct _PLAYER Bplayers[], int gamemode, int botmode)
{
	int i;
	float PlayerV = 0.0;       //基础速度
    float get_rate = 0.8;      //抢断率
	
	if(0 == gamemode)
    {
        if(0 == botmode)
        {
            PlayerV = 2.0;
            get_rate = 0.3;

        }
        else if(1 == botmode)
        {
            PlayerV = 2.5;
            get_rate = 0.7;
        }
        else if(2 == botmode)
        {
            PlayerV = 3.2;
            get_rate = 0.95;
        }
        else
        {
            exit(1);
        }

        for(i = 0; i < 4; i++)
        {
            Bplayers[i].Max_v = PlayerV;
            Bplayers[i].get_rate = get_rate;
        }
    }
}

/**********************************************************
Function：    GetKeyboardAct

Description：   获取键盘传入的传球、射门动作

Input：	   	char *key         动作标记      
			
Output：     None

Return：   None

Others：    None
**********************************************************/
void GetKeyboardAct(char *key)
{
	if(GetKey(KEY_R))
	{
		*key = 'f';
	}
	else if(GetKey(KEY_T))
	{
		*key = 'g';
	}
	else if(GetKey(KEY_MINUS))
	{
		*key = 'n';
	}
	else if(GetKey(KEY_PLUS))
	{
		*key = 'm';
	}
}



