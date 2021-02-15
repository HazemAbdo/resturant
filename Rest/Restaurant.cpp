#include <cstdlib>
#include <time.h>
#include <iostream>
#include<fstream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancelEvent.h"
#include "..\Events\PromoteEvent.h"
#include"..\SelectionSort.h"
#include"../SortFinished.h"
#include"../SortServing.h"
#include"../SortWaiting.h"

Restaurant::Restaurant()
{
	pGUI = NULL;
	AutoP = RstPrd = VIP_WT = 0;
	NormalCount=0;
	VeganCount=0;
	VIPCount=0;
	AutoPromotedCount=0;
	UrgentOrderCount=0;
	InjuredCount=0;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	File_loading();//   Reading Data from Text File

	PROG_MODE	mode = pGUI->getGUIMode();

	Modes(mode);


	CreateOutputFile();
	pGUI->PrintMessage("Simlation Finished, Click to close");
	pGUI->waitForClick();
}

//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep
			return;

		pE->Execute(this);

		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory

	}

}


Restaurant::~Restaurant()
{
	if (pGUI)
		delete pGUI;


}

void Restaurant::FillDrawingList()
{
	int total_count = NormalCount + VIPCount + VeganCount;
	Order** AllOrders = new Order * [total_count];

	Node<Order*>* VeganPtr = vegan.GetfrontPtr();
	PNode<Order*>* VIPptr = VIP.GetfrontPtr();
	Node<Order*>* NormalPtr = normal.GetHead();
	int index = 0;

	while (NormalPtr)
	{
		AllOrders[index++] = NormalPtr->getItem();
		NormalPtr = NormalPtr->getNext();
	}

	while (VeganPtr)
	{
		AllOrders[index++] = VeganPtr->getItem();
		VeganPtr = VeganPtr->getNext();
	}

	while (VIPptr)
	{
		AllOrders[index++] = VIPptr->getItem();
		VIPptr = VIPptr->getNext();
	}
	SortW(AllOrders, total_count);
	SortF(AllOrders, total_count);
	SortS(AllOrders, total_count);


	for (size_t i = 0; i < total_count; i++)
	{
		pGUI->AddToDrawingList(AllOrders[i]);
	}

	//Add cooks to DrawingList

	int num_VIP_cooks = 0;
	int num_Vegan_cooks = 0;
	int num_Normal_cooks = 0;



	//==============================================Adding VIP Cooks=====================================
	Node<Cook*>* Temp = VCook_LinkedList.GetHead();
	while (Temp)
	{
		if (Temp->getItem()->GetStatue() == Avail)
			pGUI->AddToDrawingList(Temp->getItem());
		Temp = Temp->getNext();
	}

	//==============================================Adding Normal Cooks=====================================

	Node<Cook*>* Temp2 = NCook_LinkedList.GetHead();
	while (Temp2)
	{
		if (Temp2->getItem()->GetStatue() == Avail)

			pGUI->AddToDrawingList(Temp2->getItem());
		Temp2 = Temp2->getNext();
	}

	//==============================================Adding Vegan Cooks=====================================

	Node<Cook*>* Temp3 = GCook_LinkedList.GetHead();
	while (Temp3)
	{
		if (Temp3->getItem()->GetStatue() == Avail)

			pGUI->AddToDrawingList(Temp3->getItem());
		Temp3 = Temp3->getNext();
	}
	pGUI->UpdateInterface();
	Sleep(1000);
}

void Restaurant::AddtoNormalQueue(Order* Ord)
{
	if (Ord && Ord->GetType() == TYPE_NRM)
	{
		normal.InsertEnd(Ord);
		NormalCount++;
	}
}

void Restaurant::AddtoVeganQueue(Order* Ord)
{
	if (Ord && Ord->GetType() == TYPE_VGAN)
	{
		vegan.enqueue(Ord);
		VeganCount++;
	}
}

void Restaurant::AddtoVIPQueue(Order* Ord)
{
	if (Ord && Ord->GetType() == TYPE_VIP)
	{
		VIP.enqueue(Ord);
		VIPCount++;
	}
}

void Restaurant::Cancel(int ID)
{
	if(normal.DeleteNode(ID))
		NormalCount--;
}

void Restaurant::Promote(int id, bool addMoney, int amount)
{
	PNode<Order*>* newnode;
	Node<Order*>* oldnode = normal.GetNode(id);

	if (!oldnode)
		return;

	Order* OPtr = oldnode->getItem();

	if (OPtr->getStatus() != WAIT) // served or assigned to a cook
		return;

	if (!addMoney)//autopromote
	{
		AutoPromotedCount++;
	}
	else 
	{
		OPtr->addMoney(amount);
	}

	//Status is Waiting 

	oldnode->getItem()->SetType(TYPE_VIP);
	newnode = new PNode<Order*>(oldnode->getItem());

	normal.DeleteNode(id);

	VIP.addNode(newnode);

	VIPCount++;
	NormalCount--;
}

