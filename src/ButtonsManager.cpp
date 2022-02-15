/*******************************************************************************
  ButtonsManager.cpp
  
  MIDI Electronic Accordion
  https://github.com/BarryKVibes/MidiElectronicAccordion
  Copyright 2022, Barry K Vibes
  
 *******************************************************************************
  
  This file is part of MidiElectronicAccordion.
  
  MidiElectronicAccordion is free software: you can redistribute it and/or 
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  MidiElectronicAccordion is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License along 
  with MidiElectronicAccordion. If not, see <https://www.gnu.org/licenses/>.
  
 ******************************************************************************/

// TODO: Rename to ButtonsAndSensorsManager, or AnalogAndDigitalInputManager.

#include <Arduino.h>
#include <Wire.h>

#include "MIDIAccordion.h"

#include "ButtonsManager.h"
#include "SharedMacros.h"
#include "SharedConstants.h"
#include "ToneButtonManager.h"
#include "Utilities/Utilities.h"

#include "ButtonChangedHandlers/BassButtonChangedHandler.h"
#include "ButtonChangedHandlers/ChordButtonChangedHandler.h"
#include "ButtonChangedHandlers/ToneButtonChangedHandler.h"

extern Button leftHandButtons[NumLeftHandButtons];
extern Button rightHandButtons[NumRightHandButtons];

#ifdef BUILD_RIGHT_HAND_MASTER

BassButtonChangedHandler bassButtonChangedHandler;
ChordButtonChangedHandler chordButtonChangedHandler;
ToneButtonChangedHandler toneButtonChangedHandler;

#endif // BUILD_RIGHT_HAND_MASTER

// TODO: Pass into ToneButtonChangedHandler as dependency?
ToneButtonManager gToneButtonManager;

#if defined(DEBUG_I2C)
static int numFetches = 0;
#endif

const int MaxFetches = 100;
unsigned long fetchTimesMicroseconds[MaxFetches];
unsigned long lastFetchTimeMicroseconds = 0;
unsigned long maxTimeBetweenFetchesMicroseconds = 0;

// Used only by RH Arduino to keep track of both LH Arduino and RH Arduino buttons and sensors.
// It contains utilty functions to update LH buttons given corresponding button flags.
ButtonsManager::ButtonsManager(Button* leftHandButtons, Button* rightHandButtons, Sensor* leftHandSensors, Sensor* rightHandSensors) :
  mLeftHandButtons(leftHandButtons),
  mRightHandButtons(rightHandButtons),
  mLeftHandSensors(leftHandSensors),
  mRightHandSensors(rightHandSensors)
{
}

// This method reads the digital input pin corresponding the the buttons passed in, after the debounce time has elapsed.
// It is used by both the RH and LH Arduinos.
void ButtonsManager::ReadButtons(Button* buttons, int numButtons, ButtonChangedHandlerBase& buttonChangedHandler)
{
  // DBG_PRINT_LN("ButtonsManager::ReadButtons() - Started.");
  bool newButtonState = false;
  for (byte i = 0; i < numButtons; i++)
  {
     if (!IsButtonDebounced(buttons[i]))
    {
      continue;
    }

    // Button has been debounced; OK to read the corresponding pin value.
    //int inputVal = digitalRead(buttons[i].buttonState.pin);
    int inputVal = digitalRead(buttons[i].buttonState.pin);
    // DBG_PRINT_LN("ButtonsManager::ReadButtons() - ["+String(i)+"] @ Pin "+String(buttons[i].buttonState.pin)+"= "+String(inputVal)+".");

    // Note: Input pin is pulled high; therefore logic is inverted.
    newButtonState = inputVal == 0 ? true : false;

    if (buttons[i].buttonState.active == newButtonState) {

      // Button state did not change; check next button.
      // DBG_PRINT_LN("Button at pin " + String(i) + " is active");
      continue;
    }

    // Button state changed. Save its state.
    buttons[i].buttonState.active = newButtonState;
    buttons[i].lastToggleTimeMs = millis();

    // DBG_PRINT_LN("ButtonsManager::ReadButtons() - " + GetButtonInfo(buttons, i) + " State changed to = " + String(buttons[i].buttonState.active)+".");

    buttonChangedHandler.HandleButtonChange(buttons, i);
  }

}

