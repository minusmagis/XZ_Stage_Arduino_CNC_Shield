extern float currentPosXZ[];

void GetCurrentPosition(){                    //Print the current coordinates stored in currentPosXZ[] variable in the following form: X:0.00 Z:0.00
  Serial.print("X:");
  Serial.print(currentPosXZ[0]);;
  Serial.print(" Z:");
  Serial.println(currentPosXZ[1]);
}
