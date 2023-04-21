#ifndef servotest
#define servotest

#include <Servo.h>
#include "gemt_interface.h"

GEMTtest servoDisplay;
Servo servo;

const unsigned short int  servoPWMPin = 7;

extern void manualServoTest(void);
extern void autoServoTest(void);

void runServoManualTest(void)
{  
  servo.attach(servoPWMPin);

  servoDisplay.setFirstLine("9g Servo Test Info:");
  servoDisplay.setInfoMsgLine("+ -> 5V"); 
  servoDisplay.setInfoMsgLine("- -> GND"); 
  servoDisplay.setInfoMsgLine("PWM -> 7");

  if (servoDisplay.showInfoScreen())
  {
    servoDisplay.setFirstLine("9g Manual Test: ");
    servoDisplay.showInteractiveTestScreen(manualServoTest, "Angle: ",  0, 180);
  }

  servo.detach();
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

  servoDisplay.setStaticTestFeedbackLine("Servo will now rotate");
  servoDisplay.setStaticTestFeedbackLine("to 180 degrees in 45 degree increments");
  servoDisplay.showStaticTestFeedback(0);
  delay(2000);
  
  for (int i = 0; i <= 4; ++i)
  {
    angle = (45 * i);
    String msg = String("Angle: ") + String(angle);
    servo.write(angle);
    servoDisplay.setStaticTestFeedbackLine(msg);
    servoDisplay.showStaticTestFeedback(0);
    delay(2000); // Extra delay to allow servo to reach pos
  }
  for (int i = 0; i <= 4; ++i)
  {
    angle = (180 - (45 * i));
    String msg = String("Angle: ") + String(angle);
    servo.write(angle);
    servoDisplay.setStaticTestFeedbackLine(msg);
    servoDisplay.showStaticTestFeedback(0);
    delay(2000); // Extra delay to allow servo to reach pos 
  }

  String msg = String("Angle: ") + String("0");
  servoDisplay.setStaticTestFeedbackLine(msg);
  servoDisplay.showStaticTestFeedback(1);
  servoDisplay.testingComplete = true;
}

#endif