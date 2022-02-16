/*******************************************************************************
  NoteButtonChangedHandler.cpp
  
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

#include "NoteButtonChangedHandler.h"

#include "../SharedMacros.h"

NoteButtonChangedHandler::NoteButtonChangedHandler()
{
}

// This method sends a MIDI Note On/Off command, on the passed-in channel.
void NoteButtonChangedHandler::SendMidiNoteCommand(byte noteNum, bool isActive, byte channelZeroBased, String FileName)
{
  DBG_PRINT(FileName + "::SendMidiNoteCommand(noteNum, isActive, channelZeroBased)  = (" + String(noteNum, HEX) + ", "+ String(isActive) + ", "+ String(channelZeroBased) + ")");

  // Send Note On if active, otherwise Note Off.
  if (isActive) {
    
    // Button is pressed; send Note On.
#ifdef SEND_MIDI
    midi_note_on(channelZeroBased, noteNum, DefaultVelocity);
#else
    DBG_PRINT_LN(" - MIDI Note On: 0x" + String(noteNum, HEX));
#endif
  }
  else {
    // Button is released; send Note On.
#ifdef SEND_MIDI
    midi_note_off(channelZeroBased, noteNum, DefaultVelocity);
#else
    DBG_PRINT_LN(" - MIDI Note Off: 0x" + String(noteNum, HEX));
#endif
  }
}