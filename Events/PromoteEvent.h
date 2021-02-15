#pragma once

#include "Event.h"
#include"..\Rest\Restaurant.h"

class PromoteEvent :
	public Event
{
	int money;
public:
	PromoteEvent(int eTime, int ordID, int extraMoney);
	virtual void Execute(Restaurant* pRest);	////a pointer to "Restaurant" and events need it to execute
};

