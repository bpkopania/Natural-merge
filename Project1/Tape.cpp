//#include "Tape.h"
//
//template <class T>
//T& Tape<T>::readSingle()
//{
//	T output;
//	file.read(reinterpret_cast<char*>(&output), sizeof(T));
//	return output;
//}
//
//template <class T>
//T* Tape<T>::readMultiple(int number)
//{
//	T* output = new T[number];
//	file.read(reinterpret_cast<char*>(output), sizeof(T) * number);
//	return output;
//}
//
//template <class T>
//void Tape<T>::writeSingle(T input)
//{
//	file.write(reinterpret_cast<const char*>(&input), sizeof(T));
//}
//
//template <class T>
//void Tape<T>::readMultiple(T* input, int number)
//{
//	file.read(reinterpret_cast<char*>(input), sizeof(T) * number);
//}
//
//template <class T>
//void Tape<T>::close()
//{
//	if(file)
//		file.close();
//}
//
//template <class T>
//bool Tape<T>::openToRead()
//{
//	file.open(filename, std::ios::in | std::ios::binary);
//	if (!file)
//		return false;
//	return true;
//}
//
//template <class T>
//bool Tape<T>::openToWrite()
//{
//	file.open(filename, std::ios::out | std::ios::binary);
//	if (!file)
//		return false;
//	return true;
//}