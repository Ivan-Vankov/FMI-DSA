#ifndef _STUDENT_HEADER_
#define _STUDENT_HEADER_
#include "MyString.h"
#include "Date.h"

class Student {
public:
	Student();
	Student(const Student&);
	Student(const MyString&);
	Student(const MyString& newName, int newAge, const MyString& newSubject, int newGrade, const Date& newDate);
	~Student();
public:
	MyString getName() const { return name; }
	int getAge() const { return age; }
	MyString getSubject() const { return subject; }
	int getGrade() const { return grade; }
	Date getDate() const { return date; }
private:
	void readUntilCommaOrNewLine(int& index, MyString& dest, const MyString& src);
private:
	MyString name;
	int age;
	MyString subject;
	int grade;
	Date date;
};

static std::ostream& operator<<(std::ostream& os, const Student& stud) {
	os << stud.getName() << ',' << stud.getAge() << ',' <<
		stud.getSubject() << ',' << stud.getGrade() << ',' <<
		stud.getDate();
	return os;
}

#endif