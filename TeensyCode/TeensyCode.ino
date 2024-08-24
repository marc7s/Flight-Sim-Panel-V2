/*
   Flight Sim Panel V2
   Created by Aaron Cobb, Refactored by Marcus Schagerberg
   https://www.cobbinnovation.com
   Board Used:		Teensy 4.1
   USB Type:		Flight Sim Controls + Joystick
   Upper Panel: PNL-MSTR-Auto-Trim
   Lower Panel: PNL-Mags-Lights-Fuel-Xtra


Following are the mapping between the components and the bindings, where you configurate the pin (on the Teensy board) and the joystick button/hat (that will be emulated by the Teensy)

***UPPER PANEL***		*Wired to right side of board, when looking at the side with the USB and SD card slot*
Rocker Switches
	Battery			      Rocker_Binding_Battery
	Alternator	      Rocker_Binding_Alternator
	Bus 1			        Rocker_Binding_Bus1
	Bus 2			        Rocker_Binding_Bus2
Momentary Buttons Group #1
	HDG				        Button_Binding_HDG
	NAV				        Button_Binding_NAV
	APR				        Button_Binding_APR
	AP				        Button_Binding_AP
	REV				        Button_Binding_REV
	ALT				        Button_Binding_ALT
	VS				        Button_Binding_VS
Trim Momentary Toggles
	Aileron Left	    Trim_Binding_AileronLeft
	Aileron Right	    Trim_Binding_AileronRight
	Rudder Left		    Trim_Binding_RudderLeft
	Rudder Right	    Trim_Binding_RudderRight
	Elevator Up		    Trim_Binding_ElevatorUp
	Elevator Down	    Trim_Binding_ElevatorDown
***LOWER PANEL***		*Wired to left side of board, when looking at the side with the USB and SD card slot*
Toggle Switches
  Fuel Pump Left	  Toggle_Binding_FuelPumpLeft
	Fuel Pump Right	  Toggle_Binding_FuelPumpRight
	Pitot Heat		    Toggle_Binding_PitotHeat
	LNDG			        Toggle_Binding_LNDG
	TAXI			        Toggle_Binding_TAXI
	STRB			        Toggle_Binding_STRB
	NAV				        Toggle_Binding_NAV
	BCN				        Toggle_Binding_BCN
	Panel			        Toggle_Binding_Panel
Momentary Buttons Group #2 *From left to right, starting with the button between the rotary encoders*
	BetweenRotaries	  Button_Binding_BetweenRotaries
	Unlabeled 1		    Button_Binding_Unlabeled1
	Unlabeled 2		    Button_Binding_Unlabeled2
	Unlabeled 3		    Button_Binding_Unlabeled3
	Unlabeled 4		    Button_Binding_Unlabeled4
	Unlabeled 5		    Button_Binding_Unlabeled5
Rotary Encoders *Note that they are run in low performance polling mode without interrupts* Also note that you need to wire them to ground and Vcc as well, and that CLK is CW and DT is CCW
  KnobLeft CCW	    Knob_Binding_LeftCCW
	KnobLeft CW		    Knob_Binding_LeftCW
	KnobLeft Click	  Knob_Binding_LeftClick
	KnobRight CCW	    Knob_Binding_RightCCW
	KnobRight CW	    Knob_Binding_RightCW
	KnobRight Click	  Knob_Binding_RightClick
*/
#include <Bounce.h>
struct ButtonBinding {
  int Pin;
  int JoystickButton;
  Bounce* Button = nullptr;
};

struct HatBinding {
  int Pin;
  int JoystickHat;
  Bounce* Button = nullptr;
};

