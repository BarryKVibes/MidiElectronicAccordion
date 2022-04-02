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

enum StatusIndicatorMode
{
  // Keep LED on while any note button is depressed. Does not indicate any other MIDI message type.
  // Use this mode to debug stuck notes.
  OnWhileAnyNoteButtonDepressed,

  // Momentarily flash LED for every sent MIDI event.
  FlashMidiEvents,
};

enum MidiEventType
{
  NoteOn,
  NoteOff,
  Other
};

// This class provides manages the MIDI Status indicator. 
class StatusManager
{ 
private:
  StatusIndicatorMode mStatusIndicatorMode = StatusIndicatorMode::OnWhileAnyNoteButtonDepressed;

public:
  StatusManager();

  // Reads right-hand key combinations to set the Status Indicator Mode.
  // No key - Default mode (FlashMidiEvents)
  // Lowest F - OnWhileAnyNoteOn
  void DetectStatusIndicatorMode();

  void OnMidiEvent(MidiEventType midiEventType, uint8_t value, uint8_t channel);

  void UpdateStatusIndicator();

private:
  bool IsAnyNoteOn();
};

#endif // BUILD_RIGHT_HAND_MASTER

#endif
