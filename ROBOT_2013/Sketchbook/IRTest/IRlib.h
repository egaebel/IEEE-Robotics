#define lengthArray 20

#ifndef IRlib_h
#define IRlib_h

class IRAverager
{
public:
	IRAverager(int inpin);
	float valArray[lengthArray];
	int slot;
	float sum;
	void updateIR();
	float getIR();
        int pin;
};
IRAverager::IRAverager(int inpin)
{
	slot = 0;
	sum = 0;
        pin = inpin;
	for (int i = 0; i < lengthArray; i++)
	{
		valArray[i] = 0;
	}
}

void IRAverager::updateIR()
{
	sum -= valArray[slot];
	valArray[slot] = analogRead(pin);
	valArray[slot] = 27.3382*pow(.4464,valArray[slot]);
	sum += valArray[slot];
	slot++;
	if (slot > lengthArray-1)
		slot = 0;
}

float IRAverager::getIR()
{
	float devsum = 0;
	float mean = sum/lengthArray;
	for (int i = 0; i < lengthArray-1; i++)
	{
		devsum += pow((valArray[i] - mean),2);
	}
	float stdDev = sqrt((1.0/(lengthArray-1)*devsum));

	int out = 0;
	int newSum;
	for (int i = 0; i < lengthArray-1; i++)
	{
		if (valArray[i] > mean - (2*stdDev) && valArray[i] < mean + (2*stdDev))
		{
			newSum += valArray[i];
		}
		else
			out++;
	}
	return newSum/(lengthArray-out);
}
#endif
