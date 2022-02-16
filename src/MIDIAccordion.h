/*******************************************************************************
  MIDIAccordion.h
  
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

#ifndef MIDIAccordion_H
#define MIDIAccordion_H

// This project contains the logic for both the Right Hand (master) and Left Hand (slave).
// Uncomment the corresponding #define below.
#define BUILD_RIGHT_HAND_MASTER
// #define BUILD_LEFT_HAND_SLAVE

// The following compiler directives are used for debugging.
// Comment out SEND_MIDI to debug MIDI using the Serial Monitor.
#define SEND_MIDI

// Uncomment to have maximum MIDI volume when the bellows is closed, otherwise closed bellows yields min MIDI volume.
// #define MAX_MIDI_VOLUME_WHEN_BELLOWS_IS_CLOSED

// #define DISABLE_I2C
// #define DISABLE_SENSOR_READS
#define IGNORE_BELLOWS_VOLUME
// #define ENABLE_ALL_TONE_SWITCH_BUTTONS
// #define DEBUG_I2C
// #define PRINT_LH_BUTTON_FLAGS

#endif
