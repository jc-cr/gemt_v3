#ifndef ultrasonictest
#define ultrasonictest

//#include "gemt_interface.h"
#include <HCSR04.h>

extern void ultrasonicTest(void);

GEMTtest ultrasonicDisplay;

const unsigned short int  triggerPin = 27;
const unsigned short int  echoPin = 26;


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

int distance = 0;
static unsigned long testingInterval = 3000.00; // Ping every 3 second
static unsigned long previousMillis = 0.00;
int duration = 0;

// NOTE: Since test is constatly updating, we dont have access to click feature
//        If testing interval too short we cant exit test!
extern void ultrasonicTest(void)
{
   if (millis() - previousMillis > testingInterval)
   {
    previousMillis = millis();

    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(triggerPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = round(pulseIn(echoPin, HIGH));

    // Calculating the distance
    distance = round(duration * (34 / 2000)); // was 0.34/2 .. Want to see if 34/2000 is faster

    String msg = String("Distance: ") + String(distance) + String(" cm"); 

    ultrasonicDisplay.showStaticTestFeedback(msg);
   }

  return;   
}

#endif