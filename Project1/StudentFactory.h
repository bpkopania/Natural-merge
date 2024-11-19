#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>

#include "Student.h"
#include "Tape.h"

const int packageSize = 10;

class StudentFactory
{
private:
    
	Tape<Student> dataFlow;
	std::string filename;
	int number;
	std::string names[100] = {
            "Adam", "Adrian", "Agata", "Agnieszka", "Aleksandra", "Aleksander", "Alicja", "Amelia", "Aneta", "Angelika",
            "Anna", "Antoni", "Arkadiusz", "Artur", "Barbara", "Bart³omiej", "Beata", "B³a¿ej", "Bogdan", "Boles³aw",
            "Bo¿ena", "Cezary", "Czes³aw", "Damian", "Daniel", "Dariusz", "Dawid", "Dominik", "Dorota", "Edyta",
            "El¿bieta", "Emilia", "Eryk", "Ewa", "Filip", "Franciszek", "Gabriel", "Gabriela", "Grzegorz", "Halina",
            "Hanna", "Helena", "Henryk", "Hubert", "Irena", "Iwona", "Izabela", "Jacek", "Jakub", "Jan",
            "Janina", "Joanna", "Jolanta", "Julia", "Julian", "Justyna", "Kacper", "Kamil", "Karolina", "Katarzyna",
            "Kazimierz", "Kinga", "Klaudia", "Kornelia", "Krzysztof", "Laura", "Leszek", "Lucyna", "£ukasz", "Maciej",
            "Magdalena", "Maja", "Maksymilian", "Marek", "Maria", "Marian", "Mariusz", "Marta", "Martyna", "Mateusz",
            "Micha³", "Mieczys³aw", "Monika", "Natalia", "Nikodem", "Norbert", "Olga", "Oliwia", "Patryk", "Pawe³",
            "Piotr", "Rafa³", "Renata", "Robert", "Roksana", "Ryszard", "Sebastian", "Stanis³aw", "Stefan", "Sylwia"
    };

	std::string surnames[100] = {
            "Kowalski", "Nowak", "Wiœniewski", "Wójcik", "Kowalczyk", "Kamiñski", "Lewandowski", "Zieliñski", "Szymañski", "WoŸniak",
            "D¹browski", "Koz³owski", "Jankowski", "Mazur", "Wojciechowski", "Kwiatkowski", "Krawczyk", "Kaczmarek", "Piotrowski", "Grabowski",
            "Zaj¹c", "Paw³owski", "Michalski", "Król", "Wieczorek", "Jab³oñski", "Wróbel", "Nowakowski", "Majewski", "Olszewski",
            "Stêpieñ", "Malinowski", "Jaworski", "Adamczyk", "Dudek", "Nowicki", "Pawlak", "Górski", "Witkowski", "Walczak",
            "Sikora", "Baran", "Rutkowski", "Michalak", "Szewczyk", "Ostrowski", "Tomaszewski", "Pietrzak", "Zalewski", "Wróblewski",
            "Marciniak", "Jasiñski", "Zawadzki", "B¹k", "Jakubowski", "Sadowski", "Duda", "Chmielewski", "Borkowski", "Soko³owski",
            "Szczepañski", "Sawicki", "Lis", "Kucharski", "Mazurek", "Kaleta", "Wilk", "Matuszewski", "Kubiak", "Maciejewski",
            "Szczepaniak", "Kowal", "Ko³odziej", "Czarnecki", "Urbañski", "Szymczak", "Szulc", "Kopeæ", "Wasilewski", "Czerwiñski",
            "Krajewski", "G³owacki", "Zakrzewski", "Krupa", "Laskowski", "Zió³kowski", "Makowski", "Kucharczyk", "Brzeziñski", "Kowalik",
            "Kozio³", "Kaczmarczyk", "Kowalewski", "Kozak", "Kania", "Miko³ajczyk", "Cieœlak", "Andrzejewski", "Kosiñski", "Kêdzierski"
    };

public:
	StudentFactory();
	StudentFactory(std::string filename, int number):
		filename(filename),number(number)
	{
		dataFlow = Tape<Student>(filename.c_str());
	}
	~StudentFactory(){	}

    void build();
};

