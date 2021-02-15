#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType):Event(eTime, oID)
{
	OrdType = oType;
}

ArrivalEvent::ArrivalEvent(int eTime, int oID, int oSize, int oMONEY, ORD_TYPE oType) : Event(eTime, oID)
{
	OrdType = oType;
	OrdSize = oSize;
	OrdMoney = oMONEY;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1
	Order *Ord=new Order(OrderID,OrdType,this->EventTime, this->OrdSize,this->OrdMoney);
	
	switch (OrdType)
	{
	case TYPE_NRM:
		pRest->AddtoNormalQueue(Ord);
		break;
	case TYPE_VGAN:
		pRest->AddtoVeganQueue(Ord);
		break;
	case TYPE_VIP:
		pRest->AddtoVIPQueue(Ord);
		break;

	}
	
	
	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2
	//Order* pOrd = new Order(OrderID,OrdType);
	//pRest->AddtoDemoQueue(pOrd);
}
