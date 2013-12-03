#include "gpio.h"
#include "config.h"

#include <cstring>
#include <cstdio>

bool GPIOExport( int gpio_pin )
{
  FILE *myOutputHandle = NULL;
  char export_handle[4];
  sprintf( export_handle, "%d", gpio_pin );
  if ((myOutputHandle = fopen("/sys/class/gpio/export", "ab")) == NULL){
      printf("Unable to export GPIO pin\n");
      return 1;
  }
  char setValue[4];
  strcpy( setValue, export_handle );
  fwrite( &setValue, sizeof(char), 2, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}

bool setGPIODirection( int gpio_pin, const char *setValue )
{
  FILE *myOutputHandle = NULL;
  char direction_handle[64];
  sprintf( direction_handle, "/sys/class/gpio/gpio%d/direction", gpio_pin );
  if ( ( myOutputHandle = fopen( direction_handle, "rb+" ) ) == NULL ) {
      printf( "Unable to open direction handle\n" );
      return false;
  }
  fwrite( setValue, sizeof(char), 3, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}

bool setGPIOValue( int gpio_pin, const char *setValue )
{
  FILE *myOutputHandle = NULL;
  char value_handle[64];
  sprintf( value_handle, "/sys/class/gpio/gpio%d/value", gpio_pin );
  if( ( myOutputHandle = fopen( value_handle, "rb+" ) ) == NULL ) {
    printf( "Unable to open value handle\n" );
    return false;
  }
  fwrite( setValue, sizeof(char), 1, myOutputHandle );
  fclose( myOutputHandle );
  return true;
}
