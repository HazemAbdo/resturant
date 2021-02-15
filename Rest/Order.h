#ifndef __ORDER_H_
#define __ORDER_H_
#include "..\Defs.h"
//#include"Cook.h"
class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Size;
	double totalMoney;	//Total order money
	//Cook* Order_cook;
	int ArrTime;	//arrival, service start, and finish times
	int Priority;
	int ServTime;
	int FinishTime;
	int AssignTime;
	float Autopromoted;

	bool Urgent;
	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order(int id, ORD_TYPE r_Type, int arrival, int ORD_SIZE, int ORD_MNY);
	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;

	void SetSize(int d);
	int GetSize() const;

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;

	//
	// TODO: Add More Member Functions As Needed
	int getPriority();
	void SetType(ORD_TYPE newType);
	int GetArrivalTime();
	void addMoney(int money);
	void SetFinishedTime(int t);
	int getFinishedTime();

	void SetServingTime(int t);
	int getServingTime();
	void setAutoPromoted(float ap);
	float getAutoPromoted();
	int getWaitingTime();
	void setWaitingTime(int t);

	void SetAssignTime(int t);
	int GetAssignTime();

	void make_urgent();
	bool is_urgent();
	//void SetCook(Cook*);
	//Cook* getCook();
	//

};

#endif







