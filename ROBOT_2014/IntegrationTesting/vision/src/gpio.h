#ifndef GPIO_H
#define GPIO_H

bool GPIOExport( int gpio_pin );

bool setGPIODirection( int gpio_pin, const char *setValue );

bool setGPIOValue( int gpio_pin, const char *setValue );

bool getGPIOValue( int gpio_pin, char *getValue );

bool GPIOUnexport( int gpio_pin );

#endif