void Restaurant::File_loading()
{
	pGUI->PrintMessage("Please Enter The The Name of Your Text File That Includes The Inputs Data(without .txt) .....Ex( Test1 ) ");
	string S = pGUI->GetString();
	S.append(".txt");



	int SN1 = 0, SG1 = 0, SV1 = 0, SN2 = 0, SG2 = 0, SV2 = 0;
	int BO = 0, BN1 = 0, BG1 = 0, BV1 = 0, BN2 = 0, BG2 = 0, BV2 = 0;
	int increment = 0;              //to generate Cook_IDs
	ifstream inputtxt;
	inputtxt.open(S);
	while (!inputtxt)
	{
		pGUI->PrintMessage("The File Name That U Entered Doesn't Exist...Please Try Again");
		string S = pGUI->GetString();
		S.append(".txt");
		inputtxt.open(S);

	}
	// Read the first Line from File 
	inputtxt>>N>>G>>V;
	// Read the second Line from File 
	inputtxt>>SN1>>SN2>>SG1>>SG2>>SV1>>SV2;
	// Read the third Line from File 
	inputtxt>>BO>>BN1>>BN2>>BG1>>BG2>>BV1>>BV2;
	/////////////////////////////////////////////    Populating the cooks queues/////////////////////////////////.
	for (int i = 0; i < N; i++)
	{
		Cook* ptr = new Cook(++increment, (rand() % (SN2 - SN1 + 1) + SN1), (rand() % (BN2 - BN1 + 1) + BN1), BO);
		ptr->setType(TYPE_NRM);
		NCook_LinkedList.InsertEnd(ptr);
	}
	for (int i = 0; i < G; i++)
	{

		Cook* ptr = new Cook(++increment, (rand() % (SG2 - SG1 + 1) + SG1), (rand() % (BG2 - BG1 + 1) + BG1), BO);
		ptr->setType(TYPE_VGAN);
		GCook_LinkedList.InsertEnd(ptr);

	}
	for (int i = 0; i < V; i++)
	{

		Cook* ptr = new Cook(++increment, (rand() % (SV2 - SV1 + 1) + SV1), (rand() % (BV2 - BV1 + 1) + BV1), BO);
		ptr->setType(TYPE_VIP);
		VCook_LinkedList.InsertEnd(ptr);

	}

	// Read the Forth Line from File 
	inputtxt>>InjProp>>RstPrd;
	// Read the Fifth Line from File 
	inputtxt>>AutoP>>VIP_WT;
	// Read the sixth Line from File 
	int count = 0;                                    //the number of events to be entered
	inputtxt>>count;

	//Reading the Events...
	char event_type;
	for (int i = 0; i < count; i++)
	{
		inputtxt>>event_type;
		if (event_type == 'R')
		{
			char order_type;
			int ts = 0, id = 0, size = 0, mony = 0;
			int j = 4;


			inputtxt>>order_type>>ts>>id>>size>>mony;

			if (order_type == 'N')
			{
				ArrivalEvent* arriv = new ArrivalEvent(ts, id, size, mony, TYPE_NRM);
				EventsQueue.enqueue(arriv);
			}
			else if (order_type == 'G')
			{
				ArrivalEvent* arriv = new ArrivalEvent(ts, id, size, mony, TYPE_VGAN);
				EventsQueue.enqueue(arriv);


			}
			else if (order_type == 'V')
			{
				ArrivalEvent* arriv = new ArrivalEvent(ts, id, size, mony, TYPE_VIP);
				EventsQueue.enqueue(arriv);
			}

		}
		if (event_type == 'X')
		{

			int ts = 0, id = 0;
			inputtxt>>ts>>id;

			CancelEvent* canc = new CancelEvent(ts, id);
			EventsQueue.enqueue(canc);
		}

		if (event_type == 'P')
		{
			//waiting for promotion event to be creadted
			int ts = 0, id = 0, exmony = 0;
			inputtxt>>ts>>id>>exmony;
			PromoteEvent* prom = new PromoteEvent(ts, id, exmony);
			EventsQueue.enqueue(prom);

		}


	}
	inputtxt.close();
}

void Restaurant::Count_Orders(int& size_NORMAL, int& size_VIP, int& size_VEGAN,ORD_STATUS status)
{
	size_NORMAL = 0;
	size_VIP = 0;
	size_VEGAN = 0;

	Node<Order*>* NormalServingPtr = normal.GetHead();//to count number of Normal orders and use it to move orders to serving
	while (NormalServingPtr)
	{
		if (NormalServingPtr->getItem()->getStatus() == status)
			size_NORMAL++;
		NormalServingPtr = NormalServingPtr->getNext();
	}


	PNode<Order*>* VipServingPtr = VIP.GetfrontPtr();//to count number of VIP orders and use it to move orders to serving
	while (VipServingPtr)
	{
		if (VipServingPtr->getItem()->getStatus() == status)
			size_VIP++;
		VipServingPtr = VipServingPtr->getNext();
	}


	Node<Order*>* VeganServingPtr = vegan.GetfrontPtr();//to count number of Vegan orders and use it to move orders to serving
	while (VeganServingPtr)
	{
		if (VeganServingPtr->getItem()->getStatus() == status)
			size_VEGAN++;
		VeganServingPtr = VeganServingPtr->getNext();
	}
}

void Restaurant::Count_Cooks(int& NCOOK_NUM, int& VCOOK_NUM, int& GCOOK_NUM)
{
	NCOOK_NUM = 0;
	VCOOK_NUM = 0;
	GCOOK_NUM = 0;

	Node<Cook*>* CNPtr = NCook_LinkedList.GetHead();//            to count number of Normal Cooks
	while (CNPtr)
	{
		if (CNPtr->getItem()->GetStatue() == Avail)
		{
			NCOOK_NUM++;
		}
		CNPtr = CNPtr->getNext();
	}

	Node<Cook*>* VNPtr = VCook_LinkedList.GetHead();//            to count number of VIP Cooks
	while (VNPtr)
	{
		if (VNPtr->getItem()->GetStatue() == Avail)
		{
			VCOOK_NUM++;
		}
		VNPtr = VNPtr->getNext();
	}

	Node<Cook*>* GNPtr = GCook_LinkedList.GetHead();//            to count number of Vegan Cooks
	while (GNPtr)
	{
		if (GNPtr->getItem()->GetStatue() == Avail)
		{
			GCOOK_NUM++;
		}
		GNPtr = GNPtr->getNext();
	}
}



int Restaurant::GetFirstOrderTime()
{
	if (EventsQueue.GetfrontPtr())
		return EventsQueue.GetfrontPtr()->getItem()->getEventTime();
}

