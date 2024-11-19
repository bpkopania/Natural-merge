#pragma once
#include <string>

#include "Tape.h"

template <class T> class Database
{
private:
	std::string databaseFile;
	int bufferSize;
	bool dumpAfterStep;
	bool dumpAfterSort;
	bool dumpBeforeSort;

	bool isSorted;

	Tape<T> tape1;
	Tape<T> tape2;
	Tape<T> tape3;
	Tape<T> db;
	Tape<int> runLengthTape;

	void rewriteDBforSort()
	{
		isSorted = true;
		db.openToRead();
		tape3.openToWrite();
		runLengthTape.openToWrite();
		T record = db.readSingle();
		T newRecord;
		int length = 1;
		while(record.isValid())
		{
			newRecord = db.readSingle();
			if(newRecord.isSmaller())
			{
				isSorted = false;
				runLengthTape.writeSingle(length);
				length = 1;
			}
			else
			{
				length++;
			}
			record = newRecord;
		}
		runLengthTape.writeSingle(0);
		runLengthTape.close();

		//rewrite to tape
		runLengthTape.openToRead();
		length = runLengthTape.readSingle();
		while(length>0)
		{

			length = runLengthTape.readSingle();
		}
	}

	void sortStep()
	{

	}

public:
	Database(std::string databaseFile, int bufferSize = 10)
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
		createDatabase();
	}

	void createDatabase(std::string databaseFile, int bufferSize = 10)
	{
		this->databaseFile = databaseFile;
		this->bufferSize = bufferSize;
		tape1 = Tape<T>("tape1.dat");
		tape2 = Tape<T>("tape2.dat");
		tape3 = Tape<T>("tape3.dat");
		runLengthTape = Tape<int>("runLengthTape.dat");
		db = Tape<T>(databaseFile);

		//flags of DB
		dumpAfterSort = true;
		dumpAfterStep = false;
		dumpBeforeSort = false;
	}

	void sort()
	{
		rewriteDBforSort();
		
	}

	~Database()
	{

	}



	bool getDumpAfterStep() {
		return dumpAfterStep;
	}

	void setDumpAfterStep(bool value) {
		dumpAfterStep = value;
	}

	bool getDumpAfterSort() {
		return dumpAfterSort;
	}

	void setDumpAfterSort(bool value) {
		dumpAfterSort = value;
	}

	bool getDumpBeforeSort() {
		return dumpBeforeSort;
	}

	void setDumpBeforeSort(bool value) {
		dumpBeforeSort = value;
	}
};
