// Right and left are inverted
void rotaryRight()
{
  flagProcess(DOWN);
}

void rotaryLeft()
{
  flagProcess(UP);
}

void rotaryClick()
{
  if (menu_hidden) {
    menu.show(); 
    menu.setCursor(0);
    //menu.refresh();
    menu_hidden = false;
  }
  else if (pid_screen_shown) {
    homeScreen();
  }
  else {
    if (!renderer.isInEditMode()) {
      flagProcess(ENTER);
    }
    else{
      flagProcess(BACK);
    }
  }
}

void rotaryLongPress()
{
  flagProcess(BACK);
}


void initializeRotaryEncoder() {
	// Set the Directions of the I/O Pins
	pinMode(PIN_ROTARY_CLK, INPUT_PULLUP);
	pinMode(PIN_ROTARY_DAT, INPUT_PULLUP);
	pinMode(PIN_ROTARY_SW, INPUT_PULLUP);

	// set an interrupt on PinA and PinB, looking for a rising edge signal and 
	// executing the "PinA" and "PinB" Interrupt Service Routines
	attachInterrupt(0, PinA, RISING);
	attachInterrupt(1, PinB, RISING);

	// Define the functions for Rotary Encoder Click and Long Press
	btnRot.attachClick(&rotaryClick);
	btnRot.attachLongPressStart(&rotaryLongPress);
	btnRot.setPressTicks(1000);

	rotaryDisabled = 0;
}

// https://www.instructables.com/Improved-Arduino-Rotary-Encoder-Reading/

void PinA() {

	if (rotaryDisabled) return;

	cli(); //stop interrupts happening before we read pin values
		   // read all eight pin values then strip away all but pinA and pinB's values
	reading = PIND & 0xC;

	//check that both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
	if (reading == B00001100 && aFlag) {
		rotaryRight();
		bFlag = 0; //reset flags for the next turn
		aFlag = 0; //reset flags for the next turn
	}
	//signal that we're expecting pinB to signal the transition to detent from free rotation
	else if (reading == B00000100) bFlag = 1;
	sei(); //restart interrupts
}

void PinB() {

	if (rotaryDisabled) return;

	cli(); //stop interrupts happening before we read pin values
		   //read all eight pin values then strip away all but pinA and pinB's values
	reading = PIND & 0xC;
	//check that both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge 
	if (reading == B00001100 && bFlag) {
		rotaryLeft();
		bFlag = 0; //reset flags for the next turn
		aFlag = 0; //reset flags for the next turn
	}
	//signal that we're expecting pinA to signal the transition to detent from free rotation
	else if (reading == B00001000) aFlag = 1;
	sei(); //restart interrupts
}


