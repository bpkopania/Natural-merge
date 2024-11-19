#include <iostream>
#include "Student.h"
#include "StudentFactory.h"
#include "Tape.h"

int main()
{
    setlocale(LC_ALL, "");

    char name[30] = "Bartek";
    Student s(name, 4, 3, 3);
    Entity* entity = &s;
    entity->print();
    Tape<Student> tape("tape1.dat");
    if (!tape.openToWrite())
        return 1;
    tape.writeSingle(s);
    char name2[30] = "Alicja";
	Student s2(name2, 4, 5, 5);
    tape.writeSingle(s2);
    tape.close();
    tape.openToRead();
    Student readedStudent = tape.readSingle();
    readedStudent.print();
    Student readedStudent2 = tape.readSingle();
    readedStudent2.print();
    tape.close();
    tape.openToRead();
    Student* test = tape.readMultiple(3);
    
    tape.close();
    test[0].print();
    test[1].print();
    Student testowy = test[2];
        //test[2].print();
    std::cout << testowy.isValid();

    tape.close();

    StudentFactory factory("db.dat", 100);
    factory.build();
    Tape<Student> db("db.dat");
    db.openToRead();
    Student* multipleRead = db.readMultiple(10);
    for(int i = 0; i < 10; i++)
	{
        multipleRead[i].print();
	}
    db.close();
	return 0;
}
