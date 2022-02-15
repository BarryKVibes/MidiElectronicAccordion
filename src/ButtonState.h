/*******************************************************************************
  ButtonState.h
  
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

#ifndef ButtonState_H
#define ButtonState_H

#include <Arduino.h>

// The ButtonState structure contains an indication whether the button is pressed, and its pin number.
typedef struct
{
    uint32_t    active    :  1; //  1 false/true
    uint32_t    pin       :  7; //  8 0 - 127
    uint32_t    unused0   : 10; // 18 0 - 1023
    uint32_t    unused1   :  7; // 25 0 - 127
    uint32_t    unused2   :  7; // 32 0 - 127
} ButtonState;

#endif
