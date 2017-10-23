#include <iostream>
#include <string>
#include<cstdio>
#define def_Length 10
#define def_Value 1
#define zero_Value 0
using namespace std;

void printCommands();
void mkNewVec(int newlength, int newdef, int** offArray, int** valArray, int* vecL, int*arrL, int* defVal);
void changeSize(int* vecSize, int* arrSize, int** offArray, int** valArray, int trimSize);
void assValue(int** offArray, int** valArray, int* arrSize, int* vecLenght, int val, int pos, int defValue);
int offsetPosFromOffAr(int** offArray, int arrSize, int pos);
void print(int** offArray, int** valArray, int arrSize, int vecSize, int defVal);
void delete_everything(int** offArray, int** valArray, int*vecSize, int* arrSize, int* defVal);
void setDefault(int* arrSize, int* defVal, int* newDefVal);
void deleteValFromVec(int** offArray, int** valArray, int* arrSize, int posToDel);
int findPosToIns(int** offArray, int arrSize, int pos);
void rewriteArray(int** offArray, int** valArray, int* arrSize, int findedPos, int val, int pos);
void getValFromPos(int pos, int** offArray, int** valArray, int arrLength, int defVal, int vecSize);
int sumEvenPos(int** offArray, int** valArray, int arrLength, int defVal, int vecSize);
void incorrectInput();

int main() {
	int* offArray;
	int* valArray;

	int vecLength = def_Length;
	int arrLength = zero_Value;
	int defVal = def_Value;

	bool run = true;

	printCommands();
	while (run) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << ">>> ";
		string s;
		cin >> s;

		{
			if (s == "mvec")
			{
				int length, def;
				cin >> length >> def;
				if (cin.good())
					mkNewVec(length, def, &offArray, &valArray, &vecLength, &arrLength, &defVal);
				else
					incorrectInput();
			}

			else if (s == "len")
			{
				int newLength;
				cin >> newLength;
				if (cin.good() && newLength > 0)
					changeSize(&vecLength, &arrLength, &offArray, &valArray, newLength);
				else
					incorrectInput();
			}

			else if (s == "def")
			{
				int offset, value;
				bool tr1 = false, tr2 = false;
				cin >> offset;
				if (cin.good())
					tr1 = true;
				cin >> value;
				if (cin.good())
					tr2 = true;
				if (tr1 && tr2)
					assValue(&offArray, &valArray, &arrLength, &vecLength, value, offset, defVal);
				else
					incorrectInput();

			}

			else if (s == "get")
			{
				int offset;
				cin >> offset;
				if (cin.good() && offset < vecLength)
					getValFromPos(offset, &offArray, &valArray, arrLength, defVal, vecLength);
				else
					incorrectInput();
			}

			else if (s == "print")
			{
				print(&offArray, &valArray, arrLength, vecLength, defVal);
			}

			else if (s == "del")
			{
				delete_everything(&offArray, &valArray, &vecLength, &arrLength, &defVal);
			}

			else if (s == "setDef")
			{
				int newDefVal;
				cin >> newDefVal;
				if (cin.good())
					setDefault(&arrLength, &defVal, &newDefVal);
				else
					incorrectInput();

			}

			else if (s == "sumEvenPos")
			{
				cout << "Suma elementow na parzystych pozycjach: " << sumEvenPos(&offArray, &valArray, arrLength, defVal, vecLength) << "\n";
			}

			else if (s == "exit")
			{
				delete_everything(&offArray, &valArray, &vecLength, &arrLength, &defVal);
				run = false;
			}


		}

	}

}


void printCommands() {
	cout << "mvec <len> <def> - create new vector\n";
	cout << "len <len> - change vector length\n";
	cout << "def <off> <val> - set value\n";
	cout << "get <off> - get value\n";
	cout << "print - print\n";
	cout << "del - delete everything\n";
	cout << "setDef <def> - set default\n";
	cout << "sumEvenPos - sum even positions\n";
	cout << "exit - exit\n";
}

/*Creates new vector and deletes the old one if any exists. The offset and value arrays are not allocated.
The initial values set to default*/
void mkNewVec(int newlength, int newdef, int** offArray, int** valArray, int* vecL, int*arrL, int* defVal) {
	if (*arrL > 0)
	{
		delete[] * offArray;
		delete[] * valArray;
	}

	*arrL = zero_Value;
	*vecL = newlength;
	*defVal = newdef;
}

/*Changes size of existing vector. The size must be greater than zero. If any offset values exist and they exceed the
new size they are erased from the array*/
void changeSize(int* vecSize, int* arrSize, int** offArray, int** valArray, int trimSize)
{
	if ((*vecSize) > 0)
	{
		if (trimSize < (*vecSize))
		{
			int i = 0;
			while (i < *arrSize && ((*offArray)[i]) < trimSize)
				i++;
			if (i < (*arrSize))
				*arrSize = i;
		}
		*vecSize = trimSize;
	}
	else
		incorrectInput();
}

