#include <cstring>
#include <string>
#include <cstdio>
#include "servo.h"
#include "config.h"

bool write_servo_fs( const char* path, const char* value, const int length )
{
  FILE *myOutputHandle = NULL;
  if( ( myOutputHandle = fopen( path, "rb+" ) ) == NULL ) {
    printf( "Unable to open %s\n", path );
    return false;
  }
  fwrite( value, sizeof(char), length, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}

bool enable_servo( const char* servo )
{
  FILE *myOutputHandle = NULL;

  // enable pwm hardware
  /*
  if( ( myOutputHandle = fopen( "/sys/devices/bone_capemgr.9/slots", "rb+" ) ) == NULL ) {
    printf( "Unable to open cape manager to enable pwm hardware\n" );
    return false;
  }
  fwrite( "am33xx_pwm", sizeof(char), 10, myOutputHandle );
  fflush( myOutputHandle );
  */
  char pin[6];
  strncpy( pin, servo, 5 );
  char setValue[64];
  int length = sprintf( setValue, "bone_pwm_%s", pin );
  /*
  fwrite( &setValue, sizeof(char), length, myOutputHandle );
  fclose( myOutputHandle );
  */

  // revisit: the pin takes some time to show up
  //usleep( 500000 );

  // set positive polarity
  char polarity_handle[64];
  sprintf( polarity_handle, "/sys/devices/ocp.3/pwm_test_%s/polarity", servo );
  /*
  if( ( myOutputHandle = fopen( polarity_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open polarity handle %s\n", polarity_handle );
    return false;
  }
  fwrite( "0", sizeof(char), 1, myOutputHandle );
  fclose( myOutputHandle );
  */

  // set 20ms period for 50Hz frequency
  char period_handle[64];
  sprintf( period_handle, "/sys/devices/ocp.3/pwm_test_%s/period", servo );
  /*
  if( ( myOutputHandle = fopen( period_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open period handle\n" );
    return false;
  }
  fwrite( "20000000", sizeof(char), 8, myOutputHandle );
  fclose( myOutputHandle );
  */

  // activate pwm output pin
  char run_handle[64];
  sprintf( run_handle, "/sys/devices/ocp.3/pwm_test_%s/run", servo );
  /*
  if( ( myOutputHandle = fopen( run_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open run handle\n" );
    return false;
  }
  fwrite( "1", sizeof(char), 1, myOutputHandle );
  fclose( myOutputHandle );
  */

  return write_servo_fs( "/sys/devices/bone_capemgr.9/slots", "am33xx_pwm", 10 )
    && write_servo_fs( "/sys/devices/bone_capemgr.9/slots", setValue, length )
    && write_servo_fs( polarity_handle, "0", 1 )
    && write_servo_fs( period_handle, "20000000", 8 )
    && write_servo_fs( run_handle, "1", 1 );
}

bool disable_servo( const char* servo )
{
  return write_servo_fs( run_handle, "0", 1 );
}

bool set_servo_position( const char* servo, int position )
{
  FILE *myOutputHandle = NULL;
  char duty_handle[64];
  sprintf( duty_handle, "/sys/devices/ocp.3/pwm_test_%s/duty", servo );
  if( ( myOutputHandle = fopen( duty_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open duty handle\n" );
    return false;
  }
  char setValue[8];
  int length = sprintf( setValue, "%d", position );
  fwrite( &setValue, sizeof(char), length, myOutputHandle );
  fclose( myOutputHandle );
  return true;
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
