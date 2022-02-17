/*******************************************************************************
  MIDIEventFlasher.h
  
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

#ifndef MIDIEventFlasher_H
#define MIDIEventFlasher_H

#include <Arduino.h>

// This class displays an indication that a recent MIDI event was sent.
// This class flashes the LED on Pin 13 indicating that a recent MIDI event was sent.
// The LED is on for a duration set by the constant, MidiEventFlashDurationMilliseconds.
class MIDIEventFlasher  {

public:
  // This method is the default constructor.
  MIDIEventFlasher(); 

  // This method starts the LED turning on. The LED will turn off after a configured amount of time if there is no other request to flash.
  void OnMidiEvent();

  // This method must be called periodically in order to turn off the LED after the last request to flash the LED.
  void UpdateStatusLed();

private:

  // This member is the timestamp at which the LED was turned on. If LED is off, its value is 0.
  uint32_t mFlashOnStartTimestampMilliseconds = 0;
};

#endif
