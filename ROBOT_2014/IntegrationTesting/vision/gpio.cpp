/**
 * @file gpio
 * @brief direct gpio pin control
 * @author D. Butenhoff
 */

#include <cstring>
#include <cstdio>
#include "hardware.h"
#include "gpio.h"
#include "config.h"

bool GPIOExport( int gpio_pin )
{
  char pin_hardware[4];
  int length = sprintf( pin_hardware, "%d", gpio_pin );
  return write_hardware_fs( "/sys/class/gpio/export", pin_hardware, length, "ab" )
}

bool setGPIODirection( int gpio_pin, const char *setValue )
{
  char direction_handle[64];
  sprintf( direction_handle, "/sys/class/gpio/gpio%d/direction", gpio_pin );
  return write_hardware_fs( direction_handle, setValue, 3 );
}

bool setGPIOValue( int gpio_pin, const char *setValue )
{
  char value_handle[64];
  sprintf( value_handle, "/sys/class/gpio/gpio%d/value", gpio_pin );
  return write_hardware_fs( value_handle, setValue, 1 );
}
