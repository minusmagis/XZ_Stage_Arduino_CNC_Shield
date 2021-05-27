extern const int STEPPERS_ENABLE_PIN;
extern const int MOTOR_X_STEP_PIN;
extern const int MOTOR_Y_STEP_PIN;
extern const int MOTOR_Z_STEP_PIN;
extern const int MOTOR_X_DIR_PIN;
extern const int MOTOR_Y_DIR_PIN;
extern const int MOTOR_Z_DIR_PIN;
extern const int LIMIT_SWITCH_X_PIN;
extern const int LIMIT_SWITCH_Y_PIN;
extern const int LIMIT_SWITCH_Z_PIN;

float Xstepsmm = 80;                                        //Define the steps per mm of the x axis
float Ystepsmm = 400;                                        //Define the steps per mm of the Y axis
float Zstepsmm = 400;                                       //Define the steps per mm of the z axis
int XspeedinMMs = 100;                                     //Define the speed in mm/s for the x axis
int YspeedinMMs = 4;                                     //Define the speed in mm/s for the y axis
int ZspeedinMMs = 12;                                     //Define the speed in mm/s for the z axis
int Xmmss = 100;                                            //Define the acceleration in mm/s^2 for the x axis
int Ymmss = 10;                                            //Define the acceleration in mm/s^2 for the y axis
int Zmmss = 10;                                            //Define the acceleration in mm/s^2 for the z axis

int Xmotor_dir = -1;                               // Invert the direction if the motor is plugged in reverse (1 is normau -1 is reversed)
int Ymotor_dir = 1;
int Zmotor_dir = 1;

float XStepsPerSecond = Xstepsmm * XspeedinMMs;                                       //in steps per second (has to be calibrated)
float YStepsPerSecond = Ystepsmm * YspeedinMMs;                                       //in steps per second (has to be calibrated)
float ZStepsPerSecond = Zstepsmm * ZspeedinMMs;                                       //in steps per second (has to be calibrated)
float XStepsPerSecondPerSecond = Xstepsmm * Xmmss;                              //in steps per second per second (has to be calibrated)
float YStepsPerSecondPerSecond = Ystepsmm * Ymmss;                              //in steps per second per second (has to be calibrated)
float ZStepsPerSecondPerSecond = Zstepsmm * Zmmss;                              //in steps per second per second (has to be calibrated)

void MoveSteppers(float XYZmove[]) {
  float stepsX = XYZmove[0] * Xstepsmm;
  //  Serial.print(XYZmove[0]);                                      // For Development purposes DO NOT USE LONG OR ANYTHING OTHER THAN FLOAT
  //  Serial.print(" * ");
  //  Serial.print(Xstepsmm);
  //  Serial.print(" = ");
  //  Serial.print(stepsX);
  //  Serial.println(" X steps Calculated");
  float stepsY = XYZmove[1] * Ystepsmm;
  //  Serial.print(XYZmove[1]);                                      // For Development purposes DO NOT USE LONG OR ANYTHING OTHER THAN FLOAT
  //  Serial.print(" * ");
  //  Serial.print(Xstepsmm);
  //  Serial.print(" = ");
  //  Serial.print(stepsX);
  //  Serial.println(" X steps Calculated");
  float stepsZ = XYZmove[2] * Zstepsmm;
  //  Serial.print(XYZmove[2]);
  //  Serial.print(" * ");
  //  Serial.print(Zstepsmm);
  //  Serial.print(" = ");
  //  Serial.print(stepsZ);
  //  Serial.println(" Z steps Calculated");
  moveXYZWithCoordination(stepsX, stepsY, stepsZ, XStepsPerSecond, YStepsPerSecond, ZStepsPerSecond, XStepsPerSecondPerSecond, YStepsPerSecondPerSecond, ZStepsPerSecondPerSecond);
}

void moveXYZWithCoordination(float stepsX, float stepsY, float stepsZ, float XspeedInStepsPerSecond, float YspeedInStepsPerSecond, float ZspeedInStepsPerSecond, float XaccelerationInStepsPerSecondPerSecond, float YaccelerationInStepsPerSecondPerSecond, float ZaccelerationInStepsPerSecondPerSecond)
{
  float speedInStepsPerSecond_array[] = {0,0,0};
  float accelerationInStepsPerSecondPerSecond_array[] = {0,0,0};
  float absStepsXYZ[] = {0,0,0};

  //
  // setup initial speed and acceleration values
  //
  speedInStepsPerSecond_array[0] = XspeedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_array[0] = XaccelerationInStepsPerSecondPerSecond;

  speedInStepsPerSecond_array[1] = YspeedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_array[1] = YaccelerationInStepsPerSecondPerSecond;

  speedInStepsPerSecond_array[2] = ZspeedInStepsPerSecond;
  accelerationInStepsPerSecondPerSecond_array[2] = ZaccelerationInStepsPerSecondPerSecond;

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
    absStepsXYZ[0] = stepsX;
  else
    absStepsXYZ[0] = -stepsX;

  if (stepsY >= 0)
    absStepsXYZ[1] = stepsY;
  else
    absStepsXYZ[1] = -stepsY;

  if (stepsZ >= 0)
    absStepsXYZ[2] = stepsZ;
  else
    absStepsXYZ[2] = -stepsZ;

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

  float Biggest_Move = 0;
  for (int k = 0;  k > 3; k++ ){
    if (absStepsXYZ[k] > Biggest_Move){
      Biggest_Move = absStepsXYZ[k];
    }
  }

  for (int k = 0;  k > 3;k++ ){
    float scaler = (float) absStepsXYZ[k] / (float) Biggest_Move;
    speedInStepsPerSecond_array[k] = speedInStepsPerSecond_array[k] * scaler;
    accelerationInStepsPerSecondPerSecond_array[k] = accelerationInStepsPerSecondPerSecond_array[k] * scaler;
  }


  long Final_X_Steps = round(stepsX) * Xmotor_dir;                    // We need to round to convert to int to avoid truncation problems
  long Final_Y_Steps = round(stepsY) * Ymotor_dir;
  long Final_Z_Steps = round(stepsZ) * Zmotor_dir;

//   Serial.print("Steps X= ");
//   Serial.println(Final_X_Steps);
//   Serial.print("Steps Y= ");
//   Serial.println(Final_Y_Steps);
//   Serial.print("Steps Z= ");
//   Serial.println(Final_Z_Steps);

  //
  // setup the motion for the X motor
  //
  stepperX.setSpeedInStepsPerSecond(speedInStepsPerSecond_array[0]);
  stepperX.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_array[0]);
  stepperX.setupRelativeMoveInSteps(Final_X_Steps);

  //
  // setup the motion for the Y motor
  //
  stepperY.setSpeedInStepsPerSecond(speedInStepsPerSecond_array[1]);
  stepperY.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_array[1]);
  stepperY.setupRelativeMoveInSteps(Final_Y_Steps);

  //
  // setup the motion for the Z motor
  //
  stepperZ.setSpeedInStepsPerSecond(speedInStepsPerSecond_array[2]);
  stepperZ.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_array[2]);
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
  while ((!stepperX.motionComplete()) || (!stepperY.motionComplete()) || (!stepperZ.motionComplete()))
  {
    stepperX.processMovement();
    stepperY.processMovement();
    stepperZ.processMovement();
  }
  Serial.println("Moved");
}
