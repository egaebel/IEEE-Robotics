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

#ifndef _CMUCOM4_H_
#define _CMUCOM4_H_

/**@cond CMUCOM4_PRIVATE*******************************************************/

// Handle Arduino Library renaming.
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Try to save RAM for non-Mega boards.
#if defined(__AVR_ATmega1280__) || \
    defined(__AVR_ATmega2560__) || \
    defined(__SAM3X8E__)
#define CMUCOM4_INPUT_BUFFER_SIZE   256 ///< Responce input buffer size.
#define CMUCOM4_OUTPUT_BUFFER_SIZE  256 ///< Command output buffer size.
#else
#define CMUCOM4_INPUT_BUFFER_SIZE   160 ///< Responce input buffer size.
#define CMUCOM4_OUTPUT_BUFFER_SIZE  96 ///< Command output buffer size.
#endif

/***************************************************************************//**
* This function macro expands whatever argument name that was passed to this
* function macro into a string. @par For example:
* <tt>@#define ARDUINO 100</tt> @n
* <tt>%CMUCOM4_N_TO_S(ARDUINO)</tt> exapands to @c "ARDUINO"
*******************************************************************************/
#define CMUCOM4_N_TO_S(x)           #x

/***************************************************************************//**
* This function macro expands whatever argument value that was passed to this
* function macro into a string. @par For example:
* <tt>@#define ARDUINO 100</tt> @n
* <tt>%CMUCOM4_V_TO_S(ARDUINO)</tt> exapands to @c "100"
*******************************************************************************/
#define CMUCOM4_V_TO_S(x)           CMUCOM4_N_TO_S(x)

/***************************************************************************//**
* Default firmware startup baud rate number.
*******************************************************************************/
#define CMUCOM4_SLOW_BAUD_RATE      19200

/***************************************************************************//**
* Default firmware startup baud rate string.
*******************************************************************************/
#define CMUCOM4_SLOW_BR_STRING      CMUCOM4_V_TO_S(CMUCOM4_SLOW_BAUD_RATE)

/***************************************************************************//**
* Version 1.01 firmware and below maximum baud rate number.
*******************************************************************************/
#define CMUCOM4_MEDIUM_BAUD_RATE    115200

/***************************************************************************//**
* Version 1.01 firmware and below maximum baud rate string.
*******************************************************************************/
#define CMUCOM4_MEDIUM_BR_STRING    CMUCOM4_V_TO_S(CMUCOM4_MEDIUM_BAUD_RATE)

/***************************************************************************//**
* Version 1.02 firmware and above maximum baud rate number.
*******************************************************************************/
#define CMUCOM4_FAST_BAUD_RATE      250000

/***************************************************************************//**
* Version 1.02 firmware and above maximum baud rate string.
*******************************************************************************/
#define CMUCOM4_FAST_BR_STRING      CMUCOM4_V_TO_S(CMUCOM4_FAST_BAUD_RATE)

/***************************************************************************//**
* Default firmware startup stop bits number.
*******************************************************************************/
#define CMUCOM4_SLOW_STOP_BITS      0

/***************************************************************************//**
* Default firmware startup stop bits string.
*******************************************************************************/
#define CMUCOM4_SLOW_SB_STRING      CMUCOM4_V_TO_S(CMUCOM4_SLOW_STOP_BITS)

/***************************************************************************//**
* Version 1.01 firmware and below necessary stop bits number.
*******************************************************************************/
#define CMUCOM4_MEDIUM_STOP_BITS    0

/***************************************************************************//**
* Version 1.01 firmware and below necessary stop bits string.
*******************************************************************************/
#define CMUCOM4_MEDIUM_SB_STRING    CMUCOM4_V_TO_S(CMUCOM4_MEDIUM_STOP_BITS)

/***************************************************************************//**
* Version 1.02 firmware and above necessary stop bits number.
*******************************************************************************/
#define CMUCOM4_FAST_STOP_BITS      0

/***************************************************************************//**
* Version 1.02 firmware and above necessary stop bits string.
*******************************************************************************/
#define CMUCOM4_FAST_SB_STRING      CMUCOM4_V_TO_S(CMUCOM4_FAST_STOP_BITS)

/***************************************************************************//**
* Serial CMUcom4::begin() post delay in milliseconds.
*******************************************************************************/
#define CMUCOM4_BEGIN_DELAY         1

/***************************************************************************//**
* Serial CMUcom4::end() post delay in milliseconds.
*******************************************************************************/
#define CMUCOM4_END_DELAY           1

