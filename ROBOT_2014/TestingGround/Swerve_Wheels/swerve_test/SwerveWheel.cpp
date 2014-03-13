#include "SwerveWheel.h"

void SwerveWheel::setup()
{
  hubee.setup(HUBEE_PIN_IN1, HUBEE_PIN_IN2, HUBEE_PIN_PWM, 200);
  hubee.stop();
  ax.setup();
}
int SwerveWheel::setAngle(int angle)
{
  if(angle < -90 || angle > 90)
  {
    return -1;
  }
  int mappedValue = DEAD_AHEAD + (int)(angle * 1024.0f/300.0f);
  return ax.setAngle(mappedValue);
}
int SwerveWheel::moveAngle(int angle)
{
  if(angle < -90 || angle > 90)
  {
    return -1;
  }
  int mappedValue = DEAD_AHEAD + (int)(angle * 1024.0f/300.0f);
  int output = ax.setAngle(mappedValue);
  hubee.forward();
  return output;
}
void SwerveWheel::moveForward()
{
  hubee.forward();
}
void SwerveWheel::moveBackward()
{
  hubee.backward();
}
void SwerveWheel::stop()
{
  hubee.stop();
}
