#include "PromoteEvent.h"


PromoteEvent::PromoteEvent(int eTime, int ordID, int extraMoney) :Event(eTime, ordID)
{
	money = extraMoney;
}
void PromoteEvent::Execute(Restaurant* pRest)
{
	pRest->Promote(getOrderID(), true,money);
}