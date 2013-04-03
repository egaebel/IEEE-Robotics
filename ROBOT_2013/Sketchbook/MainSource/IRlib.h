#ifndef IRLIB_H_
#define IRLIB_H_


#define ANALOG_PIN 1
#define lengthArray 20
#include <Arduino.h>
class IRAverager
{
public:
	IRAverager();
	float valArray[lengthArray];
	int slot;
	float sum;
	void updateIR();
	float getIR();
};

#endif