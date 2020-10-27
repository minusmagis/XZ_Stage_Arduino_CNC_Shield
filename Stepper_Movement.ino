extern const int STEPPERS_ENABLE_PIN;
extern const int MOTOR_X_STEP_PIN;
extern const int MOTOR_Z_STEP_PIN;
extern const int MOTOR_X_DIR_PIN;
extern const int MOTOR_Z_DIR_PIN;
extern const int LIMIT_SWITCH_X_PIN;
extern const int LIMIT_SWITCH_Z_PIN;

float Xstepsmm = 80;                                        //Define the steps per mm of the x axis
float Zstepsmm = 400;                                       //Define the steps per mm of the z axis
int XspeedinMMs = 200;                                     //Define the speed in mm/s for the x axis
int ZspeedinMMs = 30;                                     //Define the speed in mm/s for the z axis
int Xmmss = 100;                                            //Define the acceleration in mm/s^2 for the x axis
int Zmmss = 15;                                            //Define the acceleration in mm/s^2 for the z axis

int Xmotor_dir = 1;                               // Invert the direction if the motor is plugged in reverse (1 is normau -1 is reversed)
int Zmotor_dir = -1;

float XStepsPerSecond = Xstepsmm * XspeedinMMs;                                       //in steps per second (has to be calibrated)
float ZStepsPerSecond = Zstepsmm * ZspeedinMMs;                                       //in steps per second (has to be calibrated)
float XStepsPerSecondPerSecond = Xstepsmm * Xmmss;                              //in steps per second per second (has to be calibrated)
float ZStepsPerSecondPerSecond = Zstepsmm * Zmmss;                              //in steps per second per second (has to be calibrated)

void MoveSteppers(float XZmove[]) {
  float stepsX = XZmove[0] * Xstepsmm;
  //  Serial.print(XZmove[0]);                                      // For Development purposes DO NOT USE LONG OR ANYTHING OTHER THAN FLOAT
  //  Serial.print(" * ");
  //  Serial.print(Xstepsmm);
  //  Serial.print(" = ");
  //  Serial.print(stepsX);
  //  Serial.println(" X steps Calculated");
  float stepsZ = XZmove[1] * Zstepsmm;
  //  Serial.print(XZmove[1]);
  //  Serial.print(" * ");
  //  Serial.print(Zstepsmm);
  //  Serial.print(" = ");
  //  Serial.print(stepsZ);
  //  Serial.println(" Z steps Calculated");
  moveXZWithCoordination(stepsX, stepsZ, XStepsPerSecond, ZStepsPerSecond, XStepsPerSecondPerSecond, ZStepsPerSecondPerSecond);
}

