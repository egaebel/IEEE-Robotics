#include "Arduino.h"
#include "ax12.h"

static int state = 0;
static ax12 ax;
void setup()
{
  Serial.begin(9600);
  Serial.write("Setup Started\n");
  ax.setup();
  Serial.write("Setup Finished\n");
}

void loop()
{
  
  switch (state) {
    case 0:
      Serial.write("CASE 0\n");
      ax.setAngle(511);
      delay(3000);
      state++;
      break;
    case 1:
      Serial.write("CASE 1\n");
      ax.setAngle(0);
      delay(3000);
      state++;
      break;
    case 2:
      Serial.write("CASE 2\n");
      ax.setAngle(1023);
      delay(3000);
      state++;
      break;
    case 3:
      Serial.write("CASE 3\n");
      ax.setAngle(511);
      delay(3000);
      state++;
      break;
    case 4:
      Serial.write("CASE 4\n");
      ax.setAngle(1023);
      delay(3000);
      state++;
      break;
    case 5:
      Serial.write("CASE 5\n");
      ax.setAngle(0);
      delay(3000);
      state++;
      break;
    case 6:
      state = 0;
      break;
  }
  
}
