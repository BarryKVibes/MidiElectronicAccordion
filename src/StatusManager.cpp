/*******************************************************************************
  StatusManager.cpp
  
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

#include <Arduino.h>

#include "MIDIAccordion.h"
#ifdef BUILD_RIGHT_HAND_MASTER

#include "MIDIEventFlasher.h"
#include "SharedConstants.h"
#include "SharedMacros.h"
#include "StatusManager.h"

extern MIDIEventFlasher gMIDIEventFlasher;
extern StatusManager gStatusManager;

// This method is the class constructor.
StatusManager::StatusManager()
{
  ResetAllChannels();
}

void StatusManager::ResetChannel(uint8_t midiChannelZeroBased)
{
  for (int bank = 0; bank < NumNoteFlagBanks; bank++)
  {
    mNoteOnFlags[bank][midiChannelZeroBased] = 0;
  }
}

void StatusManager::ResetAllChannels()
{
  for (int channel = 0; channel < NumMidiChannels; channel++)
  {
    ResetChannel(channel);
  }
}

void StatusManager::SetStatusIndicatorMode(StatusIndicatorMode mode)
{
  mStatusIndicatorMode = mode;
  DBG_PRINT_LN("StatusManager::SetStatusIndicatorMode() - mStatusIndicatorMode = " + String(mStatusIndicatorMode) + ".");

  ResetAllChannels();
  digitalWrite(LedPin, LOW);
  UpdateStatusIndicator();
}

void StatusManager::OnMidiEvent(MidiEventType midiEventType, uint8_t value, uint8_t channel)
{
  if (mStatusIndicatorMode == StatusIndicatorMode::FlashMidiEvents)
  {
    gMIDIEventFlasher.OnMidiEvent();
    return;
  }
  
  if (midiEventType == MidiEventType::Other)
  {
    gMIDIEventFlasher.OnMidiEvent();
    return;
  }

  const byte NumBitsInMask = 32;
  uint8_t bank = value / NumBitsInMask;
  uint32_t bitNum = value % NumBitsInMask;
  unsigned long mask = (0x00000001L << bitNum);

  if (midiEventType == MidiEventType::NoteOn)
  {
    // Set flag.
    mNoteOnFlags[bank][channel] |= mask;
  }

  if (midiEventType == MidiEventType::NoteOff)
  {
    // Clear flag.
    mNoteOnFlags[bank][channel] &= ~mask;
  }

  // DBG_PRINT_LN("StatusManager::OnMidiEvent() - mNoteOnFlags[" + String(bank) + "][" + String(channel) + "] = 0x" + String(mNoteOnFlags[bank][channel], HEX) + ".");
  
  UpdateStatusIndicator();
}

void StatusManager::UpdateStatusIndicator()
{
  switch(mStatusIndicatorMode)
  {
    case StatusIndicatorMode::FlashMidiEvents:
      gMIDIEventFlasher.UpdateStatusLed();
      break;
      
    case StatusIndicatorMode::OnWhileAnyNoteButtonDepressed:
      if (IsAnyNoteOn())
      {
        // DBG_PRINT_LN("StatusManager::UpdateStatusIndicator() - A note is ON.");
        digitalWrite(LedPin, HIGH);
      }
      else
      {
        // DBG_PRINT_LN("StatusManager::UpdateStatusIndicator() - All notes are OFF.");
        digitalWrite(LedPin, LOW);
      }

      break;
  }
}

bool StatusManager::IsAnyNoteOn()
{
  for (int bank = 0; bank < NumNoteFlagBanks; bank++)
  {
    for (int channel = 0; channel < NumMidiChannels; channel++)
    {
      if (mNoteOnFlags[bank][channel] != 0)
      {
        DBG_PRINT_LN("StatusManager::IsAnyNoteOn() - On at bank = " + String(bank) + "; channel = " + String(channel) + ".");

        return true;
      }
    }
  }

  return false;
}

#endif // BUILD_RIGHT_HAND_MASTER
