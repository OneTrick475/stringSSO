#include "MyString.h"
#pragma warning(disable : 4996)

//define used here and not in the .h to keep it only visible in this file and not anywhere .h is included
#define _length _container[1] // bits from 9 to 16 in the union will be used for the length. Since we have capacity, length isnt necesarry in this implementation
// it would come into use if for example instead of always keeping a string with the exact length, we resized by x2 every time the string is full

MyString::MyString(size_t capacity) {
	_capacity = capacity - 1;

	if (isSmall()) {
		_smallString[15] = '\0';
		return;
	}

	_length = capacity - 1;

	try {
		_data = new char[capacity];
	}catch(const std::bad_alloc&) {
		std::cout << "there was a problem while allocating memory\n";
		throw;
	}
}

bool MyString::isSmall() const {
	return _capacity < 16;
}

MyString operator+(const MyString& lhs, const MyString& rhs) {
	MyString result(lhs._capacity + rhs._capacity + 1);

	if (lhs._capacity + rhs._capacity < 16) {
		result._smallString[0] = '\0';
		strcat(result._smallString, lhs.c_str());
		strcat(result._smallString, rhs.c_str());
		return result;
	}

	result[0] = '\0';
	strcat(result._data, lhs.c_str());
	strcat(result._data, rhs.c_str());

	return result;
}

MyString& MyString::operator+=(const MyString& other) {
	if (isSmall()) {
		if (_capacity + other._capacity < 16) {
			strcpy(&_smallString[_capacity], other.c_str());
			_capacity += other._capacity;
		}
		else {
			char temp[16];
			strcpy(temp, _smallString);

			_capacity += other._capacity;
			try {
				_data = new char[_capacity + 1];
			}
			catch (const std::bad_alloc&) {
				std::cout << "there was a problem while allocating memory\n";
				throw;
			}
			_data[0] = '\0';
			strcat(_data, temp);
			strcat(_data, other.c_str());
		}
		return *this;
	}

	try {
		char* result = new char[(_length += other._length) + 1];
		result[0] = '\0';
		strcat(result, _data);
		strcat(result, other._data);

		delete[] _data;
		_data = result;
	}catch(const std::bad_alloc&) {
		std::cout << "there was a problem while allocating memory\n";
		throw;
	}

	return *this;
}

MyString::MyString() {
	_smallString[0] = '\0';
}

MyString::MyString(const char* data) : MyString(strlen(data) + 1) {
	if (data == nullptr) {
		free();
		throw std::invalid_argument("Nullptr passed instead of a string");
	}

	_capacity = strlen(data);

	if (isSmall()) {
		strcpy(_smallString, data);
		return;
	}

	strcpy(_data, data);
}

MyString::MyString(const MyString& other) {
	copyFrom(other);
}

MyString::MyString(MyString&& other) {
	moveFrom(other);
}


MyString& MyString::operator=(MyString&& other) {
	if (this != &other) {
		free();
		moveFrom(other);
	}
	return *this;
}


MyString& MyString::operator=(const MyString& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

void MyString::free() {
	if (isSmall())
		return;

	delete[] _data;
	_data = nullptr;
}

MyString::~MyString() {
	free();
}

size_t MyString::length() const {
	return _capacity;
}

void MyString::moveFrom(MyString& other) {
	if(other.isSmall()) {
		strcpy(_smallString, other._smallString);
		_capacity = other._capacity;
		return;
	}

	_capacity = other._capacity;
	_length = other._length;
	_data = other._data;
	other._data = nullptr;
}


void MyString::copyFrom(const MyString& other) {
	if (other.isSmall()) {
		strcpy(_smallString, other._smallString);
		_capacity = other._capacity;
		return;
	}

	_capacity = other._capacity;
	_length = other._length;
	try {
		_data = new char[_length + 1];
		strcpy(_data, other._data);
	}catch (const std::bad_alloc&) {
		std::cout << "there was a problem while allocating memory\n";
		throw;
	}
}

//std::string doesnt make any validations for []
char& MyString::operator[](size_t index) {
	if (isSmall())
		return _smallString[index];

	return _data[index];
}

char MyString::operator[](size_t index) const {
	if (isSmall())
		return _smallString[index];

	return _data[index];
}

MyString MyString::substr(size_t begin, size_t howMany) const {
	if (begin + howMany > _capacity)
		throw std::length_error("Error, Substr out of range");

	MyString res(howMany + 1);

	res._capacity = res._length = howMany;

	for (int i = 0; i < howMany; i++)
		res[i] = (*this)[begin + i];

	res[howMany] = '\0';
	return res;
}

const char* MyString::c_str() const {
	if (isSmall()) 
		return _smallString;

	return _data;
}

std::ostream& operator<<(std::ostream& os, const MyString& str) {
	return os << str.c_str();
}

std::istream& operator>>(std::istream& is, MyString& str) {
	char buff[1024];
	is >> buff; // is.getLine(buff, 1024);

	str.free();

	str._capacity = strlen(buff);
	str._length = str._capacity;

	if (str._capacity < 16) {
		strcpy(str._smallString, buff);
		return is;
	}

	str._data = new char[str._length + 1];
	strcpy(str._data, buff);
	return is;
}

