extern float PosXZ[];                          //Define external variaables
extern float currentPosXZ[];                           //Declare external variables for use in this part of the script
extern boolean AbsolutePos;
const float XhomingSpeedInMMPerSec = 80.0;   
const float ZhomingSpeedInMMPerSec = 10.0; 
const float XmaxHomingDistanceInMM = 300;   // The axis is max 300 mm long so homing for a longer distance does not make sense
const float ZmaxHomingDistanceInMM = 200;   // The axis is max 300 mm long so homing for a longer distance does not make sense
const int directionTowardHomeX = -1;        // Direction to move toward limit switch: 1 goes positive direction, -1 backward
const int directionTowardHomeZ = 1;        // Direction to move toward limit switch: 1 goes positive direction, -1 backward
extern const int LIMIT_SWITCH_X_PIN;
extern const int LIMIT_SWITCH_Z_PIN;
boolean homez = false;
boolean homex = false;

void Home() {

  stepperZ.setSpeedInMillimetersPerSecond(ZhomingSpeedInMMPerSec*0.1);
  stepperX.setSpeedInMillimetersPerSecond(XhomingSpeedInMMPerSec*0.1);
  
  AbsolutePos = true;                             //Set the absolute positioning to true to be able to go to the home position accurately (NOTE THAT IF ON RELATIVE MODE HOMING WILL OVERRIDE TO ABSOLUTE MODE)
  PosXZ[0] = 0;                                   //Set the position to the homing position and home the steppers
  PosXZ[1] = 0;
  homex = stepperX.moveToHomeInMillimeters(directionTowardHomeX, XhomingSpeedInMMPerSec, XmaxHomingDistanceInMM, LIMIT_SWITCH_X_PIN);
  //delay(500);
  homez = stepperZ.moveToHomeInMillimeters(directionTowardHomeZ, ZhomingSpeedInMMPerSec, ZmaxHomingDistanceInMM, LIMIT_SWITCH_Z_PIN);
  //delay(500);
  if (homez && homex){
  Serial.println("Homed");  
  currentPosXZ[0] = 0;                                   //Set the position to the homing position and home the steppers
  currentPosXZ[1] = 0;
  }
  else{
  Serial.println("Failed");
  Serial.print(homez);
  Serial.print(" ");
  Serial.println(homex);
  }
}