void Restaurant::AssignOrder(int CurrentTimeStep, LinkedList<Cook*>& LastAssigned)
{

	//==================================Implementation===========================================

	LastAssigned.DeleteAll();

	//Searching for first Waiting Order in each type
	//while order is serving or finished move to next order
	Node<Order*>* NormalServingPtr = normal.GetHead();
	Node<Order*>* VeganServingPtr = vegan.GetfrontPtr();
	PNode<Order*>* VipServingPtr = VIP.GetfrontPtr();
	while (NormalServingPtr && NormalServingPtr->getItem()->getStatus() != WAIT)
	{
		NormalServingPtr = NormalServingPtr->getNext();
	}
	while (VeganServingPtr && VeganServingPtr->getItem()->getStatus() != WAIT)
	{
		VeganServingPtr = VeganServingPtr->getNext();
	}
	while (VipServingPtr && VipServingPtr->getItem()->getStatus() != WAIT)
	{
		VipServingPtr = VipServingPtr->getNext();
	}



	//Searching for first available cook in each type
	//while cook is unavail or in rest or injured move to next cook
	Node<Cook*>* NormalAvailCook = NCook_LinkedList.GetHead();
	Node<Cook*>* VeganAvailCook = GCook_LinkedList.GetHead();
	Node<Cook*>* VIPAvailCook = VCook_LinkedList.GetHead();
	while (NormalAvailCook && NormalAvailCook->getItem()->GetStatue() != Avail)
	{
		NormalAvailCook = NormalAvailCook->getNext();
	}
	while (VeganAvailCook && VeganAvailCook->getItem()->GetStatue() != Avail)
	{
		VeganAvailCook = VeganAvailCook->getNext();
	}
	while (VIPAvailCook && VIPAvailCook->getItem()->GetStatue() != Avail)
	{
		VIPAvailCook = VIPAvailCook->getNext();
	}





	//=================There is waiting VIP=====================
	while (VipServingPtr && VipServingPtr->getItem()->getStatus() == WAIT)
	{

		int ArriveTime = VipServingPtr->getItem()->GetArrivalTime();

		if (CurrentTimeStep - ArriveTime > VIP_WT)
		{
			VipServingPtr->getItem()->make_urgent();
		}



		//There is avail VIP
		if (VIPAvailCook)
		{
			VIPAvailCook->getItem()->SetStatue(Unavail);
			LastAssigned.InsertEnd(VIPAvailCook->getItem());
			VipServingPtr->getItem()->setStatus(SRV);
			VipServingPtr->getItem()->SetAssignTime(CurrentTimeStep);
			float OrderSize = VipServingPtr->getItem()->GetSize();
			int CookSpeed = VIPAvailCook->getItem()->GetSpeed();
			VipServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));

			VipServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
			VIPAvailCook->getItem()->SetOder(VipServingPtr->getItem());

			while (VIPAvailCook && VIPAvailCook->getItem()->GetStatue() != Avail)
			{
				VIPAvailCook = VIPAvailCook->getNext();
			}
			VipServingPtr = VipServingPtr->getNext();
		}
		//There is avil Normal
		else if (NormalAvailCook)
		{
			NormalAvailCook->getItem()->SetStatue(Unavail);
			LastAssigned.InsertEnd(NormalAvailCook->getItem());

			VipServingPtr->getItem()->setStatus(SRV);
			VipServingPtr->getItem()->SetAssignTime(CurrentTimeStep);

			float OrderSize = VipServingPtr->getItem()->GetSize();
			int CookSpeed = NormalAvailCook->getItem()->GetSpeed();
			VipServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));

			VipServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
			NormalAvailCook->getItem()->SetOder(VipServingPtr->getItem());
			while (NormalAvailCook && NormalAvailCook->getItem()->GetStatue() != Avail)
			{
				NormalAvailCook = NormalAvailCook->getNext();
			}
			VipServingPtr = VipServingPtr->getNext();
		}
		//There is avil Vegan
		else if (VeganAvailCook)
		{
			VeganAvailCook->getItem()->SetStatue(Unavail);
			LastAssigned.InsertEnd(VeganAvailCook->getItem());

			VipServingPtr->getItem()->setStatus(SRV);
			VipServingPtr->getItem()->SetAssignTime(CurrentTimeStep);

			float OrderSize = VipServingPtr->getItem()->GetSize();
			int CookSpeed = VeganAvailCook->getItem()->GetSpeed();
			VipServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));

			VipServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
			VeganAvailCook->getItem()->SetOder(VipServingPtr->getItem());
			while (VeganAvailCook && VeganAvailCook->getItem()->GetStatue() != Avail)
			{
				VeganAvailCook = VeganAvailCook->getNext();
			}
			VipServingPtr = VipServingPtr->getNext();
		}
		///////////////////////////////////////////////////////////////////////////             Urgent order
		//checking for cooks in break
		else if (VipServingPtr->getItem()->is_urgent())
		{
			NormalAvailCook = NCook_LinkedList.GetHead();
			VeganAvailCook = GCook_LinkedList.GetHead();
			VIPAvailCook = VCook_LinkedList.GetHead();

			while (VIPAvailCook && VIPAvailCook->getItem()->GetStatue() != Rest)
			{
				VIPAvailCook = VIPAvailCook->getNext();
			}
			while (NormalAvailCook && NormalAvailCook->getItem()->GetStatue() != Rest)
			{
				NormalAvailCook = NormalAvailCook->getNext();
			}
			while (VeganAvailCook && VeganAvailCook->getItem()->GetStatue() != Rest)
			{
				VeganAvailCook = VeganAvailCook->getNext();
			}



			//checking for cooks in rest
			if (!VIPAvailCook && !NormalAvailCook && !VeganAvailCook)
			{
				VIPAvailCook = VCook_LinkedList.GetHead();
				NormalAvailCook = NCook_LinkedList.GetHead();
				VeganAvailCook = GCook_LinkedList.GetHead();

				while (VIPAvailCook && VIPAvailCook->getItem()->GetStatue() != Injured_rst)
				{
					VIPAvailCook = VIPAvailCook->getNext();
				}
				while (NormalAvailCook && NormalAvailCook->getItem()->GetStatue() != Injured_rst)
				{
					NormalAvailCook = NormalAvailCook->getNext();
				}
				while (VeganAvailCook && VeganAvailCook->getItem()->GetStatue() != Injured_rst)
				{
					VeganAvailCook = VeganAvailCook->getNext();
				}

			}



			////////////////////////////////////////////////////////////////////////////////////assigning urgent order

			//There is  VIP
			if (VIPAvailCook)
			{
				int CookSpeed;
				if (VIPAvailCook->getItem()->GetStatue() == Rest)
					CookSpeed = (VIPAvailCook->getItem()->GetSpeed()) / 2;         //after this urgent order this cook is gonna work with his original speed
				else
				{
					CookSpeed = (VIPAvailCook->getItem()->GetSpeed()) / 2;
					VIPAvailCook->getItem()->SetSpeed(CookSpeed);                   //after this urgent order this injured cook is gonna work with his half of its original speed as an avail cook till the next break
				}
				VIPAvailCook->getItem()->SetStatue(Unavail);
				LastAssigned.InsertEnd(VIPAvailCook->getItem());

				VipServingPtr->getItem()->setStatus(SRV);
				VipServingPtr->getItem()->SetAssignTime(CurrentTimeStep);

				float OrderSize = VipServingPtr->getItem()->GetSize();
				VipServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));
				VipServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
				VIPAvailCook->getItem()->SetOder(VipServingPtr->getItem());
				while (VIPAvailCook && VIPAvailCook->getItem()->GetStatue() != Avail)
				{
					VIPAvailCook = VIPAvailCook->getNext();
				}
				VipServingPtr = VipServingPtr->getNext();
			}

			//There is  Normal
			else if (NormalAvailCook)
			{
				int CookSpeed;
				if (NormalAvailCook->getItem()->GetStatue() == Rest)
					CookSpeed = (NormalAvailCook->getItem()->GetSpeed()) / 2;         //after this urgent order this cook is gonna work with his original speed
				else
				{
					CookSpeed = (NormalAvailCook->getItem()->GetSpeed()) / 2;
					NormalAvailCook->getItem()->SetSpeed(CookSpeed);                   //after this urgent order this injured cook is gonna work with his half of its original speed as an avail cook till the next break
				}


				NormalAvailCook->getItem()->SetStatue(Unavail);
				LastAssigned.InsertEnd(NormalAvailCook->getItem());

				VipServingPtr->getItem()->setStatus(SRV);
				VipServingPtr->getItem()->SetAssignTime(CurrentTimeStep);

				float OrderSize = VipServingPtr->getItem()->GetSize();
				VipServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));
				VipServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
				NormalAvailCook->getItem()->SetOder(VipServingPtr->getItem());
				while (NormalAvailCook && NormalAvailCook->getItem()->GetStatue() != Avail)
				{
					NormalAvailCook = NormalAvailCook->getNext();
				}
				VipServingPtr = VipServingPtr->getNext();
			}

			//There is  Vegan
			else if (VeganAvailCook)
			{
				int CookSpeed;
				if (VeganAvailCook->getItem()->GetStatue() == Rest)
					CookSpeed = (VeganAvailCook->getItem()->GetSpeed()) / 2;         //after this urgent order this cook is gonna work with his original speed as an avail cook
				else
				{
					CookSpeed = (VeganAvailCook->getItem()->GetSpeed()) / 2;
					VeganAvailCook->getItem()->SetSpeed(CookSpeed);                   //after this urgent order this injured cook is gonna work with his half of its original speed as an avail cook till the next break
				}



				VeganAvailCook->getItem()->SetStatue(Unavail);
				LastAssigned.InsertEnd(VeganAvailCook->getItem());

				VipServingPtr->getItem()->setStatus(SRV);
				VipServingPtr->getItem()->SetAssignTime(CurrentTimeStep);

				float OrderSize = VipServingPtr->getItem()->GetSize();
				VipServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));
				VipServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
				VeganAvailCook->getItem()->SetOder(VipServingPtr->getItem());
				while (VeganAvailCook && VeganAvailCook->getItem()->GetStatue() != Avail)
				{
					VeganAvailCook = VeganAvailCook->getNext();
				}
				VipServingPtr = VipServingPtr->getNext();
			}
			else
				break;


			//to save time (after getting out to the other order assigning)....... as if the order has reached this if statement ->>> this means that there was no available cooks at all
			NormalAvailCook = NULL;
			VeganAvailCook = NULL;
			VIPAvailCook = NULL;

		}




		else
			break;

	}


	//================There is waiting Vegan==================== 
	while (VeganServingPtr && VeganServingPtr->getItem()->getStatus() == WAIT)
	{
		//There is avail Vegan
		if (VeganAvailCook)
		{
			VeganAvailCook->getItem()->SetStatue(Unavail);
			LastAssigned.InsertEnd(VeganAvailCook->getItem());

			VeganServingPtr->getItem()->setStatus(SRV);
			VeganServingPtr->getItem()->SetAssignTime(CurrentTimeStep);

			float OrderSize = VeganServingPtr->getItem()->GetSize();
			int CookSpeed = VeganAvailCook->getItem()->GetSpeed();
			VeganServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));

			VeganServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
			VeganAvailCook->getItem()->SetOder(VeganServingPtr->getItem());
			while (VeganAvailCook && VeganAvailCook->getItem()->GetStatue() != Avail)
			{
				VeganAvailCook = VeganAvailCook->getNext();
			}
			VeganServingPtr = VeganServingPtr->getNext();
		}
		else
			break;

	}









	//====================There is waiting Normal==================
	while (NormalServingPtr && NormalServingPtr->getItem()->getStatus() == WAIT)
	{
		int ArriveTime = NormalServingPtr->getItem()->GetArrivalTime();

		if (CurrentTimeStep - ArriveTime > AutoP)
		{
			Node<Order*>* Next = NormalServingPtr->getNext();
			NormalServingPtr->getItem()->setAutoPromoted(true);
			Promote(NormalServingPtr->getItem()->GetID(), false);
			NormalServingPtr = Next;//move to the next order
		}

		else
		{
			//There is avail VIP
			if (NormalAvailCook)
			{
				NormalAvailCook->getItem()->SetStatue(Unavail);
				LastAssigned.InsertEnd(NormalAvailCook->getItem());

				NormalServingPtr->getItem()->setStatus(SRV);
				NormalServingPtr->getItem()->SetAssignTime(CurrentTimeStep);

				float OrderSize = NormalServingPtr->getItem()->GetSize();
				int CookSpeed = NormalAvailCook->getItem()->GetSpeed();
				NormalServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));

				NormalServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
				NormalAvailCook->getItem()->SetOder(NormalServingPtr->getItem());
				while (NormalAvailCook && NormalAvailCook->getItem()->GetStatue() != Avail)
				{
					NormalAvailCook = NormalAvailCook->getNext();
				}
				NormalServingPtr = NormalServingPtr->getNext();
			}
			//There is avil Normal
			else if (VIPAvailCook)
			{
				VIPAvailCook->getItem()->SetStatue(Unavail);
				LastAssigned.InsertEnd(VIPAvailCook->getItem());

				NormalServingPtr->getItem()->setStatus(SRV);
				NormalServingPtr->getItem()->SetAssignTime(CurrentTimeStep);

				float OrderSize = NormalServingPtr->getItem()->GetSize();
				int CookSpeed = VIPAvailCook->getItem()->GetSpeed();
				NormalServingPtr->getItem()->SetServingTime(ceil((OrderSize / CookSpeed)));

				NormalServingPtr->getItem()->SetFinishedTime(ceil((OrderSize / CookSpeed)) + CurrentTimeStep);
				VIPAvailCook->getItem()->SetOder(NormalServingPtr->getItem());
				while (VIPAvailCook && VIPAvailCook->getItem()->GetStatue() != Avail)
				{
					VIPAvailCook = VIPAvailCook->getNext();
				}
				NormalServingPtr = NormalServingPtr->getNext();
			}
			else
				break;
		}
	}




}



