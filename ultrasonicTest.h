#ifndef ultrasonictest
#define ultrasonictest

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
int duration = 0;

extern void ultrasonicTest(void)
{
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);

    digitalWrite(triggerPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);

    // Calculating the distance
    distance = duration * (34 / 2000); // was 0.34/2 .. Want to see if 34/2000 is faster

    String msg = String("Distance: ") + String(distance) + String(" cm"); 
    ultrasonicDisplay.showStaticTestFeedback(msg);
}

#endif