/*******************************************************************************
  ChordButtonChangedHandler.cpp
  
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

#include <ardumidi.h>

#include "ChordButtonChangedHandler.h"

#ifdef BUILD_RIGHT_HAND_MASTER

#include "../SharedMacros.h"

// This class is used by the Right Hand Arduino upon detecting Chord button state changes from the Left Hand Arduino over I2C.
// It sends corresponding MIDI Note On/Off commands.
ChordButtonChangedHandler::ChordButtonChangedHandler() : NoteButtonChangedHandler()
{
    mMidiChannel = ChordsZeroBasedMidiChannel;
}

void ChordButtonChangedHandler::HandleButtonChange(Button* buttons, byte buttonIndex)
{
  // The following is used to map pin index to MIDI note number.
  const byte LowestNote = MIDI_C - MIDI_OCTAVE;

  // Send RH MIDI notes.
  // Convert button index to MIDI note number.
  byte noteNum = LowestNote + buttonIndex;

  SendMidiNoteCommand(noteNum, buttons[buttonIndex].buttonState.active, mMidiChannel, "ChordButtonChangedHandler");
}

#endif // BUILD_RIGHT_HAND_MASTER