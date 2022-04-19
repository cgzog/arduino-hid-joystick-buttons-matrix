// create a 16 button "joystick" with no other controls
//
// uses a matrix to read the buttons
//
// derived from an example by Matthew Heironimus from his Joystick library


#include <Joystick.h>


#define STATUS_LED_PIN  13


#define MATRIX_COLUMNS  5
#define MATRIX_ROWS     4

#define NUM_OF_BUTTONS  17      // actual number of valid matrix button locations

typedef struct {
  bool  valid;                  // is this a valid button?
  int   colPin;                 // matrix col button is connected to (inputs)
  int   joystickButtonNum;      // logical joystick button mapping
} BUTTONS;

typedef struct {
  int rowPin;                   // matrix row (outputs)
  BUTTONS row[MATRIX_COLUMNS];  // each button on this row
} MATRIX;


// all matrix entries must be initialized
//
// joystickButtonNum ignored if matrix location is not valid

MATRIX  Matrix[MATRIX_ROWS] = {  {  8, { { true,  3,  0 },
                                         { true,  4,  1 },
                                         { true,  5,  2 },
                                         { true,  6,  3 },
                                         { false, 7,  0 }
                                       }
                                 },
                                 {  9, { { true,  3,  4 },
                                         { true,  4,  5 },
                                         { true,  5,  6 },
                                         { true,  6,  7 },
                                         { true,  7,  8 }
                                       }
                                 },
                                 { 10, { { true,  3,  9 },
                                         { true,  4, 10 },
                                         { true,  5, 11 },
                                         { true,  6, 12 },
                                         { true,  7, 13 }
                                       }
                                 },
                                 { 11, { { false, 3,  0 },
                                         { true,  4, 14 },
                                         { true,  5, 15 },
                                         { true,  6, 16 },
                                         { false, 7,  0 }
                                       }
                                 }
                              };

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
                   JOYSTICK_TYPE_JOYSTICK,
                   NUM_OF_BUTTONS,        // number of buttons
                   0,                     // hat switch count (top of stick)
                   false, false, false,   // no x, y, or z axis
                   false, false,          // no rudder or throttle control
                   false, false, false);  // no accel, brake, or steering control


void setup() {

  // Initialize Button Pins
  //
  // columns are input - rows are outputs
  
  for (int i = 0 ; i < MATRIX_ROWS ; i++) {
    // every row is an output - low when active
    //
    // configure and set to HIGH as default
    pinMode(Matrix[i].rowPin, OUTPUT);
    digitalWrite(Matrix[i].rowPin, HIGH);

    // every column is an input - low when active
    //
    // combined with row, we know which button is pressed
    //
    // columns will get initialized multiple times but that's OK
    //
    // we want to make sure we don't miss any
    for (int ii = 0 ; ii < MATRIX_COLUMNS ; ii++) {
      pinMode(Matrix[i].row[ii].colPin, INPUT_PULLUP);
    }
  }

  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, LOW);

  // Initialize Joystick Library
  Joystick.begin();

  // Serial.begin(9600);
}




void loop() {

  for (int row = 0; row < MATRIX_ROWS; row++) {

    // Serial.print(row);
    // Serial.print(" (Pin ");
    // Serial.print(Matrix[row].rowPin);
    // Serial.print("): ");

    // activate row
    digitalWrite(Matrix[row].rowPin, LOW);
    
    for (int col = 0 ; col < MATRIX_COLUMNS ; col++) {

      if ( ! Matrix[row].row[col].valid) {
        // Serial.print("- ");
        continue;
      }
      
      int currentButtonState = ! digitalRead(Matrix[row].row[col].colPin);

      // if (currentButtonState) {
      //   Serial.print("H ");
      // } else {
      //   Serial.print("L ");
      // }
      
      // update joystick value
      Joystick.setButton(Matrix[row].row[col].joystickButtonNum, currentButtonState);
    }
    
    // de-activate column
    digitalWrite(Matrix[row].rowPin, HIGH);

    // Serial.println("");
  }

  // Serial.println("");

  digitalWrite(STATUS_LED_PIN, HIGH);

  delay(50);

  digitalWrite(STATUS_LED_PIN, LOW);
}