void Restaurant::FreeOrder(int CurrentTimeStep, int& NOrders, int& VOrders, int& GOrders)
{
	/*while (OrderPtr != NULL)
	{
	if(order is in service && its finished time == this current step)
	make this order DONE
	search for the corresponding cook and make it avail

	/////// Freed boolean to ckeck if previous operation is done or not.

	}*/



	Node<Order*>* NormalServingPtr1 = normal.GetHead();
	Node<Order*>* VeganServingPtr1 = vegan.GetfrontPtr();
	PNode<Order*>* VipServingPtr1 = VIP.GetfrontPtr();

	while (NormalServingPtr1)
	{
		bool Freed = false;//            to check if its cook is turned into available state or not

		if (NormalServingPtr1->getItem()->getStatus() == SRV &&      //chek if order is in service and FT==TS
			NormalServingPtr1->getItem()->getFinishedTime() <= CurrentTimeStep)
		{
			NormalServingPtr1->getItem()->setStatus(DONE);
			NOrders++;
			Node<Cook*>* NCook = NCook_LinkedList.GetHead();
			while (NCook)
			{
				if (NCook->getItem()->GetOrder() == NormalServingPtr1->getItem())
				{
					NCook->getItem()->SetStatue(Avail);
					int FO = NCook->getItem()->GetFinishedOrders();
					NCook->getItem()->SetFinishedOrders(++FO);
					Freed = true;
					break;
				}
				else
					NCook = NCook->getNext();
			}

			if (!Freed)//    if it wasn't assigned to normal cook
			{
				Node<Cook*>* VCook = VCook_LinkedList.GetHead();

				while (VCook)
				{
					if (VCook->getItem()->GetOrder() == NormalServingPtr1->getItem())
					{

						VCook->getItem()->SetStatue(Avail);
						int FO = VCook->getItem()->GetFinishedOrders();
						VCook->getItem()->SetFinishedOrders(++FO);
						Freed = true;
						break;

					}
					else
						VCook = VCook->getNext();
				}
			}
		}

		NormalServingPtr1 = NormalServingPtr1->getNext();
	}

	while (VeganServingPtr1)
	{
		if (VeganServingPtr1->getItem()->getStatus() == SRV &&
			VeganServingPtr1->getItem()->getFinishedTime()<=  CurrentTimeStep)
		{
			VeganServingPtr1->getItem()->setStatus(DONE);
			GOrders++;

			Node<Cook*>* GCook = GCook_LinkedList.GetHead();

			while (GCook)
			{
				if (GCook->getItem()->GetOrder() == VeganServingPtr1->getItem())
				{
					GCook->getItem()->SetStatue(Avail);
					int FO = GCook->getItem()->GetFinishedOrders();
					GCook->getItem()->SetFinishedOrders(++FO);
					break;
				}
				else
					GCook = GCook->getNext();
			}
		}

		VeganServingPtr1 = VeganServingPtr1->getNext();
	}

	while (VipServingPtr1)
	{
		bool Freed = false;

		if (VipServingPtr1->getItem()->getStatus() == SRV &&
			VipServingPtr1->getItem()->getFinishedTime() <=  CurrentTimeStep)
		{
			VipServingPtr1->getItem()->setStatus(DONE);
			VOrders++;

			Node<Cook*>* VCook = VCook_LinkedList.GetHead();

			while (VCook)
			{
				if (VCook->getItem()->GetOrder() == VipServingPtr1->getItem())
				{
					VCook->getItem()->SetStatue(Avail);
					int FO = VCook->getItem()->GetFinishedOrders();
					VCook->getItem()->SetFinishedOrders(++FO);
					Freed = true;
					break;
				}
				else
					VCook = VCook->getNext();
			}

			if (!Freed)
			{
				Node<Cook*>* NCook = NCook_LinkedList.GetHead();
				while (NCook)
				{
					if (NCook->getItem()->GetOrder() == VipServingPtr1->getItem())
					{
						NCook->getItem()->SetStatue(Avail);
						int FO = NCook->getItem()->GetFinishedOrders();
						NCook->getItem()->SetFinishedOrders(++FO);
						Freed = true;
						break;
					}
					else
						NCook = NCook->getNext();
				}
			}

			if (!Freed)
			{
				Node<Cook*>* GCook = GCook_LinkedList.GetHead();
				while (GCook)
				{
					if (GCook->getItem()->GetOrder() == VipServingPtr1->getItem())
					{
						GCook->getItem()->SetStatue(Avail);
						int FO = GCook->getItem()->GetFinishedOrders();
						GCook->getItem()->SetFinishedOrders(++FO);
						Freed = true;
						break;
					}
					else
						GCook = GCook->getNext();
				}
			}
		}

		VipServingPtr1 = VipServingPtr1->getNext();
	}


}

