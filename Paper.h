// Paper.h

#ifndef _PAPER_H
#define _PAPER_H

#include "Label.h"
#include "LinkedList.h"

class Paper 
{
public:
	Paper();
	Paper(const Paper& source);
	~Paper();
	 
	Paper& operator=(const Paper& source); 
	Label& operator[](int index);
	Label& GetAt(int index);
	int GetLength() const;
	Label* GetCurrent() const;

	Label* Attach(Label label);
	Label Detach();
	Label Erase();

	Label* MoveUp();
	Label* MoveDown();
	Label* MoveHome();
	Label* MoveEnd();		

private:
	LinkedList<Label> linkedList;
	int length;
	Label* current;
};

inline int Paper::GetLength() const 
{
	return this->length;
}

inline Label* Paper::GetCurrent() const 
{
	return const_cast<Label*>(this->current);
}

int CompareLabel(void* one, void* other);

#endif //_PAPER_H