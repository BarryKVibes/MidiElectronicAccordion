/*******************************************************************************
  MIDIEventFlasher.cpp
  
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

#include "MIDIEventFlasher.h"
#include "SharedConstants.h"

MIDIEventFlasher::MIDIEventFlasher()
{
}

void MIDIEventFlasher::OnMidiEvent()
{
  mFlashOnStartTimestampMilliseconds = millis();
  digitalWrite(LedPin, HIGH);
}

void MIDIEventFlasher::UpdateStatusLed()
{
  if (mFlashOnStartTimestampMilliseconds == 0)
  {
    // LED already off.
    return;
  }

  uint32_t curTimestampMilliseconds = millis();
  uint32_t elapsedTimeMilliseconds = curTimestampMilliseconds - mFlashOnStartTimestampMilliseconds;

  if (elapsedTimeMilliseconds > MidiEventFlashDurationMilliseconds)
  {
    digitalWrite(LedPin, LOW);
    mFlashOnStartTimestampMilliseconds = 0;
  }
}