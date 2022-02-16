/*******************************************************************************
  RightHandSensorChangedHandler.cpp
  
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

// TODO: Change name to RightHandSensorChangedManager, MasterSensorChangedManager?

// This class sends the sensor change event to the corresponding handler object, based on the sensorIndex.

#include "../MIDIAccordion.h"

#ifdef BUILD_RIGHT_HAND_MASTER

#include "RightHandSensorChangedHandler.h"
#include "BellowsVolumeSensorChangedHandler.h"
#include "PitchPotentiometerSensorChangedHandler.h"
#include "TonePotentiometerSensorChangedHandler.h"
#include "MelodyVolumeSensorChangedHandler.h"
#include "BassChordVolumeSensorChangedHandler.h"
#include "../Utilities/Utilities.h"
#include "../SharedMacros.h"

BellowsVolumeSensorChangedHandler bellowsVolumeSensorChangedHandler;
PitchPotentiometerSensorChangedHandler pitchPotentiometerSensorChangedHandler;
TonePotentiometerSensorChangedHandler tonePotentiometerSensorChangedHandler;
MelodyVolumeSensorChangedHandler melodyVolumeSensorChangedHandler;
BassChordVolumeSensorChangedHandler bassChordVolumeSensorChangedHandler;

// This class is the top-level handler for Right Hand Arduino sensor changes. It runs only on the RH Arduino.
// It routes sensor changes to the specific handler, based on sensor index.
RightHandSensorChangedHandler::RightHandSensorChangedHandler() : SensorChangedHandlerBase()
{
}

// This method calls the appropriate handler for the sensor at the index passed in.
void RightHandSensorChangedHandler::HandleSensorChange(Sensor* sensors, byte sensorIndex)
{
  //if(!gIsSendMidi) {DbgPrintLn("RightHandSensorChangedHandler::HandleSensorChange() - " + GetSensorInfo(sensors, sensorIndex) + ".");}

  switch(sensorIndex)
  {
    case RightHandSensorIndex::Bellows:
#ifdef IGNORE_BELLOWS_VOLUME    
      bellowsVolumeSensorChangedHandler.HandleSensorChange(sensors, sensorIndex);
#endif
      break;
      
    case RightHandSensorIndex::Pitch:
      pitchPotentiometerSensorChangedHandler.HandleSensorChange(sensors, sensorIndex);
      break;

    case RightHandSensorIndex::Tone:
      tonePotentiometerSensorChangedHandler.HandleSensorChange(sensors, sensorIndex);
      break;

    case RightHandSensorIndex::Melody:
      melodyVolumeSensorChangedHandler.HandleSensorChange(sensors, sensorIndex);
      break;

    case RightHandSensorIndex::BassChord:
      bassChordVolumeSensorChangedHandler.HandleSensorChange(sensors, sensorIndex);
      break;

    default:
      DBG_PRINT_LN("RightHandSensorChangedHandler::HandleSensorChange() - Unknown sensor at index " + String(sensorIndex) + ".");
      fatalError();
      break;
  }
}

#endif // BUILD_RIGHT_HAND_MASTER
