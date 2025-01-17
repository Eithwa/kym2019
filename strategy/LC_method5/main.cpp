/**
 * @file main.cpp
 *
 * @brief Localization challange main program
 *
 * @date July 2017
 **/
/*****************************************************************************
** Includes
*****************************************************************************/
#include "ros/ros.h"
#include "NodeHandle.hpp"
#include "Initial.hpp"
#include "Strategy.hpp"

int main(int argc, char **argv)
{
    printf("Localization 5 : straight through pathplan (fix)!!\n");
    Environment Env = InitData(Env); // Initial new environment data
    Environment OutSpeed;
    LocationStruct Location;
    Parameter Global_Param;
    NodeHandle nodehandle(argc, argv); // Declare nodehandle data
    nodehandle.on_init();              // Node Initializing
    nodehandle.setEnv(&Env);           // Set nodehandle environment datad
    nodehandle.setParam(&Global_Param);
    nodehandle.setLocationPoint(&Location);
    Strategy strategy;     // Declare strategy class data
    strategy.setEnv(&Env); // Set strategy environment datad
    strategy.setParam(&Global_Param);
    strategy.setLocationPoint(&Location);
    ros::Rate loop_rate(50);
    ros::Duration(0.25).sleep();
    while (ros::ok())
    {
        if (Env.SaveParam)
        { // Get parameter is restricted to parameter saving by web userinterface
            nodehandle.getParameter();
            Env.SaveParam = 0;
        }
        strategy.GameState(Env.GameState);
        OutSpeed = strategy.getEnv();
        if(strategy.stop_count<5){
            nodehandle.pubSpeed(&OutSpeed);
        }
        nodehandle.pubPath(&OutSpeed);
        nodehandle.pubMove();
        ros::spinOnce();
       loop_rate.sleep();
    }
    return 0;
}
