// Label.h

#ifndef _LABEL_H
#define _LABEL_H

#include "String.h"

class Label {
public:
	Label();
	Label(int capacity);
	Label(const Label& source);
	~Label();

	Label& operator=(const Label& source);
	char& operator[](int index);
	char& GetAt(int index);
	operator char*();

	int GetLength() const;
	int GetCapacity() const;
	int GetCurrent() const;

	int Write(char ch);
	int Write(char* pstr);
	int Erase(int index, int count);

	int MoveLeft();
	int MoveRight();
	int MoveHome();
	int MoveEnd();
	int Move(int index);
	void Copy(char* (*buffer));
	char* Copy(int first, int count);	

private:
	String str;
	int length;
	int capacity;
	int current;
};

inline int Label::GetLength() const {
	return this->length;
}

inline int Label::GetCapacity() const {
	return this->capacity;
}

inline int Label::GetCurrent() const {
	return this->current;
}

#endif //_LABEL_H