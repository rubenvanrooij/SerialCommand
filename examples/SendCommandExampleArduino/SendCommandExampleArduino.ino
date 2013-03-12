#include <SerialCommands.h>

/*
  Commands that can be used
  by the program
*/
command_t gCommandTable[] = {
  { "LED",          receiveLed,  },
  { "LED_STATUS",   receiveLedStatus, },
  { "HELP",         receiveHelp, },
  { NULL, NULL }
};
  
SerialCommands serialCMD;
 
void setup()
{
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  
  serialCMD.begin( gCommandTable ); // Initializes the command table
  serialCMD.sendCommand("READY");
}

void loop()
{
  serialCMD.receive(); // Listen to new commands on the serial port
}

void receiveLed()
{
   if(serialCMD.gParamValLong == 1)
   {
      digitalWrite(13, HIGH);
      serialCMD.sendCommand("SUCCES"); // Succesfully set new value
   }
   else if(serialCMD.gParamValLong == 0)
   {
      digitalWrite(13, LOW);
      serialCMD.sendCommand("SUCCES"); // Succesfully set new value
   }
   else
   {
      serialCMD.sendCommand("INVALID_PARAM"); // No valid parameter found..
   }
}

void receiveLedStatus()
{ 
  serialCMD.sendCommandWithInt("LED_STATUS", digitalRead(13)); // Send back status command with parameter
}

void receiveHelp()
{
  // Printing the help will actually also send a 'command'.
  // If you want two way communication don't use this function...  
  serialCMD.printAllCommands();
}

