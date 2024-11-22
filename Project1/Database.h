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
			tape3.writeLength(length);
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
		tape3.writeLength(-1);
		runLengthTape.close();
		db.close();
		tape3.close();
		std::cout << "Rewrite done" << std::endl;
	}

	void sortStep()
	{
		int tape = 0; // 0 -tape1; 1 tape2
		int numberOfruns=0;
		tape1.openToWrite();
		tape2.openToWrite();
		tape3.openToRead();
		//rewrite to tapes 1 and 2
		bool isTape1End = false;
		bool isTape2End = false;
		int length = tape3.readLength();
		int recordsReaded = 0;
		while(length>=0 && !tape3.isEndOfTape())
		{
			recordsReaded += length;
			numberOfruns++;
			if (tape == 0)
			{
				tape1.writeLength(length);
			}
			else
			{
				tape2.writeLength(length);
			}
			while(length>0)
			{
				int readerBufSize = length<bufferSize?length:bufferSize;
				T* buffer = tape3.readMultiple(readerBufSize);
				if(tape == 0)
				{
					tape1.writeMultiple(buffer, readerBufSize);
				}
				else
				{
					tape2.writeMultiple(buffer, readerBufSize);
				}
				length-= bufferSize;
			}
			length = tape3.readLength();
			tape = (tape+1)%2;
		}

		tape1.writeLength(-1);
		tape2.writeLength(-1);

		tape1.close();
		tape2.close();
		tape3.close();
		if (numberOfruns <= 2)
			isSorted = true;


		tape1.openToRead();
		tape2.openToRead();
		tape3.openToWrite();

		int length1, length2;

		int i = 0;
		int r1 = 0;
		int r2 = 0;


		while(!isTape1End && !isTape2End)
		{
			length1 = tape1.readLength();
			length2 = tape2.readLength();
			if(tape1.isEndOfTape() || tape2.isEndOfTape())
			{
				break;
			}

			r1++;
			r2++;
			if(length1 == -1)
			{
				isTape1End = true;
				//break;
			}
			if(length2 == -1)
			{
				isTape2End = true;
				//break;
			}
			int newLength = (length1 > 0 ? length1 : 0) + (length2 > 0 ? length2 : 0);
			tape3.writeLength(
				newLength
			);
				int readerBufSize1 = length1<bufferSize?length1:bufferSize;
				int readerBufSize2 = length2<bufferSize?length2:bufferSize;
				T* buffer1 = NULL;
					T* buffer2 = NULL;
			if(!isTape1End)
				buffer1 = tape1.readMultiple(readerBufSize1);
				if(!isTape2End)
					buffer2 = tape2.readMultiple(readerBufSize2);
				int i1 = 0;
				int i2 = 0;
				
				while(i1<length1 && i2< length2 && !isTape1End && !isTape2End)
				{
					if(buffer1[i1%bufferSize].isSmaller(buffer2[i2%bufferSize]))
					{
						tape3.writeSingle(buffer1[i1% bufferSize]);
						i1++;
						if(i1% bufferSize == 0 && i1 != length1)
						{
							buffer1 = tape1.readMultiple(readerBufSize1);
						}
					}
					else
					{
						tape3.writeSingle(buffer2[i2% bufferSize]);
						i2++;
						if(i2% bufferSize == 0 && i2 != length2)
						{
							buffer2 = tape2.readMultiple(readerBufSize2);
						}
					}
					i++;

				}
				while(i1<length1)
				{
					tape3.writeSingle(buffer1[i1% bufferSize]);
					i1++;
					i++;
					if(i1% bufferSize == 0 && i1 != length1)
					{
						delete [] buffer1;
						buffer1 = NULL;
						buffer1 = tape1.readMultiple(readerBufSize1);
					}
				}
				while(i2<length2)
				{
					tape3.writeSingle(buffer2[i2% bufferSize]);
					i2++;
					i++;
					if(i2% bufferSize == 0 && i2 != length2)
					{
						delete [] buffer2;
						buffer2 = NULL;
						buffer2 = tape2.readMultiple(readerBufSize2);
					}
				}
			if(buffer1)
				delete [] buffer1;
			if(buffer2)
				delete [] buffer2;
		}

		//rewrite the remaining part
		length1 = tape1.readLength();
		tape3.writeLength(length1);
		while (length1 > 0)
		{
			int readerBufSize = length1 < bufferSize ? length1 : bufferSize;
			T* buffer = tape1.readMultiple(readerBufSize);
			tape3.writeMultiple(buffer, readerBufSize);
			delete buffer;
			length1 -= bufferSize;
		}
		/*while (length2 > 0)
		{
			int readerBufSize = length2 < bufferSize ? length2 : bufferSize;
			T* buffer = tape2.readMultiple(readerBufSize);
			tape3.writeMultiple(buffer, readerBufSize);
			delete buffer;
			length2 -= bufferSize;
		}*/


		tape3.writeLength(-1);

		tape1.close();
		tape2.close();
		tape3.close();

		std::cout << "Step done" << std::endl;
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
		while(!isSorted)
		{
			sortStep();
			if(dumpAfterStep)
			{
				dumpTape();
				getchar();
			}
		}
	}

	~Database()
	{

	}


	void dumpTape()
	{
		tape3.openToRead();
		int recordNum=0;
		int length = tape3.readLength();
		while(length>=0)
		{
			while(length>0)
			{
				int readerBufSize = length<bufferSize?length:bufferSize;
				T* buffer = tape3.readMultiple(readerBufSize);
				for(int i = 0; i < readerBufSize; i++)
				{
					std::cout << recordNum++ << ": ";
					buffer[i].print();
				}
				length-= bufferSize;
			}
			length = tape3.readLength();
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
