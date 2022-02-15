/*******************************************************************************
  LeftHandButtonChangedHandler.h
  
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

#ifndef LeftHandButtonChangedHandler_H
#define LeftHandButtonChangedHandler_H

#include <Arduino.h>

#include "../MIDIAccordion.h"

#ifdef BUILD_LEFT_HAND_SLAVE

#include "../Button.h"
#include "ButtonChangedHandlerBase.h"

class LeftHandButtonChangedHandler : public ButtonChangedHandlerBase
{  
public:
  LeftHandButtonChangedHandler();

  virtual void HandleButtonChange(Button* button, byte buttonIndex);

protected: 
  void SetButtonFlagState(bool isActive, byte buttonIndex);
};

#endif // BUILD_LEFT_HAND_SLAVE
#endif
