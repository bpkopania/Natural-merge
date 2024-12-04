#include <iostream>

#include "Database.h"
#include "Student.h"
#include "StudentFactory.h"
#include "Tape.h"

int main()
{
    setlocale(LC_ALL, "");


    Database<Student> database("db.dat", 5);


    /*database.setDumpAfterStep(false);

    StudentFactory factory("db.dat", 100000000);
    factory.build();

    database.sort();

    return 0;*/

    
    int choice;
    bool dumping;
    std::cout << "1. Generate new database\n2. Use existing database\n3. Enter data manually\n";
    std::cin >> choice;
    /*std::cout << "Do you want to dump data after every stage?\n";
    std::cin >> dumping;*/
    database.setDumpAfterStep(true);
    switch (choice)
    {
    case 1:
    {
        std::cout << "Enter number of records to generate: ";
        int numberOfRecords;
        std::cin >> numberOfRecords;
        StudentFactory factory("db.dat", numberOfRecords);
        factory.build();
        break;
    }
    case 2:
        std::cout << "Using file db.dat";
        break;
    case 3:
    {
        Tape<Student> db("db.dat");
        db.openToWrite();
        while (true)
        {
            Student student;
            int kol1, kol2, kol3;
            char name[30];
            std::string nameString;
            std::cout << "Enter student's name (0 to exit): ";
            std::getline(std::cin, nameString);
            if (nameString == "0")
                break;
            std::cout << "Enter student's grades (3): ";
            std::cin >> kol1 >> kol2 >> kol3;
            strncpy(name, nameString.c_str(), nameString.length() + 1);
            student = Student(name, kol1, kol2, kol3);
            db.writeSingle(student);

            db.close();
            break;
        }
    }
    }

    database.sort();

    

	return 0;
}
