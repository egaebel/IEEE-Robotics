/***************************************************************************//**
* @file
* Portable %CMUcam4 interface library.
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

#ifndef _CMUCAM4_H_
#define _CMUCAM4_H_

#include "CMUcom4.h"

#include <setjmp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/***************************************************************************//**
* %CMUcam4 firmware version 1.00.
*******************************************************************************/
#define CMUCAM4_FIRMWARE_V100               100

/***************************************************************************//**
* %CMUcam4 firmware version 1.01.
*******************************************************************************/
#define CMUCAM4_FIRMWARE_V101               101

/***************************************************************************//**
* %CMUcam4 firmware version 1.02.
*******************************************************************************/
#define CMUCAM4_FIRMWARE_V102               102

/***************************************************************************//**
* %CMUcam4 firmware version 1.03.
*******************************************************************************/
#define CMUCAM4_FIRMWARE_V103               103

/***************************************************************************//**
* The native horizontal resolution.
*******************************************************************************/
#define CMUCAM4_NATIVE_H_RES                160

/***************************************************************************//**
* The native vertical resolution.
*******************************************************************************/
#define CMUCAM4_NATIVE_V_RES                120

/***************************************************************************//**
* The binary bitmap horizontal resolution.
*******************************************************************************/
#define CMUCAM4_BINARY_H_RES                (CMUCAM4_NATIVE_H_RES / 2)

/***************************************************************************//**
* The binary bitmap vertical resolution.
*******************************************************************************/
#define CMUCAM4_BINARY_V_RES                (CMUCAM4_NATIVE_V_RES / 2)

/***************************************************************************//**
* The first native row.
*******************************************************************************/
#define CMUCAM4_MIN_NATIVE_ROW              0

/***************************************************************************//**
* The first native column.
*******************************************************************************/
#define CMUCAM4_MIN_NATIVE_COLUMN           0

/***************************************************************************//**
* The last native row.
*******************************************************************************/
#define CMUCAM4_MAX_NATIVE_ROW              (CMUCAM4_NATIVE_V_RES - 1)

/***************************************************************************//**
* The last native column.
*******************************************************************************/
#define CMUCAM4_MAX_NATIVE_COLUMN           (CMUCAM4_NATIVE_H_RES - 1)

/***************************************************************************//**
* The first binary bitmap row.
*******************************************************************************/
#define CMUCAM4_MIN_BINARY_ROW              0

/***************************************************************************//**
* The first binary bitmap column.
*******************************************************************************/
#define CMUCAM4_MIN_BINARY_COLUMN           0

/***************************************************************************//**
* The last binary bitmap row.
*******************************************************************************/
#define CMUCAM4_MAX_BINARY_ROW              (CMUCAM4_BINARY_V_RES - 1)

/***************************************************************************//**
* The last binary bitmap column.
*******************************************************************************/
#define CMUCAM4_MAX_BINARY_COLUMN           (CMUCAM4_BINARY_H_RES - 1)

/***************************************************************************//**
* This is a convenient macro for comparing the pan pin to low and the tilt pin
* to low.
* @see CMUcam4::getInputs()
*******************************************************************************/
#define CMUCAM4_IN_TP_LL                    0

/***************************************************************************//**
* This is a convenient macro for comparing the pan pin to high and the tilt pin
* to low.
* @see CMUcam4::getInputs()
*******************************************************************************/
#define CMUCAM4_IN_TP_LH                    1

/***************************************************************************//**
* This is a convenient macro for comparing the pan pin to low and the tilt pin
* to high.
* @see CMUcam4::getInputs()
*******************************************************************************/
#define CMUCAM4_IN_TP_HL                    2

/***************************************************************************//**
* This is a convenient macro for comparing the pan pin to high and the tilt pin
* to high.
* @see CMUcam4::getInputs()
*******************************************************************************/
#define CMUCAM4_IN_TP_HH                    3

/***************************************************************************//**
* This is a convenient macro for specifying the pan pin to be an input and the
* tilt pin to be an input.
* @see CMUcam4::setOutputs()
*******************************************************************************/
#define CMUCAM4_DIR_TP_II                   0

/***************************************************************************//**
* This is a convenient macro for specifying the pan pin to be an output and the
* tilt pin to be an input.
* @see CMUcam4::setOutputs()
*******************************************************************************/
#define CMUCAM4_DIR_TP_IO                   1

/***************************************************************************//**
* This is a convenient macro for specifying the pan pin to be an input and the
* tilt pin to be an output.
* @see CMUcam4::setOutputs()
*******************************************************************************/
#define CMUCAM4_DIR_TP_OI                   2

/***************************************************************************//**
* This is a convenient macro for specifying the pan pin to be an output and the
* tilt pin to be an output.
* @see CMUcam4::setOutputs()
*******************************************************************************/
#define CMUCAM4_DIR_TP_OO                   3

/***************************************************************************//**
* This is a convenient macro for specifying the pan pin to output low and the
* tilt pin to output low.
* @see CMUcam4::setOutputs()
*******************************************************************************/
#define CMUCAM4_OUT_TP_LL                   0

/***************************************************************************//**
* This is a convenient macro for specifying the pan pin to output high and the
* tilt pin to output low.
* @see CMUcam4::setOutputs()
*******************************************************************************/
#define CMUCAM4_OUT_TP_LH                   1

/***************************************************************************//**
* This is a convenient macro for specifying the pan pin to output low and the
* tilt pin to output high.
* @see CMUcam4::setOutputs()
*******************************************************************************/
#define CMUCAM4_OUT_TP_HL                   2

/***************************************************************************//**
* This is a convenient macro for specifying the pan pin to output high and the
* tilt pin to output high.
* @see CMUcam4::setOutputs()
*******************************************************************************/
#define CMUCAM4_OUT_TP_HH                   3

/***************************************************************************//**
* This is a convenient macro for specifying that the LED should be off when the
* %CMUcam4 is idling.
* @see CMUcam4::LEDOn()
*******************************************************************************/
#define CMUCAM4_LED_OFF                     -1

/***************************************************************************//**
* This is a convenient macro for specifying that the LED should be on when the
* %CMUcam4 is idling.
* @see CMUcam4::LEDOn()
*******************************************************************************/
#define CMUCAM4_LED_ON                      0

/***************************************************************************//**
* This is a convenient macro for specifying the pan servo.
* @see CMUcam4::getServoPosition()
* @see CMUcam4::setServoPosition()
*******************************************************************************/
#define CMUCAM4_PAN_SERVO                   0

/***************************************************************************//**
* This is a convenient macro for specifying the tilt servo.
* @see CMUcam4::getServoPosition()
* @see CMUcam4::setServoPosition()
*******************************************************************************/
#define CMUCAM4_TILT_SERVO                  1

/***************************************************************************//**
* This is a convenient macro for specifying the histogram's red channel.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_RED_CHANNEL                 0

/***************************************************************************//**
* This is a convenient macro for specifying the histogram's green channel.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_GREEN_CHANNEL               1

/***************************************************************************//**
* This is a convenient macro for specifying the histogram's blue channel.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_BLUE_CHANNEL                2

/***************************************************************************//**
* This is a convenient macro for specifying a 1-bin histogram.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_H1_BINS                     0

/***************************************************************************//**
* This is a convenient macro for specifying a 2-bin histogram.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_H2_BINS                     1

/***************************************************************************//**
* This is a convenient macro for specifying a 4-bin histogram.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_H4_BINS                     2

/***************************************************************************//**
* This is a convenient macro for specifying a 8-bin histogram.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_H8_BINS                     3

/***************************************************************************//**
* This is a convenient macro for specifying a 16-bin histogram.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_H16_BINS                    4

/***************************************************************************//**
* This is a convenient macro for specifying a 32-bin histogram.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_H32_BINS                    5

/***************************************************************************//**
* This is a convenient macro for specifying a 64-bin histogram.
* @see CMUcam4::getHistogram()
*******************************************************************************/
#define CMUCAM4_H64_BINS                    6

/***************************************************************************//**
* This is a convenient macro for specifying a horizontal resolution of 640
* pixels for CMUcam4::dumpFrame() and CMUcam4::sendFrame().
*******************************************************************************/
#define CMUCAM4_HR_640                      0

/***************************************************************************//**
* This is a convenient macro for specifying a horizontal resolution of 320
* pixels for CMUcam4::dumpFrame() and CMUcam4::sendFrame().
*******************************************************************************/
#define CMUCAM4_HR_320                      1

/***************************************************************************//**
* This is a convenient macro for specifying a horizontal resolution of 160
* pixels for CMUcam4::dumpFrame() and CMUcam4::sendFrame().
*******************************************************************************/
#define CMUCAM4_HR_160                      2

/***************************************************************************//**
* This is a convenient macro for specifying a horizontal resolution of 80
* pixels for CMUcam4::dumpFrame() and CMUcam4::sendFrame().
*******************************************************************************/
#define CMUCAM4_HR_80                       3

/***************************************************************************//**
* This is a convenient macro for specifying a vertical resolution of 480 pixels
* to for CMUcam4::dumpFrame() and CMUcam4::sendFrame().
*******************************************************************************/
#define CMUCAM4_VR_480                      0

/***************************************************************************//**
* This is a convenient macro for specifying a vertical resolution of 240 pixels
* to for CMUcam4::dumpFrame() and CMUcam4::sendFrame().
*******************************************************************************/
#define CMUCAM4_VR_240                      1

/***************************************************************************//**
* This is a convenient macro for specifying a vertical resolution of 120 pixels
* to for CMUcam4::dumpFrame() and CMUcam4::sendFrame().
*******************************************************************************/
#define CMUCAM4_VR_120                      2

/***************************************************************************//**
* This is a convenient macro for specifying a vertical resolution of 60 pixels
* to for CMUcam4::dumpFrame() and CMUcam4::sendFrame().
*******************************************************************************/
#define CMUCAM4_VR_60                       3

/***************************************************************************//**
* The number of rows of bytes in the binary bitmap.
*******************************************************************************/
#define CMUCAM4_ID_T_R                      CMUCAM4_BINARY_V_RES

/***************************************************************************//**
* The number of columns of bytes in the binary bitmap.
*******************************************************************************/
#define CMUCAM4_ID_T_C                      (CMUCAM4_BINARY_H_RES / 8)

/***************************************************************************//**
* The number of bytes in the binary bitmap.
* @see CMUcam4_image_data_t
*******************************************************************************/
#define CMUCAM4_ID_T_LENGTH                 (CMUCAM4_ID_T_R * CMUCAM4_ID_T_C)

