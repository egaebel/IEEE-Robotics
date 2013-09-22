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

#include "CMUcam4.h"

/*******************************************************************************
* Constructor Functions
*******************************************************************************/

CMUcam4::CMUcam4()
{
    CMUcom4 _copy;

    _state = DEACTIVATED;
    _com = _copy;
}

CMUcam4::CMUcam4(int port)
{
    CMUcom4 _copy(port);

    _state = DEACTIVATED;
    _com = _copy;
}

/*******************************************************************************
* Helper Functions
*******************************************************************************/

int CMUcam4::getPixel(CMUcam4_image_data_t * pointer,
                      int row, int column)
{
    if((pointer==NULL)||
    (row<CMUCAM4_MIN_BINARY_ROW)||(CMUCAM4_MAX_BINARY_ROW<row)||
    (column<CMUCAM4_MIN_BINARY_COLUMN)||(CMUCAM4_MAX_BINARY_COLUMN<column))
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    return ((pointer->pixels[(row * CMUCAM4_ID_T_C) + (column / 8)]
    >> (7 - (column & 7))) & 1);
}

int CMUcam4::setPixel(CMUcam4_image_data_t * pointer,
                      int row, int column, int value)
{
    int bitIndex; int byteIndex;

    if((pointer==NULL)||
    (row<CMUCAM4_MIN_BINARY_ROW)||(CMUCAM4_MAX_BINARY_ROW<row)||
    (column<CMUCAM4_MIN_BINARY_COLUMN)||(CMUCAM4_MAX_BINARY_COLUMN<column))
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    bitIndex = (7 - (column & 7));
    byteIndex = ((row * CMUCAM4_ID_T_C) + (column / 8));

    pointer->pixels[byteIndex] =
    (((~(1<<bitIndex))&(pointer->pixels[byteIndex]))|((value?1:0)<<bitIndex));

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::andPixels(CMUcam4_image_data_t * destination,
                       CMUcam4_image_data_t * source0,
                       CMUcam4_image_data_t * source1)
{
    size_t index;

    if((destination == NULL) || (source0 == NULL) || (source1 == NULL))
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    for(index = 0; index < CMUCAM4_ID_T_LENGTH; index++)
    {
        destination->pixels[index] =
        (source0->pixels[index] & source1->pixels[index]);
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::orPixels(CMUcam4_image_data_t * destination,
                      CMUcam4_image_data_t * source0,
                      CMUcam4_image_data_t * source1)
{
    size_t index;

    if((destination == NULL) || (source0 == NULL) || (source1 == NULL))
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    for(index = 0; index < CMUCAM4_ID_T_LENGTH; index++)
    {
        destination->pixels[index] =
        (source0->pixels[index] | source1->pixels[index]);
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::xorPixels(CMUcam4_image_data_t * destination,
                       CMUcam4_image_data_t * source0,
                       CMUcam4_image_data_t * source1)
{
    size_t index;

    if((destination == NULL) || (source0 == NULL) || (source1 == NULL))
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    for(index = 0; index < CMUCAM4_ID_T_LENGTH; index++)
    {
        destination->pixels[index] =
        (source0->pixels[index] ^ source1->pixels[index]);
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::notPixels(CMUcam4_image_data_t * destination)
{
    size_t index;

    if(destination == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    for(index = 0; index < CMUCAM4_ID_T_LENGTH; index++)
    {
        destination->pixels[index] =
        (~destination->pixels[index]);
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::isReadOnly(CMUcam4_directory_entry_t * pointer)
{
    CMUcam4_entry_attributes_t * attributes;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    attributes = ((CMUcam4_entry_attributes_t *) pointer->attributes);
    return (attributes->readOnly == 'R');
}

int CMUcam4::isHidden(CMUcam4_directory_entry_t * pointer)
{
    CMUcam4_entry_attributes_t * attributes;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    attributes = ((CMUcam4_entry_attributes_t *) pointer->attributes);
    return (attributes->hidden == 'H');
}

int CMUcam4::isSystem(CMUcam4_directory_entry_t * pointer)
{
    CMUcam4_entry_attributes_t * attributes;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    attributes = ((CMUcam4_entry_attributes_t *) pointer->attributes);
    return (attributes->system == 'S');
}

int CMUcam4::isVolumeID(CMUcam4_directory_entry_t * pointer)
{
    CMUcam4_entry_attributes_t * attributes;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    attributes = ((CMUcam4_entry_attributes_t *) pointer->attributes);
    return (attributes->volumeID == 'V');
}

int CMUcam4::isDirectory(CMUcam4_directory_entry_t * pointer)
{
    CMUcam4_entry_attributes_t * attributes;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    attributes = ((CMUcam4_entry_attributes_t *) pointer->attributes);
    return (attributes->directory == 'D');
}

int CMUcam4::isArchive(CMUcam4_directory_entry_t * pointer)
{
    CMUcam4_entry_attributes_t * attributes;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    attributes = ((CMUcam4_entry_attributes_t *) pointer->attributes);
    return (attributes->archive == 'A');
}

/*******************************************************************************
* State Functions
*******************************************************************************/

int CMUcam4::begin()
{
    int errorValue; int retVal0; int retVal1; static int resetTries;

    resetTries = CMUCAM4_RESET_TRIES;
    errorValue = setjmp(_env);

    if(resetTries-- <= 0)
    {
        _com.end();
        return errorValue;
    }

    // Try to reset at fast, medium, and slow baud rates.

    _state = DEACTIVATED;
    _setReadTimeout(CMUCAM4_RESET_TIMEOUT);

    if(errorValue)
    {
        _com.end();
    }

    _com.begin(resetTries ? CMUCOM4_FAST_BAUD_RATE : CMUCOM4_MEDIUM_BAUD_RATE);
    _com.write((uint8_t) '\0');
    _com.write((uint8_t) '\0');
    _com.write((uint8_t) '\0');
    _com.write("\rRS\r");

    _com.end();
    _com.begin(CMUCOM4_MEDIUM_BAUD_RATE);
    _com.write((uint8_t) '\0');
    _com.write((uint8_t) '\0');
    _com.write((uint8_t) '\0');
    _com.write("\rRS\r");

    _com.end();
    _com.begin(CMUCOM4_SLOW_BAUD_RATE);
    _com.write((uint8_t) '\0');
    _com.write((uint8_t) '\0');
    _com.write((uint8_t) '\0');
    _com.write("\rRS\r");

    // Get the firmware version.

    _waitForString("\rCMUcam4 v");
    _readText();

    if(sscanf(_resBuffer, "%1d.%2d ", &retVal0, &retVal1) != 2)
    {
        longjmp(_env, CMUCAM4_UNEXPECTED_RESPONCE);
    }

    _version = ((_CMUcam4_version) ((retVal0 * 100) + retVal1));

    switch(_version)
    {
        case VERSION_100: break;
        case VERSION_101: break;
        case VERSION_102: break;
        case VERSION_103: break;

        default: longjmp(_env, CMUCAM4_UNSUPPORTED_VERSION); break;
    }

    _waitForIdle();

    // Adjust the baud rate.

    _setReadTimeout(CMUCAM4_NON_FS_TIMEOUT);
    _com.write("BM ");

    switch(_version)
    {
        case VERSION_100:
        case VERSION_101: _com.write(CMUCOM4_MEDIUM_BR_STRING); break;
        case VERSION_102:
        case VERSION_103: _com.write(resetTries ?
                                     CMUCOM4_FAST_BR_STRING :
                                     CMUCOM4_MEDIUM_BR_STRING); break;
    }

    _com.write((uint8_t) '\r');
    _waitForResponce();
    _com.end();

    switch(_version)
    {
        case VERSION_100:
        case VERSION_101: _com.begin(CMUCOM4_MEDIUM_BAUD_RATE); break;
        case VERSION_102:
        case VERSION_103: _com.begin(resetTries ?
                                     CMUCOM4_FAST_BAUD_RATE :
                                     CMUCOM4_MEDIUM_BAUD_RATE); break;
    }

    _com.write((uint8_t) '\r');
    _waitForResponce();
    _waitForIdle();

    // Adjust the stop bits.

    _setReadTimeout(CMUCAM4_NON_FS_TIMEOUT);
    _com.write("DM ");

    switch(_version)
    {
        case VERSION_100:
        case VERSION_101: _com.write(CMUCOM4_MEDIUM_SB_STRING); break;
        case VERSION_102:
        case VERSION_103: _com.write(resetTries ?
                                     CMUCOM4_FAST_SB_STRING :
                                     CMUCOM4_MEDIUM_SB_STRING); break;
    }

    _com.write((uint8_t) '\r');
    _waitForResponce();
    _waitForIdle();

    _state = ACTIVATED;
    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::end()
{
    if(_state == DEACTIVATED)
    {
        return CMUCAM4_NOT_ACTIVATED;
    }

    _state = DEACTIVATED;
    _com.end();

    return CMUCAM4_RETURN_SUCCESS;
}

/*******************************************************************************
* System Level Commands
*******************************************************************************/

int CMUcam4::getVersion()
{
    return (_state == ACTIVATED) ? _version : CMUCAM4_NOT_ACTIVATED;
}

int CMUcam4::resetSystem()
{
    return (_state == ACTIVATED) ? begin() : CMUCAM4_NOT_ACTIVATED;
}

int CMUcam4::sleepDeeply()
{
    return _commandWrapper("SD\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::sleepLightly()
{
    return _commandWrapper("SL\r", CMUCAM4_NON_FS_TIMEOUT);
}

/*******************************************************************************
* Camera Module Commands
*******************************************************************************/

int CMUcam4::cameraBrightness(int brightness)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "CB %d\r", brightness) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::cameraContrast(int contrast)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "CC %d\r", contrast) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::cameraRegisterRead(int reg)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "CR %d\r", reg) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _intCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::cameraRegisterWrite(int reg, int value, int mask)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "CW %d %d %d\r", reg, value, mask) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

/*******************************************************************************
* Camera Sensor Auto Control Commands
*******************************************************************************/

int CMUcam4::autoGainControl(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "AG %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::autoWhiteBalance(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "AW %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

/*******************************************************************************
* Camera Format Commands
*******************************************************************************/

int CMUcam4::horizontalMirror(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "HM %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::verticalFlip(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "VF %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

/*******************************************************************************
* Camera Effect Commands
*******************************************************************************/

int CMUcam4::blackAndWhiteMode(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "BW %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::negativeMode(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "NG %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

/*******************************************************************************
* Auxiliary I/O Commands
*******************************************************************************/

int CMUcam4::getButtonState()
{
    return _intCommandWrapper("GB\r", CMUCAM4_NON_FS_TIMEOUT);
}

long CMUcam4::getButtonDuration()
{
    int errorValue; int resultValue; long returnValue;

    if((errorValue = _commandWrapper("GD\r", CMUCAM4_NON_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _receiveData();
    resultValue = (sscanf(_resBuffer, "%ld ", &returnValue) == 1);

    _waitForIdle();
    return resultValue ? returnValue : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::getButtonPressed()
{
    return _intCommandWrapper("GP\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::getButtonReleased()
{
    return _intCommandWrapper("GR\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::panInput()
{
    return _intCommandWrapper("PI\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::panOutput(int direction, int output)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "PO %d %d\r", direction, output) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::tiltInput()
{
    return _intCommandWrapper("TI\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::tiltOutput(int direction, int output)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "TO %d %d\r", direction, output) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::getInputs()
{
    return _intCommandWrapper("GI\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::setOutputs(int directions, int outputs)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "SO %d %d\r", directions, outputs) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::LEDOff()
{
    return _voidCommandWrapper("L0\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::LEDOn(long frequency)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "L1 %ld\r", frequency) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

/*******************************************************************************
* Servo Commands
*******************************************************************************/

int CMUcam4::getServoPosition(int servo)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "GS %d\r", servo) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _intCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::setServoPosition(int servo, int active, int pulseLength)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "SS %d %d %d\r", servo, active, pulseLength) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::automaticPan(int active, int reverse)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "AP %d %d\r", active, reverse) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::automaticTilt(int active, int reverse)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "AT %d %d\r", active, reverse) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::autoPanParameters(int proportionalGain, int derivativeGain)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "PP %d %d\r", proportionalGain, derivativeGain) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::autoTiltParameters(int proportionalGain, int derivativeGain)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "TP %d %d\r", proportionalGain, derivativeGain) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

/*******************************************************************************
* Television Commands
*******************************************************************************/

int CMUcam4::monitorOff()
{
    return _voidCommandWrapper("M0\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::monitorOn()
{
    return _voidCommandWrapper("M1\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::monitorFreeze(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "MF %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::monitorSignal(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "MS %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

/*******************************************************************************
* Color Tracking Commands
*******************************************************************************/

int CMUcam4::getTrackingParameters(CMUcam4_tracking_parameters_t * pointer)
{
    int errorValue; int resultValue;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _commandWrapper("GT\r", CMUCAM4_NON_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _receiveData();
    resultValue = (sscanf(_resBuffer, "%d %d %d %d %d %d ",
    &(pointer->redMin),
    &(pointer->redMax),
    &(pointer->greenMin),
    &(pointer->greenMax),
    &(pointer->blueMin),
    &(pointer->blueMax)) == 6);

    _waitForIdle();
    return resultValue ? CMUCAM4_RETURN_SUCCESS : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::getTrackingWindow(CMUcam4_tracking_window_t * pointer)
{
    int errorValue; int resultValue;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _commandWrapper("GW\r", CMUCAM4_NON_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _receiveData();
    resultValue = (sscanf(_resBuffer, "%d %d %d %d ",
    &(pointer->topLeftX),
    &(pointer->topLeftY),
    &(pointer->bottomRightX),
    &(pointer->bottomRightY)) == 4);

    _waitForIdle();
    return resultValue ? CMUCAM4_RETURN_SUCCESS : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::setTrackingParameters()
{
    return _voidCommandWrapper("ST\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::setTrackingParameters(int redMin, int redMax,
                                   int greenMin, int greenMax,
                                   int blueMin, int blueMax)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "ST %d %d %d %d %d %d\r",
    redMin, redMax, greenMin, greenMax, blueMin, blueMax)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::setTrackingWindow()
{
    return _voidCommandWrapper("SW\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::setTrackingWindow(int topLeftX, int topLeftY,
                               int bottomRightX, int bottomRightY)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "SW %d %d %d %d\r",
    topLeftX, topLeftY, bottomRightX, bottomRightY)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::idleCamera()
{
    int errorValue; static int resetTries; char cmdBuffer[CMUCAM4_IC_LENGTH];

    if(_state == DEACTIVATED)
    {
        return CMUCAM4_NOT_ACTIVATED;
    }

    if(snprintf(cmdBuffer, CMUCAM4_IC_LENGTH,
    CMUCAM4_IC_STRING, (_version / 100), (_version % 100))
    >= (int) CMUCAM4_IC_LENGTH)
    {
        return CMUCAM4_COMMAND_OVERFLOW;
    }

    resetTries = CMUCAM4_IDLE_TRIES;
    errorValue = setjmp(_env);

    if(resetTries-- <= 0)
    {
        return errorValue;
    }

    _setReadTimeout(CMUCAM4_IDLE_TIMEOUT);
    _com.write((uint8_t) '\0');
    _com.write((uint8_t) '\0');
    _com.write((uint8_t) '\0');
    _com.write("\rGV\r");
    _waitForString(cmdBuffer);

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::trackColor()
{
    return _commandWrapper("TC\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::trackColor(int redMin, int redMax,
                        int greenMin, int greenMax,
                        int blueMin, int blueMax)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "TC %d %d %d %d %d %d\r",
    redMin, redMax, greenMin, greenMax, blueMin, blueMax)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _commandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::trackWindow(int redRange, int greenRange, int blueRange)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "TW %d %d %d\r", redRange, greenRange, blueRange)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _commandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::getHistogram(int channel, int bins)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "GH %d %d\r", channel, bins)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _commandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::getMean()
{
    return _commandWrapper("GM\r", CMUCAM4_NON_FS_TIMEOUT);
}

int CMUcam4::getTypeFDataPacket(CMUcam4_image_data_t * pointer)
{
    int errorValue;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('F')))
    {
        return errorValue;
    }

    if(strcmp(_resBuffer, "F ") != 0)
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _readBinary(pointer->pixels, CMUCAM4_ID_T_LENGTH, CMUCAM4_ID_T_LENGTH, 0);

    return (_readWithTimeout() == '\r')
    ? CMUCAM4_RETURN_SUCCESS : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_1_t * pointer)
{
    int errorValue; char * buffer = (_resBuffer + sizeof('H')); size_t counter;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('H')))
    {
        return errorValue;
    }

    for(counter = 0; counter < CMUCAM4_HD_1_T_LENGTH; counter++)
    {
        if((*buffer) == '\0')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }

        pointer->bins[counter] = ((uint8_t) strtol(buffer, &buffer, 10));
    }

    if((*buffer) != '\0')
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_2_t * pointer)
{
    int errorValue; char * buffer = (_resBuffer + sizeof('H')); size_t counter;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('H')))
    {
        return errorValue;
    }

    for(counter = 0; counter < CMUCAM4_HD_2_T_LENGTH; counter++)
    {
        if((*buffer) == '\0')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }

        pointer->bins[counter] = ((uint8_t) strtol(buffer, &buffer, 10));
    }

    if((*buffer) != '\0')
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_4_t * pointer)
{
    int errorValue; char * buffer = (_resBuffer + sizeof('H')); size_t counter;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('H')))
    {
        return errorValue;
    }

    for(counter = 0; counter < CMUCAM4_HD_4_T_LENGTH; counter++)
    {
        if((*buffer) == '\0')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }

        pointer->bins[counter] = ((uint8_t) strtol(buffer, &buffer, 10));
    }

    if((*buffer) != '\0')
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_8_t * pointer)
{
    int errorValue; char * buffer = (_resBuffer + sizeof('H')); size_t counter;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('H')))
    {
        return errorValue;
    }

    for(counter = 0; counter < CMUCAM4_HD_8_T_LENGTH; counter++)
    {
        if((*buffer) == '\0')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }

        pointer->bins[counter] = ((uint8_t) strtol(buffer, &buffer, 10));
    }

    if((*buffer) != '\0')
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_16_t * pointer)
{
    int errorValue; char * buffer = (_resBuffer + sizeof('H')); size_t counter;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('H')))
    {
        return errorValue;
    }

    for(counter = 0; counter < CMUCAM4_HD_16_T_LENGTH; counter++)
    {
        if((*buffer) == '\0')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }

        pointer->bins[counter] = ((uint8_t) strtol(buffer, &buffer, 10));
    }

    if((*buffer) != '\0')
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_32_t * pointer)
{
    int errorValue; char * buffer = (_resBuffer + sizeof('H')); size_t counter;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('H')))
    {
        return errorValue;
    }

    for(counter = 0; counter < CMUCAM4_HD_32_T_LENGTH; counter++)
    {
        if((*buffer) == '\0')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }

        pointer->bins[counter] = ((uint8_t) strtol(buffer, &buffer, 10));
    }

    if((*buffer) != '\0')
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::getTypeHDataPacket(CMUcam4_histogram_data_64_t * pointer)
{
    int errorValue; char * buffer = (_resBuffer + sizeof('H')); size_t counter;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('H')))
    {
        return errorValue;
    }

    for(counter = 0; counter < CMUCAM4_HD_64_T_LENGTH; counter++)
    {
        if((*buffer) == '\0')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }

        pointer->bins[counter] = ((uint8_t) strtol(buffer, &buffer, 10));
    }

    if((*buffer) != '\0')
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::getTypeSDataPacket(CMUcam4_statistics_data_t * pointer)
{
    int errorValue;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('S')))
    {
        return errorValue;
    }

    return (sscanf(_resBuffer,
    "S %d %d %d %d %d %d %d %d %d %d %d %d ",
    &(pointer->RMean),
    &(pointer->GMean),
    &(pointer->BMean),
    &(pointer->RMedian),
    &(pointer->GMedian),
    &(pointer->BMedian),
    &(pointer->RMode),
    &(pointer->GMode),
    &(pointer->BMode),
    &(pointer->RStDev),
    &(pointer->GStDev),
    &(pointer->BStDev)) == 12)
    ? CMUCAM4_RETURN_SUCCESS : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::getTypeTDataPacket(CMUcam4_tracking_data_t * pointer)
{
    int errorValue;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _responceWrapper('T')))
    {
        return errorValue;
    }

    return (sscanf(_resBuffer,
    "T %d %d %d %d %d %d %d %d ",
    &(pointer->mx),
    &(pointer->my),
    &(pointer->x1),
    &(pointer->y1),
    &(pointer->x2),
    &(pointer->y2),
    &(pointer->pixels),
    &(pointer->confidence)) == 8)
    ? CMUCAM4_RETURN_SUCCESS : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::pollMode(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "PM %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::lineMode(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "LM %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::switchingMode(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "SM %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::testMode(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "TM %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::colorTracking(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "CT %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::histogramTracking(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "HT %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::invertedFilter(int active)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "IF %d\r", active) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::noiseFilter(int threshold)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "NF %d\r", threshold) < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

/*******************************************************************************
* File System Commands
*******************************************************************************/

int CMUcam4::changeAttributes(const char * fileOrDirectoryPathName,
                              const char * attributes)
{
    if((fileOrDirectoryPathName == NULL) || (attributes == NULL))
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "CA \"%s\" \"%s\"\r", fileOrDirectoryPathName, attributes)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::changeDirectory(const char * directoryPathAndName)
{
    if(directoryPathAndName == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "CD \"%s\"\r", directoryPathAndName)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::diskInformation(CMUcam4_disk_information_t * pointer)
{
    int errorValue; int resultValue;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _commandWrapper("DI\r", CMUCAM4_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _receiveData();
    memset(pointer->volumeLabel, '\0', CMUCAM4_VL_LENGTH + 1);
    memset(pointer->fileSystemType, '\0', CMUCAM4_FST_LENGTH + 1);

    resultValue = (sscanf(_resBuffer,
    "\"%" CMUCAM4_VL_LENGTH_STR "c\" "
    "\"%" CMUCAM4_FST_LENGTH_STR "c\" "
    "%lxh %lxh %lu %lu %lu %lu ",
    pointer->volumeLabel,
    pointer->fileSystemType,
    &(pointer->diskSignature),
    &(pointer->volumeIdentification),
    &(pointer->countOfDataSectors),
    &(pointer->bytesPerSector),
    &(pointer->sectorsPerCluster),
    &(pointer->countOfClusters)) == 8);

    _waitForIdle();
    return resultValue ? CMUCAM4_RETURN_SUCCESS : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::diskSpace(CMUcam4_disk_space_t * pointer)
{
    int errorValue; int resultValue;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _commandWrapper("DS\r", CMUCAM4_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _receiveData();

    resultValue = (sscanf(_resBuffer,
    "%lu %lu ",
    &(pointer->freeSectorCount),
    &(pointer->usedSectorCount)) == 2);

    _waitForIdle();
    return resultValue ? CMUCAM4_RETURN_SUCCESS : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::formatDisk()
{
    return _voidCommandWrapper("FM\r", CMUCAM4_FS_TIMEOUT);
}

long CMUcam4::listDirectory(CMUcam4_directory_entry_t * pointer,
                            size_t size, unsigned long offset)
{
    int errorValue; unsigned long directorySize;

    if((errorValue = _commandWrapper("LS\r", CMUCAM4_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    for(directorySize = 0; 1; directorySize++)
    {
        _receiveData();

        if((*_resBuffer) == ':')
        {
            break;
        }

        if((pointer != NULL) && (offset <= directorySize) &&
        ((directorySize - offset) < ((unsigned long) size)))
        {
            memset(pointer[directorySize - offset].name,
            '\0', CMUCAM4_NAME_LENGTH + 1);
            memset(pointer[directorySize - offset].attributes,
            '\0', CMUCAM4_ATTR_LENGTH + 1);

            if(sscanf(_resBuffer,
            " \"%" CMUCAM4_NAME_LENGTH_STR "c\" "
            "%" CMUCAM4_ATTR_LENGTH_STR "c ",
            pointer[directorySize - offset].name,
            pointer[directorySize - offset].attributes) != 2)
            {
                return CMUCAM4_UNEXPECTED_RESPONCE;
            }

            pointer[directorySize - offset].size = 0;

            if(strchr(pointer[directorySize - offset].attributes, 'D') == NULL)
            {
                if(sscanf(_resBuffer,
                " \"%*" CMUCAM4_NAME_LENGTH_STR "c\" "
                "%*" CMUCAM4_ATTR_LENGTH_STR "c "
                "%lu ",
                &(pointer[directorySize - offset].size)) != 1)
                {
                    return CMUCAM4_UNEXPECTED_RESPONCE;
                }
            }
        }
    }

    return (long) directorySize; // Will be between 0 and 65,536 entries.
}

int CMUcam4::makeDirectory(const char * directoryPathAndName)
{
    if(directoryPathAndName == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "MK \"%s\"\r", directoryPathAndName)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::moveEntry(const char * oldEntryPathAndName,
                       const char * newEntryPathAndName)
{
    if((oldEntryPathAndName == NULL) || (newEntryPathAndName == NULL))
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "MV \"%s\" \"%s\"\r", oldEntryPathAndName, newEntryPathAndName)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::printLine(const char * filePathAndName, const char * textToAppend)
{
    if((filePathAndName == NULL) || (textToAppend == NULL))
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "PL \"%s\" \"%s\"\r", filePathAndName, textToAppend)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

long CMUcam4::filePrint(const char * filePathAndName, uint8_t * buffer,
                        size_t size, unsigned long offset)
{
    int errorValue; unsigned long fileSize;

    if(filePathAndName == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if(snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "PR \"%s\"\r", filePathAndName) >= CMUCAM4_CMD_BUFFER_SIZE)
    {
        return CMUCAM4_COMMAND_OVERFLOW;
    }

    if((errorValue = _commandWrapper(_cmdBuffer, CMUCAM4_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _receiveData();

    if(sscanf(_resBuffer, "%lu ", &fileSize) != 1)
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    _readBinary(buffer, size, fileSize, offset);

    _waitForIdle();
    return (long) fileSize; // Will be between 0 and 2,147,483,647 bytes.
}

int CMUcam4::removeEntry(const char * fileOrDirectoryPathAndName)
{
    if(fileOrDirectoryPathAndName == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "RM \"%s\"\r", fileOrDirectoryPathAndName)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::unmountDisk()
{
    return _voidCommandWrapper("UM\r", CMUCAM4_FS_TIMEOUT);
}

/*******************************************************************************
* Image Capture Commands
*******************************************************************************/

int CMUcam4::dumpBitmap()
{
    return _voidCommandWrapper("DB\r", CMUCAM4_FS_TIMEOUT);
}

int CMUcam4::dumpFrame(int horizontalResolution, int verticalResolution)
{
    return (snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "DF %d %d\r", horizontalResolution, verticalResolution)
    < CMUCAM4_CMD_BUFFER_SIZE)
    ? _voidCommandWrapper(_cmdBuffer, CMUCAM4_FS_TIMEOUT)
    : CMUCAM4_COMMAND_OVERFLOW;
}

int CMUcam4::sendBitmap(CMUcam4_image_data_t * pointer)
{
    int errorValue;

    if(pointer == NULL)
    {
        return CMUCAM4_RETURN_FAILURE;
    }

    if((errorValue = _commandWrapper("SB\r", CMUCAM4_NON_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _readBinary(pointer->pixels, CMUCAM4_ID_T_LENGTH, CMUCAM4_ID_T_LENGTH, 0);

    if(_readWithTimeout() != '\r')
    {
        return CMUCAM4_UNEXPECTED_RESPONCE;
    }

    _waitForIdle();
    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::sendFrame(int horizontalResolution, int verticalResolution,
                       uint16_t * buffer,
                       size_t horizonalSize, size_t horizontalOffset,
                       size_t verticalSize, size_t verticalOffset)
{
    int errorValue; int serialBuffer0; int serialBuffer1;
    size_t indexX; size_t indexY; size_t resolutionX; size_t resolutionY;

    resolutionX = (CMUCAM4_FRAME_H_RES >> horizontalResolution);
    resolutionY = (CMUCAM4_FRAME_V_RES >> verticalResolution);

    if(snprintf(_cmdBuffer, CMUCAM4_CMD_BUFFER_SIZE,
    "SF %d %d\r", horizontalResolution, verticalResolution)
    >= CMUCAM4_CMD_BUFFER_SIZE)
    {
        return CMUCAM4_COMMAND_OVERFLOW;
    }

    if((errorValue = _commandWrapper(_cmdBuffer, CMUCAM4_NON_FS_TIMEOUT)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    for(indexX = 0; indexX < resolutionX; indexX++)
    {
        _setReadTimeout(CMUCAM4_NON_FS_TIMEOUT);

        _receiveData();

        if((*_resBuffer) == ':')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }

        switch(_version)
        {
            case VERSION_100:
            case VERSION_101:

                if(strcmp(_resBuffer, "DAT:") != 0)
                {
                    return CMUCAM4_UNEXPECTED_RESPONCE;
                }

                break;

            case VERSION_102:
            case VERSION_103:

                if(strcmp(_resBuffer, "DAT: ") != 0)
                {
                    return CMUCAM4_UNEXPECTED_RESPONCE;
                }

                break;
        }

        for(indexY = 0; indexY < resolutionY; indexY++)
        {
            serialBuffer0 = (_readWithTimeout() & 0xFF);
            serialBuffer1 = (_readWithTimeout() & 0xFF);

            if((buffer != NULL) && (horizontalOffset <= indexX) &&
            ((indexX - horizontalOffset) < horizonalSize) &&
            (verticalOffset <= indexY) &&
            ((indexY - verticalOffset) < verticalSize))
            {
                buffer[((indexY - verticalOffset) * horizonalSize)
                + (indexX - horizontalOffset)]
                = ((uint16_t) (serialBuffer0 | (serialBuffer1 << 8)));
            }
        }

        if(_readWithTimeout() != '\r')
        {
            return CMUCAM4_UNEXPECTED_RESPONCE;
        }
    }

    _waitForIdle();
    return CMUCAM4_RETURN_SUCCESS;
}

/*******************************************************************************
* Private Functions
*******************************************************************************/

int CMUcam4::_voidCommandWrapper(const char * command, unsigned long timeout)
{
    int errorValue;

    if((errorValue = _commandWrapper(command, timeout)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _waitForIdle();
    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::_intCommandWrapper(const char * command, unsigned long timeout)
{
    int errorValue; int resultValue; int returnValue;

    if((errorValue = _commandWrapper(command, timeout)))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _receiveData();
    resultValue = (sscanf(_resBuffer, "%d ", &returnValue) == 1);

    _waitForIdle();
    return resultValue ? returnValue : CMUCAM4_UNEXPECTED_RESPONCE;
}

int CMUcam4::_commandWrapper(const char * command, unsigned long timeout)
{
    int errorValue;

    if((errorValue = idleCamera()))
    {
        return errorValue;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _setReadTimeout(timeout);
    _com.write(command);
    _waitForResponce();

    return CMUCAM4_RETURN_SUCCESS;
}

int CMUcam4::_responceWrapper(char responce)
{
    int errorValue;

    if(_state == DEACTIVATED)
    {
        return CMUCAM4_NOT_ACTIVATED;
    }

    if((errorValue = setjmp(_env)))
    {
        return errorValue;
    }

    _setReadTimeout(CMUCAM4_NON_FS_TIMEOUT);

    for(;;)
    {
        _receiveData();

        if((*_resBuffer) == responce)
        {
            break;
        }

        if((*_resBuffer) == ':')
        {
            return CMUCAM4_STREAM_END;
        }

        if(strcmp(_resBuffer, "F ") == 0)
        {
            _readBinary(NULL, 0, CMUCAM4_ID_T_LENGTH, 0);

            if(_readWithTimeout() != '\r')
            {
                return CMUCAM4_UNEXPECTED_RESPONCE;
            }
        }
    }

    return CMUCAM4_RETURN_SUCCESS;
}

void CMUcam4::_waitForIdle()
{
    for(;;)
    {
        _readText();

        if(_startsWithString("MSG"))
        {
            continue; // Throw the message away.
        }

        _handleError();

        if((*_resBuffer) != ':')
        {
            longjmp(_env, CMUCAM4_UNEXPECTED_RESPONCE);
        }

        break;
    }
}

void CMUcam4::_waitForResponce()
{
    _readText();

    if(strcmp(_resBuffer, "NCK") == 0)
    {
        _readText();

        if((*_resBuffer) == ':')
        {
            longjmp(_env, CMUCAM4_NCK_RESPONCE);
        }

        longjmp(_env, CMUCAM4_UNEXPECTED_RESPONCE);
    }

    if(strcmp(_resBuffer, "ACK") != 0)
    {
        longjmp(_env, CMUCAM4_UNEXPECTED_RESPONCE);
    }
}

void CMUcam4::_receiveData()
{
    for(;;)
    {
        _readText();

        if(_startsWithString("MSG"))
        {
            continue; // Throw the message away.
        }

        _handleError();

        break;
    }
}

void CMUcam4::_handleError()
{
    int errorValue; int sum; size_t index; size_t length;

    if(_startsWithString("ERR"))
    {
        sum = 0; length = strlen(_resBuffer);

        for(index = 0; index < length; index++)
        {
            sum += _resBuffer[index];
        }

        switch(sum)
        {
            case CMUCAM4_CAMERA_TIMEOUT_ERROR_SUM:
                errorValue = CMUCAM4_CAMERA_TIMEOUT_ERROR; break;

            case CMUCAM4_CAMERA_CONNECTION_ERROR_SUM:
                errorValue = CMUCAM4_CAMERA_CONNECTION_ERROR; break;

            case CMUCAM4_DISK_IO_ERROR_SUM:
                errorValue = CMUCAM4_DISK_IO_ERROR; break;

            case CMUCAM4_FILE_SYSTEM_CORRUPTED_SUM:
                errorValue = CMUCAM4_FILE_SYSTEM_CORRUPTED; break;

            case CMUCAM4_FILE_SYSTEM_UNSUPPORTED_SUM:
                errorValue = CMUCAM4_FILE_SYSTEM_UNSUPPORTED; break;

            case CMUCAM4_CARD_NOT_DETECTED_SUM:
                errorValue = CMUCAM4_CARD_NOT_DETECTED; break;

            case CMUCAM4_DISK_MAY_BE_FULL_SUM:
                errorValue = CMUCAM4_DISK_MAY_BE_FULL; break;

            case CMUCAM4_DIRECTORY_FULL_SUM:
                errorValue = CMUCAM4_DIRECTORY_FULL; break;

            case CMUCAM4_EXPECTED_AN_ENTRY_SUM:
                errorValue = CMUCAM4_EXPECTED_AN_ENTRY; break;

            case CMUCAM4_EXPECTED_A_DIRECTORY_SUM:
                errorValue = CMUCAM4_EXPECTED_A_DIRECTORY; break;

            case CMUCAM4_ENTRY_NOT_ACCESSIBLE_SUM:
                errorValue = CMUCAM4_ENTRY_NOT_ACCESSIBLE; break;

            case CMUCAM4_ENTRY_NOT_MODIFIABLE_SUM:
                errorValue = CMUCAM4_ENTRY_NOT_MODIFIABLE; break;

            case CMUCAM4_ENTRY_NOT_FOUND_SUM:
                errorValue = CMUCAM4_ENTRY_NOT_FOUND; break;

            // For v1.02 firmware and above.
            case CMUCAM4_ENTRY_ALREADY_EXISTS_SUM:
                errorValue = CMUCAM4_ENTRY_ALREADY_EXISTS; break;

            // For v1.01 firmware and below.
            case (CMUCAM4_ENTRY_ALREADY_EXISTS_SUM - 's'):
                errorValue = CMUCAM4_ENTRY_ALREADY_EXISTS; break;

            case CMUCAM4_DIRECTORY_LINK_MISSING_SUM:
                errorValue = CMUCAM4_DIRECTORY_LINK_MISSING; break;

            case CMUCAM4_DIRECTORY_NOT_EMPTY_SUM:
                errorValue = CMUCAM4_DIRECTORY_NOT_EMPTY; break;

            case CMUCAM4_NOT_A_DIRECTORY_SUM:
                errorValue = CMUCAM4_NOT_A_DIRECTORY; break;

            case CMUCAM4_NOT_A_FILE_SUM:
                errorValue = CMUCAM4_NOT_A_FILE; break;

            default:
                errorValue = CMUCAM4_UNEXPECTED_RESPONCE; break;
        }

        _readText();

        if((*_resBuffer) == ':')
        {
            longjmp(_env, errorValue);
        }

        longjmp(_env, CMUCAM4_UNEXPECTED_RESPONCE);
    }
}

void CMUcam4::_waitForString(const char * string)
{
    size_t index; size_t length = strlen(string);
    memset(_resBuffer, '\0', CMUCAM4_RES_BUFFER_SIZE);

    do
    {
        for(index = 1; index < length; index++)
        {
            _resBuffer[index - 1] = _resBuffer[index];
        }

        _resBuffer[length - 1] = _readWithTimeout();
    }
    while(strcmp(_resBuffer, string) != 0);
}

int CMUcam4::_startsWithString(const char * string)
{
    return (strncmp(_resBuffer, string, strlen(string)) == 0);
}

void CMUcam4::_readBinary(uint8_t * buffer, size_t size,
                          unsigned long packetSize,
                          unsigned long packetOffset)
{
    int serialBuffer; unsigned long serialCounter;

    for(serialCounter = 0; serialCounter < packetSize; serialCounter++)
    {
        serialBuffer = _readWithTimeout();

        if((buffer != NULL) && (packetOffset <= serialCounter) &&
        ((serialCounter - packetOffset) < ((unsigned long) size)))
        {
            buffer[serialCounter - packetOffset] = ((uint8_t) serialBuffer);
        }
    }
}

void CMUcam4::_readText()
{
    int serialBuffer; size_t serialCounter = 0;
    memset(_resBuffer, '\0', CMUCAM4_RES_BUFFER_SIZE);

    for(;;)
    {
        serialBuffer = _readWithTimeout();

        if(serialBuffer == '\r')
        {
            break;
        }

        _resBuffer[serialCounter++] = serialBuffer;

        if(serialCounter >= CMUCAM4_RES_BUFFER_SIZE)
        {
            longjmp(_env, CMUCAM4_RESPONCE_OVERFLOW);
        }

        switch(serialCounter)
        {
            case sizeof(':'):

                if((*_resBuffer) == ':')
                {
                    return; // Found the idle character.
                }

                break;

            case (sizeof("F ") - 1):

                if(strcmp(_resBuffer, "F ") == 0)
                {
                    return; // Found type F packet.
                }

                break;

            case (sizeof("DAT:") - 1):

                if(_state == ACTIVATED)
                {
                    switch(_version)
                    {
                        case VERSION_100:
                        case VERSION_101:

                            if(strcmp(_resBuffer, "DAT:") == 0)
                            {
                                return; // Found a old style DAT packet.
                            }

                            break;

                        case VERSION_102:
                        case VERSION_103:

                            break;
                    }
                }

                break;

            case (sizeof("DAT: ") - 1):

                if(_state == ACTIVATED)
                {
                    switch(_version)
                    {
                        case VERSION_100:
                        case VERSION_101:

                            break;

                        case VERSION_102:
                        case VERSION_103:

                            if(strcmp(_resBuffer, "DAT: ") == 0)
                            {
                                return; // Found a new style DAT packet.
                            }

                            break;
                    }
                }

                break;

            default: break;
        }
    }
}

void CMUcam4::_setReadTimeout(unsigned long timeout)
{
    _timeout = timeout;
    _milliseconds = _com.milliseconds();
}

int CMUcam4::_readWithTimeout()
{
    do
    {
        if((_com.milliseconds() - _milliseconds) >= _timeout)
        {
            longjmp(_env, CMUCAM4_SERIAL_TIMEOUT);
        }
    }
    while(_com.available() == 0);

    return _com.read();
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
