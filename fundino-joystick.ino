#include <TimerOne.h>

// define global variables for joystick control

#define PIN_JOY_X A0
#define PIN_JOY_Y A1
#define PIN_BUTTON_A 2
#define PIN_BUTTON_B 3
#define PIN_BUTTON_C 4
#define PIN_BUTTON_D 5
#define PIN_BUTTON_E 6
#define PIN_BUTTON_F 7

// list of buttons to check
int buttons[] = { PIN_BUTTON_A, PIN_BUTTON_B, PIN_BUTTON_C, PIN_BUTTON_D, PIN_BUTTON_E, PIN_BUTTON_F };

// button names (indexed by the PIN NUMBER!
char buttonNames[] = {' ', ' ', 'A', 'B', 'C', 'D', 'E', 'F'}; 

// the centre value of the joystick, can vary per instance
int median_x = 520;
int median_y = 509;

// treshold for joystick changes. Will treat median +- treshold as centre position
int treshold_x = 10;
int treshold_y = 10;

// the actual state of the joystick and the list of pressed buttons
struct JoystickState {
  int x;
  int y;
  String pressedButtons;
};

JoystickState joystickState = { median_x, median_y, {} } ;

void setup() {
 // Start serial because we will observe values at serial monitor
  Serial.begin(9600);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(readJoystick); 
  for (int i = 0; i < (sizeof(buttons) / sizeof(int)) ; i = i + 1) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }

}

// read the joystick and button states
void readJoystick() {
  joystickState.x = analogRead(PIN_JOY_X); 
  joystickState.y = analogRead(PIN_JOY_Y);
  for (int i = 0; i < (sizeof(buttons) / sizeof(int)) ; i = i + 1) {
    if (digitalRead(buttons[i]) == LOW) {
      joystickState.pressedButtons += buttonNames[buttons[i]];  
    }
  }
}

void loop() {
  noInterrupts();
  JoystickState joy = joystickState;
  joystickState.pressedButtons = "";
  interrupts();
  
  // Print x axis values
  String output = "Joystick position:" ;
  Serial.println(output + "  x: " + joy.x + " y: " + joy.y);
  output = "Buttons pressed: ";
  Serial.println(output + joy.pressedButtons);

  // Some delay to clearly observe your values on serial monitor.
  delay(500);

}
