void checkCommand(String command){
  int commaIndex[10];
  int parenthesisIndex[1];
  String commandArray[10];
  int arg;
  int maxArguments = 10;
  int numberOfCommas = 0;
  boolean noCommasLeft = false;

  parenthesisIndex[0] = command.indexOf('(');
  parenthesisIndex[1] = command.indexOf(')');
  commandArray[0] = command.substring(0, parenthesisIndex[0]);
  
  for (arg = 1; arg < maxArguments; arg++){
    if (arg == 1){
      commaIndex[arg] = command.indexOf(',');

      if (commaIndex[arg] == -1){
        noCommasLeft = true;
        maxArguments = 1;
        commandArray[arg] = command.substring(parenthesisIndex[0]+1, parenthesisIndex[1]);
      }else{
        commandArray[arg] = command.substring(parenthesisIndex[0]+1, commaIndex[arg]);
        numberOfCommas++;
      }
    }else if (!noCommasLeft){
      commaIndex[arg] = command.indexOf(',', commaIndex[arg-1]+1);
      
        if (commaIndex[arg] == -1 or arg + 1 == maxArguments){
          noCommasLeft = true;
          commandArray[arg] = command.substring(commaIndex[arg-1]+1, parenthesisIndex[1]);
          maxArguments = 1;
        }else{
          commandArray[arg] = command.substring(commaIndex[arg-1]+1, commaIndex[arg]);
          numberOfCommas++;
        }  
    }
  }
  
  runCommand(commandArray,numberOfCommas);
}

void runCommand(String commandList[], int numberOfCommands){

  //Make command variables accesible
  const char *command1 = commandList[1].c_str();
  const char *command2 = commandList[2].c_str();
  const char *command3 = commandList[3].c_str();
  const char *command4 = commandList[4].c_str();
  const char *command5 = commandList[5].c_str();
  const char *command6 = commandList[6].c_str();
  const char *command7 = commandList[7].c_str();
  const char *command8 = commandList[8].c_str();
  const char *command9 = commandList[9].c_str();

  boolean commandComplete = true;

  //Search for commands
  if(commandList[0] == "getID" && commandReceived()) 
  {
    getID(); 
  }else if(commandList[0] == "solidStateRelay.on" && commandReceived()){
    solidStateRelay.on();
  }else if(commandList[0] == "solidStateRelay.off" && commandReceived()){
    solidStateRelay.off();
  }else if(commandList[0] == "scan" && commandReceived()){
    Serial.println('Scanning');
    scan();
  }else if(commandList[0] == "motorTest" && commandReceived()){
    long starttime = millis();
      long endtime = starttime;
      solidStateRelay.on();
      while ((millis() - starttime) <= 2000) // do this loop for up to 1000mS
      {
        analogWrite(leftMotors, 250);
        analogWrite(rightMotors, 250);
      }
      analogWrite(leftMotors, 0);
      analogWrite(rightMotors, 0);
      solidStateRelay.off();
  }else if(commandList[0] == "turnLeft" && commandReceived()){
    long starttime = millis();
      long endtime = starttime;
      solidStateRelay.on();
      while ((millis() - starttime) <= 450) // do this loop for up to 1000mS
      {
        analogWrite(rightMotors, 200);
      }
      analogWrite(leftMotors, 0);
      analogWrite(rightMotors, 0);
      solidStateRelay.off();
  }else if(commandList[0] == "turnRight" && commandReceived()){
    long starttime = millis();
      long endtime = starttime;
      solidStateRelay.on();
      while ((millis() - starttime) <= 450) // do this loop for up to 1000mS
      {
        analogWrite(leftMotors, 200);
      }
      analogWrite(leftMotors, 0);
      analogWrite(rightMotors, 0);
      solidStateRelay.off();
  }else{
    commandComplete = false;
    Serial.println("*ER Command \"" + commandList[0] + "\" not recognized!");
  }

  if(commandComplete){
    Serial.println("*DONE");
  }
}

boolean commandReceived(){
  Serial.println("*OK");
  return true;
}


