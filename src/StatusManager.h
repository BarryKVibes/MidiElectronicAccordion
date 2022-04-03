/*******************************************************************************
  StatusManager.h
  
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

#ifndef StatusManager_H
#define StatusManager_H

#ifdef BUILD_RIGHT_HAND_MASTER

#include "SharedConstants.h"

enum StatusIndicatorMode
{
  // Momentarily flash LED for every sent MIDI event. This is the default Status Indicator Mode.
  FlashMidiEvents,

  // Keeps the Status LED on while any note button is depressed.
  // Use this mode to debug stuck notes.
  OnWhileAnyNoteButtonDepressed
};

enum MidiEventType
{
  NoteOn,
  NoteOff,
  Other
};

// This class manages the Status Indicator LED.
class StatusManager
{ 
private:
  const StatusIndicatorMode DefaultStatusIndicatorMode = StatusIndicatorMode::FlashMidiEvents;
  StatusIndicatorMode mStatusIndicatorMode = DefaultStatusIndicatorMode;

  // Tracks Note On states for MIDI notes. 128 Notes packed into eight 32-bit bytes. Size = 8 * sizeof(uint32_t) = 32 Bytes.
  static const uint8_t NumNoteFlagBanks = MaxMidiNotes / NumMidiChannels;
  uint32_t mNoteOnFlags[NumNoteFlagBanks][NumMidiChannels];

public:
  StatusManager();

  // This method sets the Status Indicator Mode.
  void SetStatusIndicatorMode(StatusIndicatorMode mode);

  // This method should be called when any MIDI Event is sent.
  void OnMidiEvent(MidiEventType midiEventType, uint8_t value, uint8_t channel);

  // This method should periodically be called to update the Status Indicator LED.
  void UpdateStatusIndicator();

  // Clears the Note On Flags for the zero-based MIDI Channel, passed in.
  void ResetChannel(uint8_t midiChannelZeroBased);

private:
  // This method returns an indication whether any notes are on.
  bool IsAnyNoteOn();

  // Clears the Note On Flags for all MIDI Channels.
  void ResetAllChannels();
};

#endif // BUILD_RIGHT_HAND_MASTER

#endif
