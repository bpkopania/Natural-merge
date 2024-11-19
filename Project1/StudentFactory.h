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
            "Anna", "Antoni", "Arkadiusz", "Artur", "Barbara", "Bart�omiej", "Beata", "B�a�ej", "Bogdan", "Boles�aw",
            "Bo�ena", "Cezary", "Czes�aw", "Damian", "Daniel", "Dariusz", "Dawid", "Dominik", "Dorota", "Edyta",
            "El�bieta", "Emilia", "Eryk", "Ewa", "Filip", "Franciszek", "Gabriel", "Gabriela", "Grzegorz", "Halina",
            "Hanna", "Helena", "Henryk", "Hubert", "Irena", "Iwona", "Izabela", "Jacek", "Jakub", "Jan",
            "Janina", "Joanna", "Jolanta", "Julia", "Julian", "Justyna", "Kacper", "Kamil", "Karolina", "Katarzyna",
            "Kazimierz", "Kinga", "Klaudia", "Kornelia", "Krzysztof", "Laura", "Leszek", "Lucyna", "�ukasz", "Maciej",
            "Magdalena", "Maja", "Maksymilian", "Marek", "Maria", "Marian", "Mariusz", "Marta", "Martyna", "Mateusz",
            "Micha�", "Mieczys�aw", "Monika", "Natalia", "Nikodem", "Norbert", "Olga", "Oliwia", "Patryk", "Pawe�",
            "Piotr", "Rafa�", "Renata", "Robert", "Roksana", "Ryszard", "Sebastian", "Stanis�aw", "Stefan", "Sylwia"
    };

	std::string surnames[100] = {
            "Kowalski", "Nowak", "Wi�niewski", "W�jcik", "Kowalczyk", "Kami�ski", "Lewandowski", "Zieli�ski", "Szyma�ski", "Wo�niak",
            "D�browski", "Koz�owski", "Jankowski", "Mazur", "Wojciechowski", "Kwiatkowski", "Krawczyk", "Kaczmarek", "Piotrowski", "Grabowski",
            "Zaj�c", "Paw�owski", "Michalski", "Kr�l", "Wieczorek", "Jab�o�ski", "Wr�bel", "Nowakowski", "Majewski", "Olszewski",
            "St�pie�", "Malinowski", "Jaworski", "Adamczyk", "Dudek", "Nowicki", "Pawlak", "G�rski", "Witkowski", "Walczak",
            "Sikora", "Baran", "Rutkowski", "Michalak", "Szewczyk", "Ostrowski", "Tomaszewski", "Pietrzak", "Zalewski", "Wr�blewski",
            "Marciniak", "Jasi�ski", "Zawadzki", "B�k", "Jakubowski", "Sadowski", "Duda", "Chmielewski", "Borkowski", "Soko�owski",
            "Szczepa�ski", "Sawicki", "Lis", "Kucharski", "Mazurek", "Kaleta", "Wilk", "Matuszewski", "Kubiak", "Maciejewski",
            "Szczepaniak", "Kowal", "Ko�odziej", "Czarnecki", "Urba�ski", "Szymczak", "Szulc", "Kope�", "Wasilewski", "Czerwi�ski",
            "Krajewski", "G�owacki", "Zakrzewski", "Krupa", "Laskowski", "Zi�kowski", "Makowski", "Kucharczyk", "Brzezi�ski", "Kowalik",
            "Kozio�", "Kaczmarczyk", "Kowalewski", "Kozak", "Kania", "Miko�ajczyk", "Cie�lak", "Andrzejewski", "Kosi�ski", "K�dzierski"
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

