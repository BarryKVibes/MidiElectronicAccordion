/*******************************************************************************
  Diagnostics.cpp
  
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

#include "../MIDIAccordion.h"

// Do not build if sending MIDI.
#ifndef SEND_MIDI

#include "../SharedMacros.h"
#include "Diagnostics.h"

Diagnostics::Diagnostics()
{
}

void Diagnostics::LogLoopTime()
{
  unsigned long curMicrosseconds = micros();
  if (mLastLoopStartTimeMicroseconds > 0)
  {
    unsigned long timeBetweenLoopsMicroseconds = curMicrosseconds - mLastLoopStartTimeMicroseconds;
    mTotalTimeBetweenLoopStartsMicroseconds += timeBetweenLoopsMicroseconds;
    mNumLoops++;
    if (mNumLoops >= 100)
    {
      unsigned long avgTimeBetweenLoopsMicroseconds = mTotalTimeBetweenLoopStartsMicroseconds / mNumLoops;
      DBG_PRINT_LN("Avg time between loops = " + String(avgTimeBetweenLoopsMicroseconds) + " Microseconds");

      mNumLoops = 0;
      mTotalTimeBetweenLoopStartsMicroseconds = 0;
    }
  }

  mLastLoopStartTimeMicroseconds = curMicrosseconds;
}

#endif // SEND_MIDI