// ***** Pin bindings *****
// Each binding has a pin number and a joystick button/hat. Configure them here below
// The bindings also contain a pointer to the joystick emulation, which is set up later
// Most also define an array of the bindings, so that they can be iterated later. Leave these arrays unless you add or remove bindings
// ***UPPER PANEL***
// Rocker Switches
ButtonBinding Rocker_Binding_Battery =          { 17, 1 };
ButtonBinding Rocker_Binding_Alternator =       { 16, 2 };
ButtonBinding Rocker_Binding_Bus1 =             { 15, 3 };
ButtonBinding Rocker_Binding_Bus2 =             { 14, 4 };
ButtonBinding* Rocker_Switches[] = { &Rocker_Binding_Battery, &Rocker_Binding_Alternator, &Rocker_Binding_Bus1, &Rocker_Binding_Bus2 };

// Momentary Buttons Group #1
ButtonBinding Button_Binding_HDG =              { 36, 5 };
ButtonBinding Button_Binding_NAV =              { 35, 6 };
ButtonBinding Button_Binding_APR =              { 34, 7 };
ButtonBinding Button_Binding_AP =               { 33, 8 };
ButtonBinding Button_Binding_REV =              { 39, 9 };
ButtonBinding Button_Binding_ALT =              { 38, 10 };
ButtonBinding Button_Binding_VS =               { 37, 11 };
ButtonBinding* MomentaryButtons1[] = { &Button_Binding_HDG, &Button_Binding_NAV, &Button_Binding_APR, &Button_Binding_AP, &Button_Binding_REV, &Button_Binding_ALT, &Button_Binding_VS };

// Trim Momentary Toggles
HatBinding Trim_Binding_AileronLeft =           { 22, 315 };
HatBinding Trim_Binding_AileronRight =          { 23, 45 };
HatBinding Trim_Binding_RudderLeft =            { 21, 270 };
HatBinding Trim_Binding_RudderRight =           { 20, 90 };
HatBinding Trim_Binding_ElevatorUp =            { 19, 0 };
HatBinding Trim_Binding_ElevatorDown =          { 18, 180 };
HatBinding* TrimToggles[] = { &Trim_Binding_AileronLeft, &Trim_Binding_AileronRight, &Trim_Binding_RudderLeft, &Trim_Binding_RudderRight, &Trim_Binding_ElevatorUp, &Trim_Binding_ElevatorDown };

// ***LOWER PANEL***
// Toggle Switches
ButtonBinding Toggle_Binding_FuelPumpLeft =     { 0, 12 };
ButtonBinding Toggle_Binding_FuelPumpRight =    { 1, 13 };
ButtonBinding Toggle_Binding_PitotHeat =        { 8, 20 };
ButtonBinding Toggle_Binding_LNDG =             { 7, 19 };
ButtonBinding Toggle_Binding_TAXI =             { 6, 18 };
ButtonBinding Toggle_Binding_STRB =             { 5, 17 };
ButtonBinding Toggle_Binding_NAV =              { 4, 16 };
ButtonBinding Toggle_Binding_BCN =              { 3, 15 };
ButtonBinding Toggle_Binding_Panel =            { 2, 14 };
ButtonBinding* ToggleSwitches[] = { &Toggle_Binding_FuelPumpLeft, &Toggle_Binding_FuelPumpRight, &Toggle_Binding_PitotHeat, &Toggle_Binding_LNDG, &Toggle_Binding_TAXI, &Toggle_Binding_STRB, &Toggle_Binding_NAV, &Toggle_Binding_BCN, &Toggle_Binding_Panel };

// Momentary Buttons Group #2
ButtonBinding Button_Binding_BetweenRotaries =  { 27, 21 };
ButtonBinding Button_Binding_Unlabeled1 =       { 28, 22 };
ButtonBinding Button_Binding_Unlabeled2 =       { 29, 23 };
ButtonBinding Button_Binding_Unlabeled3 =       { 30, 24 };
ButtonBinding Button_Binding_Unlabeled4 =       { 31, 25 };
ButtonBinding Button_Binding_Unlabeled5 =       { 32, 26 };
ButtonBinding* MomentaryButtons2[] = { &Button_Binding_BetweenRotaries, &Button_Binding_Unlabeled1, &Button_Binding_Unlabeled2, &Button_Binding_Unlabeled3, &Button_Binding_Unlabeled4, &Button_Binding_Unlabeled5 };

