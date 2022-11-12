/*******************************************************************************
  ButtonsManager.h
  
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

// TODO: Rename to ButtonsAndSensorsManager.

#ifndef ButtonsManager_H
#define ButtonsManager_H

#include "MIDIAccordion.h"

#include "Button.h"
#include "Sensor.h"
#include "ButtonChangedHandlers/ButtonChangedHandlerBase.h"
#include "SensorChangedHandlers/SensorChangedHandlerBase.h"

// TODO: Split into LeftHandButtonsManager and RightHandButtonsManager, keeping common function in base class, ButtonsManager.
class ButtonsManager {

private:
  Button* mLeftHandButtons;
  Button* mRightHandButtons;

  Sensor* mLeftHandSensors;
  Sensor* mRightHandSensors;

  uint16_t mCurBassButtonFlags = 0;
  uint16_t mCurChordButtonFlags = 0;
  uint16_t mCurToneButtonFlags = 0;

public:
  ButtonsManager(Button* leftHandButtons, Button* rightHandButtons, Sensor* leftHandSensors, Sensor* rightHandSensors);

  void ReadButtons(Button* buttons, int startButtonIndex, int endButtonIndex, ButtonChangedHandlerBase& buttonChangedHandler, bool debounce);

#ifdef BUILD_RIGHT_HAND_MASTER
  void ReadSensors(Sensor* sensors, int numSensors, SensorChangedHandlerBase& sensorChangedHandler);
#endif

#ifdef BUILD_RIGHT_HAND_MASTER
  // This method is only used by the RH Arduino.
  void FetchLeftHandArduinoButtons();
#endif

protected:
  // The default constructor is protected to prevent its usage.
  ButtonsManager();

#ifdef BUILD_RIGHT_HAND_MASTER
  // The following methods are only used by the RH Arduino.
  void UpdateNewButtonFlags(uint8_t receivedByte, int index);
  void UpdateLeftHandButtonStates();
#endif

private:
  bool IsButtonDebounced(const Button& button);

private:

  // Bank 1: Bass Buttons:    01-12
  // Bank 2: Chords Buttons:  13-24
  // Bank 3: Tone Switch:     25-38

  uint16_t mNewBassButtonFlags = 0;
  uint16_t mNewChordButtonFlags = 0;
  uint16_t mNewToneButtonFlags = 0;
};

#endif
