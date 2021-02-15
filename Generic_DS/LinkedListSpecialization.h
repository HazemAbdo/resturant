#pragma once
#include"LinkedList.h"
#include"..\Rest\Order.h"

template<>
class LinkedList<Order*>
{
private:
	Node<Order*>* Head;	//Pointer to the head of the list
	Node<Order*>* tail;
	int length;
	//You can add tail pointer too (depending on your problem)
public:


	LinkedList()
	{
		Head = nullptr;
		tail = nullptr;
		length=0;
	}

	bool isEmpty()
	{
		return length == 0;
	}
	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	//void PrintList()	const
	//{
	//	cout << "\nprinting list contents:\n\n";
	//	Node<Order*>* p = Head;

	//	while (p)
	//	{
	//		cout << "[ " << p->getItem()->GetID() << " ]";
	//		cout << "--->";
	//		p = p->getNext();
	//	}
	//	cout << "NULL\n";
	//}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: InsertBeg.
	* Creates a new node and adds it to the beginning of a linked list.
	*
	* Parameters:
	*	- data : Orderhe value to be stored in the new node.
	*/
	/*void InsertBeg(const Order& data)
	{
	Node<Order>* R = new Node<Order>(data);
	R->setNext(Head);
	Head = R;
	if(!tail)
	tail=Head;

	}*/
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: DeleteAll.
	* Deletes all nodes of the list.
	*/

	Node<Order*>*GetHead()
	{
		return Head;
	}

	void DeleteAll()
	{
		Node<Order*>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
		length = 0;
	}
	int getlength()
	{
		return length;
	}

	Node<Order*>* GetTail()
	{
		return tail;
	}

	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end if the list

	void InsertEnd(Order*& data)
	{
		Node<Order*>* newnode = new Node<Order*>(data);

		if (!tail)//empty
		{
			Head = newnode;
			tail = newnode;
			length++;
			return;
		}

		tail->setNext(newnode);
		tail = newnode;
		length++;
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	/*bool Find(int Key)
	{
	if (!Head)return false;
	Node<Order>* temp = Head;
	while (temp)
	{
	if (temp->getItem() == Key)
	return true;
	temp = temp->getNext();
	}
	}*/

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	/*int CountOccurance(const Order& value)
	{
	if (!Head)return false;
	int count = 0;
	Node<Order>* temp = Head;
	while (temp)
	{
	if (temp->getItem() == value)
	count++;
	temp = temp->getNext();
	}
	return count;
	}
	*/
	//[4] DeleteFirst
	//Deletes the first node in the list
	void DeleteFirst()
	{
		Node<Order*>* temp = Head;
		Head = Head->getNext();
		delete temp;
		length--;

	}


	//[5] DeleteLast
	//Deletes the last node in the list
	/*void DeleteLast()
	{
	if (!Head)return;
	if (!Head->getNext())
	{
	Head = NULL;
	return;
	}
	Node<Order>* Orderail = Head;
	while (Orderail->getNext()->getNext())
	{
	Orderail = Orderail->getNext();
	}
	Node<Order>* temp = Orderail->getNext();
	delete temp;
	Orderail->setNext(NULL);
	}*/

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted

	bool DeleteNode(int value)
	{
		if (!Head)return false;

		if (Head->getItem()->GetID() == value&&Head->getItem()->getStatus()==WAIT)
		{
			Node<Order*>* temp = Head;
			Head = Head->getNext();
			if (length == 1)
				tail = Head;
			delete temp;
			length--;
			return true;
		}

		Node<Order*>* temp = Head;
		Node<Order*>* prev = NULL;

		while (temp)
		{
			if (temp->getItem()->GetID() == value && temp->getItem()->getStatus() == WAIT)
			{
				prev->setNext(temp->getNext());
				if (!temp->getNext())
					tail = prev;
				delete temp;
				length--;

				return true;
			}
			else
			{
				prev = temp;
				temp = temp->getNext();
			}
		}

		return false;

	}

	Node<Order*>* RemoveNode(int value) //it is like delete node but it doesn't delete the node it returns a pointer to the node
	{
		if (!Head)return nullptr;

		if (Head->getItem()->GetID() == value)
		{
			Node<Order*>* temp = Head;
			Head = Head->getNext();
			length--;
			return temp;
		}

		Node<Order*>* temp = Head;
		Node<Order*>* prev = NULL;

		while (temp)
		{
			if (temp->getItem()->GetID() == value)
			{
				prev->setNext(temp->getNext());
				length--;
				return temp;
			}
			else
			{
				prev = temp;
				temp = temp->getNext();
			}
		}
		return nullptr;

	}
	Order*& DeleteHeadAndreturnPtrToItsOrder()
	{
		Order* temp = Head->getItem();
		Head = Head->getNext();
		if (length == 1)
			tail = NULL;
		length--;
		return temp;
	}

	Node<Order*>* GetNode(int value)
	{
		Node<Order*>* ptr = Head;
		while (ptr)
		{
			if (ptr->getItem()->GetID() == value)
				return ptr;
			ptr = ptr->getNext();
		}
		return nullptr;
	}
};

