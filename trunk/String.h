// String.h

#ifndef _STRING_H
#define _STRING_H

class String {
public:
	String(int capacity = 256); 
	String(const String& source); 
	~String(); 
	
	String& operator=(const String& source);
	char& GetAt(int index) const;
	char& operator[](int index);	
	char* operator+(int index); 

	int GetLength() const;
	int GetCapacity() const;

	int Append(char ch);
	int Append(char* pstr);
	int Insert(int index, char ch);  
	int Insert(int index, char* pstr); 
	int Delete(int index, int count);

	char* Mid(int first, int count);
	char* Left(int count);
	char* Right(int count);

private:
	char (*front);
	int capacity;
	int length;
};

inline char& String::GetAt(int index) const {
	return this->front[index];
}

inline int String::GetLength() const {
	return this->length;
}

inline int String::GetCapacity() const {
	return this->capacity;
}

#endif //_STRING_H