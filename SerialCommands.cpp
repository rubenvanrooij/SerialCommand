/**************************************************************************/
/*! 
    @file     SerialCommands.cpp
    @author   Ruben v. Rooij
	@license  BSD (see license.txt)
 
    This is library sends and receives commands on the arduino serial
    port. It's based on tasasaki's example on the Arduino form.
*/
/**************************************************************************/
#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#include "SerialCommands.h"

//#define DEBUG

/*
 command_t const *gCommandTable[COMMAND_TABLE_SIZE] = {
 {"HELP",       commandHelp, },
 {"WRITE",      commandWrite,},
 {"READ_UID",   commandReadUID, },
 {"READ_DATA",  commandReadData, },
 {NULL,       NULL }
 };
*/

/**************************************************************************/
/*! 
    @brief  Instantiates a new SerialCommands class
*/
/**************************************************************************/
SerialCommands::SerialCommands() {
    it_cmd = 0;
    it_param = 0;
    state = COMMAND;
}

/**************************************************************************/
/*!
    @brief  Initializes the commands table
 
    @param  commandTable   Command table array
 */
/**************************************************************************/
void SerialCommands::begin(command_t commandTable[])
{
    int it;
    
    for (it = 0; it<MAX_COMMANDS_COUNT; it++)
    {
        if(commandTable[it].name != NULL)
        {
            gCommandTable[it].name     = commandTable[it].name;
            gCommandTable[it].function = commandTable[it].function;
        }
        else
        {
            gCommandTable[it].name = NULL;
            gCommandTable[it].function= NULL;
        }
    }
    
    gCommandTable[MAX_COMMANDS_COUNT].name = NULL;
    gCommandTable[MAX_COMMANDS_COUNT].function = NULL;
    
#ifdef DEBUG
    // Print out command list
    Serial.println("The following commands can be used:");
    
    for (it = 0; gCommandTable[it].name != NULL; it++) {
        Serial.print("> ");
        Serial.println(gCommandTable[it].name);
    }
    
    Serial.println("");
#endif
}

/**************************************************************************/
/*!
    @brief  Receives serial data and processes it
*/
/**************************************************************************/
void SerialCommands::receive()
{
    int bCommandReady = false;
    
    while(Serial.available() > 0)
        bCommandReady = buildCommand( (char)Serial.read() );
    
    if (bCommandReady == true)
        processCommand();
}

/**************************************************************************/
/*!
    @brief      Put received characters into the command buffer or the
                parameter buffer. Once a complete command is received
                return true.
 
    @param      nextChar    the nect character to buffer
    @returns    true if a command is complete, otherwise false.
 */
/**************************************************************************/
int SerialCommands::buildCommand(char nextChar) {
    
    if ((nextChar == '\n') || (nextChar == '\t') || (nextChar == '\r'))
        return false;
    
    if (nextChar == ';') {
        gCommandBuffer[it_cmd] = '\0';
        gParamBuffer[it_param] = '\0';
        it_cmd = 0;
        it_param = 0;
        state = COMMAND;
        return true;
    }
    
    if (nextChar == ',') {
        state = PARAM;
        return false;
    }
    
    if (state == COMMAND) {

        gCommandBuffer[it_cmd] = TO_UPPER(nextChar);
        it_cmd++;
        
        if (it_cmd > MAX_COMMAND_LEN) {
            it_cmd = 0;
            return true;
        }
    }
    
    if (state == PARAM) {

        if(it_param == 0 && (nextChar == ' '))
            return false;
        
        gParamBuffer[it_param] = nextChar;
        it_param++;

        if (it_param > MAX_PARAMETER_LEN) {
            it_param = 0;
            return true;
        }
    }
    
    return false;
}

/**************************************************************************/
/*!
 @brief      Processes the command and param buffer. If a command from the 
            command list is found it calls the corresponding function.
 */
/**************************************************************************/
void SerialCommands::processCommand()
{
    int bCommandFound = false;
    int it;
    
    for(it=0; it<MAX_PARAMETER_LEN; it++)
    {
        gParamVal[it] = gParamBuffer[it];
    }
    
    gParamValLong = strtol(gParamBuffer, NULL, 0);

    gParamValChar = "";
    gParamValChar.concat( gParamBuffer );
    
    for (it = 0; gCommandTable[it].name != NULL; it++) {
                
        if (strcmp(gCommandTable[it].name, gCommandBuffer) == 0) {
            bCommandFound = true;
            break;
        }
    }
    
    if (bCommandFound == true) {
        
#ifdef DEBUG
        Serial.println("Command found:");
        Serial.print("> ");
        Serial.println(gCommandTable[it].name);
#endif 
        (*gCommandTable[it].function)();
    }
    else {

        sendCommand("NOT FOUND");
    }
}

/**************************************************************************/
/*!
 @brief     Sends a command without parameter
 
 @param     commandName     Name of the command.
 */
/**************************************************************************/
void SerialCommands::sendCommand(char *commandName)
{
    Serial.print(commandName);
    Serial.println(";");
}

/**************************************************************************/
/*!
 @brief     Sends a command with parameter
 
 @param     commandName     Name of the command.
 @param     param           Parameter/value of the command.
 */
/**************************************************************************/
void SerialCommands::sendCommandWithParam(char *commandName, char *param)
{
    Serial.print(commandName);
    Serial.print(", ");
    Serial.print(param);
    Serial.println(";");
}

/**************************************************************************/
/*!
 @brief     Sends a command with a boolean parameter
 
 @param     commandName     Name of the command.
 @param     param           Parameter/value of the command.
 */
/**************************************************************************/
void SerialCommands::sendCommandWithBool(char *commandName, bool param)
{
    sendCommandWithParam(commandName, (char*)(param ? "1" : "0"));
}

/**************************************************************************/
/*!
 @brief     Sends a command with a integer parameter
 
 @param     commandName     Name of the command.
 @param     param           Parameter/value of the command.
 */
/**************************************************************************/
void SerialCommands::sendCommandWithInt(char *commandName, int param)
{
    Serial.print(commandName);
    Serial.print(", ");
    Serial.print(param);
    Serial.println(";");
}

/**************************************************************************/
/*!
 @brief     Sends a command with an paramater that consists of an array of 
            uint8_t. The integerws will be parsed to characters (ASCII)
 
 @param     commandName     Name of the command.
 @param     param           Parameter/value of the command.
 @param     arrLength       Length of the array.
 */
/**************************************************************************/
void SerialCommands::sendCommandWithIntArrayAsCharacter(char *commandName, uint8_t param[], int arrLength)
{
    int it;
    
    Serial.print(commandName);
    Serial.print(", ");
    
    for (it=0; it < arrLength; it++)
        if (param[it] > 0x1F)
            Serial.print((char)param[it]);
    
    Serial.println(";");
}

/**************************************************************************/
/*!
 @brief     Sends a command with an paramater that consists of an array of
            uint8_t. 
 
 @param     commandName     Name of the command.
 @param     param           Parameter/value of the command.
 @param     arrLength       Length of the array.
 */
/**************************************************************************/
void SerialCommands::sendCommandWithIntArrayAsNumeric(char *commandName, uint8_t param[], int arrLength)
{
    int it;
    
    Serial.print(commandName);
    Serial.print(", ");
    
    for(it=0;it<arrLength;it++)
        Serial.print(param[it]);
    
    Serial.println(";");
}

void SerialCommands::printAllCommands()
{
    int it;;
    for (it = 0; gCommandTable[it].name != NULL; it++) {
        Serial.println(gCommandTable[it].name);
    }
}





