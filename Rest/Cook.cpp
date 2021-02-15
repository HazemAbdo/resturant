#include "Cook.h"
#include<cstdlib>
#include<time.h>
Cook::Cook(int id, int SPEED, int btime, int b): original_speed(SPEED)
{
    ID = id;
    speed = SPEED;
    BO = b;
    breaktime = btime;
    Statue = Avail;
    FinishedOrders = 0;
    RestTime = 0;
    injury_rst = 0;
}


Cook::~Cook()
{
}


int Cook::GetID() const
{
    return ID;
}


ORD_TYPE Cook::GetType() const
{
    return type;
}


void Cook::setID(int id)
{
    ID = id;
}

void Cook::setType(ORD_TYPE t)
{
    type = t;
}

void Cook::SetStatue(Cook_STATUS s)
{

    Statue = s;
}

Cook_STATUS Cook::GetStatue()
{
    return Statue;
}

void Cook::SetOder(Order* O)
{
    CookOrder = O;
}

Order* Cook::GetOrder()
{
    return  CookOrder;
}

void Cook::SetSpeed(int t)
{
    speed = t;
}

int Cook::GetSpeed()
{
    return speed;
}

int Cook::GetBO()
{
    return BO;
}

int Cook::GetBreakTime()
{
    return breaktime;
}

void Cook::SetFinishedOrders(int FO)
{
    FinishedOrders = FO;
}

int Cook::GetFinishedOrders()
{
    return FinishedOrders;
}

void Cook::SetRestTime(int t)
{
    RestTime = t;
}

int Cook::GetRestTime()
{
    return RestTime;
}

void  Cook::set_injury_rst(int rst)
{
    injury_rst = rst;
}
int  Cook::get_injury_rst()
{
    return injury_rst;
}
int  Cook::get_original_speed()
{
    return original_speed;
}