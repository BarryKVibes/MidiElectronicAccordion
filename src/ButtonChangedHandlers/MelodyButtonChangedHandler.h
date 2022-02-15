/*******************************************************************************
  MelodyButtonChangedHandler.h
  
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

#ifndef MelodyButtonChangedHandler_H
#define MelodyButtonChangedHandler_H

#include <Arduino.h>

#include "../MIDIAccordion.h"

#ifdef BUILD_RIGHT_HAND_MASTER

#include "../Button.h"
#include "NoteButtonChangedHandler.h"

class MelodyButtonChangedHandler : public NoteButtonChangedHandler
{  
public:
  MelodyButtonChangedHandler();

  virtual void HandleButtonChange(Button* button, byte buttonIndex);
};

#endif // BUILD_RIGHT_HAND_MASTER
#endif
