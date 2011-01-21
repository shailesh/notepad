// String.cpp

#include <string>
using namespace std;

#include "String.h"

String::String(int capacity) 
{ 
	this->front = new char[capacity];
	
	this->capacity = capacity;
	this->length = 0;
	
	this->front[0] = '\0';
}

String::String(const String& source) 
{ 
	this->front = new char[source.capacity];
	
	for (int i = 0; i <= source.length; i++) 
	{
		this->front[i] = source.front[i];
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

String::~String() 
{
	if (this->front != 0) 
	{
		delete [] this->front;
	}
}

String& String::operator=(const String& source) 
{
	if (this->front != 0) 
	{
		delete [] this->front;
	}
	
	this->front = new char[source.capacity];
	
	for (int i = 0; i <= source.length; i++) 
	{
		this->front[i] = source.front[i];
	}
	this->capacity = source.capacity;
	this->length = source.length;
	
	return *this;
}

String::operator char*() 
{
	return (char*)(this->front);
}

char& String::operator[](int index) 
{
	return this->front[index];
}

char* String::operator+(int index) 
{
	return this->front+index;
}

int String::Append(char ch) 
{
	if(this->length >= this->capacity-1) 
	{
		char* temp = new char[this->capacity+256];
		
		for(int i = 0; i < this->length; i++) 
		{
			temp[i] = this->front[i];
		}
		if(this->front != 0) 
		{
			delete [] this->front;
		}
		this->front = temp;
		this->capacity = this->capacity+256;
	}
	this->front[this->length] = ch;
	this->length++;
	this->front[this->length] = '\0';
	
	return this->length;
}

int String::Append(char* pstr) 
{
	int i;
	int count = strlen(pstr);
	
	if(this->length >= this->capacity-1) 
	{
		char* temp = new char[this->capacity+256];
		
		for(i = 0; i < this->length; i++) 
		{
			temp[i] = this->front[i];
		}
		if(this->front != 0) 
		{
			delete [] this->front;
		}
		this->front = temp;
		this->capacity = this->capacity+256;
	}
	for(i = 0; i < count; i++) 
	{
		this->front[this->length] = pstr[i];
		this->length++;
	}
	this->front[this->length] = '\0';
	return this->length;
}

int String::Insert(int index, char ch) 
{
	int i;
	if(this->length < this->capacity-1) 
	{
		for(i = this->length+1; i > index; i--) 
		{
			this->front[i] = this->front[i-1];
		}
	} 
	else 
	{
		char* temp = new char[this->capacity+256];
		
		for(i = 0; i < index; i++) 
		{
			temp[i] = this->front[i];
		}
		
		for(i = index+1; i <= this->length; i++) 
		{
			temp[i] = this->front[i-1];
		}
		
		if(this->front != 0) 
		{
			delete [] this->front;
		}
		this->front = temp;
		this->capacity = this->capacity+256;
	}
	
	this->front[index] = ch;
	this->length++;
	index++;
	
	return index;
}

int String::Insert(int index, char* pstr) 
{
	int i;
	int count = strlen(pstr);
	
	if(this->length < this->capacity-1) 
	{            
		for(i = this->length+strlen(pstr); i > index; i--) 
		{
			this->front[i] = this->front[i-strlen(pstr)];
		}
	} 
	else 
	{
		char* temp = new char[this->capacity+256];
		
		for(i = 0; i < index; i++) 
		{
			temp[i] = this->front[i];
		}
		
		for(i = index+count; i <= this->length; i++) 
		{
			temp[i] = this->front[i-count];
		}
		
		if(this->front != 0) 
		{
			delete [] this->front;
		}

		this->front = temp;
		this->capacity = this->capacity+256;
	}
	
	for(i = 0; i < count; i++) 
	{
		this->front[index] = pstr[i];
		this->length++;
		index++;
	}
	
	return index;
}

int String::Delete(int index, int count) 
{
	int i = index;
	
	while(i+count <= this->length+1)
	{
		this->front[i] = this->front[i+count];
		this->front[i+count] = ' ';
		i++;
	}
	
	while(i < index+count)
	{
		this->front[i] = ' ';
		i++;
	}
	
	this->length -= count;
	
	return index;
}

char* String::Mid(int first, int count) 
{
	char (*buffer) = new char[count+1];

	for(int i = 0; i < count; i++) 
	{
		buffer[i] = this->front[first+i];
	}
	buffer[count] = '\0';

	return buffer;
}

char* String::Left(int count) 
{
	char (*buffer) = new char[count+1];

	for(int i = 0; i < count; i++) 
	{
		buffer[i] = this->front[i];
	}
	buffer[count] = '\0';

	return buffer;
}

char* String::Right(int count) 
{
	char (*buffer) = new char[count+1];

	for(int i = 0; i < count; i++) 
	{
		buffer[i] = this->front[this->length-count+i];
	}
	buffer[count] = '\0';

	return buffer;
}