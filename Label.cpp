// Label.cpp

#include "Label.h"

Label::Label() 
{
	this->length = 0;
	this->capacity = 256;
	this->current = 0;
}

Label::Label(int capacity):str(capacity)
{
	this->length = 0;
	this->capacity = capacity;
	this->current = 0;
}

Label::Label(const Label& source):str(source.str) 
{
	this->length = source.length;
	this->capacity = source.capacity;
	this->current = source.current;
}

Label::~Label() {}

char& Label::GetAt(int index) 
{
	return this->str.GetAt(index);
}

char& Label::operator[](int index) 
{
	return this->str.GetAt(index);
}

Label::operator char*()
{
	return (char*)(this->str);
}

Label& Label::operator=(const Label& source) 
{
	this->str = source.str;
	this->length = source.length;
	this->capacity = source.capacity;
	this->current = source.current;

	return *this;
}

int Label::Write(char ch) 
{
	if(this->current == this->length) 
	{
		this->current = this->str.Append(ch);
	} 
	else 
	{
		this->current = this->str.Insert(this->current, ch);
	}
	this->length++;

	return this->current;
}

int Label::Write(char* pstr) 
{
	if(this->current == this->length) 
	{
		this->current = this->str.Append(pstr);
	} 
	else 
	{
		this->current = this->str.Insert(this->current, pstr);
	}
	this->length++;

	return this->current;
}

int Label::Erase(int index, int count) 
{
	this->current = this->str.Delete(index, count);
	this->length--;

	return this->current;
}

int Label::MoveLeft()
{
	this->current--;
	return this->current;
}

int Label::MoveRight()
{
	this->current++;
	return this->current;
}

int Label::MoveHome() 
{
	this->current = 0;
	return this->current;
}

int Label::MoveEnd() 
{
	this->current = this->length;
	return this->current;
}

int Label::Move(int index)
{
	if(index <= this->length)
	{
		this->current = index;
	}
	else
	{
		this->current = this->length;
	}
	return this->current;
}

char* Label::Copy()
{
	char (*buffer) = new char[this->length+1];

	for(int i = 0; i <= this->length; i++)
	{
		buffer[i] = this->str[i];
	}
	return buffer;
}

char* Label::Copy(int first, int count) 
{
	char (*temp);

	if(first == 0) 
	{
		temp = this->str.Left(count);
	} 
	else if(first+count == this->length) 
	{
		temp = this->str.Right(count);
	} 
	else 
	{
		temp = this->str.Mid(first, count);
	}

	return temp;
}