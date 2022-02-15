/*******************************************************************************
  LeftHandButtonChangedHandler.cpp
  
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

#include "LeftHandButtonChangedHandler.h"

#ifdef BUILD_LEFT_HAND_SLAVE

#include "../SharedMacros.h"

// Global File Variables
extern uint16_t gBassButtonFlags;
extern uint16_t gChordButtonFlags;
extern uint16_t gToneButtonFlags;

// This class handles Left Hand Arduino button changes. 
// It sets bits in flags that are subsequently sent to the Right Hand Arduino when requested by the RH Arduino over I2C.
// This class does not send MIDI.
LeftHandButtonChangedHandler::LeftHandButtonChangedHandler() : ButtonChangedHandlerBase()
{
}

// This function is called each time through the main loop.
// It packs the note on/off state into the bytes that are sent back to the Master in LeftHandSetupManager::OnDataRequestedByMaster().
// Bass Button Indexes:   00-11
// Chords Button Indexes: 12-23
// Toggle Button Indexes: 24-37
void LeftHandButtonChangedHandler::HandleButtonChange(Button* button, byte buttonIndex)
{   
  SetButtonFlagState(button[buttonIndex].buttonState.active, buttonIndex);
}

void LeftHandButtonChangedHandler::SetButtonFlagState(bool isActive, byte buttonIndex)
{
  // DBG_PRINT_LN("Before - ");
  // DBG_PRINT("LeftHandButtonChangedHandler::SetButtonFlagState() - BassButtonFlags: b"); PRINTBIN(gBassButtonFlags);
  // DBG_PRINT("LeftHandButtonChangedHandler::SetButtonFlagState() - ChordButtonFlags: b"); PRINTBIN(gChordButtonFlags);

  if (buttonIndex >= 0 && buttonIndex < 12)
  {
    // Bass Buttons
    uint8_t bankButtonIndex = buttonIndex - 0;
    uint16_t buttonMask = 0x00000001 << bankButtonIndex;
 
    if (isActive)
    {
      // Set the bit.
      // DbgPrint("SetBit Bass ButtonMask: b"); PRINTBIN(buttonMask);
      gBassButtonFlags = gBassButtonFlags | buttonMask;      
   }
    else {
      // Clear the bit.
      buttonMask = ~buttonMask;
      // DbgPrint("ClearBit Bass ButtonMask: b"); PRINTBIN(buttonMask);
      gBassButtonFlags = gBassButtonFlags & buttonMask;
    }
  }
  else if (buttonIndex >= 12 && buttonIndex < 24)
  {
    // Chord Buttons
    uint8_t bankButtonIndex = buttonIndex - 12;
    uint16_t buttonMask = 0x00000001 << bankButtonIndex;
    if (isActive)
    {
      // Set the bit.
      // DbgPrint("SetBit Chord ButtonMask: b"); PRINTBIN(buttonMask);
      gChordButtonFlags = gChordButtonFlags | buttonMask;
    }
    else {
      // Clear the bit.
      buttonMask = ~buttonMask;
      // DbgPrint("ClearBit Chord ButtonMask: b"); PRINTBIN(buttonMask);
      gChordButtonFlags = gChordButtonFlags & buttonMask;
    }
  }
  else if (buttonIndex >= 24 && buttonIndex < 38)
  {
    // Tone Buttons
    uint8_t bankButtonIndex = buttonIndex - 24;
    uint16_t buttonMask = 0x00000001 << bankButtonIndex;
    if (isActive)
    {
      // Set the bit.
      // DbgPrint("SetBit Tone ButtonMask: b"); PRINTBIN(buttonMask);
      gToneButtonFlags = gToneButtonFlags | buttonMask;
    }
    else {
      // Clear the bit.
      buttonMask = ~buttonMask;
      // DbgPrint("ClearBit Tone ButtonMask: b"); PRINTBIN(buttonMask);
      gToneButtonFlags = gToneButtonFlags & buttonMask;
    }
  }

  // DBG_PRINT_LN("After - ");
  // DBG_PRINT("LeftHandButtonChangedHandler::SetButtonFlagState() - BassButtonFlags: b"); PRINTBIN(gBassButtonFlags);
  // DBG_PRINT("LeftHandButtonChangedHandler::SetButtonFlagState() - ChordButtonFlags: b"); PRINTBIN(gChordButtonFlags);
  // DBG_PRINT("LeftHandButtonChangedHandler::SetButtonFlagState() - ToneButtonFlags: b"); PRINTBIN(gToneButtonFlags);
}

#endif // BUILD_LEFT_HAND_SLAVE