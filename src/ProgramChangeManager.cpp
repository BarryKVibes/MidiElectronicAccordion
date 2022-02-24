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
#include "MIDIEventFlasher.h"
#include "ProgramChangeManager.h"
#include "Utilities/Utilities.h"
#include "SharedMacros.h"
#include "SharedConstants.h"
#include "ToneButtonManager.h"

const uint8_t MaxProgramNumber = 0x7F;

extern MIDIEventFlasher gMIDIEventFlasher;
extern ToneButtonManager gToneButtonManager;

ProgramChangeManager::ProgramChangeManager()
{
}

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

void ProgramChangeManager::SetProgramNumber(uint8_t zeroBasedProgramNumber)
{
  mCurMidiProgramNum = zeroBasedProgramNumber;
}

void ProgramChangeManager::ResetProgramNumber()
{
  mCurMidiProgramNum = 0;
}
  
void ProgramChangeManager::SendCurrentProgramNumberChange(uint8_t zeroBasedMidiChannel)
{
#ifdef SEND_MIDI
  midi_program_change(zeroBasedMidiChannel, mCurMidiProgramNum);
  gMIDIEventFlasher.OnMidiEvent();
#else
  DBG_PRINT_LN("ProgramChangeManager::SendCurrentProgramNumberChange() - Sending Program Change = " + String(mCurMidiProgramNum) + ".");
#endif
}

uint8_t ProgramChangeManager::GetHighestEnabledLayersChannel()
{
  uint8_t zeroBasedMidiChannel = RightHandLayer1ZeroBasedMidiChannel;

  if (gToneButtonManager.GetIsActive(ToneButtonRole::MelodyLayer4Enabled))
  {
    zeroBasedMidiChannel = RightHandLayer4ZeroBasedMidiChannel;
  }
  else if (gToneButtonManager.GetIsActive(ToneButtonRole::MelodyLayer3Enabled))
  {
    zeroBasedMidiChannel = RightHandLayer3ZeroBasedMidiChannel;
  }
  else if (gToneButtonManager.GetIsActive(ToneButtonRole::MelodyLayer2Enabled))
  {
    zeroBasedMidiChannel = RightHandLayer2ZeroBasedMidiChannel;
  }
  else if (gToneButtonManager.GetIsActive(ToneButtonRole::MelodyLayer1Enabled))
  {
    zeroBasedMidiChannel = RightHandLayer1ZeroBasedMidiChannel;
  }

  return zeroBasedMidiChannel;
}