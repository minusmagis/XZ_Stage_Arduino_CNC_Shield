extern boolean AbsolutePos;
extern float currentPosXZ[];                           //Declare external variables for use in this part of the script
extern float PosXZ[];
extern int Feedrate;
extern boolean AllowedPosition;
int XaxisRange = 212;                                  //Declare the range of the x axis (in mm) from zero to the value
int ZaxisRange = 122;                                  //Declare the range of the z axis (in mm) from zero to the value


void SetSteppersMovement() {                             //Function for moving the steppers around sinchronously
  //  Serial.println("Move Steppers Checked");           //For Development purposes
  float XZmove[] = {0, 0};                               //This variable stores the direction vector towards which we want the XZ carriage to move
  if (AbsolutePos == true)                               //If the XZ carriage is in absolute position movement calculate the coordinates for the moving vector
  {
    //Serial.println("Absolute Check");
    for (int i = 0; i < 2; i++) {                   //Scan the PosXZ and calculate the distance between the desired position and the current position in each axis
      XZmove[i] = PosXZ[i] - currentPosXZ[i];     //Store the value of the direction vector
    }
//            Serial.print("Movement in the x position: "); //For development purposes
//            Serial.print(XZmove[0]);
//            Serial.print(" Movement in the z position: ");
//            Serial.println(XZmove[1]);                   //For development purposes
  }

  else {                                       //If the arm is in relative position movement use the coordinates as the moving vector
    {
      //Serial.println("Relative Check");
      for (int i = 0; i < 2; i++) {                   //Scan the PosXZ and set it to be the direction vector since the XZ carriage is in relative position movement
        XZmove[i] = PosXZ[i];                         //Store the value of the direction vector
      }
      //        Serial.print("Movement in the x position: "); //For development purposes
      //        Serial.print(XZmove[0]);
      //        Serial.print(" Movement in the z position: ");
      //        Serial.println(XZmove[1]);                   //For development purposes
    }
  }
  MoveSteppers(XZmove);
  currentPosXZ[0] = PosXZ[0];
  currentPosXZ[1] = PosXZ[1];
}

boolean PositionAllowed() {                                 //Check if the position to which the command tells the carriage to move is within the carriage range, and if so return true, otherwise return false
  if (AbsolutePos == true) {
    if ( (0 <= PosXZ[0] && PosXZ[0] <= XaxisRange ) && (0 <= PosXZ[1]) && ( PosXZ[1] <= ZaxisRange)) {    //If the desired position is whithin this range the movement is allowed
      //      Serial.println("Whithin range Abs");                                                        //For development purposes
      return true;

    }
    else {
      //      Serial.println("Out of range Abs");                           //For development purposes
      return false;

    }
  }
  else {
    float XZfinal[] = {0, 0};                                  //Create a variable that will hold the final position to prevent moving beyond the set boundaries of the arm in relative mode
    for (int v = 0; v < 2; v++) {                              //Set the final position to the sum of the current position plus the amount we want to move in each direction
      XZfinal[v] = PosXZ[v] + currentPosXZ[v];
    }
    if ( (0 <= XZfinal[0] && XZfinal[0] <= XaxisRange) && (0 <= XZfinal[1]) && ( XZfinal[1] <= ZaxisRange)) {  //If the final calculated position is whithin this range the movement is allowed
      //      Serial.println("Whithin range rel");                           //For development purposes
      return true;

    }
    else {
      //      Serial.println("Out of range rel");                           //For development purposes
      return false;

    }
  }
}
