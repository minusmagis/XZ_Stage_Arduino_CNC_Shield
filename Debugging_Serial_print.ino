extern float PosXZ[2];
extern char Command[];
extern float Extruder;
extern int Feedrate;
extern int toolPower;

void Debugging_Serial_print(){
  Serial.println("");
  Serial.println("--------------Debugging Section-------------------");
  Serial.print("Received Command: ");
  Serial.println(Command);
  Serial.print("Understood Command: ");
  Serial.print(Command[0]);
  Serial.println(CommandNumberExtractor());
  Serial.print("X Position = ");
  Serial.println(PosXZ[0]);
  Serial.print("Z Position = ");
  Serial.println(PosXZ[1]);
  Serial.print("Feedrate = ");
  Serial.println(Feedrate);
  Serial.println("--------------------------------------------------");
  Serial.println("");
}
