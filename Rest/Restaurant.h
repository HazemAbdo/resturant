#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Generic_DS\PriorityQueue.h"
#include "..\Events\Event.h"
#include"..\Generic_DS\LinkedList.h"
#include"..\Generic_DS\LinkedListSpecialization.h"


#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file

	
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	Queue<Order*> DEMO_Queue;	//Important: This is just for demo
	/// ==>
	
	
	
	//
	//=====================================Orders Data Structures==================================================
	LinkedList<Order*> normal;
	Queue<Order*> vegan;
	PQueue<Order*> VIP;
	//
	int N,G,V; //Cooks Count 
	int NormalCount,VeganCount,VIPCount;
	int AutoPromotedCount;
	int UrgentOrderCount;
	int InjuredCount;

	int AutoP, RstPrd, VIP_WT;
	float InjProp;

	//=====================================Cooks Data Structures==================================================

	LinkedList<Cook*> NCook_LinkedList;
	LinkedList<Cook*> GCook_LinkedList;
	LinkedList<Cook*> VCook_LinkedList;




public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	


	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	void AddtoNormalQueue( Order*Ord);	//adds an order to the normal queue
	void AddtoVeganQueue(  Order *Ord);	//adds an order to the vegan queue
	void AddtoVIPQueue( Order*Ord);	//adds an order to the VIP queue
	void Cancel(int ID); // Removes the order with id=ID  only if it is normal order 
	void Promote(int id, bool addMoney = false,int amount =0);

	void File_loading();


	void Count_Orders(int& size_NORMAL, int& size_VIP, int& size_VEGAN,ORD_STATUS status=WAIT);
	void Count_Cooks(int& NCOOK_NUM, int& VCOOK_NUM, int& GCOOK_NUM);

	int GetFirstOrderTime(); //As the first order can come in a time step not equal one and the condition 
	 //                        which breaks loop will be checked

/// ================================================================================================== 

	void AssignOrder(int CurrentTimeStep,LinkedList<Cook*>&LastAssigned);     //we send here the CurrentTimeStep to indicate if there is 
	void FreeOrder(int CurrentTimeStep, int &NOrders, int &VOrders, int &GOrders);//   To move finished Orders from IN-SERVICE to DONE state

	void Cook_Statues(int CurrentTimeStep);//          to handle rest time of cooks
	void Cook_Injury(int CurrentTimeStep);    // make a random cook injured based on the  << InjProp >>

//////////////////////////////////  Modes Functions  ///////////////////////////////////////
	void Modes(PROG_MODE mode);
	void CreateOutputFile();
};

#endif