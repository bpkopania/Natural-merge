#pragma once
#include <fstream>
#include <string>

template <class T> class Tape
{
private:
	std::fstream file;
	std::string filename;

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
		T output;
		file.read(reinterpret_cast<char*>(&output), sizeof(T));
		return output;
	}

	T* readMultiple(int number)
	{
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
		file.write(reinterpret_cast<const char*>(&input), sizeof(T));
	}

	void writeMultiple(T* input, int number)
	{
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

	Tape() {	}

	Tape(std::string filename):
		filename(filename)
	{	}

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

