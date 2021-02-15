#include "Order.h"
#include<iostream>
using namespace std;

Order::Order(int id, ORD_TYPE r_Type, int arrival, int ORD_SIZE, int ORD_MNY)
{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999
	type = r_Type;
	status = WAIT;
	ArrTime = arrival;
	totalMoney = ORD_MNY;
	Size = ORD_SIZE;
	Urgent = false;
}

Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


ORD_TYPE Order::GetType() const
{
	return type;
}


void Order::SetSize(int s)
{
	Size = s > 0 ? s : 0;
}

int Order::GetSize() const
{
	return Size;
}


void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

int Order::getPriority()
{
	if (type != TYPE_VIP)
		return -1;
	else
		return round(-.5 * ArrTime - .1 * Size + totalMoney / Size); //priority equation here 

}

void Order::SetType(ORD_TYPE newType)
{
	type = newType;
}

int Order::GetArrivalTime()
{
	return ArrTime;
}

void Order::addMoney(int money)
{
	totalMoney += money;
}

void Order::SetFinishedTime(int t)
{
	FinishTime = t;
}

int Order::getFinishedTime()
{
	return FinishTime;
}

void Order::SetServingTime(int t)
{
	ServTime = t;
}

int Order::getServingTime()
{
	return ServTime;
}
void Order::setAutoPromoted(float ap)
{
	Autopromoted = ap;
}
float Order::getAutoPromoted()
{
	return Autopromoted;
}
int Order::getWaitingTime()
{
	return FinishTime - ArrTime - ServTime;
}

void Order::SetAssignTime(int t)
{
	AssignTime = t;
}

int Order::GetAssignTime()
{
	return AssignTime;
}


void Order::make_urgent()
{
	if (type == TYPE_VIP)            //as this option is for VIP orders only
		Urgent = true;

}
bool Order::is_urgent()
{
	return Urgent;
}

