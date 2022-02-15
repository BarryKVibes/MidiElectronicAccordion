/*******************************************************************************
  VolumeChangeManager.h
  
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

#ifndef VolumeChangeManager_H
#define VolumeChangeManager_H

#include <Arduino.h>

class VolumeChangeManager  {

public:

  static const byte UninitializedMidiVolume = 0xFF;
  static const uint8_t MaxVolume = 0x7F;

  enum VolumeControlType
  {
    Bellows,
    Melody,
    BassChord,
    LastVolumeControlType
  };

public:
  VolumeChangeManager();

  void SetCurrentMidiControlVolume(VolumeControlType volumeControlType, byte midiVolumeValue);
  uint8_t GetCurrentMidiControlVolume(VolumeControlType volumeControlType);
  uint8_t GetLastSentMidiControlVolume(VolumeControlType volumeControlType);
  void UpdateMidiVolumeControl(bool forceUpdate);

private:
  bool IsVolumeChangedSignificantly(VolumeControlType volumeControlType);
  void SendMidiVolumeChangeOnChannel(byte midiVolumeValue, byte channelZeroBased);
  uint8_t GetBellowsVolume();

private:
  byte mCurMidiVolumeValue[VolumeControlType::LastVolumeControlType] = {UninitializedMidiVolume, UninitializedMidiVolume, UninitializedMidiVolume};
  byte mLastSentMidiVolumeValue[VolumeControlType::LastVolumeControlType] = {UninitializedMidiVolume, UninitializedMidiVolume, UninitializedMidiVolume};
  bool mIsAllVolumesInitialized = false;
};

#endif