void  Restaurant::Cook_Statues(int CurrentTimeStep)//to handle rest time of cooks
{

	Node<Cook*>* NormalAvailCook1 = NCook_LinkedList.GetHead();
	Node<Cook*>* VeganAvailCook1 = GCook_LinkedList.GetHead();
	Node<Cook*>* VIPAvailCook1 = VCook_LinkedList.GetHead();




	while (NormalAvailCook1)
	{
		if (NormalAvailCook1->getItem()->GetStatue() == Rest && NormalAvailCook1->getItem()->GetBreakTime() == (NormalAvailCook1->getItem()->GetRestTime()) )
		{
			NormalAvailCook1->getItem()->SetStatue(Avail);
			NormalAvailCook1->getItem()->SetFinishedOrders(0);
			NormalAvailCook1->getItem()->SetRestTime(0);
		}

		if (NormalAvailCook1->getItem()->GetStatue() == Rest)
			NormalAvailCook1->getItem()->SetRestTime((NormalAvailCook1->getItem()->GetRestTime())+1);




		if (NormalAvailCook1->getItem()->GetStatue() == Injured_rst && NormalAvailCook1->getItem()->get_injury_rst()== RstPrd)
		{
			NormalAvailCook1->getItem()->SetStatue(Avail);
			NormalAvailCook1->getItem()->set_injury_rst(0);
		}
		if (NormalAvailCook1->getItem()->GetStatue() == Injured_rst)
		{
			int restedfrominj =( NormalAvailCook1->getItem()->get_injury_rst())+1;

			NormalAvailCook1->getItem()->set_injury_rst(restedfrominj);

		}

		if (NormalAvailCook1->getItem()->GetStatue() == Injured_working)
		{
			int expected_finish=NormalAvailCook1->getItem()->GetOrder()->getFinishedTime();
			if (expected_finish == CurrentTimeStep)
				NormalAvailCook1->getItem()->SetStatue(Injured_rst);
		}



		if (NormalAvailCook1->getItem()->GetFinishedOrders() == NormalAvailCook1->getItem()->GetBO())
		{
			if(NormalAvailCook1->getItem()->GetStatue() == Injured_rst)
				NormalAvailCook1->getItem()->SetFinishedOrders(0);
			else
			{
				NormalAvailCook1->getItem()->SetStatue(Rest);
				const int original_speed = NormalAvailCook1->getItem()->get_original_speed();
				NormalAvailCook1->getItem()->SetSpeed(original_speed);                                    //to restore the original speed to the cooks who was called from injury_rst (it will have no impact on the other cooks)
			}
		}
		NormalAvailCook1 = NormalAvailCook1->getNext();
	}



	while (VeganAvailCook1)
	{
		if (VeganAvailCook1->getItem()->GetStatue() == Rest && VeganAvailCook1->getItem()->GetBreakTime() == (VeganAvailCook1->getItem()->GetRestTime()))
		{
			VeganAvailCook1->getItem()->SetStatue(Avail);
			VeganAvailCook1->getItem()->SetFinishedOrders(0);
			VeganAvailCook1->getItem()->SetRestTime(0);
		}

		if (VeganAvailCook1->getItem()->GetStatue() == Rest)
			VeganAvailCook1->getItem()->SetRestTime(1+(VeganAvailCook1->getItem()->GetRestTime()));


		if (VeganAvailCook1->getItem()->GetStatue() == Injured_rst && VeganAvailCook1->getItem()->get_injury_rst() == RstPrd)
		{
			VeganAvailCook1->getItem()->SetStatue(Avail);
			VeganAvailCook1->getItem()->set_injury_rst(0);
		}
		if (VeganAvailCook1->getItem()->GetStatue() == Injured_rst)
		{
			int restedfrominj = (VeganAvailCook1->getItem()->get_injury_rst()) + 1;

			VeganAvailCook1->getItem()->set_injury_rst(restedfrominj);

		}

		if (VeganAvailCook1->getItem()->GetStatue() == Injured_working)
		{
			int expected_finish = VeganAvailCook1->getItem()->GetOrder()->getFinishedTime();
			if (expected_finish == CurrentTimeStep)
				VeganAvailCook1->getItem()->SetStatue(Injured_rst);
		}


		if (VeganAvailCook1->getItem()->GetFinishedOrders() == VeganAvailCook1->getItem()->GetBO())
		{
			if (VeganAvailCook1->getItem()->GetStatue() == Injured_rst)
				VeganAvailCook1->getItem()->SetFinishedOrders(0);
			else
			{
				VeganAvailCook1->getItem()->SetStatue(Rest);
				const int original_speed = VeganAvailCook1->getItem()->get_original_speed();
				VeganAvailCook1->getItem()->SetSpeed(original_speed);                                    //to restore the original speed to the cooks who was called from injury_rst (it will have no impact on the other cooks)
			}
		}

		VeganAvailCook1 = VeganAvailCook1->getNext();
	}



	while (VIPAvailCook1)
	{
		if (VIPAvailCook1->getItem()->GetStatue() == Rest && VIPAvailCook1->getItem()->GetBreakTime() == (VIPAvailCook1->getItem()->GetRestTime()))
		{
			VIPAvailCook1->getItem()->SetStatue(Avail);
			VIPAvailCook1->getItem()->SetFinishedOrders(0);
			VIPAvailCook1->getItem()->SetRestTime(0);
		}

		if (VIPAvailCook1->getItem()->GetStatue() == Rest)
			VIPAvailCook1->getItem()->SetRestTime(1+ (VIPAvailCook1->getItem()->GetRestTime()));


		if (VIPAvailCook1->getItem()->GetStatue() == Injured_rst && VIPAvailCook1->getItem()->get_injury_rst() == RstPrd)
		{
			VIPAvailCook1->getItem()->SetStatue(Avail);
			VIPAvailCook1->getItem()->set_injury_rst(0);
		}
		if (VIPAvailCook1->getItem()->GetStatue() == Injured_rst)
		{
			int restedfrominj = (VIPAvailCook1->getItem()->get_injury_rst()) + 1;

			VIPAvailCook1->getItem()->set_injury_rst(restedfrominj);

		}

		if (VIPAvailCook1->getItem()->GetStatue() == Injured_working)
		{
			int expected_finish = VIPAvailCook1->getItem()->GetOrder()->getFinishedTime();
			if (expected_finish == CurrentTimeStep)
				VIPAvailCook1->getItem()->SetStatue(Injured_rst);
		}


		if (VIPAvailCook1->getItem()->GetFinishedOrders() == VIPAvailCook1->getItem()->GetBO())
		{
			if (VIPAvailCook1->getItem()->GetStatue() == Injured_rst)
				VIPAvailCook1->getItem()->SetFinishedOrders(0);
			else
			{
				VIPAvailCook1->getItem()->SetStatue(Rest);
				const int original_speed = VIPAvailCook1->getItem()->get_original_speed();
				VIPAvailCook1->getItem()->SetSpeed(original_speed);                                    //to restore the original speed to the cooks who was called from injury_rst (it will have no impact on the other cooks)
			}
		}

		VIPAvailCook1 = VIPAvailCook1->getNext();
	}

}


