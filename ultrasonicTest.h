#ifndef ultrasonictest
#define ultrasonictest

#include <HCSR04.h>

extern void ultrasonicTest(void);

GEMTtest ultrasonicDisplay;


void runUltrasonicTest(void)
{
  ultrasonicDisplay.setFirstLine("Ultrasonic Info:");
  ultrasonicDisplay.setInfoMsgLine("+ -> 5V | - -> GND"); 
  ultrasonicDisplay.setInfoMsgLine("Trigger -> 27");
  ultrasonicDisplay.setInfoMsgLine("Echo -> 26");

  if(ultrasonicDisplay.showInfoScreen())
  {
    ultrasonicDisplay.setFirstLine("Ultrasonic Test:");
    ultrasonicDisplay.showStaticTestScreen(nRFtest);
  }
}

extern void ultrasonicTest(void)
{
  

}


/*
//Function which constantly updates a Distance onto the OLED, until the user clicks the encoder to exit.
void ultrasonicSensor_Test(void)
{ 
  long      duration;
  int       distance;
  double    permDistance;
  double    samples;
  char      buffer[50]; // init buffer of 50 bytes to hold expected string size
  attachInterrupt(digitalPinToInterrupt(7), onEb1Clicked, RISING); // This is necessary to allow the user to return from test screens, the digitial pin to interrupt is the SWITCH pin. 
  //The EncoderButton.h header doesn't automatically do this for some reason.
  while(!clicked)
  {
    eb1.update();
    displayPrep();
    
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2;
    delay(500);
    sprintf(buffer, "Distance: %d cm", distance);
    display.println(buffer);
    printConfirm();
    display.display();
  }
  clicked = 0;
}
*/

#endif