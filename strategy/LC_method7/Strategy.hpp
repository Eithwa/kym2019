/**
 * @file Strategy.hpp
 *
 * @brief Localization challange pathplan strategy
 *
 * @date July 2017
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/
#ifndef _STRATEGY_HPP_
#define _STRATEGY_HPP_
/*****************************************************************************
** Includes
*****************************************************************************/
#include "../common/BaseNode.h"
#include <ros/ros.h>
#include "std_msgs/Int32.h"
#include <math.h>
#include "Env.hpp"
#include <iomanip>
//#include <unistd.h>　
/*****************************************************************************
** Define
*****************************************************************************/

class Strategy
{
  public:
    ///         public member           ///
    ///         constructor             ///
    Strategy();
    virtual ~Strategy() {}
    void setEnv(Environment *Env) { _Env = Env; }
    void setParam(Parameter *);
    void GameState(int);
    void setLocationPoint(LocationStruct *LocationPoint) { _Location = LocationPoint; }
    Environment getEnv() { return *_Env; }
    int stop_count;
  private:
    ///         private member          ///
    void StrategyHalt();
    void StrategyLocalization();
    void StrategyLocalization2();
    void Forward(RobotData &, double &, double &, double &, double, int &, double, double, double);
    void Turn(RobotData &, double &, double &, double &, double, int &, double);
    void Chase(RobotData &,double &v_x, double &v_y, double &v_yaw);
    std::vector<int> OptimatePath();
    void EraseElement(std::vector<int> &, int);
    void MinAngle(std::vector<int> &, std::vector<int> &, int);
    int ThroughPath(int, int);
    void Normalization(double &);
    int _LocationState;
    int _CurrentTarget;
    int _Last_state;
    bool back_flag;
    bool cross_center_flag;
    bool cincture_enable;

    int through_path_ary[5][5];
    TargetStruct _Target;
    LocationStruct *_Location;
    Environment *_Env;
    Parameter *_Param;
    void showInfo(double, double, double);
    void showInfo(double, double, std::vector<int>, double, double, double);
    enum state_location
    {
        forward = 0,
        back,
        finish,
        chase,
        turn,
        restart,
        error
    };
    enum hotizon_location
    {
        up,
        down,
        right,
        left
    };
};
#endif
