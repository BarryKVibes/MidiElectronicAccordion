/*******************************************************************************
  ToneButtonChangedHandler.cpp
  
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

#include "ToneButtonChangedHandler.h"

#ifdef BUILD_RIGHT_HAND_MASTER

#include "../ToneButtonManager.h"
#include "../Utilities/Utilities.h"
#include "../SharedMacros.h"

extern ToneButtonManager gToneButtonManager;

// This class is used by the Right Hand Arduino upon detecting Tone switch button state changes from the Left Hand Arduino over I2C.
// Depending on the switch, it may enable sending note On/Off commands specific MIDI channels, or may send Continuous Controller information.
ToneButtonChangedHandler::ToneButtonChangedHandler() : ButtonChangedHandlerBase()
{
}

// This method is called when the button state changes.
// Toggle Switches 0-13 correspond to buttonIndex 25-38, Arduino Mega Pins 46, 47, 48, 49, 50, 51, 52, 2, 3, 4, 5, 8, 9, 10.
void ToneButtonChangedHandler::HandleButtonChange(Button* buttons, byte buttonIndex)
{
  ButtonState buttonState = buttons[buttonIndex].buttonState;

  // DbgPrintLn("ToneButtonChangedHandler::HandleButtonChange() - " + GetButtonInfo(buttons, buttonIndex));

  // Update ToneButtonManager button states.
  int toneButtonIndex = buttonIndex - 24; // TODO: Magic number; 24 is the first Tone Switch buttonIndex.
  gToneButtonManager.SetIsActive(toneButtonIndex, buttonState.active);
}

#endif // BUILD_RIGHT_HAND_MASTER