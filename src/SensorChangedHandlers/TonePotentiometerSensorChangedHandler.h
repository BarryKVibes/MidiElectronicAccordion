/*******************************************************************************
  TonePotentiometerSensorChangedHandler.h
  
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

#ifndef TonePotentiometerSensorChangedHandler_H
#define TonePotentiometerSensorChangedHandler_H

#include "../MIDIAccordion.h"

#ifdef BUILD_RIGHT_HAND_MASTER

#include <Arduino.h>

#include "SensorChangedHandlerBase.h"

// This class sends a Program Change message based on the potentiometer position.
// To prevent noise in the potentiometer from constantly changing the program, the
// sesnor value must change by at least two.
class TonePotentiometerSensorChangedHandler : public SensorChangedHandlerBase
{
public:
  TonePotentiometerSensorChangedHandler();

  void HandleSensorChange(Sensor* sensors, byte sensorIndex);

private:
  static const byte MinMidiProgram;
  static const byte MaxMidiProgram;
};

#endif // BUILD_RIGHT_HAND_MASTER

#endif
