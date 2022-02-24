/*******************************************************************************
  TonePotentiometerSensorChangedHandler.cpp
  
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

#ifdef BUILD_RIGHT_HAND_MASTER

#include "../lib/ArduMidi/ardumidi.h"

#include "TonePotentiometerSensorChangedHandler.h"
#include "../SharedMacros.h"
#include "../ToneButtonManager.h"
#include "../ProgramChangeManager.h"

// TODO: Move common constants and code to base class.
extern ProgramChangeManager gProgramChangeManager;

// Scale min volume from 8-bit to 7-bit CC value by shifting right by one.
const byte TonePotentiometerSensorChangedHandler::MinMidiProgram = (byte)0x00;
const byte TonePotentiometerSensorChangedHandler::MaxMidiProgram = (byte)0x7F;

// This class is used by the Right Hand Arduino upon detecting Tone Control potentiometer value changes.
// It sends corresponding MIDI Channel Program Change on the channel corresponding to the highest enabled melody layer switch.
TonePotentiometerSensorChangedHandler::TonePotentiometerSensorChangedHandler() : SensorChangedHandlerBase()
{
}

void TonePotentiometerSensorChangedHandler::HandleSensorChange(Sensor* sensors, byte sensorIndex)
{
  uint16_t sensorValue = sensors[sensorIndex].sensorState.value;
  if(!SensorChangedHandlerBase::IsSensorValueChangedSignificantly(sensorValue))
  {
    // Prevent noisy pot from changing program.
    return;
  }

  mCurSensorValue = sensorValue;

  // Map sensor value from Min to Max, then scale to 7 bits for MIDI CC.
  // map(value, fromLow, fromHigh, toLow, toHigh)
  byte midiProgramValue = (byte)map(sensorValue, 0x00, 0xFF, MinMidiProgram, MaxMidiProgram);

  uint8_t zeroBasedMidiChannelForProgramChange = gProgramChangeManager.GetHighestEnabledLayersChannel();

  // DBG_PRINT_LN("TonePotentiometerSensorChangedHandler::HandleSensorChange() - Sensor Value = " + String(sensorValue) + "; Program Value = " + String(midiProgramValue) + ".");
  gProgramChangeManager.SetProgramNumber(midiProgramValue);
  gProgramChangeManager.SendCurrentProgramNumberChange(zeroBasedMidiChannelForProgramChange);
}

#endif // BUILD_RIGHT_HAND_MASTER
