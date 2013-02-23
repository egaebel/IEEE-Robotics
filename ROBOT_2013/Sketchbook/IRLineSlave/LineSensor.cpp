#include "LineSensor.h"

LineSensor::LineSensor()
{

}

void LineSensor::getTotal(boolean dataArray[16])
{
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
