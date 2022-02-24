/*******************************************************************************
  SensorChangedHandlerBase.h
  
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

#ifndef SensorChangedHandlerBase_H
#define SensorChangedHandlerBase_H

#include "../MIDIAccordion.h"

#include <Arduino.h>

#include "../Sensor.h"

class SensorChangedHandlerBase
{
public:
  SensorChangedHandlerBase();

  virtual void HandleSensorChange(Sensor* sensor, byte sensorIndex);

protected:
  bool IsSensorValueChangedSignificantly(uint16_t newSensorValue);

protected:
  uint16_t mCurSensorValue = 0;
  uint8_t mMinSensorDifference = 2;
};

#endif
