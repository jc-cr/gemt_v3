#ifndef servotest
#define servotest

//#include "gemt_interface.h"
#include <Servo.h>

GEMTtest servoDisplay;
Servo servo;

const unsigned short int  servoPWMPin = 9;

extern void manualServoTest(void);
extern void autoServoTest(void);

void runServoManualTest(void)
{  
  servo.attach(servoPWMPin);

  servoDisplay.setFirstLine("9g Servo Test Info:");
  servoDisplay.setInfoMsgLine("+ -> 5V"); 
  servoDisplay.setInfoMsgLine("- -> GND"); 
  servoDisplay.setInfoMsgLine("PWM -> 9");

  if (servoDisplay.showInfoScreen())
  {
    servoDisplay.setFirstLine("9g Manual Test: ");
    servoDisplay.showInteractiveTestScreen(manualServoTest, "Angle: ",  0, 180);
  }

  // Reset when done, else will keep populating fields like info screen
  servo.detach();
  servoDisplay.resetMembers();
}

void runServoAutoTest(void)
{
  servo.attach(servoPWMPin);

  servoDisplay.setFirstLine("9g Servo Test Info:");
  servoDisplay.setInfoMsgLine("+ -> 5V"); 
  servoDisplay.setInfoMsgLine("- -> GND"); 
  servoDisplay.setInfoMsgLine("PWM -> 9");

  if (servoDisplay.showInfoScreen())
  {
    servoDisplay.setFirstLine("9g Auto Test: ");
    servoDisplay.showStaticTestScreen(autoServoTest);
  }

  servo.detach();
}

int previousAngle;
extern void manualServoTest(void)
{
  if(servoDisplay.interactiveValue[0] != previousAngle)
  {
    servo.write(servoDisplay.interactiveValue[0]);
    previousAngle = servoDisplay.interactiveValue[0];
  }
}

extern void autoServoTest(void)
{
  servo.attach(servoPWMPin);
  servo.write(0);

  int angle = 0;

  servoDisplay.showStaticTestFeedback("Servo will now rotate to 180 degrees in 45 degree increments");
  delay(2000);
  
  for (int i = 0; i <= 4; ++i)
  {
    angle = (45 * i);
    String msg = String("Angle: ") + String(angle);
    servo.write(angle);
    servoDisplay.showStaticTestFeedback(msg);
    delay(2000); // Extra delay to allow servo to reach pos
  }
  for (int i = 0; i <= 4; ++i)
  {
    angle = (180 - (45 * i));
    String msg = String("Angle: ") + String(angle);
    servo.write(angle);
    servoDisplay.showStaticTestFeedback(msg);
    delay(2000); // Extra delay to allow servo to reach pos 
  }

  servoDisplay.testingComplete = true;
}

#endif