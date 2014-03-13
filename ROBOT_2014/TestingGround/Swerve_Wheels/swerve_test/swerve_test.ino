#include "Arduino.h"
#include "SwerveWheel.h"

static int state = 0;
static SwerveWheel sw;
void setup()
{
  Serial.begin(9600);
  Serial.write("Setup Started\n");
  sw.setup();
  Serial.write("Setup Finished\n");
  delay(2000);
}

void loop()
{
  /*
  while(state < 1024)
  {
    sw.setAngle(state);
    delay(50);
    state = (state + 10);
  }
  */
  /*
  switch (state) {
    case 0:
      Serial.write("CASE 0\n");
      //sw.moveAngle(511);
      sw.moveForward();
      delay(3000);
      state++;
      break;
    case 1:
      break;
  }
  */
  
  switch (state) {
    case 0:
      Serial.write("CASE 0\n");
      sw.moveAngle(45);
      delay(6000);
      state++;
      break;
    case 1:
      Serial.write("CASE 1\n");
      sw.moveAngle(-45);
      delay(6000);
      state++;
      break;
    case 2:
      Serial.write("CASE 2\n");
      sw.moveAngle(-90);
      delay(6000);
      state++;
      break;
    case 3:
      Serial.write("CASE 3\n");
      sw.moveAngle(90);
      delay(6000);
      state++;
      break;
    case 4:
      Serial.write("CASE 4\n");
      sw.moveAngle(30);
      delay(6000);
      state++;
      break;
    case 5:
      Serial.write("CASE 5\n");
      sw.moveAngle(-30);
      delay(6000);
      state++;
      break;
    case 6:
      sw.moveAngle(0);
      delay(6000);
      state = 0;
      break;
  }
  
}
