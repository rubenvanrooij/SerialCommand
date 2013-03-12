/**************************************************************************/
/*!
 @file     SerialCommands.h
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

#define MAX_COMMAND_LEN             (20)
#define MAX_PARAMETER_LEN           (255)
#define MAX_COMMANDS_COUNT          (10)
#define TO_UPPER(x) (((x >= 'a') && (x <= 'z')) ? ((x) - ('a' - 'A')) : (x))

enum { COMMAND, PARAM };

typedef struct {
    char          *name;
    void          (*function)(void);
} command_t;

class SerialCommands {
 public:
    
    SerialCommands();
    void begin(command_t commandTable[]);
    void receive();
    void sendCommand(char *commandName);
    void sendCommandWithParam(char *commandName, char *param);
    void sendCommandWithBool(char *commandName, bool param);
    void sendCommandWithInt(char *commandName, int param);
    void sendCommandWithIntArrayAsCharacter(char *commandName, uint8_t param[], int arrLength);
    void sendCommandWithIntArrayAsNumeric(char *commandName, uint8_t param[], int arrLength);
    void printAllCommands();
    //uint8_t getP
 
    uint8_t gParamVal[MAX_PARAMETER_LEN + 1];
    long gParamValLong;    // Stored param value as a long
    String gParamValChar;  // Stored param value as a string
 ;

 private:
    
    uint8_t it_cmd;
    uint8_t it_param;
    uint8_t state;
    
    char gCommandBuffer[MAX_COMMAND_LEN + 1];
    char gParamBuffer[MAX_PARAMETER_LEN + 1];

    command_t gCommandTable[MAX_COMMANDS_COUNT+1];

    int buildCommand(char nextChar);
    void processCommand();
};
