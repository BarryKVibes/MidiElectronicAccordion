/*******************************************************************************
  main.cpp
  
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

// This file is the main Arduino file which includes the setup() and loop() functions.
// It also includes objects used by other files, including Button and Sensor definitions.

#include <Arduino.h>

#include "MIDIAccordion.h" // MIDIAccordion.h contains compiler directives.

#if defined(BUILD_RIGHT_HAND_MASTER)
  #include "SetupManagers/RightHandSetupManager.h"
  #include "ButtonChangedHandlers/MelodyButtonChangedHandler.h"
  #include "ButtonChangedHandlers/ProgramChangeButtonChangedHandler.h"
  #include "SensorChangedHandlers/RightHandSensorChangedHandler.h"
  #include "VolumeChangeManager.h"
  #include "ProgramChangeManager.h"
  #include "MIDIEventFlasher.h"
#elif defined(BUILD_LEFT_HAND_SLAVE)
  #include "SetupManagers/LeftHandSetupManager.h"
  #include "ButtonChangedHandlers/LeftHandButtonChangedHandler.h"
#else
  #error "BUILD_RIGHT_HAND_MASTER or BUILD_LEFT_HAND_SLAVE is not defined. Need to include MIDIAccordion.h"
#endif

#include "ButtonsManager.h"
#include "Utilities/Utilities.h"
#include "SharedConstants.h"
#include "SharedMacros.h"

#ifndef SEND_MIDI
  #include "Utilities/Diagnostics.h"
#endif

// Left Hand Button configuration. This is used by both Left and Right Hand Arduinos, hence outside of #ifdef.
Button leftHandButtons[NumLeftHandButtons] = {
  // Button {ButtonState buttonState, unsigned long lastToggleTimeMs} where
  // ButtonState {active, pin, unused0, unused1, unused2}

  // Note Indexes 00-11 (Bass)
  {{false, 22, 0, 0, 0}, 0}, {{false, 23, 0, 0, 0}, 0}, {{false, 24, 0, 0, 0}, 0}, {{false, 25, 0, 0, 0}, 0},
  {{false, 26, 0, 0, 0}, 0}, {{false, 27, 0, 0, 0}, 0}, {{false, 28, 0, 0, 0}, 0}, {{false, 29, 0, 0, 0}, 0},
  {{false, 30, 0, 0, 0}, 0}, {{false, 31, 0, 0, 0}, 0}, {{false, 32, 0, 0, 0}, 0}, {{false, 33, 0, 0, 0}, 0},

  // Note Indexes 12-23 (Chords)
  {{false, 34, 0, 0, 0}, 0}, {{false, 35, 0, 0, 0}, 0}, {{false, 36, 0, 0, 0}, 0}, {{false, 37, 0, 0, 0}, 0},
  {{false, 38, 0, 0, 0}, 0}, {{false, 39, 0, 0, 0}, 0}, {{false, 40, 0, 0, 0}, 0}, {{false, 41, 0, 0, 0}, 0},
  {{false, 42, 0, 0, 0}, 0}, {{false, 43, 0, 0, 0}, 0}, {{false, 44, 0, 0, 0}, 0}, {{false, 45, 0, 0, 0}, 0},

  // Toggle Switches Indexes 24-37
  {{false, 46, 0, 0, 0}, 0}, {{false, 47, 0, 0, 0}, 0}, {{false, 48, 0, 0, 0}, 0}, {{false, 49, 0, 0, 0}, 0},
  {{false, 50, 0, 0, 0}, 0}, {{false, 51, 0, 0, 0}, 0}, {{false, 52, 0, 0, 0}, 0}, {{false, 2, 0, 0, 0}, 0},
  {{false, 3, 0, 0, 0}, 0}, {{false, 4, 0, 0, 0}, 0}, {{false, 5, 0, 0, 0}, 0}, {{false, 6, 0, 0, 0}, 0},
  {{false, 7, 0, 0, 0}, 0}, {{false, 8, 0, 0, 0}, 0}
  };

#if defined(BUILD_RIGHT_HAND_MASTER)

// Right Hand Button configuration. Only used when built for BUILD_RIGHT_HAND_MASTER.
Button rightHandButtons[NumRightHandButtons] = {
  // Button {ButtonState buttonState, unsigned long lastToggleTimeMs} where
  // ButtonState {active, pin, unused0, unused1, unused2}

  // Notes 01-12
  {{false, 22, 0, 0, 0}, 0}, {{false, 23, 0, 0, 0}, 0}, {{false, 24, 0, 0, 0}, 0}, {{false, 25, 0, 0, 0}, 0},
  {{false, 26, 0, 0, 0}, 0}, {{false, 27, 0, 0, 0}, 0}, {{false, 28, 0, 0, 0}, 0}, {{false, 29, 0, 0, 0}, 0},
  {{false, 30, 0, 0, 0}, 0}, {{false, 31, 0, 0, 0}, 0}, {{false, 32, 0, 0, 0}, 0}, {{false, 33, 0, 0, 0}, 0},

  // Notes 13-24
  {{false, 34, 0, 0, 0}, 0}, {{false, 35, 0, 0, 0}, 0}, {{false, 36, 0, 0, 0}, 0}, {{false, 37, 0, 0, 0}, 0},
  {{false, 38, 0, 0, 0}, 0}, {{false, 39, 0, 0, 0}, 0}, {{false, 40, 0, 0, 0}, 0}, {{false, 41, 0, 0, 0}, 0},
  {{false, 42, 0, 0, 0}, 0}, {{false, 43, 0, 0, 0}, 0}, {{false, 44, 0, 0, 0}, 0}, {{false, 45, 0, 0, 0}, 0},

  // Notes 25-30
  {{false, 46, 0, 0, 0}, 0}, {{false, 47, 0, 0, 0}, 0}, {{false, 48, 0, 0, 0}, 0}, {{false, 49, 0, 0, 0}, 0},
  {{false, 50, 0, 0, 0}, 0}, {{false, 51, 0, 0, 0}, 0}, {{false, 52, 0, 0, 0}, 0}, {{false, 53, 0, 0, 0}, 0},

  // Notes 31-41
  {{false, 2, 0, 0, 0}, 0}, {{false, 3, 0, 0, 0}, 0}, {{false, 4, 0, 0, 0}, 0}, {{false, 5, 0, 0, 0}, 0},
  {{false, 6, 0, 0, 0}, 0}, {{false, 7, 0, 0, 0}, 0}, {{false, 8, 0, 0, 0}, 0}, {{false, 9, 0, 0, 0}, 0},
  {{false, 10, 0, 0, 0}, 0},

  // Custom Buttons; Decrement and Increment Program Number.
  {{false, 11, 0, 0, 0}, 0}, {{false, 12, 0, 0, 0}, 0}
  };

// Right Hand Sensor configuration.
Sensor rightHandSensors[NumRightHandSensors] = {
  // Sensor {SensorState sensorState} where
  // SensorState {value, pin}

  // Potentiometers
  {0xFFFF, A0}, // Bellows Volume
  {0xFFFF, A1}, // Pitch Potentiometer
  {0xFFFF, A2}, // Tone Potentiometer
  {0xFFFF, A3}, // Melody Volume
  {0xFFFF, A4}, // Bass/Chord Volume
  };

ButtonsManager* pButtonsManager = new ButtonsManager(leftHandButtons, rightHandButtons, NULL, rightHandSensors);

// RightHandLoopHandler loopHandler;
RightHandSetupManager setupManager;
MelodyButtonChangedHandler melodyButtonChangedHandler;
ProgramChangeButtonChangedHandler programChangeButtonChangedHandler;
RightHandSensorChangedHandler sensorChangedHandler;
VolumeChangeManager gVolumeChangeManager;
ProgramChangeManager gProgramChangeManager;
MIDIEventFlasher gMIDIEventFlasher;

#elif defined(BUILD_LEFT_HAND_SLAVE)
ButtonsManager* pButtonsManager = new ButtonsManager(leftHandButtons, NULL, NULL, NULL);
LeftHandButtonChangedHandler leftHandButtonChangedHandler;
// LeftHandSensorChangedHandler sensorChangedHandler;
LeftHandSetupManager setupManager;
#endif

#ifndef SEND_MIDI
Diagnostics diagnostics;
#endif

// This function is called once, upon startup.
void setup()
{
  // Setup serial port and pin states.
  setupManager.Setup();

  DBG_PRINT_LN("MIDIAccordion::Setup() - Setup done.");

  //DBG_PRINT_LN(PrintAllButtonInfo(leftHandButtons, NumLeftHandButtons));
}

// This function is called repeatedly.
void loop()
{
#if defined(BUILD_RIGHT_HAND_MASTER)
  // LOG_LOOP_TIME();

  // Read buttons attached to Right Hand Arduino.

  // Keys
  pButtonsManager->ReadButtons(rightHandButtons, 0, 40, melodyButtonChangedHandler);

  // Custom Buttons
  pButtonsManager->ReadButtons(rightHandButtons, 41, 42, programChangeButtonChangedHandler);
  
  #ifndef DISABLE_SENSOR_READS
  pButtonsManager->ReadSensors(rightHandSensors, NumRightHandSensors, sensorChangedHandler);
  #endif // DISABLE_SENSOR_READS

  #ifndef DISABLE_I2C
  // Request LH Arduino button states.
  // DBG_PRINT_LN("Loop() BUILD_RIGHT_HAND_MASTER - Calling pButtonsManager->FetchLeftHandArduinoButtons().");
  pButtonsManager->FetchLeftHandArduinoButtons();
  #endif // DISABLE_I2C

  gMIDIEventFlasher.UpdateStatusLed();
  
#elif defined(BUILD_LEFT_HAND_SLAVE)
  // DBG_PRINT_LN("Loop() BUILD_LEFT_HAND_SLAVE - Calling pButtonsManager->ReadButtons().");
  pButtonsManager->ReadButtons(leftHandButtons, NumLeftHandButtons, buttonChangedHandler);
  // Uncomment if using sensors in the LH Arduino. pButtonsManager->ReadSensors(leftHandSensors, NumLeftHandSensors, sensorChangedHandler);
#endif
}