#ifdef BUILD_RIGHT_HAND_MASTER

// This method reads the analog input pin corresponding the the sensors passed in.
// It is currently only used by the RH Arduino. If sensors are used in the LH Arduino, remove the ifdef here and the header.
void ButtonsManager::ReadSensors(Sensor* sensors, int numSensors, SensorChangedHandlerBase& sensorChangedHandler)
{
  //DBG_PRINT_LN("ButtonsManager::ReadSensors() - Started.");
  byte newSensorValue = 0;
  for (byte i = 0; i < numSensors; i++)
  {
    int inputVal = analogRead(sensors[i].sensorState.pin);
    // DBG_PRINT_LN("ButtonsManager::ReadSensors() - ["+String(i)+"] @ Pin "+String(sensors[i].sensorState.pin)+"= "+String(inputVal)+".");

    // Sensor value is 10 bits; scale to 8 bits here. (TBD: MIDI may allow higher res controller values in two separate controller messages.)
    newSensorValue = (byte)(inputVal >> 2);

    if (sensors[i].sensorState.value == newSensorValue) {

      // Sensor state did not change; check next sensor.
      //DBG_PRINT_LN("ButtonsManager::ReadSensors() - Sensor[" + String(i) + "] at pin " + String(sensors[i].sensorState.pin) + " did not change. Value = " + String(sensors[i].sensorState.value));
      continue;
    }

    // Sensor state changed. Save its state.
    sensors[i].sensorState.value = newSensorValue;

    // DBG_PRINT_LN("ButtonsManager::ReadSensors() - " + GetSensorInfo(sensors, i) + " State changed to = " + String(sensors[i].sensorState.value) + ".");

    sensorChangedHandler.HandleSensorChange(sensors, i);
  }
}

#endif // BUILD_RIGHT_HAND_MASTER

#ifdef BUILD_RIGHT_HAND_MASTER

// The following methods are only used by the RH Arduino.