// Rotary Encoders
ButtonBinding Knob_Binding_LeftCCW =            { 12, 30 };
ButtonBinding Knob_Binding_LeftCW =             { 11, 31 };
ButtonBinding Knob_Binding_LeftClick =          { 25, 32 };
ButtonBinding Knob_Binding_RightCCW =           { 10, 27 };
ButtonBinding Knob_Binding_RightCW =            { 9, 28 };
ButtonBinding Knob_Binding_RightClick =         { 24, 29 };

// ***** End of pin bindings *****

// Set up Rotary Encoders turning
#include <Encoder.h>
Encoder Knob_Left (Knob_Binding_LeftCCW.Pin, Knob_Binding_LeftCW.Pin);
long positionKnobLeft = 0;
Encoder Knob_Right (Knob_Binding_RightCCW.Pin, Knob_Binding_RightCW.Pin);
long positionKnobRight = 0;

// Toggles a joystick button, turning it on and off after a short delay
void toggleButton(int buttonID) {
  Joystick.button(buttonID, 1);
  delay(50);
  Joystick.button(buttonID, 0);
}

void setup() {
  // ***** Set up joystick bindings *****
  // 10 = 10 ms debounce time
  // Rocker Switches
  for (auto binding : Rocker_Switches)
    binding->Button = new Bounce(binding->Pin, 10);

  // Momentary Buttons Group #1
  for (auto binding : MomentaryButtons1)
    binding->Button = new Bounce(binding->Pin, 10);
  
  // Trim Momentary Toggles
  for (auto binding : TrimToggles)
    binding->Button = new Bounce(binding->Pin, 10);

  // Toggle Switches
  for (auto binding : ToggleSwitches)
    binding->Button = new Bounce(binding->Pin, 10);

  // Momentary Buttons Group #2
  for (auto binding : MomentaryButtons2)
    binding->Button = new Bounce(binding->Pin, 10);

  //Rotary Push buttons
  Knob_Binding_LeftClick.Button = new Bounce(Knob_Binding_LeftClick.Pin, 10);
  Knob_Binding_RightClick.Button = new Bounce(Knob_Binding_RightClick.Pin, 10);

  // ***** End of joystick bindings setup *****
  
  // Same as Windows default
  Joystick.Z(512);
  Joystick.X(512);
  Joystick.Y(512);
  Joystick.Zrotate(512);
  Joystick.sliderLeft(512);
  Joystick.sliderRight(512);
  Joystick.hat(-1);
  
  // ***** Set up pullup resistors on pins *****
  // Rocker Switches Pullup
  for (auto binding : Rocker_Switches)
    pinMode(binding->Pin, INPUT_PULLUP);

  // Momentary Buttons Group #1 Pullup
  for (auto binding : MomentaryButtons1)
    pinMode(binding->Pin, INPUT_PULLUP);

  // Trim Momentary Toggles Pullup
  for (auto binding : TrimToggles)
    pinMode(binding->Pin, INPUT_PULLUP);

  // Toggle Switches Pullup
  for (auto binding : ToggleSwitches)
    pinMode(binding->Pin, INPUT_PULLUP);

  // Momentary Buttons Group #2 Pullup
  for (auto binding : MomentaryButtons2)
    pinMode(binding->Pin, INPUT_PULLUP);

  // Rotary Encoders Pullup
  pinMode(Knob_Binding_LeftClick.Pin, INPUT_PULLUP);
  pinMode(Knob_Binding_RightClick.Pin, INPUT_PULLUP);

  // ***** End of pullup resistors setup *****
}

