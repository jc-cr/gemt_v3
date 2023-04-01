#ifndef servotest
#define servotest

#include "gemt_interface.h"
#include <Servo.h>

GEMTtest servoDisplay;
Servo servo;
const unsigned short int  servoPWMPin = 9;

extern void manualServoTest(void);

void runServoManualTest(void)
{  
  servo.attach(servoPWMPin);

  servoDisplay.setFirstLine("9g Servo Connections:");
  servoDisplay.setInfoMsgLine("+ -> 5V"); 
  servoDisplay.setInfoMsgLine("- -> GND"); 
  servoDisplay.setInfoMsgLine("Trigger -> 27");
  servoDisplay.setInfoMsgLine("Echo -> 26");

  if (servoDisplay.showInfoScreen())
  {
    servoDisplay.setFirstLine("9g Manual Test: ");
    servoDisplay.showInteractiveTestScreen(manualServoTest, "Angle: ",  0, 180);
  }

  // Reset when done, else will keep populating fields like info screen
  servo.detach();
  servoDisplay.resetMembers();
}

int previousAngle;
extern void manualServoTest(void)
{
  if(servoDisplay.interactiveValue != previousAngle)
  {
    servo.write(servoDisplay.interactiveValue);
    previousAngle = servoDisplay.interactiveValue;
  }
}






#endif