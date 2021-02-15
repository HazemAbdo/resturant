
#ifndef __PNODE_H_
#define __PNODE_H_

template < typename T>
class PNode
{
private :
	T item; // A data item
	PNode<T>* next; // Pointer to next node
	int priority;
public :
	PNode();
	PNode( const T & r_Item);	//passing by const ref.
	PNode( const T & r_Item, PNode<T>* nextPNodePtr);
	void setItem( const T & r_Item);
	void setNext(PNode<T>* nextPNodePtr);
	T getItem() const ;
	PNode<T>* getNext() const ;
	int getPriority();



}; // end PNode


template < typename T>
PNode<T>::PNode() 
{
	next = nullptr;
	priority=0;
} 

template < typename T>
PNode<T>::PNode( const T& r_Item)
{
	item = r_Item;
	next = nullptr;
	priority=item->getPriority();

} 

template < typename T>
PNode<T>::PNode( const T& r_Item, PNode<T>* nextPNodePtr)
{
	item = r_Item;
	next = nextPNodePtr;
}
template < typename T>
void PNode<T>::setItem( const T& r_Item)
{
	item = r_Item;
	priority=item->getPriority();

} 

template < typename T>
void PNode<T>::setNext(PNode<T>* nextPNodePtr)
{
	next = nextPNodePtr;
} 

template < typename T>
T PNode<T>::getItem() const
{
	return item;
} 

template < typename T>
PNode<T>* PNode<T>::getNext() const
{
	return next;
} 

template < typename T>
int PNode<T>::getPriority()
{
	return priority;
}


#endif