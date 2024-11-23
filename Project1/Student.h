#pragma once
#pragma warning(disable : 4996)
#include "Entity.h"
#include <cstring>

class Student :
    public Entity
{
public:
	char name[32];
	short kol1, kol2, kol3;

	
public:
    float avg() const;
	void print() override;

    bool isGreater(Entity& other) override;
    bool equal(Entity& other) override;
    bool isSmaller(Entity& other) override;

	Student(char* name, short kol1, short kol2, short kol3);
    Student();

    Student& operator=(const Student& other) {
        if (this == &other) {
            return *this;
        }

        std::strncpy(this->name, other.name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0';
        this->kol1 = other.kol1;
        this->kol2 = other.kol2;
        this->kol3 = other.kol3;

        return *this;
    }

    bool operator<(const Student& other) const {
        return this->avg() < other.avg();
    }

    bool operator>(const Student& other) const {
        return this->avg() > other.avg();
    }

    bool operator==(const Student& other) const {
        return this->avg() == other.avg();
    }

    bool isValid() override;
};

