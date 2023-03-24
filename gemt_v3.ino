#include "gemt_interface.h"

GEMTdisplay Display;

void setup(void)
{
  delay(500);
  Display.bootUp();
  Serial.begin(115200); // DEBUGGING
}

int main(void) 
{
  // Must intialize the arduino firmware
  init();
  setup();

  while(true)
  {
    //displayMenuDriver();
    Display.debugScreen();
    delay(3);
  }
  
  return 0;
}