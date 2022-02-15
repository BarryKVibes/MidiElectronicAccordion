/*******************************************************************************
  RightHandSensorChangedHandler.h
  
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

#ifndef RightHandSensorChangedHandler_H
#define RightHandSensorChangedHandler_H

#include "../MIDIAccordion.h"

#ifdef BUILD_RIGHT_HAND_MASTER

#include <Arduino.h>

#include "../Sensor.h"
#include "SensorChangedHandlerBase.h"

class RightHandSensorChangedHandler : public SensorChangedHandlerBase
{
private:

  // This enum maps a named sensor to an index to an array of Sensor objects.
  enum RightHandSensorIndex
  {
    Bellows = 0,
    Pitch = 1,
    Tone = 2,
    Melody = 3,
    BassChord = 4,
    LastRightHandSensor
  };

public:
  RightHandSensorChangedHandler();

  void HandleSensorChange(Sensor* sensors, byte sensorIndex);
};

#endif // BUILD_RIGHT_HAND_MASTER

#endif
