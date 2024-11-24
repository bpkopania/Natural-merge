#include "StudentFactory.h"

void StudentFactory::build()
{
    if(!dataFlow.openToWrite())
	{
		std::cout << "Error while opening file" << std::endl;
		return;
	}
    int numberOfPackeges = number / packageSize;
    Student package[packageSize];
    Student singleStudent;
    std::string name;
    std::string surname;
    std::string fullname;
    char* fullnameChar;
    short kol1,kol2,kol3;
    srand(time(NULL));
    int random;
    int j = 0;
    for(int i = 0; i < numberOfPackeges; i++)
    {
        for (j = 0; j < packageSize; j++)
        {
            random = rand() % 100;
            name = names[random];
            random = rand() % 100;
            surname = surnames[random];
            fullname = name + " " + surname;
            kol1 = rand() % 6 + 1;
            kol2 = rand() % 6 + 1;
            kol3 = rand() % 6 + 1;
            fullnameChar = new char[30];
            strncpy(fullnameChar,fullname.c_str(),fullname.length()+1);
            singleStudent = Student(fullnameChar,kol1,kol2,kol3);
            package[j] = singleStudent;
        }
        dataFlow.writeMultiple(package, packageSize);
    }
    for (j = 0; j < number%packageSize; j++)
    {
        random = rand() % 100;
        name = names[random];
        random = rand() % 100;
        surname = surnames[random];
        fullname = name + " " + surname;
        kol1 = rand() % 6 + 1;
        kol2 = rand() % 6 + 1;
        kol3 = rand() % 6 + 1;
        fullnameChar = new char[30];
        strncpy(fullnameChar, fullname.c_str(), fullname.length() + 1);
        singleStudent = Student(fullnameChar, kol1, kol2, kol3);
        package[j] = singleStudent;
    }
    if(number%packageSize)
        dataFlow.writeMultiple(package, number%packageSize);
    dataFlow.close();

    std::cout << "Data has been written to file " << filename << std::endl;
}
