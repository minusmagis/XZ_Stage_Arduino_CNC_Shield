#include <SpeedyStepper.h>

const int STEPPERS_ENABLE_PIN = 8;
const int MOTOR_X_STEP_PIN = 2;
const int MOTOR_Z_STEP_PIN = 4;
const int MOTOR_X_DIR_PIN = 5;
const int MOTOR_Z_DIR_PIN = 7;
const int LIMIT_SWITCH_X_PIN = 9;
const int LIMIT_SWITCH_Z_PIN = 11;

extern float Xstepsmm;
extern float Zstepsmm;
extern int Xmmss;
extern int Zmmss;


SpeedyStepper stepperX;
SpeedyStepper stepperZ;

void setup() {
  pinMode(STEPPERS_ENABLE_PIN, OUTPUT);                  // Set the enable pin as an output
  pinMode(LIMIT_SWITCH_X_PIN, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH_Z_PIN, INPUT_PULLUP);

  stepperX.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN);        //Initialize the motors
  stepperZ.connectToPins(MOTOR_Z_STEP_PIN, MOTOR_Z_DIR_PIN);        //Initialize the motors
  stepperX.setStepsPerMillimeter(Xstepsmm);
  stepperZ.setStepsPerMillimeter(Zstepsmm);
  stepperX.setAccelerationInMillimetersPerSecondPerSecond(Xmmss);
  stepperZ.setAccelerationInMillimetersPerSecondPerSecond(Zmmss);

  digitalWrite(STEPPERS_ENABLE_PIN, LOW);                // Enable the steppers
  Serial.begin(9600);                                    // Begin Serial communication
  StartCode();                                           // Send the startcode
}

void loop() {
  GcodeCommandReceive();
  GcodeToInstructions();
}
