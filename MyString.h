#pragma once
#include <iostream>

class MyString {
	union { //for this to work we are assuming that both size_t and char* will be 8 bytes
		char _smallString[16];
		size_t _container[2]; //only for easier access to the size_t that start at byte 9
		char* _data = nullptr;
	};
	// bits from 9 to 16 in the union will be used for the length. ( container[1] )
	size_t _capacity = 0;

	void copyFrom(const MyString& other);
	void moveFrom(MyString& other);
	void free();

	explicit MyString(size_t capacity);

	bool isSmall() const;
public:
	MyString();
	MyString(const char* data);
	MyString(const MyString& other);
	MyString(MyString&& other);
	MyString& operator=(const MyString& other);
	MyString& operator=(MyString&& other);
	~MyString();

	MyString& operator+=(const MyString& other);
	char& operator[](size_t index);
	char operator[](size_t index) const;

	size_t length() const;
	const char* c_str() const;
	MyString substr(size_t begin, size_t howMany) const;

	friend MyString operator+(const MyString& lhs, const MyString& rhs);
	friend std::istream& operator>>(std::istream&, MyString& str);
};
MyString operator+(const MyString& lhs, const MyString& rhs);

std::ostream& operator<<(std::ostream& os, const MyString& str);

std::istream& operator>>(std::istream& os, const MyString& str);