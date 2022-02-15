/*******************************************************************************
  LeftHandSensorChangedHandler.cpp
  
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

#ifdef BUILD_LEFT_HAND_SLAVE

#include "LeftHandSensorChangedHandler.h"

// This class handles Left Hand Arduino sensor changes. It runs only on the LH Arduino.
// It sets global sensor values that are subsequently sent to the Right Hand Arduino when requested by the RH Arduino over I2C.
LeftHandSensorChangedHandler::LeftHandSensorChangedHandler() : SensorChangedHandlerBase()
{
}

// This function is called each time through the main loop. It sets global sensor values.
void LeftHandSensorChangedHandler::HandleSensorChange(Sensor* sensors, byte sensorIndex)
{
  // No sensors in LH Arduino,.
}

#endif // BUILD_LEFT_HAND_SLAVE
