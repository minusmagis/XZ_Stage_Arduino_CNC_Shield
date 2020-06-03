extern const int STEPPERS_ENABLE_PIN;
extern const int MOTOR_X_STEP_PIN;
extern const int MOTOR_Z_STEP_PIN;
extern const int MOTOR_X_DIR_PIN;
extern const int MOTOR_Z_DIR_PIN;
extern const int LIMIT_SWITCH_X_PIN;
extern const int LIMIT_SWITCH_Z_PIN;

int Xstepsmm = 80;                                        //Define the steps per mm of the x axis
int Zstepsmm = 400;                                       //Define the steps per mm of the z axis
int XspeedinMMs = 200;                                     //Define the speed in mm/s for the x axis
int ZspeedinMMs = 30;                                     //Define the speed in mm/s for the z axis
int Xmmss = 100;                                            //Define the acceleration in mm/s^2 for the x axis
int Zmmss = 15;                                            //Define the acceleration in mm/s^2 for the z axis

int Xmotor_dir = 1;                               // Invert the direction if the motor is plugged in reverse (1 is normau -1 is reversed)
int Zmotor_dir = -1;

float XStepsPerSecond = Xstepsmm*XspeedinMMs;                                         //in steps per second (has to be calibrated)
float ZStepsPerSecond = Zstepsmm*ZspeedinMMs;                                         //in steps per second (has to be calibrated)
float XStepsPerSecondPerSecond = Xstepsmm*Xmmss;                                //in steps per second per second (has to be calibrated)
float ZStepsPerSecondPerSecond = Zstepsmm*Zmmss;                                //in steps per second per second (has to be calibrated)

void MoveSteppers(float XZmove[]){
  long stepsX = XZmove[0]*Xstepsmm;
  long stepsZ = XZmove[1]*Zstepsmm;
  moveXZWithCoordination(stepsX,stepsZ,XStepsPerSecond,ZStepsPerSecond,XStepsPerSecondPerSecond,ZStepsPerSecondPerSecond);
}

void moveXZWithCoordination(long stepsX, long stepsZ, float XspeedInStepsPerSecond,float ZspeedInStepsPerSecond, float XaccelerationInStepsPerSecondPerSecond, float ZaccelerationInStepsPerSecondPerSecond)
{
  float speedInStepsPerSecond_X;
  float accelerationInStepsPerSecondPerSecond_X;
  float speedInStepsPerSecond_Z;
  float accelerationInStepsPerSecondPerSecond_Z;
  long absStepsX;
  long absStepsZ;

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
//    Serial.print("Steps Z= ");
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

  
  //
  // setup the motion for the X motor
  //
  stepperX.setSpeedInStepsPerSecond(speedInStepsPerSecond_X);
  stepperX.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_X);
  stepperX.setupRelativeMoveInSteps(stepsX*Xmotor_dir);


  //
  // setup the motion for the Z motor
  //
  stepperZ.setSpeedInStepsPerSecond(speedInStepsPerSecond_Z);
  stepperZ.setAccelerationInStepsPerSecondPerSecond(accelerationInStepsPerSecondPerSecond_Z);
  stepperZ.setupRelativeMoveInSteps(stepsZ*Zmotor_dir);

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
  // now execute the moves, looping until both motors have finished
  //
  while((!stepperX.motionComplete()) || (!stepperZ.motionComplete()))
  {
    stepperX.processMovement();
    stepperZ.processMovement();
  }
  Serial.println("Moved");
}
