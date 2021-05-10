extern boolean AbsolutePos;
extern float currentPosXYZ[];                           //Declare external variables for use in this part of the script
extern float PosXYZ[];
extern int Feedrate;
extern boolean AllowedPosition;
int XaxisRange = 210;                                  //Declare the range of the x axis (in mm) from zero to the value
int YaxisRange = 50;                                  //Declare the range of the y axis (in mm) from zero to the value
int ZaxisRange = 122;                                  //Declare the range of the z axis (in mm) from zero to the value


void SetSteppersMovement() {                             //Function for moving the steppers around sinchronously
  //  Serial.println("Move Steppers Checked");           //For Development purposes
  float XYZmove[] = {0, 0, 0};                           //This variable stores the direction vector towards which we want the XYZ carriage to move
  if (AbsolutePos == true)                               //If the XYZ carriage is in absolute position movement calculate the coordinates for the moving vector
  {
    //Serial.println("Absolute Check");
    for (int i = 0; i < 3; i++) {                   //Scan the PosXYZ and calculate the distance between the desired position and the current position in each axis
      XYZmove[i] = PosXYZ[i] - currentPosXYZ[i];    //Store the value of the direction vector
    }
//                  Serial.print("Movement in the x position: "); //For development purposes
//                  Serial.print(XYZmove[0]);
//                  Serial.print("Movement in the y position: "); //For development purposes
//                  Serial.print(XYZmove[1]);
//                  Serial.print(" Movement in the z position: ");
//                  Serial.println(XYZmove[2]);                   //For development purposes
    currentPosXYZ[0] = PosXYZ[0];
    currentPosXYZ[1] = PosXYZ[1];
    currentPosXYZ[2] = PosXYZ[2];
  }

  else {                                       //If the arm is in relative position movement use the coordinates as the moving vector
    {
      //Serial.println("Relative Check");
      for (int i = 0; i < 3; i++) {                   //Scan the PosXYZ and set it to be the direction vector since the XYZ carriage is in relative position movement
        XYZmove[i] = PosXYZ[i];                         //Store the value of the direction vector
      }
      //        Serial.print("Movement in the x position: "); //For development purposes
      //        Serial.print(XYZmove[0]);
      //        Serial.print("Movement in the y position: "); //For development purposes
      //        Serial.print(XYZmove[1]);
      //        Serial.print(" Movement in the z position: ");
      //        Serial.println(XYZmove[2]);                   //For development purposes
    }
    currentPosXYZ[0] = currentPosXYZ[0] + PosXYZ[0];                             // The new position will be the one we were in plus the one we have added with the relative movement
    currentPosXYZ[1] = currentPosXYZ[1] + PosXYZ[1];
    currentPosXYZ[2] = currentPosXYZ[2] + PosXYZ[2];
  }
  MoveSteppers(XYZmove);

}

boolean PositionAllowed() {                                 //Check if the position to which the command tells the carriage to move is within the carriage range, and if so return true, otherwise return false
  if (AbsolutePos == true) {
    if ( (0 <= PosXYZ[0] && PosXYZ[0] <= XaxisRange ) && (0 <= PosXYZ[1]) && ( PosXYZ[1] <= YaxisRange) && (0 <= PosXYZ[2]) && ( PosXYZ[2] <= ZaxisRange)) {    //If the desired position is whithin this range the movement is allowed
      //      Serial.println("Whithin range Abs");                                                        //For development purposes
      return true;

    }
    else {
      //      Serial.println("Out of range Abs");                           //For development purposes
      return false;

    }
  }
  else {
    float XYZfinal[] = {0, 0, 0};                                  //Create a variable that will hold the final position to prevent moving beyond the set boundaries of the arm in relative mode
    for (int v = 0; v < 3; v++) {                              //Set the final position to the sum of the current position plus the amount we want to move in each direction
      XYZfinal[v] = PosXYZ[v] + currentPosXYZ[v];
    }
    if ( (0 <= XYZfinal[0] && XYZfinal[0] <= XaxisRange) && (0 <= XYZfinal[1]) && ( XYZfinal[1] <= YaxisRange) && (0 <= XYZfinal[2]) && ( XYZfinal[2] <= ZaxisRange)) {  //If the final calculated position is whithin this range the movement is allowed
      //      Serial.println("Whithin range rel");                           //For development purposes
      return true;

    }
    else {
      //      Serial.println("Out of range rel");                           //For development purposes
      return false;

    }
  }
}
