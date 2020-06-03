boolean AbsolutePos = true;
extern float PosXZ[];

void GGcode(){                                  //Function that runs in the event of a GXX command (G1 or G0)
//  Serial.println("G Checked");                //For development purposes
  switch(CommandNumberExtractor()){             //Read the command number after the G of the G command to know exactly which command it is
    case 0:                                     //Both the case 0 and the case 1 are treated equally
      MovingCommand();                          //Jump to the Moving Command Function
    break;

    case 1:                                     //Both the case 0 and the case 1 are treated equally
      MovingCommand();                          //Jump to the Moving Command Function
    break;

    case 4:                                     //If we have command G4 Go to Wait() function
      Wait();
    break;

    case 28:                                    //Case 28 is homing so go to home function
      Home();
    break;

    case 90:
      AbsolutePos = true;                       //Set the Absolute position flag to true
    break;

    case 91:
      AbsolutePos = false;                      //Set the Absolute position flag to false to move in relative positions
    break;

    default:
    {
      UnknownCommand();                         //In the case of anything else, send: Unknown Command
    }
    break;
  }
}
