/*******************************************************************************
  BassChordVolumeSensorChangedHandler.cpp
  
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

#include "BassChordVolumeSensorChangedHandler.h"
#include "../SharedMacros.h"
#include "../VolumeChangeManager.h"
#include "../Utilities/Utilities.h"

extern VolumeChangeManager gVolumeChangeManager;

// TODO: Move common constants and code to base class.
const float BassChordVolumeSensorChangedHandler::MinVolumePercentOfMax = 0.10;

// Scale min volume from 8-bit to 7-bit CC value by shifting right by one.
const byte BassChordVolumeSensorChangedHandler::MinMidiVolume = (byte)((float)0xFF * MinVolumePercentOfMax) >> 1;
const byte BassChordVolumeSensorChangedHandler::MaxMidiVolume = (byte)0x7F;

// This class is used by the Right Hand Arduino upon detecting Volume Sensor value changes from the Left Hand Arduino over I2C.
// It sends corresponding MIDI Channel Volume Control CC command, 0x07 on MIDI Channel 1.
// This method maps the full 8-bit sensor range, to a 7-bit CC value with a minimum volume, defined by the class constant.
BassChordVolumeSensorChangedHandler::BassChordVolumeSensorChangedHandler() : SensorChangedHandlerBase()
{
}

// Sends MIDI Volume CC message on MIDI Channel 1.
void BassChordVolumeSensorChangedHandler::HandleSensorChange(Sensor* sensors, byte sensorIndex)
{
  // Send sensor value (8-bit) as (7-bit) MIDI Controller Change value (i.e., keep upper 7 bits by shifting right by 1).
  int sensorValue = sensors[sensorIndex].sensorState.value;

  // Map sensor value from Min to Max, then scale to 7 bits for MIDI CC.
  // map(value, fromLow, fromHigh, toLow, toHigh)
  byte midiVolumeValue = (byte)map(sensorValue, 0x00, 0xFF, MinMidiVolume, MaxMidiVolume);

  // DBG_PRINT_LN("BassChordVolumeSensorChangedHandler::HandleSensorChange() - " + GetSensorInfo(sensors, sensorIndex) + ".");
  gVolumeChangeManager.SetCurrentMidiControlVolume(VolumeChangeManager::VolumeControlType::BassChord, midiVolumeValue);
}

#endif // BUILD_RIGHT_HAND_MASTER
