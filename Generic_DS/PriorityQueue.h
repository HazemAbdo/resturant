#ifndef __PQUEUE_H_
#define __PQUEUE_H_


#include "PriorityNode.h"

template <typename T>
class PQueue
{
private:

	PNode<T>* frontPtr;
	PNode<T>* backPtr;
public:
	PQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	bool addNode(PNode<T>* newnodePtr);
	PNode<T>* Getbackptr();
	PNode<T>* GetfrontPtr();

	T* toArray(int& count);	//returns array of T (array if items)
	~PQueue();
};
/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: PQueue()
The constructor of the PQueue class.

*/

template <typename T>
PQueue<T>::PQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}
/////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
PNode<T>* PQueue<T>::Getbackptr()
{
	return backPtr;

}
template <typename T>
PNode<T>* PQueue<T>::GetfrontPtr()
{
	return frontPtr;

}
/*
Function: isEmpty
Sees whether this queue is empty.

Input: None.
Output: True if the queue is empty; otherwise false.
*/
template <typename T>
bool PQueue<T>::isEmpty() const
{
	if (frontPtr == nullptr)
		return true;
	else
		return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

/*Function:enqueue
Adds newEntry at the back of this queue.

Input: newEntry .
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PQueue<T>::enqueue(const T& newEntry)
{


	PNode<T>* newPNodePtr = new PNode<T>(newEntry);
	if (!newPNodePtr)
		return false;//no memory

	if (!frontPtr)
	{
		frontPtr = newPNodePtr;
		backPtr = frontPtr;
		return true;
	}
	if (newPNodePtr->getPriority() > frontPtr->getPriority())
	{
		newPNodePtr->setNext(frontPtr);
		frontPtr = newPNodePtr;
		return true;
	}



	PNode<T>* ptr = frontPtr;
	while (ptr->getNext())
	{
		if (ptr->getNext()->getPriority() < newPNodePtr->getPriority())
		{
			newPNodePtr->setNext(ptr->getNext());
			ptr->setNext(newPNodePtr);
			return true;
		}
		ptr = ptr->getNext();
	}

	ptr->setNext(newPNodePtr);
	backPtr = newPNodePtr;
	return true;


} // end enqueue


/////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Function: dequeue
Removes the front of this queue. That is, removes the item that was added
earliest.

Input: None.
Output: True if the operation is successful; otherwise false.
*/

template <typename T>
bool PQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	PNode<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// PQueue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: one node in queue
		backPtr = nullptr;

	// Free memory reserved by the dequeued node
	delete nodeToDeletePtr;


	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: peekFront
gets the front of this queue. The operation does not modify the queue.

Input: None.
Output: The front of the queue.
return: flase if PQueue is empty
*/
template <typename T>
bool PQueue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
///////////////////////////////////////////////////////////////////////////////////

template <typename T>
PQueue<T>::~PQueue()
{
}

/////////////////////////////////////////////////////////////////////////////////////////

/*
Function: toArray
returns an array of "T"
Output: count: the length of the returned array (zero if PQueue is empty)
returns: The array of T. (nullptr if PQueue is empty)
*/

template <typename T>
T* PQueue<T>::toArray(int& count)
{
	count = 0;

	if (!frontPtr)
		return nullptr;
	//counting the no. of items in the PQueue
	PNode<T>* p = frontPtr;
	while (p)
	{
		count++;
		p = p->getNext();
	}


	T* Arr = new T[count];
	p = frontPtr;
	for (int i = 0; i < count;i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}

template <typename T>
bool PQueue<T>::addNode(PNode<T>* newPNodePtr)
{
	if (!newPNodePtr)
		return false;

	if (!frontPtr)
	{
		frontPtr = newPNodePtr;
		backPtr = frontPtr;
		return true;
	}

	if (newPNodePtr->getPriority() > frontPtr->getPriority())
	{
		newPNodePtr->setNext(frontPtr);
		frontPtr = newPNodePtr;
		return true;
	}

	PNode<T>* ptr = frontPtr;
	while (ptr->getNext())
	{
		if (ptr->getNext()->getPriority() < newPNodePtr->getPriority())
		{
			newPNodePtr->setNext(ptr->getNext());
			ptr->setNext(newPNodePtr);
			return true;
		}
		ptr = ptr->getNext();
	}

	ptr->setNext(newPNodePtr);
	backPtr = ptr;

	return true;
}




#endif