void moveXZWithCoordination(float stepsX, float stepsZ, float XspeedInStepsPerSecond, float ZspeedInStepsPerSecond, float XaccelerationInStepsPerSecondPerSecond, float ZaccelerationInStepsPerSecondPerSecond)
{
  float speedInStepsPerSecond_X;
  float accelerationInStepsPerSecondPerSecond_X;
  float speedInStepsPerSecond_Z;
  float accelerationInStepsPerSecondPerSecond_Z;
  float absStepsX;
  float absStepsZ;

  //
  // setup initial speed and acceleration values
  //
  speedInStepsPerSecond_X = XspeedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_X = XaccelerationInStepsPerSecondPerSecond;

  speedInStepsPerSecond_Z = ZspeedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_Z = ZaccelerationInStepsPerSecondPerSecond;

  //  Serial.print("speedInStepsPerSecond_X = ");
  //  Serial.println(speedInStepsPerSecond_X);
  //  Serial.print("accelerationInStepsPerSecondPerSecond_X = ");
  //  Serial.println(accelerationInStepsPerSecondPerSecond_X);
  //  Serial.print("speedInStepsPerSecond_Z = ");
  //  Serial.println(speedInStepsPerSecond_Z);
  //  Serial.print("accelerationInStepsPerSecondPerSecond_Z = ");
  //  Serial.println(accelerationInStepsPerSecondPerSecond_Z);
  //  Serial.print("Steps Z= ");
  //  Serial.println(absStepsZ);

  //
  // determine how many steps each motor is moving
  //
  if (stepsX >= 0)
    absStepsX = stepsX;
  else
    absStepsX = -stepsX;

  if (stepsZ >= 0)
    absStepsZ = stepsZ;
  else
    absStepsZ = -stepsZ;

  //  Serial.print("speedInStepsPerSecond_X = ");
  //  Serial.println(speedInStepsPerSecond_X);
  //  Serial.print("accelerationInStepsPerSecondPerSecond_X = ");
  //  Serial.println(accelerationInStepsPerSecondPerSecond_X);
  //  Serial.print("speedInStepsPerSecond_Z = ");
  //  Serial.println(speedInStepsPerSecond_Z);
  //  Serial.print("accelerationInStepsPerSecondPerSecond_Z = ");
  //  Serial.println(accelerationInStepsPerSecondPerSecond_Z);
  //  Serial.print("Steps X= ");
  //  Serial.println(absStepsX);
  //  Serial.print("Steps Z= ");
  //  Serial.println(absStepsZ);

  //
  // determine which motor is traveling the farthest, then slow down the
  // speed rates for the motor moving the shortest distance
  //
  if ((absStepsX > absStepsZ) && (stepsX != 0))
  {
    //
    // slow down the motor traveling less far
    //
    float scaler = (float) absStepsZ / (float) absStepsX;
    speedInStepsPerSecond_Z = speedInStepsPerSecond_Z * scaler;
    accelerationInStepsPerSecondPerSecond_Z = accelerationInStepsPerSecondPerSecond_Z * scaler;
  }

  if ((absStepsZ > absStepsX) && (stepsZ != 0))
  {
    //
    // slow down the motor traveling less far
    //
    float scaler = (float) absStepsX / (float) absStepsZ;
    speedInStepsPerSecond_X = speedInStepsPerSecond_X * scaler;
    accelerationInStepsPerSecondPerSecond_X = accelerationInStepsPerSecondPerSecond_X * scaler;
  }

  long Final_X_Steps = round(stepsX) * Xmotor_dir;                    // We need to round to convert to int to avoid truncation problems
  long Final_Z_Steps = round(stepsZ) * Zmotor_dir;
//
//  Serial.print("Steps X= ");
//  Serial.println(Final_X_Steps);
//  Serial.print("Steps Z= ");
//  Serial.println(Final_Z_Steps);

  //
  // setup the motion for the X motor
  //
  stepperX.setSpeedInStepsPerSecond(speedInStepsPerSecond_X);
  stepperX.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_X);
  stepperX.setupRelativeMoveInSteps(Final_X_Steps);


  //
  // setup the motion for the Z motor
  //
  stepperZ.setSpeedInStepsPerSecond(speedInStepsPerSecond_Z);
  stepperZ.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_Z);
  stepperZ.setupRelativeMoveInSteps(Final_Z_Steps);

  //  Serial.print("speedInStepsPerSecond_X = ");
  //  Serial.println(speedInStepsPerSecond_X);
  //  Serial.print("accelerationInStepsPerSecondPerSecond_X = ");
  //  Serial.println(accelerationInStepsPerSecondPerSecond_X);
  //  Serial.print("speedInStepsPerSecond_Z = ");
  //  Serial.println(speedInStepsPerSecond_Z);
  //  Serial.print("accelerationInStepsPerSecondPerSecond_Z = ");
  //  Serial.println(accelerationInStepsPerSecondPerSecond_Z);
  //  Serial.print("Steps Z= ");
  //  Serial.println(absStepsZ);

  //  Serial.print("Steps X= ");
  //  Serial.println(stepsX);
  //  Serial.print("Steps Z= ");
  //  Serial.println(stepsZ);

  //
  // now execute the moves, looping until both motors have finished
  //
  while ((!stepperX.motionComplete()) || (!stepperZ.motionComplete()))
  {
    stepperX.processMovement();
    stepperZ.processMovement();
  }
  Serial.println("Moved");
}
