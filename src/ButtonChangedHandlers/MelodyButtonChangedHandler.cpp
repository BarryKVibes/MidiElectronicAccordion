/*******************************************************************************
  MelodyButtonChangedHandler.cpp
  
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

#include "MelodyButtonChangedHandler.h"

#ifdef BUILD_RIGHT_HAND_MASTER

#include "../ToneButtonManager.h"
#include "../ProgramChangeManager.h"
#include "../SharedMacros.h"
#include "../SharedConstants.h"

extern ToneButtonManager gToneButtonManager; // TODO: Inject dependency.
extern ProgramChangeManager gProgramChangeManager; // TODO: Inject dependency.

// The following is used to map pin index to MIDI note number.
const byte LowestNote = MIDI_F - MIDI_OCTAVE;

// The following are the keys that are used to decrement or increment the current program number.
const byte NoteForDecrementProgramNumber = MIDI_F + MIDI_SHARP - MIDI_OCTAVE;
const byte NoteForIncrementProgramNumber = MIDI_G + MIDI_SHARP - MIDI_OCTAVE;
const byte ButtonIndexForDecrementProgramNumber = 1;
const byte ButtonIndexForIncrementProgramNumber = 3;

// This class handles Right Hand Arduino note button changes. 
// It sends corresponding MIDI Note On/Off commands.
MelodyButtonChangedHandler::MelodyButtonChangedHandler() : NoteButtonChangedHandler()
{
  mMidiChannel = RightHandLayer1ZeroBasedMidiChannel;
}

void MelodyButtonChangedHandler::HandleButtonChange(Button* buttons, byte buttonIndex)
{
  bool isKeyDown = buttons[buttonIndex].buttonState.active;

  // DBG_PRINT_LN("MelodyButtonChangedHandler::HandleButtonChange() - buttons["+String(buttonIndex)+"] @ Pin "+String(buttons[buttonIndex].buttonState.pin)+"= "+String(buttons[buttonIndex].buttonState.active)+".");

  // Convert button index to MIDI note number.
  byte noteNum = LowestNote + buttonIndex;

  // If switch to enable Program Change by Keyboard (F# = Decrement, G# = Increment) is active,
  // check whether F# or G#, and Dec/Inc Program Number.
  if (gToneButtonManager.GetIsActive(ToneButtonRole::ProgramChangeByKeyboardEnabled))
  {
    // Program Change message requires a MIDI Channel; for Mac MainStage, changing the patch, we can use MIDI Channel 0.
    // For a Multitimbral device, MIDI Channel is important, and this code would need to be modified to send
    // Program Change on multiple MIDI Channels.
    const uint8_t ZeroBasedMidiChannelForProgramChange = 0;

    // If low F# or G#, send decrement, or increment program number and send program change MIDI message.
    if (noteNum == NoteForDecrementProgramNumber)
    {
      // Change program upon KeyDown, otherwise ignore.
      if (isKeyDown)
      {
        bool isOtherKeyDown = buttons[ButtonIndexForIncrementProgramNumber].buttonState.active;
        if (isOtherKeyDown)
        {
          gProgramChangeManager.ResetProgramNumber();
        }
        else
        {
          gProgramChangeManager.DecrementProgramNumber();
        }

        gProgramChangeManager.SendCurrentProgramNumberChange(ZeroBasedMidiChannelForProgramChange);
      }

      return;
    }

    if (noteNum == NoteForIncrementProgramNumber)
    {
      // Change program upon KeyDown, otherwise ignore.
      if (isKeyDown)
      {
        bool isOtherKeyDown = buttons[ButtonIndexForDecrementProgramNumber].buttonState.active;
        if (isOtherKeyDown)
        {
          gProgramChangeManager.ResetProgramNumber();
        }
        else
        {
          gProgramChangeManager.IncrementProgramNumber();
        }

        gProgramChangeManager.SendCurrentProgramNumberChange(ZeroBasedMidiChannelForProgramChange);
      }

      return;
    }
  }

  // Send RH MIDI notes.
  if (gToneButtonManager.GetIsActive(ToneButtonRole::MelodyLayer1Enabled))
  {
    SendMidiNoteCommand(noteNum, isKeyDown, RightHandLayer1ZeroBasedMidiChannel, "MelodyButtonChangedHandler");
  }

  if (gToneButtonManager.GetIsActive(ToneButtonRole::MelodyLayer2Enabled))
  {
    SendMidiNoteCommand(noteNum, isKeyDown, RightHandLayer2ZeroBasedMidiChannel, "MelodyButtonChangedHandler");
  }

  if (gToneButtonManager.GetIsActive(ToneButtonRole::MelodyLayer3Enabled))
  {
    SendMidiNoteCommand(noteNum, isKeyDown, RightHandLayer3ZeroBasedMidiChannel, "MelodyButtonChangedHandler");
  }

  if (gToneButtonManager.GetIsActive(ToneButtonRole::MelodyLayer4Enabled))
  {
    SendMidiNoteCommand(noteNum, isKeyDown, RightHandLayer4ZeroBasedMidiChannel, "MelodyButtonChangedHandler");
  }
}

#endif // BUILD_RIGHT_HAND_MASTER