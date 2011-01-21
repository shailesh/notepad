// String.cpp_2 Delete

#include <string>
using namespace std;

#include "String.h"

// Default constructor: default parameter
String::String(int capacity) { 
	this->front = new char[capacity];
	
	this->capacity = capacity;
	this->length = 0;

	this->front[0] = '\0';
	/*
	for(int i=0; i < this->capacity; i++) {
		this->front[i] = '\0';
	}*/
}

// Copy constructor
String::String(const String& source) { 
	this->front = new char[source.capacity];

	for (int i = 0; i <= source.length; i++) {
		this->front[i] = source.front[i];
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

//Destructor
String::~String() {
	if (this->front != 0) {
		delete [] this->front;
	}
}

String& String::operator=(const String& source) 
{
	if (this->front != 0) {
		delete [] this->front;
	}

	this->front = new char[source.capacity];

	for (int i = 0; i <= source.length; i++) {
		this->front[i] = source.front[i];
	}
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

String::operator char*() {
	return (char*)(this->front);
	//return static_cast<char*>(this->str);
}

char& String::operator[](int index) {
	return this->front[index];
}

char* String::operator+(int index) {
	return this->front+index;
}

int String::Append(char ch) {
	if(this->length >= this->capacity-1) {
		char* temp = new char[this->capacity+256];

		for(int i = 0; i < this->length; i++) {
			temp[i] = this->front[i];
		}
		if(this->front != 0) {
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

	if(this->length >= this->capacity-1) {
		char* temp = new char[this->capacity+256];

		for(i = 0; i < this->length; i++) {
			temp[i] = this->front[i];
		}
		if(this->front != 0) {
			delete [] this->front;
		}
		this->front = temp;
		this->capacity = this->capacity+256;
	}
	for(i = 0; i < count; i++) {
		this->front[this->length] = pstr[i];
		this->length++;
	}
	this->front[this->length] = '\0';
	return this->length;
}
/*
int String::Store(int index, char object) {
	if(this->length >= this->capacity-1) {
		char* temp = new char[this->capacity+256];

		for(int i = 0; i < this->length; i++) {
			temp[i] = this->front[i];
		}
		if(this->front != 0) {
			delete [] this->front;
		}
		this->front = temp;
	}
	this->front[index] = object;
	this->front[index+1] = '\0';

	this->length++;
	index++;

	return index;
}*/

int String::Insert(int index, char ch) {
	int i;
	if(this->length < this->capacity-1) {
		
		// index 이후의 배열 요소(null포함)를 한칸씩 뒤로 이동한다
		for(i = this->length+1; i > index; i--) {
			this->front[i] = this->front[i-1];
		}

	} else {
		char* temp = new char[this->capacity+256];
		// index 이전의 배열 요소를 복사한다
		for(i = 0; i < index; i++) {
			temp[i] = this->front[i];
		}
		// index 이후의 배열 요소를 한칸씩 뒤로 이동하여 복사한다
		for(i = index+1; i <= this->length; i++) {
			temp[i] = this->front[i-1];
		}

		if(this->front != 0) {
			delete [] this->front;
		}
		this->front = temp;
		this->capacity = this->capacity+256;
	}
	// null
	//this->front[this->length] = '\0';
	this->front[index] = ch;
	this->length++;
	index++;
	/*
if(index == this->length-1) {
		this->front[index+1] = '\0';
	}*/
	return index;
}

int String::Insert(int index, char* pstr) 
{
	int i;
	int count = strlen(pstr);

	if(this->length < this->capacity-1) {
		
		// index 이후의 배열 요소(null포함)를 pstr 길이만큼 뒤로 이동한다
		for(i = this->length+strlen(pstr); i > index; i--) {
			this->front[i] = this->front[i-strlen(pstr)];
		}

	} else {
		char* temp = new char[this->capacity+256];
		// index 이전의 배열 요소를 복사한다
		for(i = 0; i < index; i++) {
			temp[i] = this->front[i];
		}
		// index 이후의 배열 요소(null포함)를 pstr 길이만큼 뒤로 이동한다
		for(i = index+count; i <= this->length; i++) {
			temp[i] = this->front[i-count];
		}

		if(this->front != 0) {
			delete [] this->front;
		}
		this->front = temp;
		this->capacity = this->capacity+256;
	}

	for(i = 0; i < count; i++) {
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


char* String::Mid(int first, int count) {
	char* temp = 0;
	if(count > 0) {
		temp = new char[count];
		for(int i = 0; i < count; i++) {
			temp[i] = this->front[first+i];
		}
	}
	return temp;
}

char* String::Left(int count) {
	char* temp = new char[count];

	for(int i = 0; i < count; i++) 
	{
		temp[i] = this->front[i];
	}

	return temp;
}

char* String::Right(int count) {
	char* temp = 0;
	if(count > 0) {
		temp = new char[count];
		for(int i = 0; i < count; i++) {
			temp[i] = this->front[this->length-count+i];
		}
	}
	return temp;
}

