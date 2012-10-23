#include "IRLib.h"
#include <math.h>

IRAverager::IRAverager()
{
	slot = 0;
	sum = 0;
	for (int i = 0; i < length; i++)
	{
		valArray[i] = 0;
	}
}

void IRAverager::updateIR()
{
	sum -= valArray[slot];
	valArray[slot] = analogRead(ANALOG_PIN);
	sum += valArray[slot];
	slot++;
	if (slot > length-1)
		slot = 0;
}

float IRAverager::getIR()
{
	float devsum = 0;
	float mean = sum/length;
	for (int i = 0; i < length-1; i++)
	{
		devsum += pow((valArray[i] - mean),2);
	}
	float stdDev = sqrt((1.0/(length-1)*devsum));

	int out = 0;
	int newSum;
	for (int i = 0; i < length-1; i++)
	{
		if (valArray[i] > mean - (2*stdDev) && valArray[i] < mean + (2*stdDev))
		{
			newSum += valArray[i];
		}
		else
			out++;
	}
	return newSum/(length-out);
}