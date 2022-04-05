/*******************************************************************************
  ProgramChangeButtonChangedHandler.cpp
  
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

#include "../lib/ArduMidi/ardumidi.h"

#include "ProgramChangeButtonChangedHandler.h"

#ifdef BUILD_RIGHT_HAND_MASTER

#include "../ToneButtonManager.h"
#include "../ProgramChangeManager.h"
#include "../SharedMacros.h"
#include "../SharedConstants.h"

extern ProgramChangeManager gProgramChangeManager; // TODO: Inject dependency.

// Indexes into Button rightHandButtons[NumRightHandButtons], in main.cpp.
const byte ButtonIndexForDecrementProgramNumber = 41;
const byte ButtonIndexForIncrementProgramNumber = 42;

// This class handles Right Hand Arduino Increment/Decrement Program button changes. 
// It sends corresponding MIDI Program Change message.
ProgramChangeButtonChangedHandler::ProgramChangeButtonChangedHandler()
{
}

void ProgramChangeButtonChangedHandler::HandleButtonChange(Button* buttons, byte buttonIndex)
{
#ifndef ENABLE_CUSTOM_PROGRAM_CHANGE_BUTTONS
  return;
#endif

  bool isButtonDown = buttons[buttonIndex].buttonState.active;

  // DBG_PRINT_LN("ProgramChangeButtonChangedHandler::HandleButtonChange() - buttons["+String(buttonIndex)+"] @ Pin "+String(buttons[buttonIndex].buttonState.pin)+"= "+String(buttons[buttonIndex].buttonState.active)+".");

  uint8_t zeroBasedMidiChannelForProgramChange = gProgramChangeManager.GetHighestEnabledLayersChannel();

  // If low F# or G#, send decrement, or increment program number and send program change MIDI message.
  if (buttonIndex == ButtonIndexForDecrementProgramNumber)
  {
    // Change program upon Button Down, otherwise ignore.
    if (isButtonDown)
    {
      bool isOtherKeyDown = buttons[ButtonIndexForIncrementProgramNumber].buttonState.active;
      if (isOtherKeyDown)
      {
        gProgramChangeManager.ResetProgramNumber();
      }
      else
      {
        gProgramChangeManager.DecrementProgramNumber();
      }

      gProgramChangeManager.SendCurrentProgramNumberChange(zeroBasedMidiChannelForProgramChange);
    }

    return;
  }

  if (buttonIndex == ButtonIndexForIncrementProgramNumber)
  {
    // Change program upon KeyDown, otherwise ignore.
    if (isButtonDown)
    {
      bool isOtherKeyDown = buttons[ButtonIndexForDecrementProgramNumber].buttonState.active;
      if (isOtherKeyDown)
      {
        gProgramChangeManager.ResetProgramNumber();
      }
      else
      {
        gProgramChangeManager.IncrementProgramNumber();
      }

      gProgramChangeManager.SendCurrentProgramNumberChange(zeroBasedMidiChannelForProgramChange);
    }

    return;
  }
}

#endif // BUILD_RIGHT_HAND_MASTER