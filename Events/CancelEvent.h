#pragma once

#include"Event.h"
#include"..\Rest\Restaurant.h"

class CancelEvent:public Event
{
public:
	CancelEvent(int eTime, int ordID);
	virtual void Execute(Restaurant* pRest);	////a pointer to "Restaurant" and events need it to execute

};
