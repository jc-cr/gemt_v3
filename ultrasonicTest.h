#pragma once


#include "gemt_interface.h"
#include <HCSR04.h>

const unsigned short int  triggerPin = 30;
const unsigned short int  echoPin = 28;

UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);
GEMTtest ultrasonicDisplay;

extern void ultrasonicTest(void);

void runUltrasonicTest(void)
{
  ultrasonicDisplay.setFirstLine("Ultrasonic Info:");
  ultrasonicDisplay.setInfoMsgLine("+ -> 5V"); 
  ultrasonicDisplay.setInfoMsgLine("- -> GND"); 
  ultrasonicDisplay.setInfoMsgLine("Trigger -> 30");
  ultrasonicDisplay.setInfoMsgLine("Echo -> 28");

  if(ultrasonicDisplay.showInfoScreen())
  {
    ultrasonicDisplay.setFirstLine("Ultrasonic Test:");
    ultrasonicDisplay.showStaticTestScreen(ultrasonicTest);
  }
}

unsigned long ultrasonicTestingInterval = 500.00; // Ping every 3 second
unsigned long ultrasonicPreviousMillis = 0.00;
float distance = 0;

// NOTE: Since test is constatly updating, we dont have access to click feature
//        If testing interval too short we cant exit test!
extern void ultrasonicTest(void)
{
   if (millis() -  ultrasonicPreviousMillis > ultrasonicTestingInterval)
   {
     ultrasonicPreviousMillis = millis();

    // Calculating the distance
    distance = distanceSensor.measureDistanceCm();

    String msg = String("Distance: ") + String(distance, 2) + String(" cm"); 

    ultrasonicDisplay.setStaticTestFeedbackLine(msg);
    ultrasonicDisplay.showStaticTestFeedback(2000);
   } 
}