/***************************************************************************//**
* 1-bin histogram data structure length in bytes.
* @see CMUcam4_histogram_data_1_t
*******************************************************************************/
#define CMUCAM4_HD_1_T_LENGTH               (1 << CMUCAM4_H1_BINS)

/***************************************************************************//**
* 2-bin histogram data structure length in bytes.
* @see CMUcam4_histogram_data_2_t
*******************************************************************************/
#define CMUCAM4_HD_2_T_LENGTH               (1 << CMUCAM4_H2_BINS)

/***************************************************************************//**
* 4-bin histogram data structure length in bytes.
* @see CMUcam4_histogram_data_4_t
*******************************************************************************/
#define CMUCAM4_HD_4_T_LENGTH               (1 << CMUCAM4_H4_BINS)

/***************************************************************************//**
* 8-bin histogram data structure length in bytes.
* @see CMUcam4_histogram_data_8_t
*******************************************************************************/
#define CMUCAM4_HD_8_T_LENGTH               (1 << CMUCAM4_H8_BINS)

/***************************************************************************//**
* 16-bin histogram data structure length in bytes.
* @see CMUcam4_histogram_data_16_t
*******************************************************************************/
#define CMUCAM4_HD_16_T_LENGTH              (1 << CMUCAM4_H16_BINS)

/***************************************************************************//**
* 32-bin histogram data structure length in bytes.
* @see CMUcam4_histogram_data_32_t
*******************************************************************************/
#define CMUCAM4_HD_32_T_LENGTH              (1 << CMUCAM4_H32_BINS)

/***************************************************************************//**
* 64-bin histogram data structure length in bytes.
* @see CMUcam4_histogram_data_64_t
*******************************************************************************/
#define CMUCAM4_HD_64_T_LENGTH              (1 << CMUCAM4_H64_BINS)

/***************************************************************************//**
* Partition volume label string length in numerical form.
*******************************************************************************/
#define CMUCAM4_VL_LENGTH                   11

/***************************************************************************//**
* File system type string length in numerical form.
*******************************************************************************/
#define CMUCAM4_FST_LENGTH                  8

/***************************************************************************//**
* Directory entry name string length in numerical form.
*******************************************************************************/
#define CMUCAM4_NAME_LENGTH                 12

/***************************************************************************//**
* Directory entry attribute string length in numerical form.
*******************************************************************************/
#define CMUCAM4_ATTR_LENGTH                 6

/***************************************************************************//**
* The operation was successful. Non-negative values are usually successes.
*******************************************************************************/
#define CMUCAM4_RETURN_SUCCESS              0

/***************************************************************************//**
* The operation was unsuccessful. This is usually caused by a NULL pointer.
*******************************************************************************/
#define CMUCAM4_RETURN_FAILURE              -1

/***************************************************************************//**
* The %CMUcam4 and the interface library have not been initialized yet.
*******************************************************************************/
#define CMUCAM4_NOT_ACTIVATED               -2

/***************************************************************************//**
* The %CMUcam4 responded to the command with a NCK.
*******************************************************************************/
#define CMUCAM4_NCK_RESPONCE                -3

/***************************************************************************//**
* The %CMUcam4 firmware version is unknown and unsupported.
*******************************************************************************/
#define CMUCAM4_UNSUPPORTED_VERSION         -4

/***************************************************************************//**
* The %CMUcam4 responded to the command with something unexpected.
*******************************************************************************/
#define CMUCAM4_UNEXPECTED_RESPONCE         -5

/***************************************************************************//**
* The command buffer overflowed. This error is usually caused by passing
* strings that are too large to the interface library.
*******************************************************************************/
#define CMUCAM4_COMMAND_OVERFLOW            -6

/***************************************************************************//**
* The responce buffer overflowed. This error is usually caused by not handling
* communication with the %CMUcam4 fast enough. In particular, whenever the
* %CMUcam4 sends a large amount of binary data this can cause the responce
* buffer to overflow if not handled quickly.
*******************************************************************************/
#define CMUCAM4_RESPONCE_OVERFLOW           -7

/***************************************************************************//**
* The %CMUcam4 is not streaming data packets.
*******************************************************************************/
#define CMUCAM4_STREAM_END                  -8

/***************************************************************************//**
* The %CMUcam4 is not responding.
*******************************************************************************/
#define CMUCAM4_SERIAL_TIMEOUT              -9

/***************************************************************************//**
* The %CMUcam4 is having problems with the camera module's parallel data bus.
*******************************************************************************/
#define CMUCAM4_CAMERA_TIMEOUT_ERROR        -10

/***************************************************************************//**
* The %CMUcam4 is having problems with the camera module's serial data bus.
*******************************************************************************/
#define CMUCAM4_CAMERA_CONNECTION_ERROR     -11

/***************************************************************************//**
* An error occured communicating with the micro secure digital card.
*******************************************************************************/
#define CMUCAM4_DISK_IO_ERROR               -12

/***************************************************************************//**
* The file system is corrupted. Try non-quickly reformatting the file system.
*******************************************************************************/
#define CMUCAM4_FILE_SYSTEM_CORRUPTED       -13

/***************************************************************************//**
* The file system is unsupported. Try non-quickly reformatting the file system.
*******************************************************************************/
#define CMUCAM4_FILE_SYSTEM_UNSUPPORTED     -14

/***************************************************************************//**
* A micro secure digital card was not found.
*******************************************************************************/
#define CMUCAM4_CARD_NOT_DETECTED           -15

/***************************************************************************//**
* The micro secure digital card may be full. The error is "may be" because of
* the time it would take to verify the if disk was completely full.
*******************************************************************************/
#define CMUCAM4_DISK_MAY_BE_FULL            -16

/***************************************************************************//**
* The directory entry slots in the current directory are completely full. Some
* directory entries may use multiple directory entry slots.
*******************************************************************************/
#define CMUCAM4_DIRECTORY_FULL              -17

/***************************************************************************//**
* The target name of a file system path string is malformed.
*******************************************************************************/
#define CMUCAM4_EXPECTED_AN_ENTRY           -18

/***************************************************************************//**
* A directory name in a file system path string is malformed.
*******************************************************************************/
#define CMUCAM4_EXPECTED_A_DIRECTORY        -19

/***************************************************************************//**
* The target name of the file system path string cannot be accessed.
*******************************************************************************/
#define CMUCAM4_ENTRY_NOT_ACCESSIBLE        -20

/***************************************************************************//**
* The target name of the file system path string cannot be modified.
*******************************************************************************/
#define CMUCAM4_ENTRY_NOT_MODIFIABLE        -21

/***************************************************************************//**
* The target or a directory in a file system path string cannot be found.
*******************************************************************************/
#define CMUCAM4_ENTRY_NOT_FOUND             -22

/***************************************************************************//**
* The file or directory already exists.
*******************************************************************************/
#define CMUCAM4_ENTRY_ALREADY_EXISTS        -23

/***************************************************************************//**
* An error occured trying to move a folder.
*******************************************************************************/
#define CMUCAM4_DIRECTORY_LINK_MISSING      -24

/***************************************************************************//**
* Directories must be empty first to be deleted.
*******************************************************************************/
#define CMUCAM4_DIRECTORY_NOT_EMPTY         -25

/***************************************************************************//**
* The target of a directory only operation is not a directory.
*******************************************************************************/
#define CMUCAM4_NOT_A_DIRECTORY             -26

/***************************************************************************//**
* The target of a file only operation is not a file.
*******************************************************************************/
#define CMUCAM4_NOT_A_FILE                  -27

/**@cond CMUCAM4_PRIVATE*******************************************************/

/***************************************************************************//**
* Partition volume label string length in string form.
*******************************************************************************/
#define CMUCAM4_VL_LENGTH_STR               CMUCOM4_V_TO_S(CMUCAM4_VL_LENGTH)

/***************************************************************************//**
* File system type string length in string form.
*******************************************************************************/
#define CMUCAM4_FST_LENGTH_STR              CMUCOM4_V_TO_S(CMUCAM4_FST_LENGTH)

/***************************************************************************//**
* Directory entry name string length in string form.
*******************************************************************************/
#define CMUCAM4_NAME_LENGTH_STR             CMUCOM4_V_TO_S(CMUCAM4_NAME_LENGTH)

/***************************************************************************//**
* Directory entry attribute string length in string form.
*******************************************************************************/
#define CMUCAM4_ATTR_LENGTH_STR             CMUCOM4_V_TO_S(CMUCAM4_ATTR_LENGTH)

/***************************************************************************//**
* Error string checksum of the error string "Camera Timeout Error".
*******************************************************************************/
#define CMUCAM4_CAMERA_TIMEOUT_ERROR_SUM    ('E'+'R'+'R'+':'+' '+\
'C'+'a'+'m'+'e'+'r'+'a'+' '+\
'T'+'i'+'m'+'e'+'o'+'u'+'t'+' '+\
'E'+'r'+'r'+'o'+'r')

/***************************************************************************//**
* Error string checksum of the error string "Camera Connection Error".
*******************************************************************************/
#define CMUCAM4_CAMERA_CONNECTION_ERROR_SUM ('E'+'R'+'R'+':'+' '+\
'C'+'a'+'m'+'e'+'r'+'a'+' '+\
'C'+'o'+'n'+'n'+'e'+'c'+'t'+'i'+'o'+'n'+' '+\
'E'+'r'+'r'+'o'+'r')

/***************************************************************************//**
* Error string checksum of the error string "Disk IO Error".
*******************************************************************************/
#define CMUCAM4_DISK_IO_ERROR_SUM           ('E'+'R'+'R'+':'+' '+\
'D'+'i'+'s'+'k'+' '+\
'I'+'O'+' '+\
'E'+'r'+'r'+'o'+'r')

/***************************************************************************//**
* Error string checksum of the error string "File System Corrupted".
*******************************************************************************/
#define CMUCAM4_FILE_SYSTEM_CORRUPTED_SUM   ('E'+'R'+'R'+':'+' '+\
'F'+'i'+'l'+'e'+' '+\
'S'+'y'+'s'+'t'+'e'+'m'+' '+\
'C'+'o'+'r'+'r'+'u'+'p'+'t'+'e'+'d')