void loop() {
  // Update all the buttons.  There should not be any long delays in loop(), so this runs repetitively at a rate faster than the buttons could be pressed and released
  // ***** Joystick update *****
  // Update Rocker Switches
  for (auto binding : Rocker_Switches)
    binding->Button->update();

  // Update Momentary Buttons Group #1
  for (auto binding : MomentaryButtons1)
    binding->Button->update();

  // Update Trim Momentary Toggles
  for (auto binding : TrimToggles)
    binding->Button->update();

  // Update Toggle Switches
  for (auto binding : ToggleSwitches)
    binding->Button->update();

  // Update Momentary Buttons Group #2
  for (auto binding : MomentaryButtons2)
    binding->Button->update();

  // Update Rotary Encoders
  Knob_Binding_RightClick.Button->update();
  Knob_Binding_LeftClick.Button->update();

  // ***** End of joystick update *****
    
  // ***** Joystick status changes *****
  // Rocker Switches Update if Falling
  for (auto binding : Rocker_Switches) {
    if (binding->Button->fallingEdge() || binding->Button->risingEdge())
      toggleButton(binding->JoystickButton);
  }

  // Momentary Buttons Group #1 Update if Falling
  for (auto binding : MomentaryButtons1) {
    if (binding->Button->fallingEdge())
      Joystick.button(binding->JoystickButton, 1);
    
    if (binding->Button->risingEdge())
      Joystick.button(binding->JoystickButton, 0);
  }

  // Trim Momentary Toggles Update if Falling
  for (auto binding : TrimToggles) {
    if (binding->Button->fallingEdge())
      Joystick.hat(binding->JoystickHat);

    if (binding->Button->risingEdge())
      Joystick.hat(-1);
  }

  // Toggle Switches Update if Falling or Rising
  for (auto binding : ToggleSwitches) {
    if (binding->Button->fallingEdge() || binding->Button->risingEdge())
      toggleButton(binding->JoystickButton);
  }

  // Momentary Buttons Group #2 Update if Falling or Rising
  for (auto binding : MomentaryButtons2) {
    if (binding->Button->fallingEdge())
      Joystick.button(binding->JoystickButton, 1);
    
    if (binding->Button->risingEdge())
      Joystick.button(binding->JoystickButton, 0);
  }

  // Rotary Push Buttons Update if Falling or Rising
  if (Knob_Binding_RightClick.Button->fallingEdge())
	  Joystick.button(Knob_Binding_RightClick.JoystickButton, 1);
  if (Knob_Binding_RightClick.Button->risingEdge())
	  Joystick.button(Knob_Binding_RightClick.JoystickButton, 0);
  
  if (Knob_Binding_LeftClick.Button->fallingEdge())
	  Joystick.button(Knob_Binding_LeftClick.JoystickButton, 1);
  if (Knob_Binding_LeftClick.Button->risingEdge())
	  Joystick.button(Knob_Binding_LeftClick.JoystickButton, 0);

  // Rotary Encoders Update if Falling or Rising
  long newKnobRight;
  newKnobRight = Knob_Right.read();
  if (newKnobRight > (positionKnobRight + 3)) {
    toggleButton(Knob_Binding_RightCW.JoystickButton);
    Knob_Right.write(0);
  }
  if (newKnobRight < (positionKnobRight - 3)) {
    toggleButton(Knob_Binding_RightCCW.JoystickButton);
    Knob_Right.write(0);
  }
  
  long newKnobLeft;
  newKnobLeft = Knob_Left.read();
  if (newKnobLeft > (positionKnobLeft + 3)) {
    toggleButton(Knob_Binding_LeftCW.JoystickButton);
    Knob_Left.write(0);
  }
  if (newKnobLeft < (positionKnobLeft - 3)) {
    toggleButton(Knob_Binding_LeftCCW.JoystickButton);
    Knob_Left.write(0);
  }

  // ***** End of joystick status changes *****
}