void  Restaurant::Cook_Injury(int CurrentTimeStep)
{

	float random_prop = (rand() % 100);
	random_prop= random_prop / 100.0;
	if (random_prop > InjProp)
		return;

	Node<Cook*>* NormalBusyCook1 = NCook_LinkedList.GetHead();
	Node<Cook*>* VeganBusyCook1 = GCook_LinkedList.GetHead();
	Node<Cook*>* VIPBusyCook1 = VCook_LinkedList.GetHead();
	Node<Cook*>* FirstBusyCook = NULL;

	//check in the first unavailable normal cook
	while (NormalBusyCook1 && NormalBusyCook1->getItem()->GetStatue() != Unavail)
	{
		NormalBusyCook1 = NormalBusyCook1->getNext();
	}

	if (NormalBusyCook1)
	{
		FirstBusyCook = NormalBusyCook1;
		int FirstBusyCook_FO = FirstBusyCook->getItem()->GetFinishedOrders();

		while (NormalBusyCook1 )
		{

			if (NormalBusyCook1->getItem()->GetStatue() == Unavail)
			{
				int BusyCook_FO = NormalBusyCook1->getItem()->GetFinishedOrders();
				if (BusyCook_FO > FirstBusyCook_FO)
				{
					FirstBusyCook = NormalBusyCook1;
					FirstBusyCook_FO = BusyCook_FO;
				}
			}


			NormalBusyCook1 = NormalBusyCook1->getNext();
		}

	}



	//check in the first unavailable vegan cook
	while (VeganBusyCook1 && VeganBusyCook1->getItem()->GetStatue() != Unavail)
	{
		VeganBusyCook1 = VeganBusyCook1->getNext();
	}

	if (VeganBusyCook1)
	{
		if(!FirstBusyCook)
			FirstBusyCook = VeganBusyCook1;

		int FirstBusyCook_FO = FirstBusyCook->getItem()->GetFinishedOrders();

		while (VeganBusyCook1)
		{

			if (VeganBusyCook1->getItem()->GetStatue() == Unavail)
			{
				int BusyCook_FO = VeganBusyCook1->getItem()->GetFinishedOrders();
				if (BusyCook_FO > FirstBusyCook_FO)
				{
					FirstBusyCook = VeganBusyCook1;
					FirstBusyCook_FO = BusyCook_FO;
				}
			}


			VeganBusyCook1 = VeganBusyCook1->getNext();
		}

	}


	//check in the first unavailable vegan cook
	while (VIPBusyCook1 && VIPBusyCook1->getItem()->GetStatue() != Unavail)
	{
		VIPBusyCook1 = VIPBusyCook1->getNext();
	}

	if (VIPBusyCook1)
	{
		if (!FirstBusyCook)
			FirstBusyCook = VIPBusyCook1;

		int FirstBusyCook_FO = FirstBusyCook->getItem()->GetFinishedOrders();

		while (VIPBusyCook1)
		{

			if (VIPBusyCook1->getItem()->GetStatue() == Unavail)
			{
				int BusyCook_FO = VIPBusyCook1->getItem()->GetFinishedOrders();
				if (BusyCook_FO > FirstBusyCook_FO)
				{
					FirstBusyCook = VIPBusyCook1;
					FirstBusyCook_FO = BusyCook_FO;
				}
			}


			VIPBusyCook1 = VIPBusyCook1->getNext();
		}

	}






	if (FirstBusyCook)
	{
		FirstBusyCook->getItem()->SetStatue(Injured_working);
		InjuredCount++;

		int serv_time = FirstBusyCook->getItem()->GetOrder()->getServingTime();
		int finish_time = FirstBusyCook->getItem()->GetOrder()->getFinishedTime();
		int cookspeed = FirstBusyCook->getItem()->GetSpeed();
		int assign_time = finish_time - serv_time;

		//num_of_unfinished_dishes = num_of_dishes - num_of_finished_dishes
		int num_of_unfinished_dishes = (FirstBusyCook->getItem()->GetOrder()->GetSize())      -    ((CurrentTimeStep - assign_time) * cookspeed);

		if(cookspeed >1)                   //because if the cookspeep=1 this will make an error as 1/2=0 ,so it will remain 1 (Handled Exception)
			cookspeed = cookspeed / 2;




		int new_finishtime = ceil((float(num_of_unfinished_dishes) / cookspeed)) + CurrentTimeStep;
		FirstBusyCook->getItem()->GetOrder()->SetFinishedTime(new_finishtime);
		FirstBusyCook->getItem()->GetOrder()->SetServingTime(new_finishtime - assign_time);

	}

}