/***************************************************************************//**
* Error string checksum of the error string "File System Unsupported".
*******************************************************************************/
#define CMUCAM4_FILE_SYSTEM_UNSUPPORTED_SUM ('E'+'R'+'R'+':'+' '+\
'F'+'i'+'l'+'e'+' '+\
'S'+'y'+'s'+'t'+'e'+'m'+' '+\
'U'+'n'+'s'+'u'+'p'+'p'+'o'+'r'+'t'+'e'+'d')

/***************************************************************************//**
* Error string checksum of the error string "Card Not Detected".
*******************************************************************************/
#define CMUCAM4_CARD_NOT_DETECTED_SUM       ('E'+'R'+'R'+':'+' '+\
'C'+'a'+'r'+'d'+' '+\
'N'+'o'+'t'+' '+\
'D'+'e'+'t'+'e'+'c'+'t'+'e'+'d')

/***************************************************************************//**
* Error string checksum of the error string "Disk May Be Full".
*******************************************************************************/
#define CMUCAM4_DISK_MAY_BE_FULL_SUM        ('E'+'R'+'R'+':'+' '+\
'D'+'i'+'s'+'k'+' '+\
'M'+'a'+'y'+' '+\
'B'+'e'+' '+\
'F'+'u'+'l'+'l')

/***************************************************************************//**
* Error string checksum of the error string "Directory Full Sum".
*******************************************************************************/
#define CMUCAM4_DIRECTORY_FULL_SUM          ('E'+'R'+'R'+':'+' '+\
'D'+'i'+'r'+'e'+'c'+'t'+'o'+'r'+'y'+' '+\
'F'+'u'+'l'+'l')

/***************************************************************************//**
* Error string checksum of the error string "Expected An Entry".
*******************************************************************************/
#define CMUCAM4_EXPECTED_AN_ENTRY_SUM       ('E'+'R'+'R'+':'+' '+\
'E'+'x'+'p'+'e'+'c'+'t'+'e'+'d'+' '+\
'A'+'n'+' '+\
'E'+'n'+'t'+'r'+'y')

/***************************************************************************//**
* Error string checksum of the error string "Expected A Directory".
*******************************************************************************/
#define CMUCAM4_EXPECTED_A_DIRECTORY_SUM    ('E'+'R'+'R'+':'+' '+\
'E'+'x'+'p'+'e'+'c'+'t'+'e'+'d'+' '+\
'A'+' '+\
'D'+'i'+'r'+'e'+'c'+'t'+'o'+'r'+'y')

/***************************************************************************//**
* Error string checksum of the error string "Entry Not Accessible".
*******************************************************************************/
#define CMUCAM4_ENTRY_NOT_ACCESSIBLE_SUM    ('E'+'R'+'R'+':'+' '+\
'E'+'n'+'t'+'r'+'y'+' '+\
'N'+'o'+'t'+' '+\
'A'+'c'+'c'+'e'+'s'+'s'+'i'+'b'+'l'+'e')

/***************************************************************************//**
* Error string checksum of the error string "Entry Not Modifiable".
*******************************************************************************/
#define CMUCAM4_ENTRY_NOT_MODIFIABLE_SUM    ('E'+'R'+'R'+':'+' '+\
'E'+'n'+'t'+'r'+'y'+' '+\
'N'+'o'+'t'+' '+\
'M'+'o'+'d'+'i'+'f'+'i'+'a'+'b'+'l'+'e')

/***************************************************************************//**
* Error string checksum of the error string "Entry Not Found".
*******************************************************************************/
#define CMUCAM4_ENTRY_NOT_FOUND_SUM         ('E'+'R'+'R'+':'+' '+\
'E'+'n'+'t'+'r'+'y'+' '+\
'N'+'o'+'t'+' '+\
'F'+'o'+'u'+'n'+'d')

/***************************************************************************//**
* Error string checksum of the error string "Entry Already Exists".
*******************************************************************************/
#define CMUCAM4_ENTRY_ALREADY_EXISTS_SUM    ('E'+'R'+'R'+':'+' '+\
'E'+'n'+'t'+'r'+'y'+' '+\
'A'+'l'+'r'+'e'+'a'+'d'+'y'+' '+\
'E'+'x'+'i'+'s'+'t'+'s')

/***************************************************************************//**
* Error string checksum of the error string "Directory Link Missing".
*******************************************************************************/
#define CMUCAM4_DIRECTORY_LINK_MISSING_SUM  ('E'+'R'+'R'+':'+' '+\
'D'+'i'+'r'+'e'+'c'+'t'+'o'+'r'+'y'+' '+\
'L'+'i'+'n'+'k'+' '+\
'M'+'i'+'s'+'s'+'i'+'n'+'g')

/***************************************************************************//**
* Error string checksum of the error string "Directory Not Empty".
*******************************************************************************/
#define CMUCAM4_DIRECTORY_NOT_EMPTY_SUM     ('E'+'R'+'R'+':'+' '+\
'D'+'i'+'r'+'e'+'c'+'t'+'o'+'r'+'y'+' '+\
'N'+'o'+'t'+' '+\
'E'+'m'+'p'+'t'+'y')

/***************************************************************************//**
* Error string checksum of the error string "Not A Directory".
*******************************************************************************/
#define CMUCAM4_NOT_A_DIRECTORY_SUM         ('E'+'R'+'R'+':'+' '+\
'N'+'o'+'t'+' '+\
'A'+' '+\
'D'+'i'+'r'+'e'+'c'+'t'+'o'+'r'+'y')

/***************************************************************************//**
* Error string checksum of the error string "Not A File".
*******************************************************************************/
#define CMUCAM4_NOT_A_FILE_SUM              ('E'+'R'+'R'+':'+' '+\
'N'+'o'+'t'+' '+\
'A'+' '+\
'F'+'i'+'l'+'e')

/***************************************************************************//**
* Responce (input) storage buffer size alias.
*******************************************************************************/
#define CMUCAM4_RES_BUFFER_SIZE CMUCOM4_INPUT_BUFFER_SIZE
#if (CMUCAM4_RES_BUFFER_SIZE < 1) // Responce buffer size limit.
#error "Error: The responce (input) buffer size is too small!"
#endif

/***************************************************************************//**
* Command (output) storage buffer size alias.
*******************************************************************************/
#define CMUCAM4_CMD_BUFFER_SIZE CMUCOM4_OUTPUT_BUFFER_SIZE
#if (CMUCAM4_CMD_BUFFER_SIZE < 1) // Command buffer size limit.
#error "Error: The command (output) buffer size is too small!"
#endif

/***************************************************************************//**
* Number of CMUcam4::begin() reset tries.
*******************************************************************************/
#define CMUCAM4_RESET_TRIES                 4

/***************************************************************************//**
* Number of milliseconds to wait for CMUcam4::begin() reset tries.
*******************************************************************************/
#define CMUCAM4_RESET_TIMEOUT               2250

/***************************************************************************//**
* Number of CMUcam4::idleCamera() idle tries.
*******************************************************************************/
#define CMUCAM4_IDLE_TRIES                  4

/***************************************************************************//**
* Number of milliseconds to wait for CMUcam4::idleCamera() idle tries.
*******************************************************************************/
#define CMUCAM4_IDLE_TIMEOUT                750

/***************************************************************************//**
* Non-file system related operations timeout in milliseconds.
*******************************************************************************/
#define CMUCAM4_NON_FS_TIMEOUT              1000

/***************************************************************************//**
* File system related operations timeout in milliseconds.
*******************************************************************************/
#define CMUCAM4_FS_TIMEOUT                  3600000

/***************************************************************************//**
* The CMUcam4::idleCamera() shift register comparison string.
*******************************************************************************/
#define CMUCAM4_IC_STRING                   ":ACK\rCMUcam4 v%d.%02d\r:"

/***************************************************************************//**
* The CMUcam4::idleCamera() shift register comparison length.
*******************************************************************************/
#define CMUCAM4_IC_LENGTH                   sizeof(CMUCAM4_IC_STRING)

/***************************************************************************//**
* The frame horizontal resolution.
*******************************************************************************/
#define CMUCAM4_FRAME_H_RES                 640

/***************************************************************************//**
* The native vertical resolution.
*******************************************************************************/
#define CMUCAM4_FRAME_V_RES                 480

/**@endcond********************************************************************/

/***************************************************************************//**
* %CMUcam4 tracking parameters structure.
* @see CMUcam4::getTrackingParameters()
*******************************************************************************/
typedef struct CMUcam4_tracking_parameters_t
{
    int redMin; ///< The lower red or V threshold value - [0 : 255].
    int redMax; ///< The upper red or V threshold value - [0 : 255].
    int greenMin; ///< The lower green or Y threshold value - [0 : 255].
    int greenMax; ///< The upper green or Y threshold value - [0 : 255].
    int blueMin; ///< The lower blue or U threshold value - [0 : 255].
    int blueMax; ///< The upper blue or U threshold value - [0 : 255].
}
CMUcam4_tracking_parameters_t;

/***************************************************************************//**
* %CMUcam4 tracking window structure.
* @see CMUcam4::getTrackingWindow()
*******************************************************************************/
typedef struct CMUcam4_tracking_window_t
{
    int topLeftX; ///< The X1 tracking window coordinate - [0 : 159].
    int topLeftY; ///< The Y1 tracking window coordinate - [0 : 119].
    int bottomRightX; ///< The X2 tracking window coordinate - [X1 : 159].
    int bottomRightY; ///< The Y2 tracking window coordinate - [Y1 : 119].
}
CMUcam4_tracking_window_t;

/***************************************************************************//**
* %CMUcam4 binary bitmap structure. The binary bitmap is 80x60 pixels where
* each pixel is one bit. A 0 bit represents an untracked pixel. A 1 bit
* represents a tracked pixel. Basic frame differencing can be accomplished by
* post processing the binary bitmap among other interesting things. The binary
* bitmap has 4,800 pixels contained in 600 bytes. The MSB of every byte is the
* left most pixel while the LSB is the right most pixel.
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
* @see CMUcam4::getPixel()
* @see CMUcam4::setPixel()
*******************************************************************************/
typedef struct CMUcam4_image_data_t
{
    uint8_t pixels[CMUCAM4_ID_T_LENGTH]; ///< The binary bitmap array.
}
CMUcam4_image_data_t;

