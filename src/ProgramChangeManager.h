/*******************************************************************************
  ProgramChangeManager.h
  
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

#ifndef ProgramChangeManager_H
#define ProgramChangeManager_H

// This class is used by the Right Hand Arduino to keep track of the current program number, 
// increment/decrement the current program number, and send the current program number Program Change message.
class ProgramChangeManager  {

public:

  // This method is the default constructor.
  ProgramChangeManager();

  // This method increments the program number member variable. It does not send the Program Change message.
  void IncrementProgramNumber();

  // This method decrements the program number member variable. It does not send the Program Change message.
  void DecrementProgramNumber();
  
  // This method sets the program number member variable to 0. It does not send the Program Change message.
  void ResetProgramNumber();

  // This method sends the current program number on the zero-based MIDI channel, passed in.
  void SendCurrentProgramNumberChange(uint8_t zeroBasedMidiChannel);

  // This method returns the zero-based MIDI channel corresponding to the highest enabled Melody Layer.
  // If no layers are enabled, this method returns 0.
  uint8_t GetHighestEnabledLayersChannel();

private:
  uint8_t mCurMidiProgramNum = 0;
};

#endif
