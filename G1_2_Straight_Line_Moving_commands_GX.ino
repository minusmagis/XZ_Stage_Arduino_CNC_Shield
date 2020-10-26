float PosXZ[] = {0,0};
int Feedrate = 500;
float currentPosXZ[] = {0,0};                   //The values at which the servos naturally start
extern boolean AbsolutePos;

float Round_To_decimals (float To_Round,int decimals = 1)       // This function takes any float and rounds it to a certain number of decimals
{
  float Rounded = To_Round;
   decimals = constrain(decimals, 0, 9);
  for (int i = decimals; i>0;i--){
    Rounded *= 10;
  }
  Rounded = round(Rounded);
    for (int i = decimals; i>0;i--){
    Rounded /= 10;
  }
  return Rounded;
}

void MovingCommand() {
  bool endMarker = false;                                 //Define a variable to break the for loop from inside a case structure
  if (AbsolutePos == false) {
    for (int v = 0; v < 2; v++) {                           //Reinitialize the value of desired position before reading the new desired value
      PosXZ[v] = 0;
    }
  }
  else{
    for (int v = 0; v < 2; v++) {                           //Reinitialize the value of desired position before reading the new desired value
      PosXZ[v] = currentPosXZ[v];
    }
  }
  for (int i = 0; i < 64; i++) {                          //Read the entire command in search for the attributes of the movement such as position velocity or if the laser should be on or off
    if (Command[i] == ' ' && endMarker == false) {        //Detect the next space that will indicate that a new attribute is expected (G1 X23.34)
      switch (Command[i + 1]) {                           //The switch will read the letter immediately after the space which will indicate the attribute of this movement instruction
        case 'X':                                         //If the case is X it means we want to change the X position so we will scan the following numbers to know to which position we have to move
          {
            PosXZ[0] = Round_To_decimals(SubCommandExtractor(i).toFloat());
            //        Serial.println("X checked");                    //For development purposes
          }
          break;                                          //Break the loop to continue looking for other attributes

        case 'Z':                                         //If the case is Y it means we want to change the Y position so we will scan the following numbers to know to which position we have to move
          {
            PosXZ[1] = Round_To_decimals(SubCommandExtractor(i).toFloat());
            //        Serial.println("Z checked");                //For development purposes
          }
          break;                                           //Break the loop to continue looking for other attributes

        case 'F':                                         //If the case is Y it means we want to change the Y position so we will scan the following numbers to know to which position we have to move
          {
            Feedrate = round(SubCommandExtractor(i).toFloat());
            //        Serial.println("F checked");                //For development purposes
          }
          break;

        case ';':                                         //If the case is ;
          endMarker = true;                               //Set the endmarker flag to true to stop the loop
          break;

        case '\n':                                         //If the case is ;
          endMarker = true;                               //Set the endmarker flag to true to stop the loop
          break;

        case '\0':                                         //If the case is ;
          endMarker = true;                               //Set the endmarker flag to true to stop the loop
          break;

        case ' ':                                         //If the case is ;
          endMarker = true;                               //Set the endmarker flag to true to stop the loop
          break;

        default:
          {
            UnknownCommand();                                 //In the case of anything else, send: Unknown Command
            endMarker = true;                                 //And break the for loop that was looking for other attributes of the G command
          }
          break;
      }
    }
    else if (endMarker == true) {
      break;
    }
  }
    if (PositionAllowed() == true){
      SetSteppersMovement();
    }
    else{
      Serial.println("Coordinate out of range!");
    }
}
