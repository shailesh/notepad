// LinkedList.h Visual Studio 6.0

#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

template <typename T>
class LinkedList {
public:
	class Node {
		friend LinkedList;
	public:
		Node() 
		{
			this->previous = this;
			this->next = this;
		}

		Node(T object) 
		{
			this->previous = this;
			this->object = object;
			this->next = this;
		}

		Node(Node* previous, T object) 
		{
			this->previous = previous;
			this->object = object;
			this->next = this;
		}

		Node(T object, Node* next) 
		{
			this->previous = this;
			this->object = object;
			this->next = next;
		}

		Node(Node* previous, T object, Node* next) 
		{
			this->previous = previous;
			this->object = object;
			this->next = next;
		}

		Node(const Node& source) 
		{
			this->previous = source.previous;
			this->object = source.object;
			this->next = source.next;
		}

		Node& operator=(const Node& source) 
		{
			this->previous = source.previous;
			this->object = source.object;
			this->next = source.next;

			return *this;
		}

		T& GetObject() const 
		{
			return const_cast<T&>(this->object);
		}

	private:
		Node* previous;
		T object;
		Node* next;
	};

public:
	LinkedList();
	LinkedList(const LinkedList& source);
	~LinkedList();

	T& GetAt(Node* index);
	T& GetAt(int index);
	T& operator[](int index);
	LinkedList& operator=(const LinkedList& source);

	int GetLength() const;
	Node* GetCurrent() const;

	Node* AppendFromHead(T object);
	Node* AppendFromTail(T object);
	Node* InsertBefore(Node* index, T object);
	Node* InsertAfter(Node* index, T object);
	Node* DeleteFromHead();
	Node* DeleteFromTail();
	Node* Delete(Node* index);
	Node* LinearSearchUnique(void* key, int (*compare)(void*, void*));
	void LinearSearchDuplicate(void* key, Node** (*indexes), int* count, int (*compare)(void*, void*));
	void BubbleSort();

	Node* First();
	Node* Previous();
	Node* Next();
	Node* Last();	

private:
	Node* head;
	Node* tail;
	int length;
	Node* current;
};

template <typename T>
LinkedList<T>::LinkedList() 
{
	this->head = 0;
	this->tail = 0;
	this->length = 0;
	this->current = 0;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& source) 
{
	this->head = 0;
	this->tail = 0;
	this->length = 0;
	this->current = 0;
	Node* pnode;

	for(int i=0; i < source.length; i++) 
	{
		Node* temp;
		if(this->head != 0) 
		{
			pnode = pnode->next;
			temp = new Node(this->current, pnode->object);
			this->head = temp;
		}
		else 
		{
			pnode = source.head;
			temp = new Node(pnode->object);
			this->head = temp;
		}
		this->current = temp;
		this->tail = temp;
	}
	this->length = source.length;
}

template <typename T>
LinkedList<T>::~LinkedList() 
{
	this->current = this->head;

	while(this->current != this->tail) 
	{
		this->head = this->current->next;
		delete this->current;
		this->current = this->head;
	}
	delete this->current;
}

template <typename T>
T& LinkedList<T>::GetAt(int index) 
{
	this->current = this->head;
	int i = 0;

	while(i < index && i < this->length) 
	{
		this->current = this->current->next;
		i++;
	}
	return this->current->object;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& source) 
{
	this->current = this->head;
	while(this->current != this->tail)
	{
		this->head = this->current->next;
		delete this->current;
		this->current = this->head;
	}
	delete this->current;

	this->head = 0;
	this->tail = 0;
	this->current = 0;

	for(int i = 0; i < source.length; i++) 
	{
		Node* psource;
		Node* copy;

		if(i > 0) 
		{
			psource = psource->next;
			copy = new Node(this->current, psource->object);
			this->current->next = copy;
		}
		else 
		{
			psource = source.head;
			copy = new Node(psource->object);
			this->head = copy;
		}

		this->current = copy;
		this->tail = copy;
	}
	this->length = source.length;

	return *this;
}

