#pragma once
#include <iostream>

class Entity
{
public:
	virtual void print() = 0;

	virtual bool isGreater(Entity& other) = 0;
	virtual bool equal(Entity& other) = 0;
	virtual bool isSmaller(Entity& other) = 0;

	virtual bool isValid();
};

