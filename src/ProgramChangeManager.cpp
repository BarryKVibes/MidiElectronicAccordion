/*******************************************************************************
  ProgramChangeManager.cpp
  
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

#include "lib/ArduMidi/ardumidi.h"

#include "MIDIAccordion.h"

#include "ProgramChangeManager.h"
#include "Utilities/Utilities.h"
#include "SharedMacros.h"
#include "SharedConstants.h"

const uint8_t MaxProgramNumber = 0x7F;

// This class is used by the Right Hand Arduino to keep track of the current program number, 
// increment/decrement the current program number, and send the current program number Program Change message.

// This method is the default constructor.
ProgramChangeManager::ProgramChangeManager()
{
}

// This method increments the program number member variable. It does not send the Program Change message.
void ProgramChangeManager::IncrementProgramNumber()
{
  if (mCurMidiProgramNum < MaxProgramNumber)
  {
    mCurMidiProgramNum++;
  }
  else
  {
    mCurMidiProgramNum = 0;
  }
}

// This method decrements the program number member variable. It does not send the Program Change message.
void ProgramChangeManager::DecrementProgramNumber()
{
  if (mCurMidiProgramNum == 0)
  {
    mCurMidiProgramNum = MaxProgramNumber;
  }
  else
  {
    mCurMidiProgramNum--;
  }
}

// This method sets the program number member variable to 0. It does not send the Program Change message.
void ProgramChangeManager::ResetProgramNumber()
{
  mCurMidiProgramNum = 0;
}

// This method sends the current program number on the zero-based MIDI channel, passed in.
void ProgramChangeManager::SendCurrentProgramNumberChange(uint8_t zeroBasedMidiChannel)
{
#ifdef SEND_MIDI
  midi_program_change(zeroBasedMidiChannel, mCurMidiProgramNum);
#else
  DBG_PRINT_LN("ProgramChangeManager::SendCurrentProgramNumberChange() - Sending Program Change = " + String(mCurMidiProgramNum) + ".");
#endif
}