// Get LH Arduino button states via I2C.
// This method sends an I2C Request to the LH Arduino and then reads a byte at a time from the Wire object.
void ButtonsManager::FetchLeftHandArduinoButtons(ButtonChangedHandlerBase& buttonChangedHandler)
{
#ifdef DISABLE_I2C
  return;
#endif // DISABLE_I2C

#ifdef DEBUG_I2C
  // DBG_PRINT_LN();
  // DBG_PRINT_LN("ButtonsManager::FetchLeftHandArduinoButtons() - Sending I2C request to slave.");

  unsigned long startTimeMicroseconds = micros();
  unsigned long intervalBetweenFetchesMicroseconds = 0;
  if (lastFetchTimeMicroseconds > 0)
  {
     intervalBetweenFetchesMicroseconds = startTimeMicroseconds - lastFetchTimeMicroseconds;
  }

  lastFetchTimeMicroseconds = startTimeMicroseconds;
  if (intervalBetweenFetchesMicroseconds > maxTimeBetweenFetchesMicroseconds)
  {
    maxTimeBetweenFetchesMicroseconds = intervalBetweenFetchesMicroseconds;
  }
#endif // DEBUG_I2C

  // Request LH button states from LH (slave) Arduino via I2C.
  // DBG_PRINT_LN("ButtonsManager::FetchLeftHandArduinoButtons - Master requesting " + String(NumBytesExpectedFromLeftHandArduino) + " bytes from Slave1");

  // NOTE: Slave must be connected to Master SDA and SCL lines, otherwise Master will freeze.
  // Request 6 bytes from LH Arduino; 2 for Bass, 2 for Chords, 2 for Tone Switches.
  // These contain the flags representing the button/switch states.
  Wire.requestFrom((uint8_t)LeftHandI2CDeviceId, (uint8_t)NumBytesExpectedFromLeftHandArduino);    // Request 6 bytes from slave device.

  // DBG_PRINT("Master receiving from slave: ");

  int numBytesAvailable;
  int numBytesReceived = 0;

  while ((numBytesAvailable = Wire.available()) != 0) { // slave may send less than requested

    uint8_t receivedByte = Wire.read(); // receive a byte as character

    UpdateNewButtonFlags(receivedByte, numBytesReceived);

    numBytesReceived++;

    // DBG_PRINT(String(receivedByte, HEX));
    // DBG_PRINT(" ");
  }

  // Verify expected number of bytes received.
  if (numBytesReceived != NumBytesExpectedFromLeftHandArduino)
  {
    DBG_PRINT_LN("ButtonsManager::FetchLeftHandArduinoButtons - Unexpected number of bytes received from LH slave; Expected " + String(NumBytesExpectedFromLeftHandArduino) + "  but received " + String(numBytesReceived) + ".");

    // Left Hand Arduino may not be ready.
    // Ignore results; we'll get updated button states the next time around.
    return;
  }

#if defined(DEBUG_I2C) && !defined(SEND_MIDI)
#ifdef PRINT_LH_BUTTON_FLAGS
  DBG_PRINT("Master: Received BassButtonFlags: b"); PRINTBIN(mNewBassButtonFlags);
  DBG_PRINT("Master: Received ChordButtonFlags: b"); PRINTBIN(mNewChordButtonFlags);
  DBG_PRINT("Master: Received ToneButtonFlags: b"); PRINTBIN(mNewToneButtonFlags);
#endif

  // // Cast 16-bit words as arrays of bytes.
  // uint8_t* BassButtonBytes = (byte*)&mNewBassButtonFlags;
  // uint8_t* ChordButtonBytes = (byte*)&mNewChordButtonFlags;
  // uint8_t* ToneButtonBytes = (byte*)&mNewToneButtonFlags;

  // DBG_PRINT_LN(String("Master: Received BassButtonBytes=") + String(BassButtonBytes[0], HEX) + String(BassButtonBytes[1], HEX));
  // DBG_PRINT_LN(String("Master: Received ChordButtonBytes=") + String(ChordButtonBytes[0], HEX) + String(ChordButtonBytes[1], HEX));
  // DBG_PRINT_LN(String("Master: Received ToneButtonBytes=") + String(ToneButtonBytes[0], HEX) + String(ToneButtonBytes[1], HEX));

  uint32_t endTimeMicroseconds = micros();
  unsigned long fetchTimeMicroseconds = endTimeMicroseconds - startTimeMicroseconds;
  fetchTimesMicroseconds[numFetches] = fetchTimeMicroseconds;
  numFetches++;

  DBG_PRINT_LN("ButtonsManager::FetchLeftHandArduinoButtons - Master: Fetch time = " + String(fetchTimeMicroseconds) + " Microseconds.");

  if (numFetches >= MaxFetches)
  {
    numFetches = 0;
    unsigned long totalFetchTimeMicroseconds = 0;
    for (int i = 0; i < MaxFetches; i++)
    {
      totalFetchTimeMicroseconds += fetchTimesMicroseconds[i];
    }
    unsigned long avgFetchTimeMicroseconds = totalFetchTimeMicroseconds / MaxFetches;

    DBG_PRINT_LN("ButtonsManager::FetchLeftHandArduinoButtons - Master: Average time for I2C request/response = " + String(avgFetchTimeMicroseconds) + " Microseconds. Max fetch interval = " + String(maxTimeBetweenFetchesMicroseconds) + " Microseconds.");
  }
  // DBG_PRINT_LN("Master: Elapsed time for I2C request/response = " + String(endTimeMicroSeconds) + " - " + String(startTimeMicroSeconds) + " = " + String(endTimeMicroSeconds - startTimeMicroSeconds) + " Microseconds.");

#endif // DEBUG_I2C && !SEND_MIDI

  UpdateLeftHandButtonStates(buttonChangedHandler);
}

