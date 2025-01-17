#include <stdio.h>
#include <iostream>
#include <math.h>
#include "../common/defend_Env.h"
#include <ros/ros.h>

#define BallRadius 0.1055
#define CarRadius 0.34

#define L 0
#define M 1
#define R 2
#define stop 3
#define pullback_M 4

class FIRA_pathplan_class
{
private:
    //start---simulator---
    bool opponent;
    Environment env;
    int mTeam;
    //end  ---simulator---


    //start---utility---
    double head2Obj(Vector3D robot,Vector3D dst,double robotRot);
    double vecAngle(Vector2d a,Vector2d b);
    //end---utility---

    int shootblock_reset;


public:

    //start---simulator---
    void setOpponent(bool iBool){opponent = iBool;}
    void setTeam(int team){mTeam = team;}
    void setEnv(Environment iEnv);
    void shoot_init(){shoot = 0;}
    int getShoot(){return shoot;}
    Environment* getEnv(){return &env;}
    //end  ---simulator---

    FIRA_pathplan_class();
    void personalStrategy(int, int);
//    void strategy();

    void strategy_dst(double destination_x,double destination_y);
    void strategy_dst_head2ball(double destination_x,double destination_y);

//    void strategy_dontboom();

    void strategy_head2ball(int);

//--------------strategy case-------------------
//--------------------------------------------------
    void strategy_Goalkeeper_init(int);
    void strategy_Goalkeeper_block(int);
    void strategy_Goalkeeper_push(int);
    void strategy_Goalkeeper_goalkick(int);
    void strategy_Goalkeeper_shootblock(int);


    void strategy_Attack(int);
    void strategy_typeS_Attack(int);
    void strategy_Zone_Attack(int);
    void strategy_typeU_Attack(int);
    void strategy_Dorsad_Attack(int);
    void strategy_Shoot_Attack(int);
    void strategy_Support(int);
    void strategy_Halt(int);
    void strategy_PenaltyKick(int);
    void strategy_ThrowIn(int);
    void strategy_CornerKick(int);
    void strategy_AvoidBarrier(int);
    void strategy_Chase(int);
    void strategy_Straight_Chase(int);
    void strategy_KO5_Chase(int);
    void strategy_KO5_Attack(int);
    void strategy_SideSpeedUp(int);
//--------------------------------------------------

//--------------role case-------------------
    void role_Play();
    void role_Halt();
    void role_FreeKick();
    void role_PenaltyKick();
    void role_FreeBall();
    void role_ThrowIn();
    void role_CornerKick();
    void role_GoalKick();
    void role_AvoidBarrier();
//--------------------------------------------------

//    void teamStrategy();

    //==========for ROS special===============//
    std::string teamColor;
    double beta_const = 0.9;
    double long_rush_alpha;
    double long_rush_dis_br;
    double long_rush_speed_const;
    double short_rush_dis_dr;
    double short_rush_alpha;
    double short_rush_dis_br;
    double short_rush_speed_const;
    double close_ball_dis_const;
    double close_ball_speed_const;
    double far_ball_speed_const;
    double head2ball_speed;

    std::vector<double> SPlanning_Velocity;
    std::vector<double> Distance_Settings;
    std::vector<double> Attack_Strategy;
    std::vector<double> Chase_Strategy;
    std::vector<double> Zone_Attack;
    std::vector<double> TypeS_Attack;
    std::vector<double> TypeU_Attack;
    std::vector<double> Dorsad_Attack;
    std::vector<double> Corner_Kick;
    std::vector<double> SideSpeedUp;
    std::vector<double> Goalkeeper;

    // Robot shoot signal publisher
    int shoot = 0;

    void loadParam(ros::NodeHandle *n);
};



