#ifndef LRFLIB_H_
#define LRFLIB_H_

#include <Arduino.h>

class LRFAverager
{
	public:
		LRFAverager();
                void initialize();
		int get10ReadingAverage();
		int getSingleReading();
                void setSerialPort(int port);
                void stopReading();
                void startReading();
                void free();
        private:
                int serial_port;
                void lrfWrite(char inputChar);
                char lrfRead();
                void lrfFlush();
                int lrfAvailable();
};

#endif
