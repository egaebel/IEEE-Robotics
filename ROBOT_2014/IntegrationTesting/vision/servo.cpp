/**
 * @file servo
 * @brief servo control code using pwm hardware
 * @author D. Butenhoff
 */

#include <cstring>
#include <string>
#include <cstdio>
#include "hardware.h"
#include "servo.h"
#include "config.h"

bool enable_servo( const char* servo )
{
  // enable pwm hardware
  char pin[6];
  strncpy( pin, servo, 5 );
  char pin_hardware[64];
  int length = sprintf( pin_hardware, "bone_pwm_%s", pin );

  if( write_hardware_fs( "/sys/devices/bone_capemgr.9/slots", "am33xx_pwm", 10 )
    && write_hardware_fs( "/sys/devices/bone_capemgr.9/slots", pin_hardware, length ) )
  {
    // revisit: the pin takes some time to show up
    usleep( 500000 );

    // set positive polarity
    char polarity_handle[64];
    sprintf( polarity_handle, "/sys/devices/ocp.3/pwm_test_%s/polarity", servo );

    // set 20ms period for 50Hz frequency
    char period_handle[64];
    sprintf( period_handle, "/sys/devices/ocp.3/pwm_test_%s/period", servo );

    // activate pwm output pin
    char run_handle[64];
    sprintf( run_handle, "/sys/devices/ocp.3/pwm_test_%s/run", servo );

    return write_hardware_fs( polarity_handle, "0", 1 )
      && write_hardware_fs( period_handle, "20000000", 8 )
      && write_hardware_fs( run_handle, "1", 1 );
  }
  return false;
}

bool disable_servo( const char* servo )
{
  char run_handle[64];
  sprintf( run_handle, "/sys/devices/ocp.3/pwm_test_%s/run", servo );
  return write_hardware_fs( run_handle, "0", 1 );
}

bool set_servo_position( const char* servo, int position )
{
  char duty_handle[64];
  sprintf( duty_handle, "/sys/devices/ocp.3/pwm_test_%s/duty", servo );
  char setValue[8];
  int length = sprintf( setValue, "%d", position );
  return write_hardware_fs( duty_handle, setValue, length );
}

bool check_servo_range( const char* servo, int min_position, int ctr_position, int max_position )
{
  set_servo_position( servo, min_position );
  usleep( 500000 );
  set_servo_position( servo, max_position );
  usleep( 500000 );
  set_servo_position( servo, ctr_position );
  return true;
}
