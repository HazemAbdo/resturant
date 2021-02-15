#pragma once

#include "..\Defs.h"
#include"Order.h"
class Cook
{
    int ID;
    ORD_TYPE type;  //for each order type there is a corresponding type (VIP, Normal, Vegan)
    int speed;    //dishes it can prepare in one clock tick (in one timestep)
    Order * CookOrder;
    Cook_STATUS Statue;
    int BO;         //the number of orders a cook must prepare before taking a break
    int breaktime;
    int FinishedOrders;
    int RestTime;

    int injury_rst;    //the rst period wich an injured cook should rst before being avil again
    const int  original_speed;     //the cook's original speed

public:
    Cook(int id, int SPEED, int btime, int b);
    virtual ~Cook();
    int GetID() const;
    ORD_TYPE GetType() const;
    void setID(int);
    void setType(ORD_TYPE);
    void SetStatue(Cook_STATUS);
    Cook_STATUS GetStatue();
    void SetOder(Order*);
    Order* GetOrder();
    void SetSpeed(int t);
    int GetSpeed();
    int GetBO();
    int GetBreakTime();
    void SetFinishedOrders(int FO);
    int GetFinishedOrders();
    void SetRestTime(int t);
    int GetRestTime();

    void set_injury_rst(int rst);
    int get_injury_rst();

    int get_original_speed();

};