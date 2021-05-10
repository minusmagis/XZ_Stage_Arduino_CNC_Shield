extern float PosXYZ[];                          //Define external variaables
extern float currentPosXYZ[];                           //Declare external variables for use in this part of the script
extern boolean AbsolutePos;
const float XhomingSpeedInMMPerSec = 80.0;   
const float YhomingSpeedInMMPerSec = 80.0;
const float ZhomingSpeedInMMPerSec = 5.0; 
const float XmaxHomingDistanceInMM = 300;   // The axis is max 300 mm long so homing for a longer distance does not make sense
const float YmaxHomingDistanceInMM = 150;   // The axis is max 300 mm long so homing for a longer distance does not make sense
const float ZmaxHomingDistanceInMM = 200;   // The axis is max 300 mm long so homing for a longer distance does not make sense
const int directionTowardHomeX = -1;        // Direction to move toward limit switch: 1 goes positive direction, -1 backward
const int directionTowardHomeY = -1;        // Direction to move toward limit switch: 1 goes positive direction, -1 backward
const int directionTowardHomeZ = 1;        // Direction to move toward limit switch: 1 goes positive direction, -1 backward
extern const int LIMIT_SWITCH_X_PIN;
extern const int LIMIT_SWITCH_Y_PIN;
extern const int LIMIT_SWITCH_Z_PIN;
boolean homez = false;
boolean homey = false;
boolean homex = false;

void Home() {
  Serial.println("Homing X");
  
  AbsolutePos = true;                              //Set the absolute positioning to true to be able to go to the home position accurately (NOTE THAT IF ON RELATIVE MODE HOMING WILL OVERRIDE TO ABSOLUTE MODE)
  PosXYZ[0] = 0;                                   //Set the position to the homing position and home the steppers
  PosXYZ[1] = 0;
  PosXYZ[2] = 0;
  Serial.println("Homing X");
  homex = stepperX.moveToHomeInMillimeters(directionTowardHomeX, XhomingSpeedInMMPerSec, XmaxHomingDistanceInMM, LIMIT_SWITCH_X_PIN);
  Serial.println("Homing Y");
  homey = stepperY.moveToHomeInMillimeters(directionTowardHomeY, YhomingSpeedInMMPerSec, YmaxHomingDistanceInMM, LIMIT_SWITCH_Y_PIN);
  //delay(500);
  Serial.println("Homing Z");
  homez = stepperZ.moveToHomeInMillimeters(directionTowardHomeZ, ZhomingSpeedInMMPerSec, ZmaxHomingDistanceInMM, LIMIT_SWITCH_Z_PIN);
  //delay(500);
  if (homex && homey && homez ){
  Serial.println("Homed");  
  currentPosXYZ[0] = 0;                                   //Set the position to the homing position and home the steppers
  currentPosXYZ[1] = 0;
  currentPosXYZ[2] = 0;
  }
  else{
  Serial.println("Failed");
  Serial.print(homex);
  Serial.print(" ");
  Serial.println(homey);
  Serial.print(" ");
  Serial.println(homez);
  }
}
