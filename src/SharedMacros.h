/*******************************************************************************
  SharedMacros.h
  
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

#ifndef SharedMacros_H
#define SharedMacros_H

#include "MIDIAccordion.h"

#ifdef SEND_MIDI
  #define DBG_PRINT_LN(str)
  #define DBG_PRINT(str)
#else
  #define DBG_PRINT_LN(str) Serial.println(str);
  #define DBG_PRINT(str) Serial.print(str);
#endif

#ifdef SEND_MIDI
  extern void LogLoopTime();
  #define LOG_LOOP_TIME()
#else
  #define LOG_LOOP_TIME() diagnostics.LogLoopTime()
#endif // SEND_MIDI

// Macros
#define COUNT_ENTRIES(ARRAY)        (sizeof(ARRAY) / sizeof(ARRAY[0]))
#define PRINTBIN(Num) for (uint32_t t = (1UL<< (sizeof(Num)*8)-1); t; t >>= 1) {Serial.write(Num  & t ? '1' : '0');} Serial.println(""); // Prints a binary number with leading zeros (Automatic Handling)

#endif
