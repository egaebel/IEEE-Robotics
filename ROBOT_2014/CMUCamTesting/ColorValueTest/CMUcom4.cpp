/***************************************************************************//**
* @file
* Portable serial and timer wrapper library.
*
* @version @n 1.1
* @date @n 2/7/2013
*
* @authors @n Kwabena W. Agyeman & Christopher J. Leaf
* @copyright @n (c) 2013 Kwabena W. Agyeman & Christopher J. Leaf
* @n All rights reserved - Please see the end of the file for the terms of use
*
* @par Update History:
* @n v0.1 - Beta code - 3/20/2012
* @n v0.9 - Original release - 4/18/2012
* @n v1.0 - Documented and updated release - 8/3/2012
* @n v1.1 - Added support for the Arduino Due, fixed the send frame command,
            and fixed a number of compile time warnings - 2/7/2013.
*******************************************************************************/

#include "CMUcom4.h"

/*******************************************************************************
* Constructor Functions
*******************************************************************************/

CMUcom4::CMUcom4()
{
    _port = CMUCOM4_SERIAL;
}

CMUcom4::CMUcom4(int port)
{
    _port = port;
}

/*******************************************************************************
* Public Functions
*******************************************************************************/

void CMUcom4::begin(unsigned long baud)
{
    delayMilliseconds(CMUCOM4_BEGIN_DELAY);

#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: Serial1.begin(baud); break;
        case CMUCOM4_SERIAL2: Serial2.begin(baud); break;
        case CMUCOM4_SERIAL3: Serial3.begin(baud); break;
        default: Serial.begin(baud); break;
    }
#else
    Serial.begin(baud);
#endif

    delayMilliseconds(CMUCOM4_BEGIN_DELAY);
}

void CMUcom4::end()
{
    delayMilliseconds(CMUCOM4_END_DELAY);

#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: Serial1.end(); break;
        case CMUCOM4_SERIAL2: Serial2.end(); break;
        case CMUCOM4_SERIAL3: Serial3.end(); break;
        default: Serial.end(); break;
    }
#else
    Serial.end();
#endif

    delayMilliseconds(CMUCOM4_END_DELAY);
}

int CMUcom4::read()
{
#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: return Serial1.read(); break;
        case CMUCOM4_SERIAL2: return Serial2.read(); break;
        case CMUCOM4_SERIAL3: return Serial3.read(); break;
        default: return Serial.read(); break;
    }
#else
    return Serial.read();
#endif
}

size_t CMUcom4::write(uint8_t c)
{
#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: return Serial1.write(c); break;
        case CMUCOM4_SERIAL2: return Serial2.write(c); break;
        case CMUCOM4_SERIAL3: return Serial3.write(c); break;
        default: return Serial.write(c); break;
    }
#else
    return Serial.write(c);
#endif
}

size_t CMUcom4::write(const char * str)
{
#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: return Serial1.write(str); break;
        case CMUCOM4_SERIAL2: return Serial2.write(str); break;
        case CMUCOM4_SERIAL3: return Serial3.write(str); break;
        default: return Serial.write(str); break;
    }
#else
    return Serial.write(str);
#endif
}

size_t CMUcom4::write(const uint8_t * buffer, size_t size)
{
#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: return Serial1.write(buffer, size); break;
        case CMUCOM4_SERIAL2: return Serial2.write(buffer, size); break;
        case CMUCOM4_SERIAL3: return Serial3.write(buffer, size); break;
        default: return Serial.write(buffer, size); break;
    }
#else
    return Serial.write(buffer, size);
#endif
}

int CMUcom4::available()
{
#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: return Serial1.available(); break;
        case CMUCOM4_SERIAL2: return Serial2.available(); break;
        case CMUCOM4_SERIAL3: return Serial3.available(); break;
        default: return Serial.available(); break;
    }
#else
    return Serial.available();
#endif
}

void CMUcom4::flush()
{
#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: Serial1.flush(); break;
        case CMUCOM4_SERIAL2: Serial2.flush(); break;
        case CMUCOM4_SERIAL3: Serial3.flush(); break;
        default: Serial.flush(); break;
    }
#else
    Serial.flush();
#endif
}

int CMUcom4::peek()
{
#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
    switch(_port)
    {
        case CMUCOM4_SERIAL1: return Serial1.peek(); break;
        case CMUCOM4_SERIAL2: return Serial2.peek(); break;
        case CMUCOM4_SERIAL3: return Serial3.peek(); break;
        default: return Serial.peek(); break;
    }
#else
    return Serial.peek();
#endif
}

void CMUcom4::delayMilliseconds(unsigned long ms)
{
    return delay(ms);
}

unsigned long CMUcom4::milliseconds()
{
    return millis();
}

/***************************************************************************//**
* @file
* @par MIT License - TERMS OF USE:
* @n Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* @n
* @n The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* @n
* @n THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/
