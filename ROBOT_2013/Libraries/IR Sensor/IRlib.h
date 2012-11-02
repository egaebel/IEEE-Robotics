#define ANALOG_PIN 1
#define lengthArray 20

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