//////////////////////////////////  Modes Functions  ///////////////////////////////////////

void Restaurant :: Modes(PROG_MODE mode)
{
	if(mode<0||mode>=MODE_CNT) // invalid mode 
		return;

	int CurrentTimeStep = 1;
	int NOrder = 0;
	int GOrder = 0;
	int VOrder = 0;


	while (
		(normal.GetTail() && normal.GetTail()->getItem()->getStatus() != DONE)
		||(vegan.Getbackptr() && vegan.Getbackptr()->getItem()->getStatus() != DONE)
		||(VIP.Getbackptr() && VIP.Getbackptr()->getItem()->getStatus() != DONE)
		||!EventsQueue.isEmpty() 
		)
	{

		int VCOOK_Num_orders = 0;
		int NCOOK_Num_orders = 0;
		int GCOOK_Num_orders = 0;
		//---------->condition to check if empty list loaded<--------------------


		ExecuteEvents(CurrentTimeStep);  //execute all events at current time step
		if (EventsQueue.isEmpty()
			&& !normal.GetTail()
			&& !vegan.Getbackptr()
			&& !VIP.Getbackptr())
		{
			pGUI->PrintMessage("Empty List...");
			break;
		}


		int size_NORMAL = 0;
		int size_VIP = 0;
		int size_VEGAN = 0;

		int NCOOK_NUM;
		int VCOOK_NUM;
		int GCOOK_NUM;

		if(mode !=MODE_SLNT)
		{
			////print current timestep,Number Of Orders and Number of Cooks


			Count_Orders(size_NORMAL, size_VIP, size_VEGAN);
			Count_Cooks(NCOOK_NUM, VCOOK_NUM, GCOOK_NUM);

			pGUI->PrintMessage(to_string(CurrentTimeStep),to_string(size_NORMAL),to_string(size_VIP), to_string(size_VEGAN), to_string(NCOOK_NUM), to_string(VCOOK_NUM), to_string(GCOOK_NUM));
		}
		//================================Modify Cook's Statues=================================================================

		Cook_Statues(CurrentTimeStep);

		//================================Moving Finshed Orders from IN-SERV to DONE============================================
		
		FreeOrder(CurrentTimeStep,NOrder,VOrder,GOrder);

		//================================Moving Orders frow Waiting to IN-SERVING============================================
		LinkedList<Cook*>LastAssigned;
		AssignOrder(CurrentTimeStep,LastAssigned);



		Cook_Injury(CurrentTimeStep);

		if(mode !=MODE_SLNT)
		{
			Count_Orders(size_NORMAL, size_VIP, size_VEGAN);//to update the numbers after moving operation
			Count_Cooks(NCOOK_NUM, VCOOK_NUM, GCOOK_NUM);

			pGUI->PrintMessage(to_string(CurrentTimeStep),to_string(size_NORMAL),to_string(size_VIP), to_string(size_VEGAN),
				to_string(NCOOK_NUM), to_string(VCOOK_NUM), to_string(GCOOK_NUM),LastAssigned.GetHead(),
				to_string(NOrder), to_string(VOrder), to_string(GOrder));
			FillDrawingList();
			pGUI->ResetDrawingList();
		}

		CurrentTimeStep++;

		switch (mode)
		{
		case MODE_INTR:
			pGUI->waitForClick();
			break;
		case MODE_STEP:
			Sleep(1000);
			break;

		}

	}


}



