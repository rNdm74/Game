#ifndef __worlds__LINKEDLIST_H__
#define __worlds__LINKEDLIST_H__

#include "cocos2d.h"

using namespace cocos2d;



template<typename T>
class LinkedListElement
{	
	template<typename T>
	class LinkedList;

private:
	LinkedListElement* next;
	LinkedListElement* prev;
	const bool isSentinel;

public:
	LinkedListElement() : next(this), prev(this), isSentinel(false) { }

	/*
	* Get the next element in the list, or NULL if this is the last element in
	* the list.
	*/
	T* getNext() 
	{
		return next->asT();
	}
	const T* getNext() const 
	{
		return next->asT();
	}

	/*
	* Get the previous element in the list, or NULL if this is the first element
	* in the list.
	*/
	T* getPrevious() 
	{
		return prev->asT();
	}
	const T* getPrevious() const 
	{
		return prev->asT();
	}

	/*
	* Insert elem after this element in the list.  |this| must be part of a
	* linked list when you call setNext(); otherwise, this method will assert.
	*/
	void setNext(T* elem) 
	{
		assert(isInList());
		setNextUnsafe(elem);
	}

	/*
	* Insert elem before this element in the list.  |this| must be part of a
	* linked list when you call setPrevious(); otherwise, this method will
	* assert.
	*/
	void setPrevious(T* elem) 
	{
		assert(isInList());
		setPreviousUnsafe(elem);
	}

	/*
	* Remove this element from the list which contains it.  If this element is
	* not currently part of a linked list, this method asserts.
	*/
	void remove() 
	{
		assert(isInList());

		prev->next = next;
		next->prev = prev;
		next = this;
		prev = this;
	}

	/*
	* Return true if |this| part is of a linked list, and false otherwise.
	*/
	bool isInList() const 
	{
		assert((next == this) == (prev == this));
		return next != this;
	}

private:
	friend class LinkedList<T>;

	enum NodeKind 
	{
		NODE_KIND_NORMAL,
		NODE_KIND_SENTINEL
	};

	LinkedListElement(NodeKind nodeKind) : next(this), prev(this), isSentinel(nodeKind == NODE_KIND_SENTINEL)
	{
	}

	/*
	* Return |this| cast to T* if we're a normal node, or return NULL if we're
	* a sentinel node.
	*/
	T* asT() 
	{
		if (isSentinel)
			return NULL;

		return static_cast<T*>(this);
	}
	const T* asT() const 
	{
		if (isSentinel)
			return NULL;

		return static_cast<const T*>(this);
	}

	/*
	* Insert elem after this element, but don't check that this element is in
	* the list.  This is called by LinkedList::insertFront().
	*/
	void setNextUnsafe(T* elem) 
	{
		LinkedListElement *listElem = static_cast<LinkedListElement*>(elem);
		assert(!listElem->isInList());

		listElem->next = this->next;
		listElem->prev = this;
		this->next->prev = listElem;
		this->next = listElem;
	}

	/*
	* Insert elem before this element, but don't check that this element is in
	* the list.  This is called by LinkedList::insertBack().
	*/
	void setPreviousUnsafe(T* elem) 
	{
		LinkedListElement<T>* listElem = static_cast<LinkedListElement<T>*>(elem);
		assert(!listElem->isInList());

		listElem->next = this;
		listElem->prev = this->prev;
		this->prev->next = listElem;
		this->prev = listElem;
	}

private:
	LinkedListElement& operator=(const LinkedList<T>& other); 
	LinkedListElement(const LinkedList<T>& other);
};

template<typename T>
class LinkedList
{
private:
	LinkedListElement<T> sentinel;

public:
	LinkedList() : sentinel(LinkedListElement<T>::NODE_KIND_SENTINEL) { }

	/*
	* Add elem to the front of the list.
	*/
	void insertFront(T* elem) 
	{
		/* Bypass setNext()'s this->isInList() assertion. */
		sentinel.setNextUnsafe(elem);
	}

	/*
	* Add elem to the back of the list.
	*/
	void insertBack(T* elem) 
	{
		sentinel.setPreviousUnsafe(elem);
	}

	/*
	* Get the first element of the list, or NULL if the list is empty.
	*/
	T* getFirst() 
	{
		return sentinel.getNext();
	}
	const T* getFirst() const 
	{
		return sentinel.getNext();
	}

	/*
	* Get the last element of the list, or NULL if the list is empty.
	*/
	T* getLast() 
	{
		return sentinel.getPrevious();
	}
	const T* getLast() const 
	{
		return sentinel.getPrevious();
	}

	/*
	* Get and remove the first element of the list.  If the list is empty,
	* return NULL.
	*/
	T* popFirst() 
	{
		T* ret = sentinel.getNext();
		if (ret)
			static_cast<LinkedListElement<T>*>(ret)->remove();
		return ret;
	}

	/*
	* Get and remove the last element of the list.  If the list is empty,
	* return NULL.
	*/
	T* popLast() 
	{
		T* ret = sentinel.getPrevious();
		if (ret)
			static_cast<LinkedListElement<T>*>(ret)->remove();
		return ret;
	}

	/*
	* Return true if the list is empty, or false otherwise.
	*/
	bool isEmpty() const 
	{
		return !sentinel.isInList();
	}

	/*
	* Remove all the elements from the list.
	*
	* This runs in time linear to the list's length, because we have to mark
	* each element as not in the list.
	*/
	void clear() 
	{
		while (popFirst())
			continue;
	}

	void debugAssertIsSane() const 
	{
#ifdef DEBUG
		const LinkedListElement<T>* slow;
		const LinkedListElement<T>* fast1;
		const LinkedListElement<T>* fast2;

		/*
		* Check for cycles in the forward singly-linked list using the
		* tortoise/hare algorithm.
		*/
		for (slow = sentinel.next,
			fast1 = sentinel.next->next,
			fast2 = sentinel.next->next->next;
			slow != sentinel && fast1 != sentinel && fast2 != sentinel;
			slow = slow->next, fast1 = fast2->next, fast2 = fast1->next)
		{
			assert(slow != fast1);
			assert(slow != fast2);
		}

		/* Check for cycles in the backward singly-linked list. */
		for (slow = sentinel.prev, 
			fast1 = sentinel.prev->prev, 
			fast2 = sentinel.prev->prev->prev; 
			slow != sentinel && fast1 != sentinel && fast2 != sentinel;
			slow = slow->prev, fast1 = fast2->prev, fast2 = fast1->prev)
		{
			assert(slow != fast1);
			assert(slow != fast2);
		}

		/*
		* Check that |sentinel| is the only node in the list with
		* isSentinel == true.
		*/
		for (const LinkedListElement<T>* elem = sentinel.next; elem != sentinel; elem = elem->next)
		{
			assert(!elem->isSentinel);
		}

		/* Check that the next/prev pointers match up. */
		const LinkedListElement<T>* prev = sentinel;
		const LinkedListElement<T>* cur = sentinel.next;
		do 
		{
			assert(cur->prev == prev);
			assert(prev->next == cur);

			prev = cur;
			cur = cur->next;
		} while (cur != sentinel);

#endif /* ifdef DEBUG */

	}

private:
	LinkedList& operator=(const LinkedList<T>& other);
	LinkedList(const LinkedList<T>& other);
};
#endif /* defined(__worlds__LINKEDLIST_H__) */
