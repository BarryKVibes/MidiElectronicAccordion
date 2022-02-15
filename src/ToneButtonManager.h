/*******************************************************************************
  ToneButtonManager.h
  
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

#ifndef ToneButtonManager_H
#define ToneButtonManager_H

#include <Arduino.h>

#include "Button.h"

// Enum that converts Tone Button name to button index.
enum ToneButtonRole
{
  // 00 (On)  - Set ModulationWheelValue to 75%. [Deep Vibrato]
  // 00 (Off) - Set ModulationWheelValue to 25%. [Light Vibrato]
  DeepVibrato = 0,

  // 01 (On/Off)  - TBD [Fast/Slow Vibrato] - TBD: Sustain pedal on/off to control Leslie speed?
  FastVibrato = 1,

  // 02 (On)  - Set Modulation Wheel control to the value of ModulationWheelValue variable. [Vibrato On]
  // 02 (Off) - Set Modulation Wheel control to 0. [Vibrato Off]
  VibratoEnabled = 2,

  // 03 (On/Off) - Enable/Disable sending RH notes on MIDI Channel 1. [Low 1 On/Off]
  MelodyLayer1Enabled = 3, 

  // 04 (On/Off) - Enable/Disable sending RH notes on MIDI Channel 5. [Low 2 On/Off]
  MelodyLayer2Enabled = 4, 

  // 05 (On/Off) - Enable/Disable sending RH notes on MIDI Channel 6. [Low 3 On/Off]
  MelodyLayer3Enabled = 5, 
  
  // 06 (On/Off) - Enable/Disable sending RH notes on MIDI Channel 7. [Middle 1 On/Off]
  MelodyLayer4Enabled = 6, 
  
  // 07 (On/Off)  - TBD [Middle 2 On/Off]
  TBD07Enabled = 7,
  
  // 08 (On/Off)  - TBD [Middle 3 On/Off]
  TBD08Enabled = 8,
  
  // 09 (On/Off)  - TBD [High 1 On/Off]
  TBD09Enabled = 9,

  // 10 (On/Off)  - TBD [High 2 On/Off]
  TBD10Enabled = 10,

  // 11 (On/Off)  - TBD [Bass Sustain Short/Long]
  TBD11Enabled = 11,

  // 12 (On/Off)  - Enable/Disable Bellows-Controlled Volume. When state changes, ToneButtonManager forces the Volume Change CC value to be updated. [Bass Loud/Soft]
  BellowsControlledVolumeEnabled = 12,

  // 13 (On/Off)  - Enable/Disable Program Change by Keyboard. [Chord Loud/Soft]
  ProgramChangeByKeyboardEnabled = 13,

  Last = 14
};

class ToneButtonManager  {
  
public:
  ToneButtonManager();

  void SetIsActive(byte buttonIndex, bool isActive);
  bool GetIsActive(ToneButtonRole toneButtonRole);
  void SendAllNotesOffOnChannel(byte channelZeroBased);
  
private:
  bool mToneButtonStates[14] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false};
};

#endif
