/*******************************************************************************
  VolumeChangeManager.cpp
  
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

#include "lib/ArduMidi/ardumidi.h"

#include "MIDIAccordion.h"

#include "VolumeChangeManager.h"
#include "Utilities/Utilities.h"
#include "SharedMacros.h"
#include "SharedConstants.h"
#include "ToneButtonManager.h"

const uint8_t MinMidiVolumeValueDifference = 2;

// Channel Volume Control affects only one channel.
const uint8_t ChannelVolumeControl = 0x07;

extern ToneButtonManager gToneButtonManager; // TODO: Inject dependency.

// This class is used by the Right Hand Arduino to keep track of the Tone Button states.
VolumeChangeManager::VolumeChangeManager()
{
}

// Sets the current MIDI Control Volume for the Volume Control Type, passed in.
// If the volume value changes significantly, this method sends a Volume Control message on the active channels corresponding to the VolumeControlType.
void VolumeChangeManager::SetCurrentMidiControlVolume(VolumeControlType volumeControlType, byte midiVolumeValue)
{
  //DBG_PRINT_LN("VolumeChangeManager::SetCurrentMidiControlVolume() - VolumeControlType = " + String(volumeControlType) + "; midiVolumeValue = " + String(midiVolumeValue) + ".");
#ifdef IGNORE_BELLOWS_VOLUME
if (volumeControlType == VolumeControlType::Bellows)
{
  mCurMidiVolumeValue[volumeControlType] = MaxVolume;
}
else
{
  mCurMidiVolumeValue[volumeControlType] = midiVolumeValue;
}
#else
  mCurMidiVolumeValue[volumeControlType] = midiVolumeValue;
#endif

  const bool IsForceUpdate = false;
  UpdateMidiVolumeControl(IsForceUpdate);
}

// Returns the current MIDI Volume Control value.
uint8_t VolumeChangeManager::GetCurrentMidiControlVolume(VolumeControlType volumeControlType)
{
  if (volumeControlType == VolumeControlType::Bellows)
  {
    return GetBellowsVolume();
  }

  return mCurMidiVolumeValue[volumeControlType];
}

// Returns the last sent MIDI Volume Control value.
uint8_t VolumeChangeManager::GetLastSentMidiControlVolume(VolumeControlType volumeControlType)
{
  return mLastSentMidiVolumeValue[volumeControlType];
}

void VolumeChangeManager::SendMidiVolumeChangeOnChannel(byte midiVolumeValue, byte channelZeroBased)
{
#ifdef SEND_MIDI
  midi_controller_change(channelZeroBased, ChannelVolumeControl, midiVolumeValue);
#else
  DBG_PRINT_LN("VolumeChangeManager::SendMidiVolumeChangeOnChannel() - Sending MIDI Volume: 0x" + String(midiVolumeValue, HEX) + " = " + String(midiVolumeValue) + " on Channel " + String(channelZeroBased + 1) + ".");
#endif
}

bool VolumeChangeManager::IsVolumeChangedSignificantly(VolumeControlType volumeControlType)
{
  uint8_t lastSentMidiVolumeValue = mLastSentMidiVolumeValue[volumeControlType];
  uint8_t curMidiVolumeValue = GetCurrentMidiControlVolume(volumeControlType);

  // Send Volume Change only if difference between previous and current values is significant.
  byte volumeDifference;
  if (lastSentMidiVolumeValue > curMidiVolumeValue)
  {
    volumeDifference = lastSentMidiVolumeValue - curMidiVolumeValue;
  }
  else
  {
    volumeDifference = curMidiVolumeValue - lastSentMidiVolumeValue;
  }

  if (volumeDifference >= MinMidiVolumeValueDifference)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// Sends MIDI Volume Control Message on corresponding active channels, if difference between previous and current values is significant.
// If forceUpdate is passed true, send MIDI volume regardless of significant change.
// It averages Bellows + Melody volumes before sending Volume Change CC Message to Melody MIDI Channels 1, 5, 6, 7.
// It averages Bellows + Bass volumes before sending Volume Change CC Message to Bass MIDI Channel 2, and Chord MIDI Channel 4.
void VolumeChangeManager::UpdateMidiVolumeControl(bool forceUpdate)
{
  if (!mIsAllVolumesInitialized)
  {
    // Prevent update if any of the MIDI Volumes have not been initialized.
    if (mCurMidiVolumeValue[VolumeControlType::Bellows] == UninitializedMidiVolume ||
        mCurMidiVolumeValue[VolumeControlType::Melody] == UninitializedMidiVolume ||
        mCurMidiVolumeValue[VolumeControlType::BassChord] == UninitializedMidiVolume)
    {
      DBG_PRINT_LN("VolumeChangeManager::UpdateMidiVolumeControl() - Bellows: " + String(mCurMidiVolumeValue[VolumeControlType::Bellows]) + "; Melody: " + String(mCurMidiVolumeValue[VolumeControlType::Melody]) + "; BassChord: " + String(mCurMidiVolumeValue[VolumeControlType::BassChord]) + ".");

      return;
    }

    DBG_PRINT_LN("VolumeChangeManager::UpdateMidiVolumeControl() - All curMidiVolumeValues have been initialized.");
    mIsAllVolumesInitialized = true;
  }

#ifdef IGNORE_BELLOWS_VOLUME
  bool isBellowsVolumeChangedSignificantly = false;
#else
  bool isBellowsVolumeChangedSignificantly = IsVolumeChangedSignificantly(VolumeControlType::Bellows);
#endif
  bool isMelodyVolumeChangedSignificantly = IsVolumeChangedSignificantly(VolumeControlType::Melody);
  bool isBassChordVolumeChangedSignificantly = IsVolumeChangedSignificantly(VolumeControlType::BassChord);

#ifdef IGNORE_BELLOWS_VOLUME
    bool useBellowsVolume = false;
#else
    bool useBellowsVolume = gToneButtonManager.GetIsActive(ToneButtonRole::BellowsControlledVolumeEnabled);
#endif // IGNORE_BELLOWS_VOLUME

  // Did Bellows or Melody volumes change significantly?
  if (isBellowsVolumeChangedSignificantly || isMelodyVolumeChangedSignificantly || forceUpdate)
  {
    // Get average MIDI Volume between Bellows and Melody Volume Controls.
    uint8_t bellowsVolume = GetCurrentMidiControlVolume(VolumeControlType::Bellows);
    uint8_t melodyVolume = GetCurrentMidiControlVolume(VolumeControlType::Melody);

    uint16_t avgMidiVolume;
    if (useBellowsVolume)
    {
      avgMidiVolume = (bellowsVolume + melodyVolume) / 2;
    }
    else
    {
      avgMidiVolume = melodyVolume;
    }

    mLastSentMidiVolumeValue[VolumeControlType::Bellows] = bellowsVolume;
    mLastSentMidiVolumeValue[VolumeControlType::Melody] = melodyVolume;

    DBG_PRINT_LN("VolumeChangeManager::UpdateMidiVolumeControl() - bellowsVolume = " + String(bellowsVolume) + "; melodyVolume = " + String(melodyVolume) + ".");

    // Set MIDI Volume for all Melody Layer MIDI Channels, not just the active ones, so that when it becomes active, the volume will be set.
    SendMidiVolumeChangeOnChannel(avgMidiVolume, RightHandLayer1ZeroBasedMidiChannel);
    SendMidiVolumeChangeOnChannel(avgMidiVolume, RightHandLayer2ZeroBasedMidiChannel);
    SendMidiVolumeChangeOnChannel(avgMidiVolume, RightHandLayer3ZeroBasedMidiChannel);
    SendMidiVolumeChangeOnChannel(avgMidiVolume, RightHandLayer4ZeroBasedMidiChannel);
  }

  // Did Bellows or Bass/Chord volumes change significantly?
  if (isBellowsVolumeChangedSignificantly || isBassChordVolumeChangedSignificantly || forceUpdate)
  {
    uint8_t bellowsVolume = GetCurrentMidiControlVolume(VolumeControlType::Bellows);
    uint8_t bassChordVolume = GetCurrentMidiControlVolume(VolumeControlType::BassChord);

    uint16_t avgMidiVolume;
    if (useBellowsVolume)
    {
      avgMidiVolume = (bellowsVolume + bassChordVolume) / 2;
    }
    else
    {
      avgMidiVolume = bassChordVolume;
    }

    mLastSentMidiVolumeValue[VolumeControlType::Bellows] = bellowsVolume;
    mLastSentMidiVolumeValue[VolumeControlType::BassChord] = bassChordVolume;

    DBG_PRINT_LN("VolumeChangeManager::UpdateMidiVolumeControl() - bellowsVolume = " + String(bellowsVolume) + "; bassChordVolume = " + String(bassChordVolume) + ".");

    // Set MIDI Volume for Bass MIDI Channel, and BassChord MIDI Channel.
    SendMidiVolumeChangeOnChannel(avgMidiVolume, BassNotesZeroBasedMidiChannel);
    SendMidiVolumeChangeOnChannel(avgMidiVolume, ChordsZeroBasedMidiChannel);
  }
}

// Returns the Bellows volume. If the Bellows-Controlled Volume switch is enabled, this method returns the MIDI volume value 
// based on the Bellows potentiometer, otherwise returns the maximum volume.
uint8_t VolumeChangeManager::GetBellowsVolume()
{
  uint8_t bellowsVolume;

#ifdef IGNORE_BELLOWS_VOLUME
  bellowsVolume = MaxVolume;
  return bellowsVolume;
#endif

  // If Bellows-Controlled Volume switch is enabled, set bellows volume to the Bellows Potentiometer volume, otherwise use Max volume.
  if (gToneButtonManager.GetIsActive(ToneButtonRole::BellowsControlledVolumeEnabled))
  {
    // DBG_PRINT_LN("VolumeChangeManager::GetBellowsVolume() - BellowsControlledVolumeEnabled is ENABLED.");
    bellowsVolume = mCurMidiVolumeValue[VolumeControlType::Bellows];
  }
  else
  {
    // DBG_PRINT_LN("VolumeChangeManager::GetBellowsVolume() - BellowsControlledVolumeEnabled is DISABLED.");
    bellowsVolume = MaxVolume;
  }

  return bellowsVolume;
}
