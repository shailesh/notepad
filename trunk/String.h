// String.h_2 Delete

#ifndef _STRING_H
#define _STRING_H

class String {
public:
	String(int capacity = 256); // Default constructor: default parameter
	String(const String& source); // Copy constructor
	~String(); // Destructor
	
	char& GetAt(int index) const;
	char& operator[](int index); // Returns the character at a given position ? operator substitution for GetAt.
	
	// Assignment / Concatenation
	String& operator=(const String& source);
	char* operator+(int index); 
	char* operator+=(int index); //Concatenates a new string to the end of an existing string.
	operator char*();

	int GetLength() const;
	int GetCapacity() const;

	int Append(char ch);
	int Append(char* pstr);
	int Insert(int index, char ch); // int Insert(int nIndex, TCHAR ch);  
	int Insert(int index, char* pstr); // int Insert(int nIndex, LPCTSTR pstr); 
	int Delete(int index, int count);

	
	bool IsEmpty();

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