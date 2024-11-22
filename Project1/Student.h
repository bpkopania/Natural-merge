#pragma once
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
    Student() = default;

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

