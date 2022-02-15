/*******************************************************************************
  LeftHandSetupManager.cpp
  
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

#include "../SharedConstants.h"
#include "LeftHandSetupManager.h"
#include "../SharedMacros.h"
#include "../Utilities/Utilities.h"

// Global External Variables
extern Button leftHandButtons[NumLeftHandButtons];

// Global Variables
uint16_t gBassButtonFlags = 0;
uint16_t gChordButtonFlags = 0;
uint16_t gToneButtonFlags = 0;

// Local File Variable
static int sCurrentMessageIndex = 0;

// // Forward declarations
// void OnDataRequestedByMaster();
// void OnDataReceivedFromMaster(int howMany);

LeftHandSetupManager::LeftHandSetupManager() : SetupManagerBase()
{
}
 
// Setup for LH MIDI Accordion.
// Initializes serial port for either sending MIDI or writing to the Serial Monitor.
void LeftHandSetupManager::Setup()
{
  // Setup LED pin early to allow it to enable flashing error code.
  pinMode(ledPin, OUTPUT);

#ifdef SEND_MIDI
// TODO: LH Arduino does not send MIDI; probably should remove this. TBD: Still should prevent debug prints if in non-debug version of code.
    Serial.begin(31250);
#else
    // Write to Serial Monitor.
    // Serial.begin(115200);
    Serial.begin(9600);
    DBG_PRINT_LN("LeftHandSetup::Setup() - Starting Setup");
#endif
  
  DBG_PRINT_LN("LeftHandSetup::Setup() - Starting.");

  //DBG_PRINT_LN("LeftHandSetup::Setup() - Setting pin mode.");

  int numButtons = GetNumButtons();
  for (char i = 0; i < numButtons; i++)
  {
    pinMode(GetButtonAt(i).buttonState.pin, INPUT_PULLUP);
  }

  Wire.onRequest(OnDataRequestedByMaster); // register event to handle Master's request for slave data.

  DBG_PRINT_LN("LeftHandSetup::Setup() - Starting I2C.");

  // Setup I2C
  Wire.begin(LeftHandI2CDeviceId);      // join i2c bus with address #device
  Wire.onRequest(OnDataRequestedByMaster); // register event to handle Master's request for slave data.
  Wire.onReceive(OnDataReceivedFromMaster); // Register event to handle Master's startup Sync byte.

  DBG_PRINT_LN("LeftHandSetup::Setup() - Setup done.");
}

int LeftHandSetupManager::GetNumButtons()
{
  return COUNT_ENTRIES(leftHandButtons);
}

Button& LeftHandSetupManager::GetButtonAt(int index)
{
  return leftHandButtons[index];
}

// --- I2C Callbacks

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void LeftHandSetupManager::OnDataRequestedByMaster()
{ 
  //if(!gIsSendMidi) { DbgPrintLn("LeftHandSetupManager OnDataRequestedByMaster() - Entered."); }

  static bool busy = false;

  if (busy)
  {
    DBG_PRINT_LN("LeftHandSetupManager OnDataRequestedByMaster() - Slave received request from Master but Slave is busy from previous request. currentMessageIndex = " + String(sCurrentMessageIndex));
    
    return;
  }

  busy = true;

#ifdef PRINT_LH_BUTTON_FLAGS
    DBG_PRINT_LN();
    DBG_PRINT_LN("LeftHandSetupManager OnDataRequestedByMaster() - Slave received request from Master.");
 #endif

  //blinkOnce();
  
  // Master expects 6 bytes of data; three 16-bit words for Bass, Chord, Tone buttons, 1 byte for Volume Pot value.
  // Cast 16-bit words as arrays of bytes.
  uint8_t* BassButtonBytes = (byte*)&gBassButtonFlags;
  uint8_t* ChordButtonBytes = (byte*)&gChordButtonFlags;
  uint8_t* ToneButtonBytes = (byte*)&gToneButtonFlags;

#ifdef PRINT_LH_BUTTON_FLAGS
  DBG_PRINT("LeftHandSetupManager OnDataRequestedByMaster() - BassButtonFlags: b"); PRINTBIN(gBassButtonFlags);
  DBG_PRINT("LeftHandSetupManager OnDataRequestedByMaster() ChordButtonFlags: b"); PRINTBIN(gChordButtonFlags);
  DBG_PRINT("LeftHandSetupManager OnDataRequestedByMaster() ToneButtonFlags: b"); PRINTBIN(gToneButtonFlags);

  DBG_PRINT_LN(String("LeftHandSetupManager OnDataRequestedByMaster() - Slave Write BassButtonBytes=") + String(BassButtonBytes[0], HEX) + String(BassButtonBytes[1], HEX));
  DBG_PRINT_LN(String("LeftHandSetupManager OnDataRequestedByMaster() - Slave Write ChordButtonBytes=") + String(ChordButtonBytes[0], HEX) + String(ChordButtonBytes[1], HEX));
  DBG_PRINT_LN(String("LeftHandSetupManager OnDataRequestedByMaster() - Slave Write ToneButtonBytes=") + String(ToneButtonBytes[0], HEX) + String(ToneButtonBytes[1], HEX));
#endif

  Wire.write(BassButtonBytes, 2);
  Wire.write(ChordButtonBytes, 2);
  Wire.write(ToneButtonBytes, 2);

  sCurrentMessageIndex++;

  busy = false;
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void LeftHandSetupManager::OnDataReceivedFromMaster(int eventType)
{
  DBG_PRINT_LN("LeftHandSetupManager OnDataReceivedFromMaster() - Event type = " + String(eventType, HEX)+ ".");
}


