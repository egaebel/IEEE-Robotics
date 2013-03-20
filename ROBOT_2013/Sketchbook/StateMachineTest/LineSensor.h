#ifndef LineSensor_h
#define LineSensor_h

#include <Arduino.h>
#include "common.h"

class LineSensor
{
  public:
    LineSensor() {

    }

    bool detectFront() {

    	return true;
    }

    bool detectRight() {

    	return true;
    }

    bool detectLeft() {

    	return true;
    }

    void getTotal(bool dataArray[16]) {

	    for(int i=0;i<16;i++)
	  	{
	    	if(analogRead(i) < 40)
	    	{
	      		dataArray[i] = true;
	    	}
	    	else
	    	{
	      		dataArray[i] = false;
	    	}
	  	}
	}
};

#endif