void Restaurant :: CreateOutputFile()
{
	ofstream output("Output.txt");
	output<<"FT	ID	AT 	WT	ST\n";

	int AllOrdersCount=NormalCount+VeganCount+VIPCount;

	float AvgWait=0; float AvgServ=0;


	Order**AllOrders=new Order*[AllOrdersCount];

	Node<Order*>*normalPtr=normal.GetHead();
	Node<Order*>*veganPtr=vegan.GetfrontPtr();
	PNode<Order*>*vipPtr=VIP.GetfrontPtr();


	int i=0;
	while(normalPtr)
	{
		AvgWait+=normalPtr->getItem()->getWaitingTime();
		AvgServ+=normalPtr->getItem()->getServingTime();

		AllOrders[i++]=normalPtr->getItem();
		normalPtr=normalPtr->getNext();

	}
	while(veganPtr)
	{
		AvgWait+=veganPtr->getItem()->getWaitingTime();
		AvgServ+=veganPtr->getItem()->getServingTime();

		AllOrders[i++]=veganPtr->getItem();
		veganPtr=veganPtr->getNext();
	}
	while(vipPtr)
	{
		if(vipPtr->getItem()->is_urgent())
			UrgentOrderCount++;

		AvgWait+=vipPtr->getItem()->getWaitingTime();
		AvgServ+=vipPtr->getItem()->getServingTime();

		AllOrders[i++]=vipPtr->getItem();
		vipPtr=vipPtr->getNext();
	}

	AvgWait/=AllOrdersCount;
	AvgServ/=AllOrdersCount;

	selectionSort(AllOrders,AllOrdersCount);//sorting the array by ft 
	for(int i=0; i<AllOrdersCount ; i++)
	{ 
		output<<AllOrders[i]->getFinishedTime()<<"   "<<AllOrders[i]->GetID()<<"   "<<AllOrders[i]->GetArrivalTime()<<"   "<<AllOrders[i]->getWaitingTime()<<"   "<<AllOrders[i]->getServingTime()<<endl;
	}

	output<<"Orders: "<<AllOrdersCount<<"[Norm:"<<NormalCount<<", Veg:"<<VeganCount<<", VIP:"<<VIPCount<<"]"<<endl;
	output<<"cooks: "<<N+G+V<<"	[Norm:"<<N<<", Veg:"<<G<<", VIP:"<<V<<",  injured:"<<InjuredCount<<']'<<endl;
	output<<"Avg Wait ="<< AvgWait<<",  Avg Serv = "<<AvgServ<<endl;
	if ((NormalCount + AutoPromotedCount) > 0)
		output << "Urgent orders: " << UrgentOrderCount << ",   Auto-promoted: " << float(AutoPromotedCount) / (NormalCount + AutoPromotedCount) * 100 << '%' << endl;
	else
		output << "Urgent orders: " << UrgentOrderCount << ",   Auto-promoted: 0%" << endl;
}