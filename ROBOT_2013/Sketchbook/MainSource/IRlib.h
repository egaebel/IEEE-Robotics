#ifndef IRLIB_H_
#define IRLIB_H_

#define lengthArray 20
#include <Arduino.h>
class IRAverager
{
public:
	IRAverager(int pin);
	float valArray[lengthArray];
	int slot;
	float sum;
	void updateIR();
	float getIR();
};

#endif