/***************************************************************************//**
* %CMUcam4 1-bin histogram structure. The sum of all the bins is less than or
* equal to 255. Each bin contains the percentage (0 to 255) of pixels in the
* image that fell within that bin. E.g. Each bin in a 1-bin histogram
* represents 256 pixel values.
* @par For example:
* <tt>{ bin0[0 - 255] }</tt>
* @see CMUcam4::getHistogram(int channel, int bins)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_1_t * pointer)
*******************************************************************************/
typedef struct CMUcam4_histogram_data_1_t
{
    uint8_t bins[CMUCAM4_HD_1_T_LENGTH]; ///< 1-bin histogram array.
}
CMUcam4_histogram_data_1_t;

/***************************************************************************//**
* %CMUcam4 2-bin histogram structure. The sum of all the bins is less than or
* equal to 255. Each bin contains the percentage (0 to 255) of pixels in the
* image that fell within that bin. E.g. Each bin in a 2-bin histogram
* represents 128 pixel values.
* @par For example:
* <tt>{ bin0[0 - 127], bin1[128 - 255] }</tt>
* @see CMUcam4::getHistogram(int channel, int bins)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_2_t * pointer)
*******************************************************************************/
typedef struct CMUcam4_histogram_data_2_t
{
    uint8_t bins[CMUCAM4_HD_2_T_LENGTH]; ///< 2-bin histogram array.
}
CMUcam4_histogram_data_2_t;

/***************************************************************************//**
* %CMUcam4 4-bin histogram structure. The sum of all the bins is less than or
* equal to 255. Each bin contains the percentage (0 to 255) of pixels in the
* image that fell within that bin. E.g. Each bin in a 4-bin histogram
* represents 64 pixel values.
* @par For example:
* <tt>{ bin0[0 - 63], bin1[64 - 127], bin2[128 - 191], bin3[192 - 255] }</tt>
* @see CMUcam4::getHistogram(int channel, int bins)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_4_t * pointer)
*******************************************************************************/
typedef struct CMUcam4_histogram_data_4_t
{
    uint8_t bins[CMUCAM4_HD_4_T_LENGTH]; ///< 4-bin histogram array.
}
CMUcam4_histogram_data_4_t;

/***************************************************************************//**
* %CMUcam4 8-bin histogram structure. The sum of all the bins is less than or
* equal to 255. Each bin contains the percentage (0 to 255) of pixels in the
* image that fell within that bin. E.g. Each bin in a 8-bin histogram
* represents 32 pixel values.
* @par For example:
* <tt>{ bin0[0 - 31], bin1[32 - 63], ..., bin7[224 - 255] }</tt>
* @see CMUcam4::getHistogram(int channel, int bins)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_8_t * pointer)
*******************************************************************************/
typedef struct CMUcam4_histogram_data_8_t
{
    uint8_t bins[CMUCAM4_HD_8_T_LENGTH]; ///< 8-bin histogram array.
}
CMUcam4_histogram_data_8_t;

/***************************************************************************//**
* %CMUcam4 16-bin histogram structure. The sum of all the bins is less than or
* equal to 255. Each bin contains the percentage (0 to 255) of pixels in the
* image that fell within that bin. E.g. Each bin in a 16-bin histogram
* represents 16 pixel values.
* @par For example:
* <tt>{ bin0[0 - 15], bin1[16 - 31], ..., bin15[240 - 255] }</tt>
* @see CMUcam4::getHistogram(int channel, int bins)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_16_t * pointer)
*******************************************************************************/
typedef struct CMUcam4_histogram_data_16_t
{
    uint8_t bins[CMUCAM4_HD_16_T_LENGTH]; ///< 16-bin histogram array.
}
CMUcam4_histogram_data_16_t;

/***************************************************************************//**
* %CMUcam4 32-bin histogram structure. The sum of all the bins is less than or
* equal to 255. Each bin contains the percentage (0 to 255) of pixels in the
* image that fell within that bin. E.g. Each bin in a 32-bin histogram
* represents 8 pixel values.
* @par For example:
* <tt>{ bin0[0 - 7], bin1[8 - 15], ..., bin31[248 - 255] }</tt>
* @see CMUcam4::getHistogram(int channel, int bins)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_32_t * pointer)
*******************************************************************************/
typedef struct CMUcam4_histogram_data_32_t
{
    uint8_t bins[CMUCAM4_HD_32_T_LENGTH]; ///< 32-bin histogram array.
}
CMUcam4_histogram_data_32_t;

/***************************************************************************//**
* %CMUcam4 64-bin histogram structure. The sum of all the bins is less than or
* equal to 255. Each bin contains the percentage (0 to 255) of pixels in the
* image that fell within that bin. E.g. Each bin in a 64-bin histogram
* represents 4 pixel values.
* @par For example:
* <tt>{ bin0[0 - 3], bin1[4 - 7], ..., bin63[252 - 255] }</tt>
* @see CMUcam4::getHistogram(int channel, int bins)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_64_t * pointer)
*******************************************************************************/
typedef struct CMUcam4_histogram_data_64_t
{
    uint8_t bins[CMUCAM4_HD_64_T_LENGTH]; ///< 64-bin histogram array.
}
CMUcam4_histogram_data_64_t;

/***************************************************************************//**
* %CMUcam4 statistics data structure. Please note that the distribution of each
* color channel is not normal.
* @see CMUcam4::getMean()
* @see CMUcam4::getTypeSDataPacket()
*******************************************************************************/
typedef struct CMUcam4_statistics_data_t
{
    int RMean; ///< Red or V channel mean - [0 : 255].
    int GMean; ///< Green or Y channel mean - [0 : 255].
    int BMean; ///< Blue or U channel mean - [0 : 255].
    int RMedian; ///< Red or V channel median - [0 : 255].
    int GMedian; ///< Green or Y channel median - [0 : 255].
    int BMedian; ///< Blue or U channel median - [0 : 255].
    int RMode; ///< Red or V channel mode - [0 : 255].
    int GMode; ///< Green or Y channel mode - [0 : 255].
    int BMode; ///< Blue or U channel mode - [0 : 255].
    int RStDev; ///< Red or V channel standard deviation - [0 : 255].
    int GStDev; ///< Green or Y channel standard deviation - [0 : 255].
    int BStDev; ///< Blue or U channel standard deviation - [0 : 255].
}
CMUcam4_statistics_data_t;

/***************************************************************************//**
* %CMUcam4 tracking data structure. The middle mass is also called the centroid
* and is the average position of all the tracked pixels in the image. The
* bounding box surrounds all the tracked pixels in the image. The @c pixels
* value can be used to infer the size or distance of the color blob of tracked
* pixels - the higher the @c pixels then the larger or closer the color blob.
* The @c confidence value can be used to infer the density or quality of the
* lock on the target color blob of tracked pixels - the higher the @c
* confidence then the better the lock on the color blob.
* @see CMUcam4::trackColor()
* @see CMUcam4::trackWindow()
* @see CMUcam4::getTypeTDataPacket()
*******************************************************************************/
typedef struct CMUcam4_tracking_data_t
{
    int mx; ///< The middle mass X position of the tracked pixels - [0:159].
    int my; ///< The middle mass Y position of the tracked pixels - [0:119].
    int x1; ///< The upper left X coordinate of the bounding box - [0:159].
    int y1; ///< The upper left Y coordinate of the bounding box - [0:119].
    int x2; ///< The bottom right X coordinate of the bounding box - [X1:159].
    int y2; ///< The bottom right Y coordinate of the bounding box - [Y1:119].
    int pixels; ///< The percentage of tracked pixels in the image - [0:255].
    int confidence; ///< The density of tracked pixels in the image - [0:255].
}
CMUcam4_tracking_data_t;

/***************************************************************************//**
* Disk information data structure. @par Member relationships:
* <tt>diskSize = bytesPerSector * countOfDataSectors</tt>
* @n <tt>countOfDataSectors = sectorsPerCluster * countOfClusters</tt>
* @see CMUcam4::diskInformation()
*******************************************************************************/
typedef struct CMUcam4_disk_information_t
{
    unsigned long diskSignature; ///< 32-bit disk signature.
    unsigned long volumeIdentification; ///< 32-bit volume identification.
    unsigned long countOfDataSectors; ///< Count of data sectors.
    unsigned long bytesPerSector; ///< Bytes per sector.
    unsigned long sectorsPerCluster; ///< Sectors per cluster.
    unsigned long countOfClusters; ///< Count of clusters.
    /// Volume label string.
    char volumeLabel[CMUCAM4_VL_LENGTH + 1];
    /// File system type string. Either "FAT 16" or "FAT 32".
    char fileSystemType[CMUCAM4_FST_LENGTH + 1];
}
CMUcam4_disk_information_t;

/***************************************************************************//**
* Disk space data structure. @par Member relationships:
* <tt>diskSize = bytesPerSector * (freeSectorCount + usedSectorCount)</tt>
* @n <tt>countOfDataSectors = (freeSectorCount + usedSectorCount)</tt>
* @see CMUcam4::diskSpace()
*******************************************************************************/
typedef struct CMUcam4_disk_space_t
{
    unsigned long freeSectorCount; ///< Count of free sectors.
    unsigned long usedSectorCount; ///< Count of used sectors.
}
CMUcam4_disk_space_t;

/***************************************************************************//**
* File or directory attributes data structure. The
* CMUcam4_entry_attributes_t data structure is the
* CMUcam4_directory_entry_t.attributes string.
* @see CMUcam4::listDirectory()
* @see CMUcam4::isReadOnly()
* @see CMUcam4::isHidden()
* @see CMUcam4::isSystem()
* @see CMUcam4::isVolumeID()
* @see CMUcam4::isDirectory()
* @see CMUcam4::isArchive()
*******************************************************************************/
typedef struct CMUcam4_entry_attributes_t
{
    char readOnly; ///< Will be either 'R' or '_'.
    char hidden; ///< Will be either 'H' or '_'.
    char system; ///< Will be either 'S' or '_'.
    char volumeID; ///< Constant value of '_'.
    char directory; ///< Will be either 'D' or '_'.
    char archive; ///< Will be either 'A' or '_'.
    char nullTerminator; ///< Constant value of '\0'.
}
CMUcam4_entry_attributes_t;

