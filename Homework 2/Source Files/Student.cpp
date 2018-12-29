#include "Student.h"
#include <exception>

Student::Student() :name("Unnamed"), age(0), subject("Unspecified"), grade(2), date() {
}

Student::Student(const Student& rhs) : name(rhs.name), age(rhs.age),
subject(rhs.subject), grade(rhs.grade), date(rhs.date) {

}

Student::Student(const MyString& buffer) {
	MyString studentBuffer;
	int index = 0;
	readUntilCommaOrNewLine(index, studentBuffer, buffer);
	name = studentBuffer;
	studentBuffer.free();
	readUntilCommaOrNewLine(++index, studentBuffer, buffer);
	age = atoi(studentBuffer);
	studentBuffer.free();
	readUntilCommaOrNewLine(++index, studentBuffer, buffer);
	subject = studentBuffer;
	studentBuffer.free();
	readUntilCommaOrNewLine(++index, studentBuffer, buffer);
	grade = atoi(studentBuffer);
	studentBuffer.free();
	readUntilCommaOrNewLine(++index, studentBuffer, buffer);
	date = studentBuffer;
}

Student::Student(const MyString& newName,
	int newAge,
	const MyString& newSubject,
	int newGrade,
	const Date& newDate) :
	name(newName),
	age(-1),
	subject(newSubject),
	grade(-1),
	date(newDate) {
	if (newAge < 0 || newAge > 200) {
		throw std::invalid_argument("Can't have a negative age.");
	}
	age = newAge;
	if (newGrade < 2 || newGrade > 6) {
		throw std::invalid_argument("Invalid grade.");
	}
	grade = newGrade;
}

Student::~Student() {}

void Student::readUntilCommaOrNewLine(int& index, MyString& dest, const MyString& src) {
	while (src[index] != ',' && src[index] != '\0') {
		dest.add(src[index]);
		++index;
	}
}