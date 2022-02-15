/*******************************************************************************
  ToneButtonManager.cpp
  
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

#include <ardumidi.h>

#include "Button.h"
#include "MIDIAccordion.h"
#include "ToneButtonManager.h"
#include "Utilities/Utilities.h"
#include "SharedMacros.h"
#include "SharedConstants.h"
#include "VolumeChangeManager.h"

extern VolumeChangeManager gVolumeChangeManager;

// This class is used by the Right Hand Arduino to keep track of the Tone Button states.
// If the state changes, this class reacts to the change depending on which switch was toggled.
// If toggle from Active to Inactive:
// - ToneButtonRole::MelodyLayer1Enabled: Sends All Notes Off on MIDI Channel corresponding to RH Layer 1.
// - ToneButtonRole::MelodyLayer2Enabled: Sends All Notes Off on MIDI Channel corresponding to RH Layer 2.
// - ToneButtonRole::MelodyLayer3Enabled: Sends All Notes Off on MIDI Channel corresponding to RH Layer 3.
// - ToneButtonRole::MelodyLayer4Enabled: Sends All Notes Off on MIDI Channel corresponding to RH Layer 4.
// If toggle to either state:
// - ToneButtonRole::BellowsControlledVolumeEnabled: Update MIDI Volume.
ToneButtonManager::ToneButtonManager()
{
}

// This method sets the state of the Tone Button at the button index passed in.
void ToneButtonManager::SetIsActive(byte buttonIndex, bool isActive)
{
  if (buttonIndex > ToneButtonRole::Last)
  {
    fatalError();
  }

  // Tone buttons do not send MIDI notes; they send control information, or set flags used by other entities.
  // The following corresponds to switches on the Tombo Accordix electronic accordion.
  mToneButtonStates[buttonIndex] = isActive;
  DBG_PRINT_LN("ToneButtonManager::SetState() - toneButtonStates[" + String(buttonIndex) + "] = " + String(mToneButtonStates[buttonIndex]));

  if (!isActive)
  {
    // Make sure there are no hanging notes when the instrument button is turned off.
    switch (buttonIndex)
    {
      case ToneButtonRole::MelodyLayer1Enabled:
        SendAllNotesOffOnChannel(RightHandLayer1ZeroBasedMidiChannel);
        break;
        
      case ToneButtonRole::MelodyLayer2Enabled:
        SendAllNotesOffOnChannel(RightHandLayer2ZeroBasedMidiChannel);
        break;

      case ToneButtonRole::MelodyLayer3Enabled:
        SendAllNotesOffOnChannel(RightHandLayer3ZeroBasedMidiChannel);
        break;

      case ToneButtonRole::MelodyLayer4Enabled:
        SendAllNotesOffOnChannel(RightHandLayer4ZeroBasedMidiChannel);
        break;
     }
  }

  // Regardless of button state...
  switch (buttonIndex)
  {
    case ToneButtonRole::BellowsControlledVolumeEnabled:

      const bool IsForceUpdate = true;
      gVolumeChangeManager.UpdateMidiVolumeControl(IsForceUpdate);
      break;
  }
}

// This method return an indication whether the Tone Button is active.
bool ToneButtonManager::GetIsActive(ToneButtonRole toneButtonRole)
{
#ifdef ENABLE_ALL_TONE_SWITCH_BUTTONS
  return true;
#endif

  int buttonIndex = (int)toneButtonRole;
  // DBG_PRINT_LN("ToneButtonManager::GetIsActive() - toneButtonStates[" + String(buttonIndex) + "] = " + String(toneButtonStates[buttonIndex]));

  return mToneButtonStates[buttonIndex];
}

// This method sends MIDI All Notes Off CC message on zero-based MIDI Channel passed in.
void ToneButtonManager::SendAllNotesOffOnChannel(byte channelZeroBased)
{
#ifdef SEND_MIDI
  // Channel Volume Control affects only once channel.
  const byte ChannelAllNotesOffControl = 123;
  const byte AllNotesOffValue = 0; // Per MIDI Spec, https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message

  midi_controller_change(channelZeroBased, ChannelAllNotesOffControl, AllNotesOffValue);
#else
  DBG_PRINT_LN("ToneButtonManager::SendAllNotesOffOnChannel() - Zero-Based Channel = 0x" + String(channelZeroBased) + ".");
#endif
}