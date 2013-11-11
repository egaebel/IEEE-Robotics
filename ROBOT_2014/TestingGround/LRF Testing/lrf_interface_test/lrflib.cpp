#include "LRFlib.h"
#include <cmath>
#include "Arduino.h"

LRFAverager::LRFAverager() {
        serial_port = 1;
}

void LRFAverager::initialize() {
        switch(serial_port)
        {
          case 1:
          Serial1.begin(9600);
          break;
          case 2:
          Serial2.begin(9600);
          break;
          case 3:
          Serial3.begin(9600);
          break;
          default:
          Serial.begin(9600);
        }
	delay(100);
	lrfWrite('U');
        lrfWrite('L');
        delay(100);
}


/*
 * Returns an average distance from LRF (created from ten readings) in millimetres
 * or -1 no value returned.
 */
int LRFAverager::get10ReadingAverage() {
	int idx = 0;
	int average = 0;
	int seenError = 0;
	for(idx = 0; idx < 10; idx++)
	{
		int retValue = getSingleReading();
		if(retValue >=0)
		{
			average = average + getSingleReading();
		}
		else if(seenError > 0)
		{
			return -1;
		}
		else
		{
			seenError = 1;
			idx--;
		}
	}
	average = (int)((average * 1.0)/10);
	return average;
}
/*
 * Returns the distance from LRF in millimetres or -1 no value returned.
 */
int LRFAverager::getSingleReading() {
        lrfFlush();
	char lrfData[32];
	int offset = 0;
	lrfData[0] = 0;
        while (lrfAvailable() <= 0)
        {
                delay(10);  
        }
        delay(20);
	while (lrfAvailable() > 0)
        {
		// read what the LRF sends back 
		lrfData[offset] = lrfRead();
		if(lrfData[offset] == ':' || lrfData[offset] == '?')
		{
			lrfData[offset] = 0; // null terminate the string of bytes and break out of the loop
			break;
		}
		offset++;
	}
	if(offset > 0)
	{ 
		//integer to convert lrfData string to an integer value
		int distance = atoi(&lrfData[4]);
		return distance;
	}
	return -1;
}

void LRFAverager::lrfWrite(char inputChar)
{
        switch(serial_port)
        {
          case 1:
          Serial1.write(inputChar);
          break;
          case 2:
          Serial2.write(inputChar);
          break;
          case 3:
          Serial3.write(inputChar);
          break;
          default:
          Serial.write(inputChar);
        }
}

char LRFAverager::lrfRead()
{
        switch(serial_port)
        {
          case 1:
          return Serial1.read();
          break;
          case 2:
          return Serial2.read();
          break;
          case 3:
          return Serial3.read();
          break;
          default:
          return Serial.read();
        }
}

void LRFAverager::setSerialPort(int port)
{
        if(port > 0 && port <= 3)
        {
          free();
          serial_port = port;
          initialize();
        }
}

void LRFAverager::free()
{
        switch(serial_port)
        {
          case 1:
          Serial1.write('Z');
          Serial1.flush();
          Serial1.end();
          break;
          case 2:
          Serial2.write('Z');
          Serial2.flush();
          Serial2.end();
          break;
          case 3:
          Serial3.write('Z');
          Serial3.flush();
          Serial3.end();
          break;
          default:
          Serial.write('Z');
          Serial.flush();
          Serial.end();
        }
}

void LRFAverager::stopReading()
{
         switch(serial_port)
        {
          case 1:
          Serial1.write('Z');
          Serial1.flush();
          break;
          case 2:
          Serial2.write('Z');
          Serial2.flush();
          break;
          case 3:
          Serial3.write('Z');
          Serial3.flush();
          break;
          default:
          Serial.write('Z');
          Serial.flush();
        }
}

void LRFAverager::startReading()
{
        stopReading();
        switch(serial_port)
        {
          case 1:
          Serial1.write('L');
          break;
          case 2:
          Serial2.write('L');
          break;
          case 3:
          Serial3.write('L');
          break;
          default:
          Serial.write('L');
        }
}

void LRFAverager::lrfFlush()
{
        switch(serial_port)
        {
          case 1:
          Serial1.flush();
          break;
          case 2:
          Serial2.flush();
          break;
          case 3:
          Serial3.flush();
          break;
          default:
          Serial.flush();
        }
}

int LRFAverager::lrfAvailable()
{
        switch(serial_port)
        {
          case 1:
          return Serial1.available();
          break;
          case 2:
          return Serial2.available();
          break;
          case 3:
          return Serial3.available();
          break;
          default:
          return Serial.available();
        }
}
