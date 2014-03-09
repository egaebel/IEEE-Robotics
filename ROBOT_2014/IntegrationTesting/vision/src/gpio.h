#ifndef GPIO_H
#define GPIO_H

bool GPIOExport( int gpio_pin );

bool setGPIODirection( int gpio_pin, const char *setValue );

bool setGPIOValue( int gpio_pin, const char *setValue );

char getGPIOValue( int gpio_pin );

bool GPIOUnexport( int gpio_pin );

#endif
