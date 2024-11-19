#pragma once
#include <string>

#include "Tape.h"
#include "Entity.h"

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


		//Tape<int> ffffff("runLengthTape.dat");
		/*runLengthTape.openToWrite();
		runLengthTape.writeSingle(5);
		runLengthTape.close();
		runLengthTape.openToRead();
		int aaaa = runLengthTape.readSingle();*/

		if(!runLengthTape.openToWrite())
		{
			std::cout << "Error opening runLengthTape\n";
			return;
		}
		//runLengthTape.writeSingle(5);
		/*runLengthTape.close();
		runLengthTape.openToRead();
		int temp = runLengthTape.readSingle();*/
		T record = db.readSingle();
		T newRecord;
		int length = 1;
		int recordInDB=0;
		while(record.isValid() && !db.isEndOfTape())
		{
			newRecord = db.readSingle();

			if(newRecord.isSmaller(record))
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
		runLengthTape.writeSingle(--length);
		runLengthTape.writeSingle(-1);
		runLengthTape.close();
		db.close();
		db.openToRead();

		//rewrite to tape
		runLengthTape.openToRead();
		length = runLengthTape.readSingle();
		while(length>=0)
		{
			//length = runLengthTape.readSingle();
			while(length>0)
			{
				int readerBufSize = length<bufferSize?length:bufferSize;
				T* buffer = db.readMultiple(readerBufSize);
				tape3.writeMultiple(buffer, readerBufSize);
				length-=10;
			}
			length = runLengthTape.readSingle();
		}
		runLengthTape.close();
		db.close();
		tape3.close();
		std::cout << "Rewrite done" << std::endl;
	}

	void sortStep()
	{

	}

public:
	Database(std::string databaseFile, int bufferSize = 10)
	{
		static_assert(std::is_base_of<Entity, T>::value, "T must be derived from Entity");
		createDatabase(databaseFile,bufferSize);
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
