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
	Tape<int> runLengthTape2;

	void rewriteDBforSort()
	{
		isSorted = true;
		db.openToRead();
		tape3.openToWrite();

		if(!runLengthTape.openToWrite())
		{
			std::cout << "Error opening runLengthTape\n";
			return;
		}

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
			//tape3.writeLength(length);
			//length = runLengthTape.readSingle();
			while(length>0)
			{
				int readerBufSize = length<bufferSize?length:bufferSize;
				T* buffer = db.readMultiple(readerBufSize);
				tape3.writeMultiple(buffer, readerBufSize);
				length-= bufferSize;
			}
			length = runLengthTape.readSingle();
		}
		//tape3.writeLength(-1);
		runLengthTape.close();
		db.close();
		tape3.close();
		std::cout << "Rewrite done" << std::endl;
	}

	void sortStep()
	{
		/**
		 * File Buffers and cursors
		 ***/
		/*T* readerBuffer1 = NULL;
		T* readerBuffer2 = NULL;
		T* readerBuffer3 = NULL;*/

		T* writterBuffer1 = new T[bufferSize];
		T* writterBuffer2 = new T[bufferSize];
		T* writterBuffer3 = new T[bufferSize];
		/*auto writterBuffer1 = std::make_unique<T[]>(bufferSize);
		auto writterBuffer2 = std::make_unique<T[]>(bufferSize);
		auto writterBuffer3 = std::make_unique<T[]>(bufferSize);*/


		//int readerBuf1Cursor, readerBuf2Cursor, readerBuf3Cursor;
		int writerBuf1Cursor, writerBuf2Cursor, writerBuf3Cursor;


		int tape = 0; // 0 -tape1; 1 tape2
		int numberOfruns=0;
		tape1.openToWrite();
		tape2.openToWrite();
		tape3.openToRead();
		//rewrite to tapes 1 and 2
		bool isTape1End = false;
		bool isTape2End = false;
		int recordsReaded = 1;

		
		/**
		 *split into two tapes
		 **/
		//readerBuf3Cursor = 0;

		writerBuf1Cursor = 1;
		writerBuf2Cursor = 0;
		int maxReaded;
		double previousAVG = 0;
		T currentRecord, previousRecord;
		previousRecord = tape3.readSingle();
		writterBuffer1[0] = previousRecord;


		while(!tape3.isEndOfTape())
		{
			currentRecord = tape3.readSingle();
			if (tape3.isEndOfTape())
				break;
			recordsReaded++;
			if(!currentRecord.isSmaller(previousRecord))
			{
				if (tape == 0)
				{
					writterBuffer1[writerBuf1Cursor++] = currentRecord;
				}
				else
				{
					writterBuffer2[writerBuf2Cursor++] = currentRecord;
				}
			}
			else
			{
				numberOfruns++;
				tape = (tape + 1) % 2;
				if (tape == 0)
				{
					writterBuffer1[writerBuf1Cursor++] = currentRecord;
				}
				else
				{
					writterBuffer2[writerBuf2Cursor++] = currentRecord;
				}
			}
			if(writerBuf1Cursor == bufferSize)
			{
				tape1.writeMultiple(writterBuffer1, bufferSize);
				writerBuf1Cursor = 0;
			}
			if(writerBuf2Cursor == bufferSize)
			{
				tape2.writeMultiple(writterBuffer2, bufferSize);
				writerBuf2Cursor = 0;
			}
			previousRecord = currentRecord;
		}

		//write the rest
		if (writerBuf1Cursor > 0)
		{
			tape1.writeMultiple(writterBuffer1, writerBuf1Cursor);
			writerBuf1Cursor = 0;
		}
		if(writerBuf2Cursor > 0)
		{
			tape2.writeMultiple(writterBuffer2, writerBuf2Cursor);
			writerBuf2Cursor = 0;
		}


		tape1.close();
		tape2.close();
		tape3.close();

		/**
		 * Number of new runs is two times smaller than number of runs in previous step
		 * when we merge two runs togehter we have only one that is sorted
		 **/
		if (numberOfruns < 2)
			isSorted = true;


		tape1.openToRead();
		tape2.openToRead();
		tape3.openToWrite();

		/**
		 * Merge tapes 1 and 2
		 **/
		T previousRecordTape1;
		T previousRecordTape2;
		T newRecordTape1 = tape1.readSingle();
		T newRecordTape2 = tape2.readSingle();
		T nullRecord;

		writerBuf3Cursor = 0;
		while(!tape1.isEndOfTape() && !tape2.isEndOfTape())
		{
			while(newRecordTape1.isGreater(previousRecordTape1) && newRecordTape2.isGreater(previousRecordTape2))
			{
				if(newRecordTape1.isSmaller(newRecordTape2))
				{
					writterBuffer3[writerBuf3Cursor++] = newRecordTape1;
					previousRecordTape1 = newRecordTape1;
					newRecordTape1 = tape1.readSingle();
					if(tape1.isEndOfTape())
						break;
				}
				else
				{
					writterBuffer3[writerBuf3Cursor++] = newRecordTape2;
					previousRecordTape2 = newRecordTape2;
					newRecordTape2 = tape2.readSingle();
					if(tape2.isEndOfTape())
						break;
				}
				if(writerBuf3Cursor == bufferSize)
				{
					tape3.writeMultiple(writterBuffer3, bufferSize);
					writerBuf3Cursor = 0;
				}
			}

			//write remaining parts of run tapes
			while(newRecordTape1.isGreater(previousRecordTape1))
			{
				writterBuffer3[writerBuf3Cursor++] = newRecordTape1;
				previousRecordTape1 = newRecordTape1;
				newRecordTape1 = tape1.readSingle();
				if(writerBuf3Cursor == bufferSize)
				{
					tape3.writeMultiple(writterBuffer3, bufferSize);
					writerBuf3Cursor = 0;
				}
			}
			while(newRecordTape2.isGreater(previousRecordTape2))
			{
				writterBuffer3[writerBuf3Cursor++] = newRecordTape2;
				previousRecordTape2 = newRecordTape2;
				newRecordTape2 = tape2.readSingle();
				if(writerBuf3Cursor == bufferSize)
				{
					tape3.writeMultiple(writterBuffer3, bufferSize);
					writerBuf3Cursor = 0;
				}
			}
			previousRecordTape2 = nullRecord;
			previousRecordTape1 = nullRecord;
		}

		//write remaining the potenial last run of tape1
		while(!tape1.isEndOfTape())
		{
			writterBuffer3[writerBuf3Cursor++] = newRecordTape1;
			previousRecordTape1 = newRecordTape1;
			newRecordTape1 = tape1.readSingle();
			if(writerBuf3Cursor == bufferSize)
			{
				tape3.writeMultiple(writterBuffer3, bufferSize);
				writerBuf3Cursor = 0;
			}
		}
		if(writerBuf3Cursor > 0)
		{
			tape3.writeMultiple(writterBuffer3, writerBuf3Cursor);
			writerBuf3Cursor = 0;
		}


		/**
		 * free file Buffers and cursors
		 ***/

		/*if(readerBuffer1 != NULL)
			delete [] readerBuffer1;
		if(readerBuffer2 != NULL)
			delete[] readerBuffer2;
		if(readerBuffer3)
			delete[] readerBuffer3;*/

		if(writterBuffer1)
			delete[] writterBuffer1;
		if(writterBuffer2)
			delete[] writterBuffer2;
		if(writterBuffer3)
			delete[] writterBuffer3;

		tape1.close();
		tape2.close();
		tape3.close();

		//std::cout << "Step done" << std::endl;
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
		int numerOfStages = 0;
		rewriteDBforSort();
		if(dumpBeforeSort)
		{
			dumpTape();
			getchar();
		}
		while(!isSorted)
		{
			numerOfStages++;
			sortStep();
			if(dumpAfterStep)
			{
				dumpTape();
				getchar();
			}
		}
		if(dumpAfterSort)
		{
			dumpTape();
			getchar();
		}
		std::cout << "Number of stages: " << numerOfStages << std::endl;
		std::cout << "Number of access to files " << tape1.getNumberOfAcces() + tape2.getNumberOfAcces() + tape3.getNumberOfAcces() << std::endl;
	}

	~Database()
	{

	}


	void dumpTape()
	{
		tape3.openToRead();
		int recordNum=0;
		while(!tape3.isEndOfTape())
		{
			T readed = tape3.readSingle();
			if (tape3.isEndOfTape())
				break;
			printf("%9d ", recordNum++);
			readed.print();
		}
		tape3.close();
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