template <typename T>
int LinkedList<T>::GetLength() const
{
	return this->length;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::GetCurrent() const 
{
	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::AppendFromHead(T object) 
{
	if(this->head != 0) 
	{
		this->current = new Node(object, this->head);
		this->head->previous = this->current; 
	} 
	else 
	{
		this->current = new Node(object);
		this->tail = this->current;
	}
	this->head = this->current;
	this->length++;

	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::AppendFromTail(T object) 
{
	if(this->tail != 0)
	{
		this->current = new Node(this->tail, object);
		this->tail->next = this->current; 
	} 
	else
	{
		this->current = new Node(object);
		this->head = this->current;
	}
	this->tail = this->current;
	this->length++;

	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::InsertBefore(Node* index, T object) 
{
	if(this->head != index) 
	{
		this->current = new Node(index->previous, object, index);
		index->previous->next = this->current; 
	}
	else 
	{
		this->current = new Node(object, index);
		this->head = this->current;
	}
	index->previous = this->current;
	this->length++;

	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::InsertAfter(Node* index, T object) 
{
	if(this->tail != index)
	{
		this->current = new Node(index, object, index->next);
		index->next->previous = this->current; 
	} 
	else
	{
		this->current = new Node(index, object);
		this->tail = this->current;
	}
	index->next = this->current;
	this->length++;

	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::DeleteFromHead() 
{
	this->current = this->head;

	if(this->length > 1) 
	{
		this->current->next->previous = this->current->next;
		this->head = this->current->next;
	}
	else 
	{
		this->head = 0;
		this->tail = 0;
	}

	if(this->current != 0)
	{
		delete this->current;
		this->current = 0;
	}

	this->length--;
	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::DeleteFromTail() 
{
	this->current = this->tail;

	if(this->length > 1) 
	{
		this->current->previous->next = this->current->previous;
		this->tail = this->current->previous;
	} 
	else
	{
		this->head = 0;
		this->tail = 0;
	}

	if(this->current != 0)
	{
		delete this->current;
		this->current = 0;
	}

	this->length--;

	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::Delete(Node* index)
{
	this->current = index;

	if(this->length > 1) 
	{
		if(this->current == this->head) 
		{
			this->current->next->previous = this->current->next;
			this->head = this->current->next;
		}
		else if(this->current == this->tail) 
		{
			this->current->previous->next = this->current->previous;
			this->tail = this->current->previous;
		}
		else
		{
			this->current->next->previous = this->current->previous;
			this->current->previous->next = this->current->next;
		}
	}
	else 
	{
		this->head = 0;
		this->tail = 0;
	}

	if(this->current != 0)
	{
		delete this->current;
		this->current = 0;
	}
	this->length--;

	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::LinearSearchUnique(void* key, int (*compare)(void*, void*)) 
{
	Node* index = this->head;
	int i = 0;
	while(i < this->length && compare(&(index->object), key) != 0) 
	{
		i++;
		index = index->next;
	}
	if(this->length < i)
	{
		index = 0;
	}
	return index;
}

template <typename T>
void LinkedList<T>::LinearSearchDuplicate(void* key, Node** (*indexes), int* count, int (*compare)(void*, void*))
{
	*indexes = new Node*[this->length];
	*count = 0;
	Node* index = this->head;

	for(int i=0; i < this->length; i++) 
	{
		if(compare(&(index->object), key) == 0 ) 
		{
			(*indexes)[*count] = index;
			(*count)++;
		}
		index = index->next;
	}
}

template <typename T>
T&  LinkedList<T>::operator[](int index) 
{
	this->current = this->head;
	int i = 0;
	while(i<index && i<this->length) 
	{
		this->current = this->current->next;
		i++;
	}
	return this->currrent->object;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::First() 
{
	this->current = this->head;
	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::Previous()
{
	this->current = this->current->previous;
	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::Next()
{
	this->current = this->current->next;
	return this->current;
}

template <typename T>
LinkedList<T>::Node* LinkedList<T>::Last() 
{
	this->current = this->tail;
	return this->current;
}

#endif //_LINKEDLIST_H