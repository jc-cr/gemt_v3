#ifndef ultrasonictest
#define ultrasonictest

//#include "gemt_interface.h"
#include <HCSR04.h>

const unsigned short int  triggerPin = 27;
const unsigned short int  echoPin = 26;

UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);
GEMTtest ultrasonicDisplay;

extern void ultrasonicTest(void);

void runUltrasonicTest(void)
{
  ultrasonicDisplay.setFirstLine("Ultrasonic Info:");
  ultrasonicDisplay.setInfoMsgLine("+ -> 5V"); 
  ultrasonicDisplay.setInfoMsgLine("- -> GND"); 
  ultrasonicDisplay.setInfoMsgLine("Trigger -> 27");
  ultrasonicDisplay.setInfoMsgLine("Echo -> 26");

  if(ultrasonicDisplay.showInfoScreen())
  {
    ultrasonicDisplay.setFirstLine("Ultrasonic Test:");
    ultrasonicDisplay.showStaticTestScreen(ultrasonicTest);
  }
}


unsigned long ultrasonicTestingInterval = 3000.00; // Ping every 3 second
unsigned long ultrasonicPreviousMillis = 0.00;
int distance = 0;

// NOTE: Since test is constatly updating, we dont have access to click feature
//        If testing interval too short we cant exit test!
extern void ultrasonicTest(void)
{
   if (millis() -  ultrasonicPreviousMillis > ultrasonicTestingInterval)
   {
     ultrasonicPreviousMillis = millis();

    // Calculating the distance
    distance = round(distanceSensor.measureDistanceCm());

    String msg = String("Distance: ") + String(distance) + String(" cm"); 

    ultrasonicDisplay.setStaticTestFeedbackLine(msg);
    ultrasonicDisplay.showStaticTestFeedback();
   } 
}

#endif