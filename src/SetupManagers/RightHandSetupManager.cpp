/*******************************************************************************
  RightHandSetupManager.cpp
  
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

#include <Wire.h>

#include "RightHandSetupManager.h"
#include "../Utilities/Utilities.h"
#include "../SharedMacros.h"

// Global Variables
extern Button rightHandButtons[NumRightHandButtons];

RightHandSetupManager::RightHandSetupManager() : SetupManagerBase()
{
}
 
// Setup for RH MIDI Accordion.
// Initializes serial port for either sending MIDI or writing to the Serial Monitor.
void RightHandSetupManager::Setup()
{
  // Setup LED pin early to allow it to enable flashing error code.
  pinMode(LedPin, OUTPUT);

#ifdef SEND_MIDI
  Serial.begin(BaudRateMidi);
#else
  // Write to Serial Monitor.
  Serial.begin(BaudRateSerialMonitor);
#endif

  int numButtons = GetNumButtons();
  for (char i = 0; i < numButtons; i++)
  {
    pinMode(GetButtonAt(i).buttonState.pin, INPUT_PULLUP);
  }

#ifndef DISABLE_I2C
  // if(!gIsSendMidi) { DbgPrintLn("RightHandSetup::Setup() - Starting I2C."); }

  // Setup Master I2C
  // Initiate the Wire library and join the I2C bus as a master.
  Wire.begin();
#endif // DISABLE_I2C

  // Indicated that RH Arduino is ready.
  blinkOnce();

  // if(!gIsSendMidi) { DbgPrintLn("RightHandSetup::Setup() - Setup done."); }
}

int RightHandSetupManager::GetNumButtons()
{
  return COUNT_ENTRIES(rightHandButtons);
}

Button& RightHandSetupManager::GetButtonAt(int index)
{
  return rightHandButtons[index];
}