// This method updates all Left Hand Buttons states after the button flags have been updated from the I2C response from the LH Arduino.
// This method also includes the Volume Potentiometer analog input value.
// This method is called by the RH Arduino. It only updates the button state after the debounce time has elapsed.
void ButtonsManager::UpdateLeftHandButtonStates(ButtonChangedHandlerBase& buttonChangedHandler)
{
  unsigned long lastToggleTimeMs = millis();

  // TODO: Replace magic numbers here and elsewhere.
  Button* bassButtons = &mLeftHandButtons[0];
  Button* chordButtons = &mLeftHandButtons[12];
  Button* toneButtons = &mLeftHandButtons[24];

  // Bass Button Flags
  if (mCurBassButtonFlags != mNewBassButtonFlags)
  {
    // Determine which bits changed, and update corresponding mLeftHandButtons.
    // Note Indexes 00-11 (Bass)
    for (int i = 0; i < 12; i++)
    {
      uint16_t buttonMask = 0x00000001 << i;
      uint16_t oldFlag = mCurBassButtonFlags & buttonMask;
      uint16_t newFlag = mNewBassButtonFlags & buttonMask;
      if (oldFlag != newFlag)
      {
        Button& curButton = bassButtons[i];
        if (IsButtonDebounced(curButton))
        {
          // DBG_PRINT("ButtonsManager.Update() - OldBassButtonFlags: b"); PRINTBIN(mCurBassButtonFlags);
          // DBG_PRINT("ButtonsManager.Update() - NewBassButtonMask: b"); PRINTBIN(mNewBassButtonFlags);

          bool isActive = newFlag != 0;
          curButton.buttonState.active = isActive;
          curButton.lastToggleTimeMs = lastToggleTimeMs;

          // DBG_PRINT_LN("ButtonsManager.Update() - BassButton["+ String(i) + "]: isActive = " + String(curButton.buttonState.active));
          bassButtonChangedHandler.HandleButtonChange(mLeftHandButtons, i + 0);
        }
      }
    }
  }

  // Chord Button Flags
  if (mCurChordButtonFlags != mNewChordButtonFlags)
  {
    // Note Indexes 12-23 (Chords)
    for (int i = 0; i < 12; i++)
    {
      uint16_t buttonMask = 0x00000001 << i;
      uint16_t oldFlag = mCurChordButtonFlags & buttonMask;
      uint16_t newFlag = mNewChordButtonFlags & buttonMask;

      if (oldFlag != newFlag)
      {
        Button& curButton = chordButtons[i];
        if (IsButtonDebounced(curButton))
        {
          // DBG_PRINT("ButtonsManager.Update() - OldChordButtonFlags: b"); PRINTBIN(mCurChordButtonFlags);
          // DBG_PRINT("ButtonsManager.Update() - NewChordButtonMask: b"); PRINTBIN(mNewChordButtonFlags);

          bool isActive = newFlag != 0;
          curButton.buttonState.active = isActive;
          curButton.lastToggleTimeMs = lastToggleTimeMs;

          // DBG_PRINT_LN("ButtonsManager.Update() - ChordButton["+ String(i) + "]: isActive = " + String(curButton.buttonState.active));
          chordButtonChangedHandler.HandleButtonChange(mLeftHandButtons, i + 12);
        }
      }
    }
  }

  // Tone Button Flags
  if (mCurToneButtonFlags != mNewToneButtonFlags)
  {
     // Toggle Switches Indexes 24-37
    for (int i = 0; i < 14; i++) // TODO: Magic numbers; here and elsewhere.
    {
      uint16_t buttonMask = 0x00000001 << i;
      uint16_t oldFlag = mCurToneButtonFlags & buttonMask;
      uint16_t newFlag = mNewToneButtonFlags & buttonMask;

      if (oldFlag != newFlag)
      {
        Button& curButton = toneButtons[i];
        if (IsButtonDebounced(curButton))
        {
          // DBG_PRINT("ButtonsManager.Update() - OldToneButtonFlags: b"); PRINTBIN(mCurToneButtonFlags);
          // DBG_PRINT("ButtonsManager.Update() - NewToneButtonMask: b"); PRINTBIN(mNewToneButtonFlags);

          bool isActive = newFlag != 0;
          curButton.buttonState.active = isActive;
          curButton.lastToggleTimeMs = lastToggleTimeMs;

          // DBG_PRINT_LN("ButtonsManager.Update() - ToneButton["+ String(i) + "]: isActive = " + String(curButton.buttonState.active));
          toneButtonChangedHandler.HandleButtonChange(mLeftHandButtons, i + 24);
        }
      }
    }
  }

  // Update Cur with New.
  mCurBassButtonFlags = mNewBassButtonFlags;
  mCurChordButtonFlags = mNewChordButtonFlags;
  mCurToneButtonFlags = mNewToneButtonFlags;
 }

