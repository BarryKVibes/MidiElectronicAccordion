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

#include "StatusManager.h"
#include "MIDIEventFlasher.h"
#include "SharedConstants.h"
#include "SharedMacros.h"

extern MIDIEventFlasher gMIDIEventFlasher;

// Tracks note-on states for MIDI notes. 255 Notes packed into eight 32-bit bytes.
uint32_t noteOnFlags[8][16]; // TODO: Magic numbers here and elsewhere.

// This method is the class constructor.
StatusManager::StatusManager()
{
}
 
void StatusManager::OnMidiEvent(MidiEventType midiEventType, uint8_t value, uint8_t channel)
{
  if (midiEventType == MidiEventType::Other)
  {
    UpdateStatusIndicator();
    return;
  }

  uint8_t bank = value / 32;
  uint32_t bitNum = value % 32;
  unsigned long mask = (0x00000001L << bitNum);

  if (midiEventType == MidiEventType::NoteOn)
  {
    // Set flag.
    noteOnFlags[bank][channel] |= mask;
  }

  if (midiEventType == MidiEventType::NoteOff)
  {
    // Clear flag.
    noteOnFlags[bank][channel] &= ~mask;
  }

  // DBG_PRINT_LN("StatusManager::OnMidiEvent() - noteOnFlags[" + String(bank) + "][" + String(channel) + "] = 0x" + String(noteOnFlags[bank][channel], HEX) + ".");

  UpdateStatusIndicator();
}

void StatusManager::UpdateStatusIndicator()
{
  DBG_PRINT_LN("StatusManager::UpdateStatusIndicator() - Entered.");
  switch(mStatusIndicatorMode)
  {
    case StatusIndicatorMode::FlashMidiEvents:
      gMIDIEventFlasher.OnMidiEvent();
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
  for (int bank = 0; bank < 8; bank++)
  {
    for (int channel = 0; channel < 16; channel++)
    {
      if (noteOnFlags[bank][channel] != 0)
      {
        return true;
      }
    }
  }

  return false;
}
