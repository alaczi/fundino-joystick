#include <TimerOne.h>

// define the PINS of the joystick shield
// Joystick axises
#define PIN_JOY_X A0
#define PIN_JOY_Y A1
// Shield buttons
// Button A or UP
#define PIN_BUTTON_A 2
// Button B or RIGHT
#define PIN_BUTTON_B 3
// Button C or DOWN
#define PIN_BUTTON_C 4
// Button D or LEFT
#define PIN_BUTTON_D 5
// BUTTON E or MENU?
#define PIN_BUTTON_E 6
// BUTTON F or SETUP?
#define PIN_BUTTON_F 7

// list of buttons to check (using their pin numbers
int buttons[] = { PIN_BUTTON_A, PIN_BUTTON_B, PIN_BUTTON_C, PIN_BUTTON_D, PIN_BUTTON_E, PIN_BUTTON_F };

// button names - for debugging
char buttonNames[] = {'A', 'B', 'C', 'D', 'E', 'F'}; 

// the centre value of the joystick, can vary per instance
int medianX = 520;
int medianY = 509;

// treshold for joystick changes. Will treat median +- treshold as centre position
int tresholdX = 10;
int tresholdY = 10;

// the delay in the main loop in miliseconds this will be the time when we can do something with the data
const int mainLoopDelay = 100;

// controller sample frequency in microseconds divide by 16 to avoid loosing data (integers hold 16 bits)
const int controllerSampling =  (mainLoopDelay / 16) * 1000;

// the actual state of the controller: joystick and the list of button events
struct ControllerState {
  // the joy x position from 0-1023
  int x;
  // the joy y position from 0-1023
  int y;
  // Holds the 6 button changes as integer. Each bit represents a status: 0: button is down (pressed) 1: button is up
  unsigned int buttonStatuses[6];
  // send the sampling frequency also, so the client can calculate button simple / double / keep down for buttons
  int sampling;
};
// helper to be able to reset the button statuses (fill the whole int with 1)
const unsigned int resetStatuses[6] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF};

// global to hold the actual data from the controller shield
ControllerState controllerState = { medianX, medianY, {0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF,0xFFFF}, controllerSampling } ;

void setup() {
 // Start serial to observe the values with the serial monitor
  Serial.begin(115200);
  // Sets the timer of the software interrupt
  Timer1.initialize(controllerSampling);
  // Attach the reader function to the interrupt
  Timer1.attachInterrupt(readController); 
  // set the pins as input and put them to HIGH
  for (int i = 0; i < (sizeof(buttons) / sizeof(int)) ; i = i + 1) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
}

// read the joystick and button states
void readController() {
  controllerState.x = analogRead(PIN_JOY_X); 
  controllerState.y = analogRead(PIN_JOY_Y);
  for (int i = 0; i < (sizeof(buttons) / sizeof(int)) ; i = i + 1) {
    int actualStatus = digitalRead(buttons[i]);
    // shift the 
    controllerState.buttonStatuses[i] = controllerState.buttonStatuses[i] << 1;
    if (actualStatus == HIGH) {
      controllerState.buttonStatuses[i] |= 1u;
    }
  }
}

void loop() {
  // disabling interrupts while making a copy of the controller status
  noInterrupts();
  ControllerState state = controllerState;
  //reset the buttons history
  memcpy(controllerState.buttonStatuses,resetStatuses, 6); 
  interrupts();
  
  // Print x axis values
  String output = "Joystick position:" ;
  Serial.println(output + "  x: " + state.x + " y: " + state.y);
  output = "Buttons pressed: ";
  for (int i = 0; i < (sizeof(buttons) / sizeof(int)) ; i = i + 1) {
    Serial.print(output);
    Serial.println(state.buttonStatuses[i], BIN);
  }

  // Some delay to observe your values on serial monitor or send the data to a display etc.
  delay(mainLoopDelay);
}