// This method updates the button flag members with the received byte from the LH Arduino over I2C.
void ButtonsManager::UpdateNewButtonFlags(uint8_t receivedByte, int receivedByteIndex)
{
  // Left Hand slave sends bytes in the following order.
  //  (2) Bass Button bytes
  //  (2)Chord Button bytes
  //  (2)Tone Button bytes

  // Cast 16-bit words as arrays of bytes.
  uint8_t* BassButtonBytes = (byte*)&mNewBassButtonFlags;
  uint8_t* ChordButtonBytes = (byte*)&mNewChordButtonFlags;
  uint8_t* ToneButtonBytes = (byte*)&mNewToneButtonFlags;

  // DBG_PRINT_LN("ButtonsManager.UpdateNewButtonFlags() - receivedByte[" + String(receivedByteIndex) + "] = " + String(receivedByte, HEX));
  // DBG_PRINT_LN("ButtonsManager.UpdateNewButtonFlags() - Before mNewBassButtonFlags = " + String(mNewBassButtonFlags, HEX));
  // DBG_PRINT_LN("ButtonsManager.UpdateNewButtonFlags() - Before mNewChordButtonFlags = " + String(mNewChordButtonFlags, HEX));
  // DBG_PRINT_LN("ButtonsManager.UpdateNewButtonFlags() - Before mNewToneButtonFlags = " + String(mNewToneButtonFlags, HEX));

  switch(receivedByteIndex)
  {
    case 0:
      BassButtonBytes[0] = receivedByte;
      break;
    case 1:
      BassButtonBytes[1] = receivedByte;
      break;
    case 2:
      ChordButtonBytes[0] = receivedByte;
      break;
    case 3:
      ChordButtonBytes[1] = receivedByte;
      break;
    case 4:
      ToneButtonBytes[0] = receivedByte;
      break;
    case 5:
      ToneButtonBytes[1] = receivedByte;
      break;
  }

  // DBG_PRINT_LN("ButtonsManager.UpdateNewButtonFlags() - After mNewBassButtonFlags = " + String(mNewBassButtonFlags, HEX));
  // DBG_PRINT_LN("ButtonsManager.UpdateNewButtonFlags() - After mNewChordButtonFlags = " + String(mNewChordButtonFlags, HEX));
  // DBG_PRINT_LN("ButtonsManager.UpdateNewButtonFlags() - After mNewToneButtonFlags = " + String(mNewToneButtonFlags, HEX));
}
#endif // BUILD_RIGHT_HAND_MASTER

// TODO: bjk 220111 Move debounce into button class, per https://roboticsbackend.com/arduino-object-oriented-programming-oop/
// Returns true if it is OK to handle button state toggle.
// This function returns true if the time between the current time and
// the last button state toggle time is greater than the debounce time.
bool ButtonsManager::IsButtonDebounced(const Button& button)
{
  unsigned long curTimeMs = millis();

  // Ignore current state until past the button settling time.
  unsigned long elapsedTimeMs = curTimeMs - button.lastToggleTimeMs;

  if (elapsedTimeMs < DebounceDelayMs) {
    // Last button toggle time is too recent. Prevent changing its state.
//    if (button.buttonState.active) {
//      DBG_PRINT_LN("Button " + String(button.buttonState.pin) + " elapsedTimeMs = " + String(elapsedTimeMs));
//    }

    return false;
  }
  else {
//    if (button.buttonState.active) {
//      DBG_PRINT_LN("Debounce Button " + String(button.buttonState.pin) + " completed." + " elapsedTimeMs = " + String(elapsedTimeMs));
//    }

    return true;
  }
}
