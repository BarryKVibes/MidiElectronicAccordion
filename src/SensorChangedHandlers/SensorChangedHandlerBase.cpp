/*******************************************************************************
  SensorChangedHandlerBase.cpp
  
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

#include "SensorChangedHandlerBase.h"

SensorChangedHandlerBase::SensorChangedHandlerBase()
{
}

void SensorChangedHandlerBase::HandleSensorChange(Sensor* sensor, byte sensorIndex)
{
}

bool SensorChangedHandlerBase::IsSensorValueChangedSignificantly(uint16_t newSensorValue)
{
  uint8_t sensorDifference;
  if (newSensorValue > mCurSensorValue)
  {
    sensorDifference = newSensorValue - mCurSensorValue;
  }
  else
  {
    sensorDifference = mCurSensorValue - newSensorValue;
  }

  if (sensorDifference >= mMinSensorDifference)
  {
    return true;
  }
  else
  {
    return false;
  }
}

#endif // BUILD_RIGHT_HAND_MASTER