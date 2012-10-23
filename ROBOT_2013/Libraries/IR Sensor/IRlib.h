#define ANALOG_PIN 1
#define length 20

class IRAverager
{
public:
	IRAverager();
	float valArray[length];
	int slot;
	float sum;
	void updateIR();
	float getIR();
};