/**@endcond********************************************************************/

/***************************************************************************//**
* This is a convenient macro for specifying the Serial port when initializing a
* CMUcam4 or CMUcom4 object.
*******************************************************************************/
#define CMUCOM4_SERIAL              0

/***************************************************************************//**
* This is a convenient macro for specifying the Serial1 port on an Arduino Mega
* when initializing a CMUcam4 or CMUcom4 object.
*******************************************************************************/
#define CMUCOM4_SERIAL1             1

/***************************************************************************//**
* This is a convenient macro for specifying the Serial2 port on an Arduino Mega
* when initializing a CMUcam4 or CMUcom4 object.
*******************************************************************************/
#define CMUCOM4_SERIAL2             2

/***************************************************************************//**
* This is a convenient macro for specifying the Serial3 port on an Arduino Mega
* when initializing a CMUcam4 or CMUcom4 object.
*******************************************************************************/
#define CMUCOM4_SERIAL3             3

/***************************************************************************//**
* This is a hardware abstraction layer for the %CMUcam4 class. The %CMUcom4
* class targets the Ardunio prototyping platform by default.
*******************************************************************************/
class CMUcom4
{

public:

/***************************************************************************//**
* Initialize the %CMUcom4 object to use the default Serial port.
*******************************************************************************/
CMUcom4();

/***************************************************************************//**
* Initialize the %CMUcom4 object to use the @c port Serial port.
* @param [in] port The port.
* @see CMUCOM4_SERIAL
* @see CMUCOM4_SERIAL1
* @see CMUCOM4_SERIAL2
* @see CMUCOM4_SERIAL3
*******************************************************************************/
CMUcom4(int port);

/***************************************************************************//**
* Arduino Serial.begin() wrapper.
* @param [in] baud In bits per second.
* @see http://arduino.cc/en/Serial/Begin
*******************************************************************************/
void begin(unsigned long baud);

/***************************************************************************//**
* Arduino Serial.end() wrapper.
* @see http://arduino.cc/en/Serial/End
*******************************************************************************/
void end();

/***************************************************************************//**
* Arduino Serial.read() wrapper.
* @return The first byte of incoming serial data.
* @see http://arduino.cc/en/Serial/Read
*******************************************************************************/
int read();

/***************************************************************************//**
* Arduino Serial.write() wrapper.
* @param [in] buffer An array to send as a series of bytes.
* @param [in] size The size of the buffer.
* @return The number of bytes written.
* @see http://arduino.cc/en/Serial/Write
*******************************************************************************/
size_t write(const uint8_t * buffer, size_t size);

/***************************************************************************//**
* Arduino Serial.write() wrapper.
* @param [in] str A string to send as a series of bytes.
* @return The number of bytes written.
* @see http://arduino.cc/en/Serial/Write
*******************************************************************************/
size_t write(const char * str);

/***************************************************************************//**
* Arduino Serial.write() wrapper.
* @param [in] c A character to send as a single byte.
* @return The number of bytes written.
* @see http://arduino.cc/en/Serial/Write
*******************************************************************************/
size_t write(uint8_t c);

/***************************************************************************//**
* Arduino Serial.available() wrapper.
* @return The number of bytes available to be read.
* @see http://arduino.cc/en/Serial/Available
*******************************************************************************/
int available();

/***************************************************************************//**
* Arduino Serial.flush() wrapper.
* @see http://arduino.cc/en/Serial/Flush
*******************************************************************************/
void flush();

/***************************************************************************//**
* Arduino Serial.peek() wrapper.
* @return The first byte of incoming serial data available.
* @see http://arduino.cc/en/Serial/Peek
*******************************************************************************/
int peek();

/***************************************************************************//**
* Arduino delay() wrapper.
* @param [in] ms The number of milliseconds to pause for.
* @see http://arduino.cc/en/Reference/Delay
*******************************************************************************/
void delayMilliseconds(unsigned long ms);

/***************************************************************************//**
* Arduino millis() wrapper.
* @return Number of milliseconds since the program started.
* @see http://arduino.cc/en/Reference/Millis
*******************************************************************************/
unsigned long milliseconds();

private:

/***************************************************************************//**
* Selected serial port storage.
* @see CMUCOM4_SERIAL1
* @see CMUCOM4_SERIAL2
* @see CMUCOM4_SERIAL3
*******************************************************************************/
int _port;
};

#endif

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