/*Function assignes given value to a given position of a vector. If position exceeds the lenght of a vector
an information about an error is prompted. Othewrise the value is assigned.*/
void assValue(int** offArray, int** valArray, int* arrSize, int* vecLenght, int val, int pos, int defValue)
{
	if (pos >= *vecLenght || pos < 0)
		incorrectInput();
	else
	{
		int posInOffset = offsetPosFromOffAr(offArray, *arrSize, pos);
		if (posInOffset >= 0) {
			if (val == defValue)
				deleteValFromVec(offArray, valArray, arrSize, posInOffset);
			else
				(*valArray)[posInOffset] = val;
		}
		else
		{
			if (val != defValue) 
			{
				int findedPos = findPosToIns(offArray, *arrSize, pos);
				rewriteArray(offArray, valArray, arrSize, findedPos, val, pos);
				for (int i = 0; i < *arrSize; i++)
					cout << (*offArray)[i];
			}

		}
	}
}

/*Chceck whether the possition is currently in offset. If it is in the offset array then return its index.
Otherwise return -1*/
int offsetPosFromOffAr(int** offArray, int arrSize, int pos) {
	for (int i = 0; i < arrSize; i++)
		if ((*offArray)[i] == pos)
			return i;
	return -1;
}

/*Prints the vector as a string. The positions which don't exist in the offset array are replaced by
default's vector value.*/
void print(int** offArray, int** valArray, int arrSize, int vecSize, int defVal)
{
	int* outputArr = new int[vecSize];
	for (int i = 0; i < vecSize; i++)
		outputArr[i] = defVal;
	for (int i = 0; i < arrSize; i++)
		outputArr[(*offArray)[i]] = (*valArray)[i];
	cout << "len: " << vecSize << " values: ";
	for (int i = 0; i < vecSize; i++)
		cout << outputArr[i] << ", ";
	cout << endl;
}

/*Deletes everythin from a vector. The initial values are asisigned to zero*/
void delete_everything(int** offArray, int** valArray, int* vecSize, int* arrSize, int* defVal) {
	if (*arrSize > 0) {
		delete[] * offArray;
		delete[] * valArray;
	}
	*vecSize = zero_Value;
	*arrSize = zero_Value;
	*defVal = zero_Value;
}

/*Sets defaults value to a vector. Works only if all values are default values(array size is equal zero*/
void setDefault(int* arrSize, int* defVal, int* newDefVal)
{
	if ((*arrSize) != 0)
		incorrectInput();
	else
		*defVal = *newDefVal;
}



/*Delete value from a given position. Allocates new space for a smaller array,
rewrites the old array and omits the deleting value*/
void deleteValFromVec(int** offArray, int** valArray, int* arrSize, int posToDel)
{
	(*arrSize)--;
	int* newOffArray = new int[*arrSize];
	int* newValArray = new int[*arrSize];
	for (int i = 0; i < posToDel; i++)
	{
		newOffArray[i] = (*offArray)[i];
		newValArray[i] = (*valArray)[i];
	}

	for (int i = *arrSize; i > posToDel; i--)
	{
		newOffArray[i - 1] = (*offArray)[i];
		newValArray[i - 1] = (*valArray)[i];
	}

	delete[] * offArray;
	delete[] * valArray;
	*offArray = newOffArray;
	*valArray = newValArray;
}

/*Find position in offArray to insert new offset - search for the position using ascending order sort*/
int findPosToIns(int** offArray, int arrSize, int pos)
{
	int i = 0;
	while (i < arrSize && (pos >((*offArray)[i])))
		i++;

	return i;
}

int sumEvenPos(int** offArray, int** valArray, int arrLength, int defVal, int vecSize)
{
	int noOfPosInOff = 0;
	int suma = 0;
	for (int i = 0; i < arrLength; i++)
	{
		if ((((*offArray)[i]) % 2) == 0)
		{
			noOfPosInOff++;
			suma += (*valArray[i]);
		}
		
	}
	suma += ((vecSize +1 - noOfPosInOff)/2)*defVal;
	return suma;
}

/*Resizes array by one position - allocates space for new array greater by one from old size,
rewrites old values to new array, deletes old array */
void rewriteArray(int** offArray, int** valArray, int* arrSize, int findedPos, int val, int pos) {

	(*arrSize)++;
	int* newOffArray = new int[*arrSize];
	int* newValArray = new int[*arrSize];

	if (*arrSize > 1) { //if new array has more than 1 position, in other case nold array consists of no elements->no need to rewrite
		for (int i = 0; i < findedPos; i++)
		{
			newOffArray[i] = (*offArray)[i];
			newValArray[i] = (*valArray)[i];
		}

		for (int i = *arrSize - 1; i > findedPos; i--)
		{
			newOffArray[i] = (*offArray)[i - 1];
			newValArray[i] = (*valArray)[i - 1];
		}

		delete[] * offArray;
		delete[] * valArray;
	}

	newOffArray[findedPos] = pos;
	newValArray[findedPos] = val;
	*offArray = newOffArray;
	*valArray = newValArray;
}

/*Gets a value from a given position within a range of vector size.
If the value deosn't exist in offset array a default value is returned*/
void getValFromPos(int pos, int** offArray, int** valArray, int arrLength, int defVal, int vecSize)
{
	if (pos >= vecSize || pos < 0)
		incorrectInput();
	else
	{
		int posInOffArr = offsetPosFromOffAr(offArray, arrLength, pos);
		if (posInOffArr > -1)
			cout << "Position: " << pos << " Value: " << (*valArray)[(*offArray)[posInOffArr]];
		else
			cout << "Position: " << pos << " Value: default value = " << defVal;
	}

}

/*Inform abour incorrect data */
void incorrectInput()
{
	cout << "Bledne parametry albo wektor nie istnieje, funkcja nie moze zostac wykonana\nSprobuj ponownie\n";
}