/***************************************************************************//**
* File or directory entry data structure. The
* CMUcam4_directory_entry_t.attributes string is the CMUcam4_entry_attributes_t
* data structure.
* @see CMUcam4::listDirectory()
* @see CMUcam4::isReadOnly()
* @see CMUcam4::isHidden()
* @see CMUcam4::isSystem()
* @see CMUcam4::isVolumeID()
* @see CMUcam4::isDirectory()
* @see CMUcam4::isArchive()
*******************************************************************************/
typedef struct CMUcam4_directory_entry_t
{
    unsigned long size; ///< Entry size in bytes. Between 0 and 2,147,483,647.
    char name[CMUCAM4_NAME_LENGTH + 1]; ///< Entry name.
    char attributes[CMUCAM4_ATTR_LENGTH + 1]; ///< Entry attributes.
}
CMUcam4_directory_entry_t;

/***************************************************************************//**
* The %CMUcam4 class implements a generic C++ interface library for the
* %CMUcam4. This interface library provides a wrapper function for all native
* %CMUcam4 functions. Addtionally, this interface library has built in utility
* functions for manipulating data structures and low level camera settings.
*******************************************************************************/
class CMUcam4
{

public:

/***************************************************************************//**
* Initialize the %CMUcam4 object to use the default Serial port.
*******************************************************************************/
CMUcam4();

/***************************************************************************//**
* Initialize the %CMUcam4 object to use the @c port Serial port.
* @param [in] port The port.
* @see CMUCOM4_SERIAL
* @see CMUCOM4_SERIAL1
* @see CMUCOM4_SERIAL2
* @see CMUCOM4_SERIAL3
*******************************************************************************/
CMUcam4(int port);

/***************************************************************************//**
* Gets a pixel (0 or 1) from a binary bitmap data structure.
* @param [in] pointer A pointer to the binary bitmap data structure.
* @param [in] row The pixel row. Must be between 0 and 59.
* @param [in] column The pixel column. Must be between 0 and 79.
* @return The pixel value on success and a negative error value on failure.
* @see CMUcam4_image_data_t
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
* @see CMUCAM4_GET_PIXEL()
*******************************************************************************/
int getPixel(CMUcam4_image_data_t * pointer, int row, int column);

/***************************************************************************//**
* Gets a pixel (0 or 1) from a binary bitmap data structure. This function
* macro is included for speed and not safety. Please use it carefully!
* @param [in] pointer A pointer to the binary bitmap data structure.
* @param [in] row The pixel row. Must be between 0 and 59.
* @param [in] column The pixel column. Must be between 0 and 79.
* @return The pixel value (0 or 1).
* @see CMUcam4_image_data_t
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
* @see CMUcam4::getPixel()
*******************************************************************************/
#define CMUCAM4_GET_PIXEL(pointer, row, column) \
\
(((pointer)->pixels[((row) * CMUCAM4_ID_T_C) + ((column) / 8)] \
>> (7 - ((column) & 7))) & 1)

/***************************************************************************//**
* Sets a pixel (0 or 1) in a binary bitmap data structure.
* @param [out] pointer A pointer to the binary bitmap data structure.
* @param [in] row The pixel row. Must be between 0 and 59.
* @param [in] column The pixel column. Must be between 0 and 79.
* @param [in] value The pixel value (0 or 1).
* @return 0 on success and a negative error value on failure.
* @see CMUcam4_image_data_t
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
* @see CMUCAM4_SET_PIXEL()
*******************************************************************************/
int setPixel(CMUcam4_image_data_t * pointer, int row, int column, int value);

/***************************************************************************//**
* Sets a pixel (0 or 1) to a binary bitmap data structure. This function
* macro is included for speed and not safety. Please use it carefully!
* @param [out] pointer A pointer to the binary bitmap data structure.
* @param [in] row The pixel row. Must be between 0 and 59.
* @param [in] column The pixel column. Must be between 0 and 79.
* @param [in] value The pixel value (0 or 1).
* @see CMUcam4_image_data_t
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
* @see CMUcam4::setPixel()
*******************************************************************************/
#define CMUCAM4_SET_PIXEL(pointer, row, column, value) \
do { \
int bitIndex = (7 - ((column) & 7)); \
int byteIndex = (((row) * CMUCAM4_ID_T_C) + ((column) / 8)); \
\
(pointer)->pixels[byteIndex] = \
(((~(1<<bitIndex))&((pointer)->pixels[byteIndex]))|(((value)?1:0)<<bitIndex)) \
; } while(false)

/***************************************************************************//**
* Logically AND's two binary bitmaps together - useful for image manipulation.
* @param [out] destination A pointer to a binary bitmap data structure.
* @param [in] source0 A pointer to a binary bitmap data structure.
* @param [in] source1 A pointer to a binary bitmap data structure.
* @return Zero on success and a negative value if any parameter is invalid.
* @see CMUcam4_image_data_t
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
*******************************************************************************/
int andPixels(CMUcam4_image_data_t * destination,
              CMUcam4_image_data_t * source0,
              CMUcam4_image_data_t * source1);

/***************************************************************************//**
* Logically OR's two binary bitmaps together - useful for image manipulation.
* @param [out] destination A pointer to a binary bitmap data structure.
* @param [in] source0 A pointer to a binary bitmap data structure.
* @param [in] source1 A pointer to a binary bitmap data structure.
* @return Zero on success and a negative value if any parameter is invalid.
* @see CMUcam4_image_data_t
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
*******************************************************************************/
int orPixels(CMUcam4_image_data_t * destination,
             CMUcam4_image_data_t * source0,
             CMUcam4_image_data_t * source1);

/***************************************************************************//**
* Logically XOR's two binary bitmaps together - useful for image manipulation.
* @param [out] destination A pointer to a binary bitmap data structure.
* @param [in] source0 A pointer to a binary bitmap data structure.
* @param [in] source1 A pointer to a binary bitmap data structure.
* @return Zero on success and a negative value if any parameter is invalid.
* @see CMUcam4_image_data_t
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
*******************************************************************************/
int xorPixels(CMUcam4_image_data_t * destination,
              CMUcam4_image_data_t * source0,
              CMUcam4_image_data_t * source1);

/***************************************************************************//**
* Logically NOT's a binary bitmap - useful for image manipulation.
* @param [in, out] destination A pointer to a binary bitmap data structure.
* @return Zero on success and a negative value if any parameter is invalid.
* @see CMUcam4_image_data_t
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4::sendBitmap()
*******************************************************************************/
int notPixels(CMUcam4_image_data_t * destination);

/***************************************************************************//**
* Returns if a directory entry is read-only (0 or 1).
* @param [in] pointer A pointer to a directory entry data structure.
* @return 1 if read-only, 0 if not, and a negative value if @c pointer is NULL.
* @see CMUcam4_directory_entry_t
* @see CMUcam4::listDirectory()
*******************************************************************************/
int isReadOnly(CMUcam4_directory_entry_t * pointer);

/***************************************************************************//**
* Returns if a directory entry is hidden (0 or 1).
* @param [in] pointer A pointer to a directory entry data structure.
* @return 1 if hidden, 0 if not, and a negative value if @c pointer is NULL.
* @see CMUcam4_directory_entry_t
* @see CMUcam4::listDirectory()
*******************************************************************************/
int isHidden(CMUcam4_directory_entry_t * pointer);

/***************************************************************************//**
* Returns if a directory entry is system (0 or 1).
* @param [in] pointer A pointer to a directory entry data structure.
* @return 1 if system, 0 if not, and a negative value if @c pointer is NULL.
* @see CMUcam4_directory_entry_t
* @see CMUcam4::listDirectory()
*******************************************************************************/
int isSystem(CMUcam4_directory_entry_t * pointer);

/***************************************************************************//**
* Returns if a directory entry is the volume ID (0 or 1). This function is
* included for completeness only. The %CMUcam4's %listDirectory function does
* not list the volume ID located in the root directory.
* @param [in] pointer A pointer to a directory entry data structure.
* @return 1 if ID, 0 if not, and a negative value if @c pointer is NULL.
* @see CMUcam4_directory_entry_t
* @see CMUcam4::listDirectory()
*******************************************************************************/
int isVolumeID(CMUcam4_directory_entry_t * pointer);

/***************************************************************************//**
* Returns if a directory entry is a directory (0 or 1).
* @param [in] pointer A pointer to a directory entry data structure.
* @return 1 if a directory, 0 if not, and a negative value if @c pointer is
* NULL.
* @see CMUcam4_directory_entry_t
* @see CMUcam4::listDirectory()
*******************************************************************************/
int isDirectory(CMUcam4_directory_entry_t * pointer);

/***************************************************************************//**
* Returns if a directory entry is ready to archive (0 or 1).
* @param [in] pointer A pointer to a directory entry data structure.
* @return 1 if ready to archive, 0 if not, and a negative value if @c pointer
* is NULL.
* @see CMUcam4_directory_entry_t
* @see CMUcam4::listDirectory()
*******************************************************************************/
int isArchive(CMUcam4_directory_entry_t * pointer);

/***************************************************************************//**
* Activates the interface library.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::end()
*******************************************************************************/
int begin();

/***************************************************************************//**
* Deactivates the interface library.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::begin()
*******************************************************************************/
int end();

/***************************************************************************//**
* Gets the %CMUcam4's firmware version number.
* @return The version number on success and a negative error value on failure.
* @see CMUCAM4_FIRMWARE_V100
* @see CMUCAM4_FIRMWARE_V101
* @see CMUCAM4_FIRMWARE_V102
* @see CMUCAM4_FIRMWARE_V103
*******************************************************************************/
int getVersion();

/***************************************************************************//**
* Resets the %CMUcam4 device.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int resetSystem();

/***************************************************************************//**
* Causes the %CMUcam4 to sleep deeply. This disables servo outputs.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int sleepDeeply();

/***************************************************************************//**
* Causes the %CMUcam4 to sleep lightly. This does not disable servo outputs.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int sleepLightly();

/***************************************************************************//**
* Change the camera brightness setting - useful for low brightness situations.
* @param [in] brightness Between -127 and 127. The default is 0.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int cameraBrightness(int brightness);

/***************************************************************************//**
* Change the camera contrast setting - useful for low contrast situations.
* @param [in] contrast Between -31 and 31. The default is 0.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int cameraContrast(int contrast);

/***************************************************************************//**
* Read a camera register value.
* @param [in] reg The register address. Between 0 and 255.
* @return The register value on success and a negative error value on failure.
*******************************************************************************/
int cameraRegisterRead(int reg);

/***************************************************************************//**
* Write a camera register value.
* @param [in] reg The register address. Between 0 and 255.
* @param [in] value The value to write. Between 0 and 255.
* @param [in] mask The mask of bit positions to write to. Between 0 and 255.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int cameraRegisterWrite(int reg, int value, int mask);

/***************************************************************************//**
* Turns the camera automatic gain control off or on.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int autoGainControl(int active);

/***************************************************************************//**
* Turns the camera automatic white balance off or on.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int autoWhiteBalance(int active);

/***************************************************************************//**
* Horizontally mirror the camera module image.
* @param [in] active 0 for normal mode and 1 for horizontally mirrored mode.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int horizontalMirror(int active);

/***************************************************************************//**
* Vertically flip the camera module image.
* @param [in] active 0 for normal mode and 1 for vertically flipped mode.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int verticalFlip(int active);

/***************************************************************************//**
* Cause the camera to output a black and white image.
* @param [in] active 0 for normal mode and 1 for black and white mode.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int blackAndWhiteMode(int active);

/***************************************************************************//**
* Cause the camera to output a negative image.
* @param [in] active 0 for normal mode and 1 for negative mode.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int negativeMode(int active);

/***************************************************************************//**
* Returns the user button state on the %CMUcam4. 0 for released. 1 for pressed.
* @return 0 or 1 on success and a negative error value on failure.
*******************************************************************************/
int getButtonState();

/***************************************************************************//**
* Returns the user button duration in the current state on the %CMUcam4 in
* milliseconds. Between 0 and 65,535 milliseconds.
* @return The button duration in state in milliseconds on success and a
* negative error value on failure.
*******************************************************************************/
long getButtonDuration();

/***************************************************************************//**
* Returns if the user button was pressed. 0 for no and 1 for yes.
* @return 0 or 1 on success and a negative error value on failure.
*******************************************************************************/
int getButtonPressed();

/***************************************************************************//**
* Returns if the user button was released. 0 for no and 1 for yes.
* @return 0 or 1 on success and a negative error value on failure.
*******************************************************************************/
int getButtonReleased();

/***************************************************************************//**
* Gets the digital state of the pan pin on the %CMUcam4.
* @return 0 or 1 on success and a negative error value on failure.
*******************************************************************************/
int panInput();

/***************************************************************************//**
* Sets the digital state of the pan pin on the %CMUcam4.
* @param [in] direction The boolean direction state. 0 for input. 1 for output.
* @param [in] output The boolean output state. 0 for low. 1 for high.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int panOutput(int direction, int output);

/***************************************************************************//**
* Gets the digital state of the tilt pin on the %CMUcam4.
* @return 0 or 1 on success and a negative error value on failure.
*******************************************************************************/
int tiltInput();

/***************************************************************************//**
* Sets the digital state of the tilt pin on the %CMUcam4.
* @param [in] direction The boolean direction state. 0 for input. 1 for output.
* @param [in] output The boolean output state. 0 for low. 1 for high.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int tiltOutput(int direction, int output);

/***************************************************************************//**
* Gets the digital states of the pan and tilt pins on the %CMUcam4.
* The pan pin is bit 0. The tilt pin is bit 1.
* @return 0, 1, 2, or 3 on success and a negative error value on failure.
*******************************************************************************/
int getInputs();

/***************************************************************************//**
* Sets the digital states of the pan and tilt pins on the %CMUcam4.
* The pan pin is bit 0. The tilt pin is bit 1.
* @param [in] directions The boolean direction states.
*                        0 for input. 1 for output.
* @param [in] outputs The boolean output states.
*                     0 for low. 1 for high.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int setOutputs(int directions, int outputs);

/***************************************************************************//**
* Disables the auxiliary LED on the %CMUcam4.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int LEDOff();

/***************************************************************************//**
* Enables the auxiliary LED on the %CMUcam4.
* @param [in] frequency The frequency to blink the auxiliary to at.
                        Between 0 Hz and 10,000,000 Hz. -1 to turn off.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int LEDOn(long frequency);

/***************************************************************************//**
* Gets the pan or tilt servo pulse length in microseconds.
* Between 750 to 2,250 us - 0 when the servo is disabled.
* @param [in] servo The servo number. 0 for pan and 1 for tilt.
* @return The pulse length on success and a negative error value on failure.
*******************************************************************************/
int getServoPosition(int servo);

/***************************************************************************//**
* Sets the pan or tilt servo pulse length in microseconds.
* Between 750 to 2,250 us.
* @param [in] servo The servo number. 0 for pan and 1 for tilt.
* @param [in] active The servo state. 0 for disabled and 1 for enabled.
* @param [in] pulseLength The servo pulse length in microseconds.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int setServoPosition(int servo, int active, int pulseLength);

/***************************************************************************//**
* Setup automatic pan control.
* @param [in] active The function state. 0 for disabled and 1 for enabled.
* @param [in] reverse 0 for not reversed and 1 for reversed.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int automaticPan(int active, int reverse);

/***************************************************************************//**
* Setup automatic tilt control.
* @param [in] active The function state. 0 for disabled and 1 for enabled.
* @param [in] reverse 0 for not reversed and 1 for reversed.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int automaticTilt(int active, int reverse);

/***************************************************************************//**
* Setup automatic pan control gain values.
* @param [in] proportionalGain PD loop P gain. Between 0 (disable) and 1000.
* @param [in] derivativeGain PD loop D gain. Between 0 (disable) and 1000.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int autoPanParameters(int proportionalGain, int derivativeGain);

/***************************************************************************//**
* Setup automatic tilt control gain values.
* @param [in] proportionalGain PD loop P gain. Between 0 (disable) and 1000.
* @param [in] derivativeGain PD loop D gain. Between 0 (disable) and 1000.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int autoTiltParameters(int proportionalGain, int derivativeGain);

/***************************************************************************//**
* Turn the television monitor signal off.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int monitorOff();

/***************************************************************************//**
* Turn the television monitor signal on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int monitorOn();

/***************************************************************************//**
* Freeze the television monitor signal.
* @param [in] active 0 to unfreeze and 1 to freeze.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int monitorFreeze(int active);

/***************************************************************************//**
* Change the television monitor signal.
* @param [in] active 0 for NTSC and 1 for PAL.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int monitorSignal(int active);

/***************************************************************************//**
* Populates a CMUcam4_tracking_parameters_t data structure with the current
* color tracking threshold parameters in use by the %CMUcam4.
* @param [out] pointer A pointer to a CMUcam4_tracking_parameters_t data
* structure.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4_tracking_parameters_t
*******************************************************************************/
int getTrackingParameters(CMUcam4_tracking_parameters_t * pointer);

/***************************************************************************//**
* Populates a CMUcam4_tracking_window_t data structure with the current
* color tracking window parameters in use by the %CMUcam4.
* @param [out] pointer A pointer to a CMUcam4_tracking_window_t data
* structure.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4_tracking_window_t
*******************************************************************************/
int getTrackingWindow(CMUcam4_tracking_window_t * pointer);

/***************************************************************************//**
* Reset the color tracking threshold parameters to track all possible colors.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int setTrackingParameters();

/***************************************************************************//**
* Set the color tracking threshold parameters to track a range of colors. A
* pixel's red, green, and blue components must fall between the @c redMin,
* @c redMax, @c greenMin, @c greenMax, @c blueMin, and @c blueMax thresholds
* for the pixel to be tracked.
* @param [in] redMin The minimum red threshold - between 0 to 255.
* @param [in] redMax The maximum red threshold - between 0 to 255.
* @param [in] greenMin The minimum green threshold - between 0 to 255.
* @param [in] greenMax The maximum green threshold - between 0 to 255.
* @param [in] blueMin The minimum blue threshold - between 0 to 255.
* @param [in] blueMax The maximum blue threshold - between 0 to 255.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int setTrackingParameters(int redMin, int redMax,
                          int greenMin, int greenMax,
                          int blueMin, int blueMax);

/***************************************************************************//**
* Reset the color tracking window parameters to track all possible pixels.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int setTrackingWindow();

/***************************************************************************//**
* Set the color tracking window parameters to track a range of pixels. A
* pixel's horizontal position and vertical position must fall between the
* @c topLeftX, @c topLeftY, @c bottomRightX, and @c bottomRightY positions for
* the pixel to be tracked.
* @param [in] topLeftX Top left X position (X1) - between 0 and 159.
* @param [in] topLeftY Top left Y position (Y1) - between 0 and 119.
* @param [in] bottomRightX Bottom right X position (X2) - between X1 and 159.
* @param [in] bottomRightY Bottom right Y position (Y2) - between Y2 and 119.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int setTrackingWindow(int topLeftX, int topLeftY,
                      int bottomRightX, int bottomRightY);

/***************************************************************************//**
* Causes the %CMUcam4 to enter idle mode and do nothing. The %CMUcam4 will
* automatically exit streaming mode if any function other than the
* getType_DataPackets functions is called.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int idleCamera();

/***************************************************************************//**
* Causes the %CMUcam4 to enter stream mode and begin sending type T data
* packets. Only the getType_DataPackets functions may be called after calling
* this function to get packets. The %CMUcam4 will automatically exit streaming
* mode if any function other than the getType_DataPackets functions is called.
* This function doesn't change the color tracking settings when called.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getTypeTDataPacket()
*******************************************************************************/
int trackColor();

/***************************************************************************//**
* Causes the %CMUcam4 to enter stream mode and begin sending type T data
* packets. Only the getType_DataPackets functions may be called after calling
* this function to get packets. The %CMUcam4 will automatically exit streaming
* mode if any function other than the getType_DataPackets functions is called.
* This function does change the color tracking settings when called.
* @param [in] redMin The minimum red threshold - between 0 to 255.
* @param [in] redMax The maximum red threshold - between 0 to 255.
* @param [in] greenMin The minimum green threshold - between 0 to 255.
* @param [in] greenMax The maximum green threshold - between 0 to 255.
* @param [in] blueMin The minimum blue threshold - between 0 to 255.
* @param [in] blueMax The maximum blue threshold - between 0 to 255.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getTypeTDataPacket()
*******************************************************************************/
int trackColor(int redMin, int redMax,
               int greenMin, int greenMax,
               int blueMin, int blueMax);

/***************************************************************************//**
* Causes the %CMUcam4 to enter stream mode and begin sending type T data
* packets. Only the getType_DataPackets functions may be called after calling
* this function to get packets. The %CMUcam4 will automatically exit streaming
* mode if any function other than the getType_DataPackets functions is called.
* This function automatically changes the color tracking settings when called.
* @param [in] redRange The red average from the center of the color tracking
* window plus and minus the @c redRange is used to set the redMax and redMin
* values for the color tracking threshold parameters.
* @param [in] greenRange The green average from the center of the color
* tracking window plus and minus the @c greenRange is used to set the greenMax
* and greenMin values for the color tracking threshold parameters.
* @param [in] blueRange The blue average from the center of the color
* tracking window plus and minus the @c blueRange is used to set the blueMax
* and blueMin values for the color tracking threshold parameters.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getTypeTDataPacket()
*******************************************************************************/
int trackWindow(int redRange, int greenRange, int blueRange);

/***************************************************************************//**
* Causes the %CMUcam4 to enter stream mode and begin sending type H data
* packets. Only the getType_DataPackets functions may be called after calling
* this function to get packets. The %CMUcam4 will automatically exit streaming
* mode if any function other than the getType_DataPackets functions is called.
* The red and blue channels can have up to 32 bins. The green channel can have
* up to 64 bins.
* @param [in] channel The color channel of the histogram to send:
* 0 = Red Channel, 1 = Green Channel, 2 = Blue Channel.
* @param [in] bins The number of bins of the histogram channel to send:
* 0=1 bin, 1=2 bins, 2=4 bins, 3=8 bins, 4=16 bins, 5=32 bins, 6=64 bins.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_1_t * pointer)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_2_t * pointer)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_4_t * pointer)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_8_t * pointer)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_16_t * pointer)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_32_t * pointer)
* @see CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_64_t * pointer)
*******************************************************************************/
int getHistogram(int channel, int bins);

/***************************************************************************//**
* Causes the %CMUcam4 to enter stream mode and begin sending type S data
* packets. Only the getType_DataPackets functions may be called after calling
* this function to get packets. The %CMUcam4 will automatically exit streaming
* mode if any function other than the getType_DataPackets functions is called.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getTypeSDataPacket()
*******************************************************************************/
int getMean();

/***************************************************************************//**
* Waits for a type F data packet to appear in the data stream from the %CMUcam4
* and stores that type F data packet in a CMUcam4_image_data_t data strucutre.
* A stream of type F data packets must be started first for this function not
* to fail and timeout waiting for a type F data packet.
* @param [out] pointer A pointer to a CMUcam4_image_data_t data
* structure to store the received type F data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::lineMode()
*******************************************************************************/
int getTypeFDataPacket(CMUcam4_image_data_t * pointer);

/***************************************************************************//**
* Waits for a type H-1 data packet to appear in the data stream from the
* %CMUcam4 and stores that type H data packet in a CMUcam4_histogram_data_1_t
* data structure. A stream of type H-1 data packets must be started first for
* this function not to fail and timeout waiting for a type H-1 data packet.
* @param [out] pointer A pointer to a CMUcam4_histogram_data_1_t data
* structure to store the received type H-1 data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getHistogram()
* @see CMUcam4::lineMode()
*******************************************************************************/
int getTypeHDataPacket(CMUcam4_histogram_data_1_t * pointer);

/***************************************************************************//**
* Waits for a type H-2 data packet to appear in the data stream from the
* %CMUcam4 and stores that type H data packet in a CMUcam4_histogram_data_2_t
* data structure. A stream of type H-2 data packets must be started first for
* this function not to fail and timeout waiting for a type H-2 data packet.
* @param [out] pointer A pointer to a CMUcam4_histogram_data_2_t data
* structure to store the received type H-2 data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getHistogram()
* @see CMUcam4::lineMode()
*******************************************************************************/
int getTypeHDataPacket(CMUcam4_histogram_data_2_t * pointer);

/***************************************************************************//**
* Waits for a type H-4 data packet to appear in the data stream from the
* %CMUcam4 and stores that type H data packet in a CMUcam4_histogram_data_4_t
* data structure. A stream of type H-4 data packets must be started first for
* this function not to fail and timeout waiting for a type H-4 data packet.
* @param [out] pointer A pointer to a CMUcam4_histogram_data_4_t data
* structure to store the received type H-4 data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getHistogram()
* @see CMUcam4::lineMode()
*******************************************************************************/
int getTypeHDataPacket(CMUcam4_histogram_data_4_t * pointer);

/***************************************************************************//**
* Waits for a type H-8 data packet to appear in the data stream from the
* %CMUcam4 and stores that type H data packet in a CMUcam4_histogram_data_8_t
* data structure. A stream of type H-8 data packets must be started first for
* this function not to fail and timeout waiting for a type H-8 data packet.
* @param [out] pointer A pointer to a CMUcam4_histogram_data_8_t data
* structure to store the received type H-8 data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getHistogram()
* @see CMUcam4::lineMode()
*******************************************************************************/
int getTypeHDataPacket(CMUcam4_histogram_data_8_t * pointer);

/***************************************************************************//**
* Waits for a type H-16 data packet to appear in the data stream from the
* %CMUcam4 and stores that type H data packet in a CMUcam4_histogram_data_16_t
* data structure. A stream of type H-16 data packets must be started first for
* this function not to fail and timeout waiting for a type H-16 data packet.
* @param [out] pointer A pointer to a CMUcam4_histogram_data_16_t data
* structure to store the received type H-16 data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getHistogram()
* @see CMUcam4::lineMode()
*******************************************************************************/
int getTypeHDataPacket(CMUcam4_histogram_data_16_t * pointer);

/***************************************************************************//**
* Waits for a type H-32 data packet to appear in the data stream from the
* %CMUcam4 and stores that type H data packet in a CMUcam4_histogram_data_32_t
* data structure. A stream of type H-32 data packets must be started first for
* this function not to fail and timeout waiting for a type H-32 data packet.
* @param [out] pointer A pointer to a CMUcam4_histogram_data_32_t data
* structure to store the received type H-32 data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getHistogram()
* @see CMUcam4::lineMode()
*******************************************************************************/
int getTypeHDataPacket(CMUcam4_histogram_data_32_t * pointer);

/***************************************************************************//**
* Waits for a type H-64 data packet to appear in the data stream from the
* %CMUcam4 and stores that type H data packet in a CMUcam4_histogram_data_64_t
* data structure. A stream of type H-64 data packets must be started first for
* this function not to fail and timeout waiting for a type H-64 data packet.
* @param [out] pointer A pointer to a CMUcam4_histogram_data_64_t data
* structure to store the received type H-64 data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getHistogram()
* @see CMUcam4::lineMode()
*******************************************************************************/
int getTypeHDataPacket(CMUcam4_histogram_data_64_t * pointer);

/***************************************************************************//**
* Waits for a type S data packet to appear in the data stream from the %CMUcam4
* and stores that type S data packet in a CMUcam4_statistics_data_t data
* structure. A stream of type S data packets must be started first for this
* function not to fail and timeout waiting for a type S data packet.
* @param [out] pointer A pointer to a CMUcam4_statistics_data_t data structure
* to store the received type S data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getMean()
* @see CMUcam4::lineMode()
* @see CMUcam4::switchingMode()
*******************************************************************************/
int getTypeSDataPacket(CMUcam4_statistics_data_t * pointer);

/***************************************************************************//**
* Waits for a type T data packet to appear in the data stream from the %CMUcam4
* and stores that type T data packet in a CMUcam4_tracking_data_t data
* strucutre. A stream of type T data packets must be started first for this
* function not to  fail and timeout waiting for a type T data packet.
* @param [out] pointer A pointer to a CMUcam4_tracking_data_t data structure to
* store the received type T data packet to.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::trackColor()
* @see CMUcam4::trackWindow()
* @see CMUcam4::lineMode()
* @see CMUcam4::switchingMode()
*******************************************************************************/
int getTypeTDataPacket(CMUcam4_tracking_data_t * pointer);

/***************************************************************************//**
* Turn poll mode off or on. Poll mode causes the %CMUcam4 to send one and only
* one type F, H, S, or T packet after calling CMUcam4::trackColor(),
* CMUcam4::trackWindow(), CMUcam4::getHistogram(), or CMUcam4::getMean()
* instead of a stream of packets that the %CMUcam4 would normally send.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int pollMode(int active);

/***************************************************************************//**
* Turn line mode off or on. Line mode causes the %CMUcam4 to send type F data
* packets after sending type H, S, or T data packets.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
* @see CMUcam4::getTypeFDataPacket()
* @see CMUcam4_image_data_t
*******************************************************************************/
int lineMode(int active);

/***************************************************************************//**
* Turn switching mode off or on. Switching mode causes the %CMUcam4 to send
* type S data packets after sending type T data packets and vice versa.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int switchingMode(int active);

/***************************************************************************//**
* Turn test mode off or on. Test mode causes the %CMUcam4 to process an image
* of color bars instead of processing the normal image.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int testMode(int active);

/***************************************************************************//**
* Turn color tracking off or on.
* If off, then the %CMUcam4 operates in RGB mode. In RGB mode R maps to the red
* channel, G maps to the green channel, and B maps to the blue channel.
* @n @n If on, then the %CMUcam4 operates in YUV mode. In YUV mode Y maps to
* the green channel, U maps to the blue channel, and V maps to the red channel.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int colorTracking(int active);

/***************************************************************************//**
* Turn histogram tracking off or on.
* If off, then the %CMUcam4 calculates the histogram and image statisitcs on
* all tracked and untracked pixels inside of the color tracking window.
* @n @n If on, then the %CMUcam4 calculates the histogram and image statisitcs
* on all tracked pixels inside of the color tracking window.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int histogramTracking(int active);

/***************************************************************************//**
* Turn the inverted filter off or on.
* If off, then the %CMUcam4 tracks all pixels that fall inside of the color
* tracking bounds.
* @n @n If on, then the %CMUcam4 tracks all pixels that fall outside of the
* color tracking bounds.
* @param [in] active 0 for off and 1 for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int invertedFilter(int active);

/***************************************************************************//**
* Turn the noise filter off or on.
* @par For example:
* 0 to filter out no pixels
* @n 1 to filter out the leading 1 pixels in any group of pixels in a row
* @n 2 to filter out the leading 2 pixels in any group of pixels in a row
* @n ...
* @n 255 to filter out the leading 255 pixels in any group of pixels in a row
* @param [in] threshold 0 for off and 1 or more for on.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int noiseFilter(int threshold);

/***************************************************************************//**
* Change a file's or directory's attributes on the disk.
* @param [in] fileOrDirectoryPathName The file or directory to change.
* @param [in] attributes The new attribute string. E.g. "RHSA" or "_".
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int changeAttributes(const char * fileOrDirectoryPathName,
                     const char * attributes);

/***************************************************************************//**
* Change the working directory.
* @param [in] directoryPathAndName The directory to change to.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int changeDirectory(const char * directoryPathAndName);

/***************************************************************************//**
* Get information about the disk's geometry.
* @param [out] pointer A pointer to a CMUcam4_disk_information_t structure to
* be filled with information about the disk's geometry.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int diskInformation(CMUcam4_disk_information_t * pointer);

/***************************************************************************//**
* Get information about the disk's free and used sector space.
* @param [out] pointer A pointer to a CMUcam4_disk_space_t structure to be
* filled with information about the disk's free and used sector space.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int diskSpace(CMUcam4_disk_space_t * pointer);

/***************************************************************************//**
* Deletes all files and directories on the disk.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int formatDisk();

/***************************************************************************//**
* Reads all or part of the entries in the working directory to a buffer.
* @param [out] pointer The array to store directory entry information to.
* @param [in] size The size of the CMUcam4_directory_entry_t array.
* @param [in] offset The number of entries to skip before storing.
* @return The absolute number of directory entries in the working directory on
* success and a negative error value on failure.
*******************************************************************************/
long listDirectory(CMUcam4_directory_entry_t * pointer,
                   size_t size, unsigned long offset);

/***************************************************************************//**
* Makes a new directory on the disk.
* @param [in] directoryPathAndName The new directory to make.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int makeDirectory(const char * directoryPathAndName);

/***************************************************************************//**
* Moves a file or directory on the disk.
* @param [in] oldEntryPathAndName The source entry to move.
* @param [in] newEntryPathAndName The destination of the entry to move.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int moveEntry(const char * oldEntryPathAndName,
              const char * newEntryPathAndName);

/***************************************************************************//**
* Prints a line of text to a file.
* @param [in] filePathAndName The file to append to.
* @param [in] textToAppend The string to append to the file.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int printLine(const char * filePathAndName, const char * textToAppend);

/***************************************************************************//**
* Reads all or part of a file to a buffer.
* @param [in] filePathAndName The file to read.
* @param [out] buffer The buffer to store all or part of the file to.
* @param [in] size The size of the buffer in bytes.
* @param [in] offset The offset in the file to start to read from in bytes.
* @return The absolute file size in bytes on success and a negative error value
* on failure.
*******************************************************************************/
long filePrint(const char * filePathAndName, uint8_t * buffer,
               size_t size, unsigned long offset);

/***************************************************************************//**
* Deletes a file or empty directory on the disk.
* @param [in] fileOrDirectoryPathAndName The file or directory to delete.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int removeEntry(const char * fileOrDirectoryPathAndName);

/***************************************************************************//**
* Unmounts the micro secure digital card. Other file system functions
* automatically mount the disk by default. This function must be called to
* explictly unmount the disk before removal.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int unmountDisk();

/***************************************************************************//**
* Saves an 80x60 binary bitmap to the disk in BMP file format. The saved binary
* bitmap is the 160x120 segmented color tracking image down-sampled to 80x60.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int dumpBitmap();

/***************************************************************************//**
* Saves an 640:320:160:80x480:240:120:60 image to the disk in BMP file format.
* Use this function to get feedback about what the %CMUcam4 sees.
* @param [in] horizontalResolution The horizontal resolution to save the image
* with: 0=640 pixels, 1=320 pixels, 2=160 pixels, 3=80 pixels.
* @param [in] verticalResolution The vertical resolution to save the image
* with: 0=480 pixels, 1=240 pixels, 2=160 pixels, 3=60 pixels.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int dumpFrame(int horizontalResolution, int verticalResolution);

/***************************************************************************//**
* Receive an 80x60 binary bitmap from the %CMUcam4. The sent binary bitmap is
* the 160x120 segmented color tracking image down-sampled to 80x60.
* @param [out] pointer The address of where to put the received binary bitmap.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int sendBitmap(CMUcam4_image_data_t * pointer);

/***************************************************************************//**
* Receive an 640:320:160:80x480:240:120:60 image from the %CMUcam4. Use this
* function to get feedback about what the %CMUcam4 sees.
* @param [in] horizontalResolution The horizontal resolution to send the image
* in: 0=640 pixels, 1=320 pixels, 2=160 pixels, 3=80 pixels.
* @param [in] verticalResolution The vertical resolution to send the image in:
* 0=480 pixels, 1=240 pixels, 2=160 pixels, 3=60 pixels.
* @param [out] buffer The address of where to put the received RGB565 bitmap.
* @param [in] horizonalSize The horizontal size of the array in 16-bit words.
* @param [in] horizontalOffset A horizontal pixel offset within the image to
* start to store image data from. The @c horizontalOffset plus the array @c
* horizonalSize must be less than or equal the selected horizontal resolution.
* @param [in] verticalSize The vertical size of the array.
* @param [in] verticalOffset A vertical pixel offset within the image to start
* to store image data from. The @c verticalOffset plus the array @c
* verticalSize must be less than or equal the selected vertical resolution.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int sendFrame(int horizontalResolution, int verticalResolution,
              uint16_t * buffer,
              size_t horizonalSize, size_t horizontalOffset,
              size_t verticalSize, size_t verticalOffset);

private:

/***************************************************************************//**
* Sends a command to the %CMUcam4 and receives a void responce.
* @param [in] command The command string to be sent.
* @param [in] timeout The timeout for the operation in milliseconds.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int _voidCommandWrapper(const char * command, unsigned long timeout);

/***************************************************************************//**
* Sends a command to the %CMUcam4 and receives an integer responce.
* @param [in] command The command string to be sent.
* @param [in] timeout The timeout for the operation in milliseconds.
* @return The integer value on success and a negative error value on failure.
*******************************************************************************/
int _intCommandWrapper(const char * command, unsigned long timeout);

/***************************************************************************//**
* Generic %CMUcam4 command wrapper. This function wakes the %CMUcam4 up from
* sleep deeply or sleep lightly if not already awake and idles the %CMUcam4 if
* not already idle before sending the passed @c command.
* @param [in] command The command string to be sent.
* @param [in] timeout The timeout for the operation in milliseconds.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int _commandWrapper(const char * command, unsigned long timeout);

/***************************************************************************//**
* Handles receiving and parsing type F, H, S, and T data packets.
* @param [in] responce The type of data packet to look for.
*                      Either 'F', 'H', 'S', or 'T'.
* @return 0 on success and a negative error value on failure.
*******************************************************************************/
int _responceWrapper(char responce);

/***************************************************************************//**
* Waits for the %CMUcam4 to respond with the idle character.
*******************************************************************************/
void _waitForIdle();

/***************************************************************************//**
* Waits for the %CMUcam4 to respond with "ACK" or "NCK".
*******************************************************************************/
void _waitForResponce();

/***************************************************************************//**
* Receives data and handles "MSG" and "ERR" strings.
*******************************************************************************/
void _receiveData();

/***************************************************************************//**
* Maps error strings to error numbers.
*******************************************************************************/
void _handleError();

/***************************************************************************//**
* Checks if the responce buffer starts with a particular string, repeatedly in
* a loop, until timeout.
* @param [in] string The string to check against the responce buffer.
*******************************************************************************/
void _waitForString(const char * string);

/***************************************************************************//**
* Checks if the responce buffer starts with a particular string.
* @param [in] string The string to check against the responce buffer.
* @return 1 if true and 0 if false.
*******************************************************************************/
int _startsWithString(const char * string);

/***************************************************************************//**
* Reads binary data from the serial port.
* @param [out] buffer The address of the buffer of where to put the packet.
* @param [in] size The size of the buffer in bytes.
* @param [in] packetSize The size of the binary data packet in bytes.
* @param [in] packetOffset Offset inside of the binary data packet to start
* to read from in bytes.
*******************************************************************************/
void _readBinary(uint8_t * buffer, size_t size,
                 unsigned long packetSize,
                 unsigned long packetOffset);

/***************************************************************************//**
* Reads text data from the serial port terminated by the '@\r' character. This
* function will immediately return if
* ':' is the first character encountered,
* if "F " is the first string encountered,
* if "DAT:" is the first string encountered (for v1.01 and below firmware),
* or if "DAT: " is the first string encountered (for v1.02 and above firmware).
*******************************************************************************/
void _readText();

/***************************************************************************//**
* Sets the millisecond timeout.
* @param [in] timeout The timeout in milliseconds.
*******************************************************************************/
void _setReadTimeout(unsigned long timeout);

/***************************************************************************//**
* Reads a byte from the serial port with a timeout.
* @return A byte from the serial port.
*******************************************************************************/
int _readWithTimeout();

/***************************************************************************//**
* Millisecond timeout storage for use with CMUcam4::_readWithTimeout().
*******************************************************************************/
unsigned long _timeout;

/***************************************************************************//**
* Millisecond snapshot storage for use with CMUcam4::_readWithTimeout().
*******************************************************************************/
unsigned long _milliseconds;

/***************************************************************************//**
* Interface library activation state.
*******************************************************************************/
enum _CMUcam4_state
{
    DEACTIVATED, ///< The interface library state is deactivated.
    ACTIVATED ///< The interface library state is activated.
}
_state; ///< State booking variable.

/***************************************************************************//**
* Camera board firmware version.
*******************************************************************************/
enum _CMUcam4_version
{
    VERSION_100 = CMUCAM4_FIRMWARE_V100, ///< %CMUcam4 firmware version 1.00.
    VERSION_101 = CMUCAM4_FIRMWARE_V101, ///< %CMUcam4 firmware version 1.01.
    VERSION_102 = CMUCAM4_FIRMWARE_V102, ///< %CMUcam4 firmware version 1.02.
    VERSION_103 = CMUCAM4_FIRMWARE_V103 ///< %CMUcam4 firmware version 1.03.
}
_version; ///< Version booking variable.

/***************************************************************************//**
* Responce (input) storage buffer.
*******************************************************************************/
char _resBuffer[CMUCAM4_RES_BUFFER_SIZE];

/***************************************************************************//**
* Command (output) storage buffer.
*******************************************************************************/
char _cmdBuffer[CMUCAM4_CMD_BUFFER_SIZE];

/***************************************************************************//**
* Portable serial and timer wrapper library.
*******************************************************************************/
CMUcom4 _com;

/***************************************************************************//**
* Stack environment storage for throwing and catching errors.
*******************************************************************************/
jmp_buf _env;
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
