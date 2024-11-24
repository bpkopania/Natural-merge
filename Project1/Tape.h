#pragma once
#include <fstream>
#include <string>

template <class T> class Tape
{
private:
	std::fstream file;
	std::string filename;
	int numberOfAcces;

public:
	Tape& operator=(const Tape& other)
	{
		if (this == &other)
			return *this;
		filename = other.filename;
		return *this;
	}

	T readSingle()
	{
		numberOfAcces++;
		T output;
		file.read(reinterpret_cast<char*>(&output), sizeof(T));
		return output;
	}

	T* readMultiple(int number)
	{
		numberOfAcces++;
		T* output = new T[number];
		file.read(reinterpret_cast<char*>(output), sizeof(T) * number);
		return output;
	}

	int readLength()
	{
		int runLength;
		file.read(reinterpret_cast<char*>(&runLength), sizeof(int));
		return runLength;
	}

	void writeSingle(T input)
	{
		numberOfAcces++;
		file.write(reinterpret_cast<const char*>(&input), sizeof(T));
	}

	void writeMultiple(T* input, int number)
	{
		numberOfAcces++;
		file.write(reinterpret_cast<char*>(input), sizeof(T) * number);
	}

	void writeLength(int length)
	{
		file.write(reinterpret_cast<char*>(&length), sizeof(int));
	}

	bool isEndOfTape()
	{
		return file.eof() && file.fail();
	}

	int readedRecords()
	{
		return file.gcount() / sizeof(T);
	}

	int getNumberOfAcces()
	{
		return numberOfAcces;
	}

	Tape()
	{
		numberOfAcces = 0;
	}

	Tape(std::string filename):
		filename(filename)
	{
		numberOfAcces = 0;
	}

	void close()
	{
		file.clear();
		file.seekg(0, std::ios::beg);
		if (file)
			file.close();
	}

	bool openToRead()
	{
		file.open(filename, std::ios::in | std::ios::binary);
		if (!file)
			return false;
		return true;
	}

	bool openToWrite()
	{
		file.open(filename, std::ios::out | std::ios::binary);
		if (!file)
			return false;
		return true;
	}

	~Tape() { close(); }
};

