/*******************************************************************************
  SharedConstants.h
  
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

#ifndef SharedConstants_H
#define SharedConstants_H

#include <Arduino.h>

// Constants
const int LedPin = 13;
const uint32_t MidiEventFlashDurationMilliseconds = 10;

// MIDI Baud rate is 31250 bits per second.
const unsigned long BaudRateMidi = 31250;

// 9600 is the default rate. This rate is used to show that after uploading a new program, a previous session's debug message is still in the serial buffer.
const unsigned long BaudRateSerialMonitor = 9600;
// const unsigned long BaudRateSerialMonitor = 115200;

const byte LeftHandI2CDeviceId = 1;

const int NumRightHandButtons = 41;
const int NumLeftHandButtons = 38;

const int NumLeftHandSensors = 0;
const int NumRightHandSensors = 5; // Bellows Slide Pot, and 4 Rotary Potentiometers.

// The debounce time, in milliseconds. This is the duration to ignore button state changes.
// It is an unsigned longs because the time, measured in milliseconds, will quickly become a bigger number than can be stored in an int.
const unsigned long DebounceDelayMs = 25;

// Number of data bytes (button bit mask bytes) expected from LH Arduino.
// 2 Bytes for Bass Button bit mask.
// 2 Bytes for Chord Button bit mask.
// 2 Bytes for Tone Button bit mask.
const int NumBytesExpectedFromLeftHandArduino = 6;

// Zero-based MIDI Channels.
// MIDI channel from 0 to 15 ( 1-16 in "real world")
const uint8_t RightHandLayer1ZeroBasedMidiChannel = 0; // MIDI Channel 1
const uint8_t RightHandLayer2ZeroBasedMidiChannel = 4; // MIDI Channel 5
const uint8_t RightHandLayer3ZeroBasedMidiChannel = 5; // MIDI Channel 6
const uint8_t RightHandLayer4ZeroBasedMidiChannel = 6; // MIDI Channel 7
const uint8_t BassNotesZeroBasedMidiChannel = 1; // MIDI Channel 2
const uint8_t ChordsZeroBasedMidiChannel = 3; // MIDI Channel 4

const uint8_t DefaultVelocity = 127;

#endif
