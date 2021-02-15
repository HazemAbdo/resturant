#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Node.h"
#include<iostream>
using namespace std;

template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list
	Node<T>*tail;
	//You can add tail pointer too (depending on your problem)
public:


	LinkedList()
	{
		Head = nullptr;
		tail=nullptr;
	}

	//List is being desturcted ==> delete all items in the list
	~LinkedList()
	{
		DeleteAll();
	}
	Node<T>* GetHead()
	{
		return Head;
	}
	////////////////////////////////////////////////////////////////////////
	/*
	* Function: PrintList.
	* prints the values of all nodes in a linked list.
	*/
	//void PrintList()	const
	//{
	//	cout << "\nprinting list contents:\n\n";
	//	Node<T>* p = Head;

	//	while (p)
	//	{
	//		cout << "[ " << p->getItem() << " ]";
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
	*	- data : The value to be stored in the new node.
	*/
	/*void InsertBeg(const T& data)
	{
	Node<T>* R = new Node<T>(data);
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
	void DeleteAll()
	{
		Node<T> *P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}



	////////////////     Requirements   ///////////////////
	//
	// Implement the following member functions


	//[1]InsertEnd 
	//inserts a new node at end if the list
	void InsertEnd(const T& data)
	{
		Node<T>* newnode = new Node<T>(data);
		if(!tail)//empty
		{
			Head=newnode;
			tail=newnode;
			return;
		}
		tail->setNext(newnode);
		tail=newnode;
	}

	//[2]Find 
	//searches for a given value in the list, returns true if found; false otherwise.
	/*bool Find(int Key)
	{
	if (!Head)return false;
	Node<T>* temp = Head;
	while (temp)
	{
	if (temp->getItem() == Key)
	return true;
	temp = temp->getNext();
	}
	}*/

	//[3]CountOccurance
	//returns how many times a certain value appeared in the list
	/*int CountOccurance(const T& value)
	{
	if (!Head)return false;
	int count = 0;
	Node<T>* temp = Head;
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
		Node<T>* temp = Head;
		Head = Head->getNext();
		delete temp;
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
	Node<T>* Tail = Head;
	while (Tail->getNext()->getNext())
	{
	Tail = Tail->getNext();
	}
	Node<T>* temp = Tail->getNext();
	delete temp;
	Tail->setNext(NULL);
	}*/

	//[6] DeleteNode
	//deletes the first node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	bool DeleteNode(const T& value)
	{
		if (!Head)return false;
		if (Head->getItem() == value)
		{
			Node<T>* temp = Head;
			Head = Head->getNext();
			delete temp;
			return true;
		}
		Node<T>* temp = Head;
		Node<T>* prev = NULL;
		while (temp)
		{
			if (temp->getItem() == value)
			{
				prev->setNext(temp->getNext());
				delete temp;
				return true;
			}
			else
			{
				prev = temp;
				temp = temp->getNext();
			}
		}
	}

	//[7] DeleteNodes
	//deletes ALL node with the given value (if found) and returns true
	//if not found, returns false
	//Note: List is not sorted
	/*bool DeleteNodes(const T& value)
	{

	if(!Head)
	return false;

	if(Head->getItem()==value)
	{
	Node<T>*temp=Head;
	Head=Head->getNext();
	delete temp;
	return true;

	}

	Node<T>*ptr=Head;

	while(ptr->getNext())
	{
	if(ptr->getNext()->getItem()==value)
	{
	Node<T>*temp=ptr->getNext();
	ptr->setNext(ptr->getNext()->getNext());
	delete temp;
	return true;
	}
	else
	ptr=ptr->getNext();
	}


	return false;

	}

	/*
	//[8]Merge
	//Merges the current list to another list L by making the last Node in the current list 
	//point to the first Node in list L
	void Merge(const LinkedList& L)
	{
	if (!Head)
	{
	Head = L.Head;
	return;
	}
	Node<T>* Tail = Head;
	while (Tail->getNext())
	{
	Tail = Tail->getNext();
	}
	Tail->setNext(L.Head);
	}

	//[9] Reverse
	//Reverses the linked list (without allocating any new Nodes)
	void Reverse()
	{
	Node<T>* curr = Head;
	Node<T>* temp = NULL;
	Node<T>* temp1 = NULL;
	while (curr)
	{
	temp = curr;
	curr = curr->getNext();
	temp->setNext(temp1);
	temp1 = temp;
	}
	Head = temp;
	}

	//[10] ----->insert sorted

	bool insert_sorted(T value)
	{
	Node<T>* newnode = new Node<T>();
	newnode->setItem(value);
	if (!Head)
	{
	Head = newnode;
	}
	else if (Head->getNext())
	{
	if (Head->getNext()->getItem() > Head->getItem())
	{

	if (Head->getItem() > value)
	{
	newnode->setNext(Head);
	Head = newnode;
	return true;
	}
	Node<T>* curr = Head;
	Node<T>* prev = NULL;
	while (curr)
	{
	if (curr->getItem() == value)
	{
	cout << "Already exist...\n";
	return false;
	}
	else if (curr->getItem() > value)
	{
	prev->setNext(newnode);
	newnode->setNext(curr);
	return true;
	}
	else if (curr->getItem() < value)
	{
	prev = curr;
	curr = curr->getNext();
	}
	}
	prev->setNext(newnode);
	newnode->setNext(curr);
	return true;
	}
	else
	{
	if (Head->getItem() < value)
	{
	newnode->setNext(Head);
	Head = newnode;
	return true;
	}
	Node<T>* curr = Head;
	Node<T>* prev = NULL;
	while (curr)
	{
	if (curr->getItem() == value)
	{
	cout << "Already exist...\n";
	return false;
	}
	else if (curr->getItem() < value)
	{
	prev->setNext(newnode);
	newnode->setNext(curr);
	return true;
	}
	else if (curr->getItem() > value)
	{
	prev = curr;
	curr = curr->getNext();
	}
	}
	}
	}
	else
	{
	if (Head->getItem() > value)
	{
	newnode->setNext(Head);
	Head = newnode;
	}
	else
	{
	Head->setNext(newnode);
	}
	}
	}
	//[11]----->Remove from pos

	void Remove_from_pos(int pos)
	{
	if (pos < 0)
	{
	cout << "Out of range...!" << endl;
	return;
	}
	else if (pos == 0)
	{
	DeleteFirst();
	}
	else
	{
	Node<T>* curr = Head;
	Node<T>* prev = NULL;
	for (int i = 0;i < pos;i++)
	{
	prev = curr;
	curr = curr->getNext();
	}
	prev->setNext(curr->getNext());
	delete curr;
	}
	}
	//[12]----->Remove Min

	LinkedList<T>* Remove_min()
	{
	if (!Head)return NULL;
	int min = Head->getItem();
	Node<T>* temp = Head;
	while (temp)
	{
	if (temp->getItem() < min)
	{
	min = temp->getItem();
	}
	temp = temp->getNext();
	}
	DeleteNodes(min);
	return this;
	}

	//[13]----->Print Kth element

	void printKth(int k)
	{
	if (!Head)return;

	if (k < 1)
	{
	cout << "Beyond List Length\n";
	}

	else
	{
	Node<T>* temp = Head;
	for (int i = 1;i < k;i++)
	temp = temp->getNext();
	cout << "The " << k << " Element is " << temp->getItem() << endl;

	}
	}

	//[14]----->sign spilt

	void signsplit(LinkedList& pos, LinkedList& neg)
	{
	if (!Head)return;
	Node<T>* temp = Head;
	int i = 0;
	while (temp)
	{
	if (temp->getItem() < 0)
	{
	neg.InsertEnd(temp->getItem());
	}
	else if (temp->getItem() > 0)
	{
	pos.InsertEnd(temp->getItem());
	}
	temp = temp->getNext();

	}
	}

	//[15]----->Merge sorted
	void merge_sorted(const LinkedList<T> l, LinkedList<T>& merged)
	{
	if (!l.Head)
	{
	merged.Head = Head;
	return;
	}
	if (!Head)
	{
	merged.Head = l.Head;
	return;
	}
	Node<T>* temp1 = Head;
	Node<T>* temp2 = l.Head;
	while (temp1 && temp2)
	{
	if (temp1->getItem() < temp2->getItem())
	{
	merged.InsertEnd(temp1->getItem());
	temp1 = temp1->getNext();
	}
	else if (temp1->getItem() > temp2->getItem())
	{
	merged.InsertEnd(temp2->getItem());
	temp2 = temp2->getNext();
	}
	else//if the 2 elements are equal
	{
	merged.InsertEnd(temp1->getItem());
	temp1 = temp1->getNext();
	merged.InsertEnd(temp2->getItem());
	temp2 = temp2->getNext();
	}

	}
	while (temp1)//if size1>size2
	{
	merged.InsertEnd(temp1->getItem());
	temp1 = temp1->getNext();
	}
	while (temp2)//if size2>size1
	{
	merged.InsertEnd(temp2->getItem());
	temp2 = temp2->getNext();
	}
	}

	//[16]----->Sum Lists

	void sumLists(LinkedList<T> l, LinkedList<T>& p)
	{
	if (!Head || !l.Head)
	return;
	int length = 0;
	Node<T>* temp = Head;
	Node<T>* temp1 = l.Head;
	while (temp && temp1)
	{
	length++;
	temp = temp->getNext();
	temp1 = temp1->getNext();
	}
	if (temp || temp1)return;
	temp = Head;
	temp1 = l.Head;
	while (temp)
	{
	p.InsertEnd(temp->getItem() + temp1->getItem());
	temp = temp->getNext();
	temp1 = temp1->getNext();
	}
	}

	//[17]------->Reorderd_X
	void Reorder_X(int x)
	{
	int f = CountOccurance(x);
	if (Find(x))
	{
	LinkedList<T>M;
	for (size_t i = 0; i < f; i++)
	{
	M.InsertEnd(x);
	}
	Node<T>* temp = Head;
	while (temp)
	{
	if (temp->getItem() < x)
	{
	M.InsertBeg(temp->getItem());
	}
	else if (temp->getItem() > x)
	{
	M.InsertEnd(temp->getItem());
	}
	temp = temp->getNext();
	}
	this->Head = M.Head;
	}
	else
	return;
	}
	//[18]------>Insert_at_pos
	void insert_at_pos(int pos, int element)
	{
	if (pos < 0)
	{
	cout << "Out of range...!" << endl;
	return;
	}

	else if (pos == 0)
	{
	InsertBeg(element);
	return;
	}
	Node<T>* newnode = new Node<T>();
	newnode->setItem( element);
	Node<T>* curr = new Node<T>();//moving ptr
	curr = Head;
	for (int i = 0;i < pos - 1;i++)
	curr = curr->getNext();
	newnode->setNext(curr->getNext());
	curr->setNext(newnode);
	}

	//[19]----->swap

	bool swap(const T& val1, const T& val2)
	{
	if (!Head)return false;
	int i = 0;
	int pos1 = 0, pos2 = 0;
	Node<T>* temp = Head;
	while (temp)
	{
	if (temp->getItem() == val1)
	{
	pos1 = i;
	}
	else if (temp->getItem() == val2)
	{
	pos2 = i;
	break;
	}
	temp = temp->getNext();
	i++;
	}
	Remove_from_pos(pos1);
	insert_at_pos(pos1, val2);
	Remove_from_pos(pos2);
	insert_at_pos(pos2, val1);
	}
	//[20] Shift Largest

	void Shift_Largest()
	{
	if (!Head)return;
	Node<T>* curr = Head->getNext();
	Node<T>* temp =Head;
	Node<T>* temp1 = NULL;
	while (curr)
	{
	if (curr->getItem() < temp->getItem())
	{
	if (temp == Head)
	{
	Head = curr;
	Node<T>* p = curr->getNext();
	curr->setNext( temp);
	temp->setNext(p);
	temp = Head;
	curr = curr->getNext();
	continue;
	}
	temp1->setNext(curr);
	Node<T>* p = curr->getNext();
	curr->setNext(temp);
	temp->setNext(p);
	}
	temp1 = temp;
	temp = curr;
	curr = curr->getNext();
	}
	}
	*/
};
#endif