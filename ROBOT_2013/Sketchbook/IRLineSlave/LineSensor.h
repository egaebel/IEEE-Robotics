#include <Arduino.h>
#ifndef LineSensor_h
#define LineSensor_h

class LineSensor
{
  public:
          LineSensor();
          void getTotal(boolean dataArray[16]);
};

#endif
